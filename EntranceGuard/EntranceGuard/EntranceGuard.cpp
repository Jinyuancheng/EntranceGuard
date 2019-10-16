#include "EntranceGuard.h"
#include <windows.h>
#include <QPushButton>
#include <QStringList>
#include <QStringListModel>
#include <functional>
#include <sstream>
#include <algorithm>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValueRef>
#include <QVariantList>
#include <QVariantMap>
#include <QNetworkRequest>
#include <QEventLoop>

#include "../Utils/json.h"
#include "../Utils/utils.h"
#include "./CHttpRequest.h"

/*\ 引用外部的配置文件信息 \*/
extern CSaveiniInfo* gl_opSaveIniInfo;

std::vector<NET_DVR_CARD_CFG_V50>	gl_vecCardInfo;			/*\ 存储卡的信息 \*/
DWORD								gl_dwOptType = -1;		/*\ 操作类型,用来进行数据的类型判断 \*/
std::function<void(void)>			gl_funcShowUserInfo;	/*\ 用来显示用户信息 \*/
QString								gl_qsUserId = -1;		/*\ 服务器端用户id \*/

/****************************************!
*@brief  构造函数
*@author Jinzi
*@date   2019/09/10 11:00:12
*@param[in]
*@param[out]
*@return
****************************************/
EntranceGuard::EntranceGuard(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	/*\ 分配内存 \*/
	m_oMenu = new QMenu();
	m_oFmMenJinLogin = new fmMenJinLogin(this);
	m_oFmAddUser = new fmAddUser(this);
	m_opFmChangeUser = new fmChangeUser(this);
	m_opLongConnInfo = new CLongConnStatus();
	/*\ 初始化m_opLongConnInfo \*/
	InitSaveLongInfo();

	m_opCardInfo = new NET_DVR_CARD_CFG_COND();
	m_opSendDataInfo = new NET_DVR_CARD_CFG_SEND_DATA();
	m_opHttpManager = new QNetworkAccessManager(this);
	m_opHttpRequest = new CHttpRequest();

	/*\ 初始化变量,回调函数 \*/
	m_funcMsgHikCallBack = (MSGCallBack_V31)MsgHikCallBack;
	m_funcMsgRemoteConfig = (fRemoteConfigCallback)RemoteConfigCallBack;
	gl_funcShowUserInfo = std::bind(&EntranceGuard::ShowMenJinUserInfo, this);
	/*\ 给子窗口函数指针赋值 \*/
	InitChildWinFuncAssign();
	/*\ 初始化海康sdk \*/
	InitHikSDK();
	/*\ 中文必须使用fromLocal8Bit函数,否则中文会乱码 \*/
	m_oAction1 = new QAction(QString::fromLocal8Bit("登录门禁"), this);
	m_oAction2 = new QAction(QString::fromLocal8Bit("登出门禁"), this);
	m_oAction3 = new QAction(QString::fromLocal8Bit("登出所有主机"), this);
	/*\ 将子菜单添加到菜单中 \*/
	m_oMenu->addAction(m_oAction1);
	m_oMenu->addAction(m_oAction2);
	m_oMenu->addAction(m_oAction3);
	/*\ 绑定信号和槽 \*/
	InitSignalAndSlot();
	/*\ 显示所有用户信息 \*/
	ShowAllUserInfoWithListView();
}

/****************************************!
*@brief  给子窗口赋值
*@author Jinzi
*@date   2019/09/24 11:35:30
*@param[in]
*@param[out]
*@return
****************************************/
void EntranceGuard::InitChildWinFuncAssign()
{
	///////////////////// 2019/09/24 11:36:38 登录窗口 ==> BEGIN /////////////////////////////////////////////////////
	m_oFmMenJinLogin->m_funcSetLoginInfo = std::bind(&EntranceGuard::SetListLoinInfo, this, std::placeholders::_1);
	m_oFmMenJinLogin->m_funcSetLabelInfo = std::bind(&EntranceGuard::SetLabelShowInfo, this, std::placeholders::_1);
	m_oFmMenJinLogin->m_funcGetListViewCheck = std::bind(&EntranceGuard::GetListViewCheck, this);
	///////////////////// 2019/09/24 11:36:38 登录窗口 ==> END /////////////////////////////////////////////////////

	///////////////////// 2019/09/24 11:36:56 修改用户窗口 ==> BEGIN /////////////////////////////////////////////////////
	m_opFmChangeUser->m_funcRetUserSelMenJinLoginHandle =
		std::bind(&EntranceGuard::RetUserSelMenJinLoginHandle, this);
	m_opFmChangeUser->m_funcRetUserSelJobNum =
		std::bind(&EntranceGuard::RetUserSelRowJobNum, this);
	m_opFmChangeUser->m_funcRetUserSelCardNum =
		std::bind(&EntranceGuard::RetUserSelRowCardNum, this);
	///////////////////// 2019/09/24 11:36:56 修改用户窗口 ==> END /////////////////////////////////////////////////////

	///////////////////// 2019/09/24 11:37:23 添加用户窗口 ==> BEGIN /////////////////////////////////////////////////////
	m_oFmAddUser->m_funcGetListViewCheck = std::bind(&EntranceGuard::GetListViewCheck, this);
	///////////////////// 2019/09/24 11:37:23 添加用户窗口 ==> END /////////////////////////////////////////////////////
}

/****************************************!
*@brief  绑定信号和槽
*@author Jinzi
*@date   2019/09/10 10:51:27
*@param[in]
*@param[out]
*@return
****************************************/
void EntranceGuard::InitSignalAndSlot()
{
	/*\ 显示登录页面,使用triggered()信号函数,来触发 \*/
	connect(m_oAction1, SIGNAL(triggered()), m_oFmMenJinLogin, SLOT(show()));
	/*\ 门禁主机登出 \*/
	connect(m_oAction2, SIGNAL(triggered()), m_oFmMenJinLogin, SLOT(HikMenJinLoginOut()));
	/*\ 登出所有门禁主机 \*/
	connect(m_oAction3, SIGNAL(triggered()), m_oFmMenJinLogin, SLOT(HikMenJinLoginOutAll()));
	/*\ 查询按钮 \*/
	connect(ui.m_btnSelect, SIGNAL(clicked()), this, SLOT(SelectMenJinInfo()));
	/*\ 获取设备能力集按钮 \*/
	connect(ui.m_btnDevAblity, SIGNAL(clicked()), this, SLOT(GetDevAbilty()));
	/*\ 清空人员信息按钮 \*/
	connect(ui.m_btnDelAllUser, SIGNAL(clicked()), this, SLOT(DelMenJinOnAllUserInfo()));
	/*\ 添加用户按钮 \*/
	connect(ui.m_btnAddUser, SIGNAL(clicked()), this, SLOT(JuageShowAddUserFm()));
	/*\ 下发信息 \*/
	connect(ui.m_btnSend, SIGNAL(clicked()), this, SLOT(BeSentDownMenJin()));
	/*\ 删除人员信息按钮 \*/
	connect(ui.m_btnDelUser, SIGNAL(clicked()), this, SLOT(DelUserInfoWithMenJin()));
	/*\ 修改人员信息按钮 \*/
	connect(ui.m_btnModifUser, SIGNAL(clicked()), this, SLOT(ShowFmChangeUserInfo()));
	/*\ 页面关闭退出登录 \*/
	connect(this, SIGNAL(close()), this, SLOT(HikMenJinLoginOutAll()));
}

/****************************************!
*@brief  析构函数
*@author Jinzi
*@date   2019/09/03 16:30:59
*@param[in]
*@param[out]
*@return
****************************************/
EntranceGuard::~EntranceGuard()
{
	if (m_opCardInfo != nullptr)
	{
		delete m_opCardInfo;
	}
	if (m_opSendDataInfo != nullptr)
	{
		delete m_opSendDataInfo;
	}
	if (m_opLongConnInfo != nullptr)
	{
		delete m_opLongConnInfo;
	}
}

/****************************************!
*@brief  鼠标右键点击事件
*@author Jinzi
*@date   2019/09/02 13:39:32
*@param[in]
   _oEvent		:	鼠标事件
*@param[out]
*@return
****************************************/
void EntranceGuard::mousePressEvent(QMouseEvent* _oEvent)
{
	if (_oEvent->buttons() == Qt::RightButton)
	{
		QRect oLblRect = ui.m_lblOptInfo->rect();
		/*\ 判断焦点是否在矩形内 \*/
		if (oLblRect.contains(_oEvent->pos()))
		{
			m_oMenu->exec(_oEvent->globalPos());
		}
	}
}

/****************************************!
*@brief  初始化存储长连接信息的结构体
*@author Jinzi
*@date   2019/09/16 12:02:21
*@param[in]
*@param[out]
*@return
****************************************/
void EntranceGuard::InitSaveLongInfo()
{
	m_opLongConnInfo->m_iLoginHandle = -1;
	m_opLongConnInfo->m_iLongConnHandle = -1;
	m_opLongConnInfo->m_bIsLongConn = false;
}

/****************************************!
*@brief  初始化海康sdk
*@author Jinzi
*@date   2019/09/02 16:22:15
*@param[in]
*@param[out]
*@return
****************************************/
void EntranceGuard::InitHikSDK()
{
	if (NET_DVR_Init())
	{
		NET_DVR_SetDVRMessageCallBack_V31(this->m_funcMsgHikCallBack, nullptr);
	}
}

/****************************************!
*@brief  海康回调函数
*@author Jinzi
*@date   2019/09/02 16:27:42
*@param[in]
   _lCommand		:	上传的消息类型，不同的报警信息对应不同的类型，通过类型区分是什么报警信息
   _pAlarmer		:	报警设备信息，包括设备序列号、IP地址、登录IUserID句柄等
   _pAlarmInfo		:	报警信息，通过lCommand值判断pAlarmer对应的结构体
   _dwBufLen		:	报警信息缓存大小
   _pUser			:	用户数据
*@param[out]
*@return
****************************************/
BOOL MsgHikCallBack(LONG _lCommand, NET_DVR_ALARMER* _pAlarmer,
	char* _pAlarmInfo, DWORD _dwBufLen, void* _pUser)
{
	return TRUE;
}

/****************************************!
*@brief  清除海康sdk
*@author Jinzi
*@date   2019/09/02 16:34:24
*@param[in]
*@param[out]
*@return
****************************************/
void EntranceGuard::CleanHikSDK()
{
	NET_DVR_Cleanup();
}

/****************************************!
*@brief  显示treeView
*@author Jinzi
*@date   2019/09/03 10:03:30
*@param[in]
*@param[out]
*@return
****************************************/
void EntranceGuard::ShowListView()
{
	QStringList oStrList;
	for (std::vector<CLoginInfo>::iterator it = m_vecLoginInfo.begin();
		it != m_vecLoginInfo.end(); it++)
	{
		oStrList.append((*it).m_sNodeName);
	}
	QStringListModel* oStrListModel = new QStringListModel(oStrList);
	ui.m_listDevInfo->setModel(oStrListModel);
}

/****************************************!
*@brief  子窗口调用该函数，为主窗体赋值(门禁登录成功)
*@author Jinzi
*@date   2019/09/03 10:47:51
*@param[in]
   _lstListLoginInfo :	门禁的登录信息
*@param[out]
*@return
****************************************/
void EntranceGuard::SetListLoinInfo(std::vector<CLoginInfo>& _lstListLoginInfo)
{
	if (!m_vecLoginInfo.empty())
	{
		m_vecLoginInfo.clear();
	}
	m_vecLoginInfo = _lstListLoginInfo;
	m_oFmAddUser->SetLoginInfo(_lstListLoginInfo);
	/*\ 显示登录信息 \*/
	this->ShowListView();
}

/****************************************!
*@brief  设置label显示数据
*@author Jinzi
*@date   2019/09/03 13:47:54
*@param[in]
   _sShowInfo : 要显示的信息
*@param[out]
*@return
****************************************/
void EntranceGuard::SetLabelShowInfo(const char* _cchpShowInfo)
{
	/*\ 更改显示信息 \*/
	ui.m_lblOptInfo->setText(QString::fromLocal8Bit(_cchpShowInfo));
}

/****************************************!
*@brief  返回treeVeiw控件原则的行数
*@author Jinzi
*@date   2019/09/03 14:16:50
*@param[in]
*@param[out]
*@return 成功返回选择的位置 失败返回-1
****************************************/
int EntranceGuard::GetListViewCheck()
{
	int iIndex = -1;
	iIndex = ui.m_listDevInfo->currentIndex().row();
	return iIndex;
}

/****************************************!
*@brief  查询门禁主机信息
*@author Jinzi
*@date   2019/09/03 16:21:05
*@param[in]
*@param[out]
*@return
****************************************/
void EntranceGuard::SelectMenJinInfo()
{
	int iIndex = ui.m_listDevInfo->currentIndex().row();
	if (iIndex == -1)
	{
		MessageBoxA(nullptr, "请选择一个门禁主机", "提示", MB_OK | MB_ICONWARNING);
	}
	else
	{
		/*\ 在此处添加查人员信息的代码 \*/
		if (!m_vecLoginInfo.empty())
		{
			m_opCardInfo->dwSize = sizeof(NET_DVR_CARD_CFG_COND);
			m_opCardInfo->dwCardNum = 0xffffffff;
			m_opCardInfo->byCheckCardNo = 1;

			/*\ 登录句柄赋值\*/
			m_opLongConnInfo->m_iLoginHandle = m_vecLoginInfo[iIndex].m_iLoginHandle;
			m_opLongConnInfo->m_iLongConnHandle = this->CreateLongConn(
				m_opLongConnInfo->m_iLoginHandle,
				NET_DVR_GET_CARD_CFG_V50,
				m_opCardInfo,
				sizeof(NET_DVR_CARD_CFG_COND)
			);
			if (m_opLongConnInfo->m_iLongConnHandle == -1)
			{
				int iErr = NET_DVR_GetLastError();
				return;
			}
			m_opLongConnInfo->m_bIsLongConn = true;
			/*\ 设置操作类型 \*/
			gl_dwOptType = NET_DVR_GET_CARD_CFG_V50;
			/*\ 发送获取卡信息的数据 \*/
			//m_opSendDataInfo->dwSize = sizeof(*m_opSendDataInfo);
			////std::string sCardNumber = "123456789";
			////strcpy((char*)m_opSendDataInfo->byCardNo, sCardNumber.c_str());
			//NET_DVR_SendRemoteConfig(
			//	m_opLongConnInfo->m_iLoginHandle,
			//	ENUM_ACS_SEND_DATA,
			//	(char*)m_opSendDataInfo,
			//	sizeof(NET_DVR_CARD_CFG_SEND_DATA)
			//);
			//NET_DVR_StopRemoteConfig(m_opLongConnInfo->m_iLongConnHandle);
		}
		else
		{
			MessageBoxA(nullptr, "请登录门禁主机后再进行该操作", "提示", MB_OK | MB_ICONWARNING);
		}
	}
}

/****************************************!
*@brief  配置远程连接信息的回调函数
*@author Jinzi
*@date   2019/09/04 8:40:07
*@param[in]
   _dwType		:	回调的类型
   _lpBuffer	:	存储数据缓冲区的指针
   _dwBufLen	:	缓冲区的大小
   _pUserData	:	用户数据
*@param[out]
*@return
****************************************/
void RemoteConfigCallBack(DWORD _dwType, void* _lpBuffer,
	DWORD _dwBufLen, void* _pUserData)
{
	switch (_dwType)
	{
		/*\ 回调状态,表示数据全部接受完毕 \*/
	case NET_SDK_CALLBACK_TYPE_STATUS:
		if (*static_cast<int*>(_lpBuffer) == NET_SDK_CALLBACK_STATUS_SUCCESS)
		{
			switch (gl_dwOptType)
			{
				/*\ 获取门禁人员信息 \*/
			case NET_DVR_GET_CARD_CFG_V50:
				/*\ 显示数据 \*/
				if (gl_funcShowUserInfo != nullptr)
				{
					gl_funcShowUserInfo();
				}
				break;
				/*\ 下发门禁人员信息 \*/
			case NET_DVR_SET_CARD_CFG_V50:
				break;


			}

		}
		break;
		/*\ 回调数据 \*/
	case NET_SDK_CALLBACK_TYPE_DATA:
		switch (gl_dwOptType)
		{
			/*\ 获取门禁人员信息 \*/
		case NET_DVR_GET_CARD_CFG_V50:
			NET_DVR_CARD_CFG_V50* opData = new NET_DVR_CARD_CFG_V50();
			opData = static_cast<NET_DVR_CARD_CFG_V50*>(_lpBuffer);
			gl_vecCardInfo.push_back(*opData);
			break;
		}

	}
}

/****************************************!
*@brief  得到设备能力集 （获取卡的参数）
*@author Jinzi
*@date   2019/09/04 8:53:38
*@param[in]
*@param[out]
*@return
****************************************/
void EntranceGuard::GetDevAbilty()
{
	/*\ 先判断是哪个设备 \*/
	int iIndex = ui.m_listDevInfo->currentIndex().row();
	if (!m_vecLoginInfo.empty())
	{
		MessageBoxA(nullptr, "未开放", "提示", MB_OK);
	}
	else
	{
		MessageBoxA(nullptr, "请先选择一个设备", "提示", MB_OK | MB_ICONWARNING);
	}
}

/****************************************!
*@brief  清除一个门禁上的所有用户信息
*@author Jinzi
*@date   2019/09/04 9:40:14
*@param[in]
*@param[out]
*@return
****************************************/
void EntranceGuard::DelMenJinOnAllUserInfo()
{
	/*\ 判断是哪个门禁 \*/
	int iIndex = ui.m_listDevInfo->currentIndex().row();
	if (iIndex == -1)
	{
		MessageBoxA(nullptr, "请选择一个门禁", "提示", MB_OK | MB_ICONWARNING);
	}
	else
	{
		std::string sStart = "确定要清空门禁: ";
		std::string sEnd = " 的所有用户信息吗?";
		std::string sInfo = sStart + m_vecLoginInfo[iIndex].m_sNodeName.toLocal8Bit().data() + sEnd;
		int iBtnType = MessageBoxA(nullptr, sInfo.c_str(), "提示", MB_OKCANCEL);
		/*\ 1.表示用户点击了确认 \*/
		if (iBtnType == 1)
		{
			if (m_vecLoginInfo.empty())
			{
				MessageBoxA(nullptr, "请先登录门禁主机", "提示", MB_OK | MB_ICONWARNING);
				return;
			}
			NET_DVR_ACS_PARAM_TYPE* oClearMenJinData = new NET_DVR_ACS_PARAM_TYPE();
			oClearMenJinData->dwSize = sizeof(NET_DVR_ACS_PARAM_TYPE);
			oClearMenJinData->wLocalControllerID = 0;
			oClearMenJinData->dwParamType = 0x0003ffff;

			if (!NET_DVR_RemoteControl(
				m_vecLoginInfo[iIndex].m_iLoginHandle,
				NET_DVR_CLEAR_ACS_PARAM,
				oClearMenJinData,
				sizeof(NET_DVR_ACS_PARAM_TYPE)
			))
			{
				MessageBoxA(nullptr, "清空门禁数据失败", "提示", MB_OK | MB_ICONWARNING);
				int iError = NET_DVR_GetLastError();
				if (oClearMenJinData != nullptr)
				{
					delete oClearMenJinData;
					oClearMenJinData = nullptr;
				}
				return;
			}
			else
			{
				MessageBoxA(nullptr, "清空门禁数据成功", "提示", MB_OK);
				if (m_opLongConnInfo != nullptr)
				{
					delete oClearMenJinData;
					oClearMenJinData = nullptr;
				}
			}
		}
	}
}

/****************************************!
*@brief  判断是否可以显示添加用户窗口
*@author Jinzi
*@date   2019/09/10 11:01:53
*@param[in]
*@param[out]
*@return
****************************************/
void EntranceGuard::JuageShowAddUserFm()
{
	if (m_vecLoginInfo.size() > 0)
	{
		m_oFmAddUser->show();
	}
	else
	{
		MessageBoxA(nullptr, "请先登录门禁主机", "提示", MB_OK | MB_ICONWARNING);
	}
}

/****************************************!
*@brief  建立长连接
*@author Jinzi
*@date   2019/09/16 13:12:14
*@param[in]
   _iLoginHandle	:	登录句柄
   _uiType			:	连接类型
   _lpBuffer		:	根据类型的不同，数据结构也不同
   _dwLength		:	传入的数据的长度
   _lpUserData		:	用户数据，默认为null
*@param[out]
*@return     失败返回-1,成功返回连接句柄
****************************************/
int EntranceGuard::CreateLongConn(int _iLoginHandle, DWORD _uiType,
	LPVOID _lpBuffer, DWORD _dwLength, LPVOID _lpUserData)
{
	int iConnHandle = -1;
	iConnHandle = NET_DVR_StartRemoteConfig(_iLoginHandle, _uiType, _lpBuffer,
		_dwLength, m_funcMsgRemoteConfig, _lpUserData);
	return iConnHandle;
}

/****************************************!
*@brief  关闭长连接
*@author Jinzi
*@date   2019/09/16 13:18:33
*@param[in]
   _iLongConnHandle	:	长连接句柄
*@param[out]
*@return     成功返回true，失败返回false
****************************************/
bool EntranceGuard::CloseLongConn(int _iLongConnHandle)
{
	bool bIsSucc = false;
	if (NET_DVR_StopRemoteConfig(_iLongConnHandle))
	{
		bIsSucc = true;
	}
	return bIsSucc;
}

/****************************************!
*@brief  显示门禁主机的用户信息
*@author Jinzi
*@date   2019/09/16 14:55:57
*@param[in]
*@param[out]
*@return
****************************************/
void EntranceGuard::ShowMenJinUserInfo()
{
	if (gl_vecCardInfo.size() == 0)
	{
		return;
	}
	QStandardItemModel* opModelRet = (QStandardItemModel*)ui.m_tbUserInfo->model();
	if (opModelRet != nullptr)
	{
		opModelRet->clear();
	}
	QStandardItemModel* opModel = new QStandardItemModel();
	opModel->setColumnCount(5);
	opModel->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("卡号"));
	opModel->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("卡类型"));
	opModel->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("姓名"));
	opModel->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("用户类型"));
	opModel->setHeaderData(4, Qt::Horizontal, QString::fromLocal8Bit("工号"));
	/*\ 设置表的属性 \*/
	ui.m_tbUserInfo->setModel(opModel);
	/*\ 设置左对齐 \*/
	ui.m_tbUserInfo->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	//表列随着表格变化而自适应变化
	ui.m_tbUserInfo->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);
	//表行随着表格变化而自适应变化
	ui.m_tbUserInfo->verticalHeader()->sectionResizeMode(QHeaderView::Stretch);
	for (int i = 0; i < gl_vecCardInfo.size(); i++)
	{
		/*\ 卡号 \*/
		opModel->setItem(i, 0, new QStandardItem((const char*)gl_vecCardInfo[i].byCardNo));
		/*\ 卡类型 \*/
		//卡类型：1- 普通卡（默认），2- 残疾人卡，3- 黑名单卡，4- 巡更卡，5- 胁迫卡，6- 超级卡，7- 来宾卡，8- 解除卡，
		//9- 员工卡，10- 应急卡，11- 应急管理卡（用于授权临时卡权限，本身不能开门），默认普通卡 
		switch (gl_vecCardInfo[i].byCardType)
		{
		case 1:
			opModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("普通卡")));
			break;
		case 2:
			opModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("残疾人卡")));
			break;
		case 3:
			opModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("黑名单卡")));
			break;
		case 4:
			opModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("巡更卡")));
			break;
		case 5:
			opModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("胁迫卡")));
			break;
		case 6:
			opModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("超级卡")));
			break;
		case 7:
			opModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("来宾卡")));
			break;
		case 8:
			opModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("解除卡")));
			break;
		case 9:
			opModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("员工卡")));
			break;
		case 10:
			opModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("应急卡")));
			break;
		case 11:
			opModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("应急管理卡")));
			break;
		}
		/*\ 姓名 \*/
		opModel->setItem(i, 2, new QStandardItem(QString::fromLocal8Bit((const char*)gl_vecCardInfo[i].byName)));
		/*\ 用户类型 \*/
		switch (gl_vecCardInfo[i].byUserType)
		{
		case 0://普通
			opModel->setItem(i, 3, new QStandardItem(QString::fromLocal8Bit("普通用户")));
			break;
		case 1://管理员
			opModel->setItem(i, 3, new QStandardItem(QString::fromLocal8Bit("管理员")));
			break;
		}
		char chbuf[16] = { 0 };
		_ultoa((unsigned long)gl_vecCardInfo[i].dwEmployeeNo, chbuf, 10);
		/*\ 工号 \*/
		opModel->setItem(i, 4, new QStandardItem(chbuf));
		//for (int j = 0; j < 5; j++)
		//{
		//	//设置字符位置 
		//	opModel->item(i, j)->setTextAlignment(Qt::AlignCenter);
		//}
		//设置字符颜色 
		opModel->item(i, 0)->setForeground(QBrush(QColor(255, 0, 0)));
	}
	this->SetLabelShowInfo("查询成功");
	/*\ 初始化m_opLongConnInfo \*/
	InitSaveLongInfo();
	/*\ 将gl_vecCardInfo结构体清空 \*/
	gl_vecCardInfo.clear();
	this->CloseLongConn(m_opLongConnInfo->m_iLongConnHandle);
}

/****************************************!
*@brief  显示所有用户信息
*@author Jinzi
*@date   2019/09/18 13:24:35
*@param[in]
*@param[out]
*@return
****************************************/
void EntranceGuard::ShowAllUserInfoWithListView()
{
	std::string sUrl = "http://" + gl_opSaveIniInfo->sIp + ":" + gl_opSaveIniInfo->sPort + "/patroluser/getPatrolUserList";
	QString qsUrl = sUrl.c_str();
	//QString qsReqData = "{\"user\":\"jinzi\"}";
	QNetworkRequest oRequestInfo;
	/*\ 设置请求url \*/
	oRequestInfo.setUrl(QUrl(qsUrl));
	/*\ 设置请求头信息 \*/
	oRequestInfo.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
	/*\ 设置要发送的数据json \*/
	QByteArray oReqData;
	/*\ 发送post请求 \*/
	QNetworkReply* pReply = m_opHttpManager->post(oRequestInfo, oReqData);
	/*\ 使用同步的方式进行等待服务器回调数据 \*/
	QEventLoop oLoop;
	QObject::connect(pReply, SIGNAL(finished()), &oLoop, SLOT(quit()));
	oLoop.exec();
	this->HttpGetAllUserSuccDataHandle(pReply);
	/*\ 异步方式等待服务器回调 \*/
	//QObject::connect(pReply, SIGNAL(finished()), this, SLOT(HttpGetAllUserSuccDataHandle()));
}

/****************************************!
*@brief  下发门禁信息
*@author Jinzi
*@date   2019/09/16 16:19:05
*@param[in]
*@param[out]
*@return
****************************************/
void EntranceGuard::BeSentDownMenJin()
{
	MessageBoxA(nullptr, "下发", "提示", MB_OK);
}

/****************************************!
*@brief  处理获取所有用户信息后的返回数据
*@author Jinzi
*@date   2019/09/18 14:13:21
*@param[in]
*@param[out]
*@return
****************************************/
void EntranceGuard::HttpGetAllUserSuccDataHandle(QNetworkReply* _opReply)
{
	//QNetworkReply* pReplay = qobject_cast<QNetworkReply*>(sender());
	if (_opReply == nullptr)
	{
		MessageBoxA(nullptr, "服务器错误", "提示", MB_OK | MB_ICONWARNING);
		return;
	}
	// 保存接受的数据;
	QByteArray replyContent = _opReply->readAll();
	QJsonObject jsonResData = QJsonDocument::fromJson(replyContent).object();
	if (jsonResData.value("code").toString() == 0)
	{
		QJsonObject::iterator it = jsonResData.find("data");
		QJsonValueRef arrData = it.value();
		QVariantList jsonList = arrData.toArray().toVariantList();
		if (!m_vecSvrRetAllUserInfo.empty())
		{
			m_vecSvrRetAllUserInfo.clear();
		}
		/*\ 将用户信息存储起来 \*/
		for (int i = 0; i < jsonList.count(); i++)
		{
			QVariantMap map = jsonList[i].toMap();
			CSvrRetAllUserInfo* opSvrInfo = new CSvrRetAllUserInfo();
			opSvrInfo->m_qsId = map.value("id").toString();
			opSvrInfo->m_qsUserName = map.value("userName").toString();
			opSvrInfo->m_qsJobNumber = map.value("jobNumber").toString();
			opSvrInfo->m_qsCardNumber = map.value("cardNumber").toString();
			m_vecSvrRetAllUserInfo.push_back(*opSvrInfo);
		}
	}
}

/****************************************!
*@brief  删除门禁中指定人员的信息
*@author Jinzi
*@date   2019/09/20 15:14:48
*@param[in]
*@param[out]
*@return
****************************************/
void EntranceGuard::DelUserInfoWithMenJin()
{
	int iIndex = ui.m_tbUserInfo->currentIndex().row();
	if (m_vecLoginInfo.empty())
	{
		MessageBoxA(nullptr, "请先登录门禁主机", "提示", MB_OK | MB_ICONWARNING);
		return;
	}
	if (iIndex == -1)
	{
		MessageBoxA(nullptr, "请先查询用户信息", "提示", MB_OK | MB_ICONWARNING);
		return;
	}

	/*\ 用来获取所有的用户数据,并保存到m_vecSvrRetAllUserInfo中 \*/
	this->ShowAllUserInfoWithListView();

	QAbstractItemModel* oAbModel = ui.m_tbUserInfo->model();
	QModelIndex ModelIndex = oAbModel->index(iIndex, 0);
	/*\ 卡号 \*/
	QString qsCardNumber = oAbModel->data(ModelIndex).toString();

	/*\ 得到工号对应的用户id \*/
	QString qsUserId = RetUserIdWithJobNum(qsCardNumber);
	/*\ 判断是否寻找到了该id \*/
	if (qsUserId == "")
	{
		MessageBoxA(nullptr, "该用户不存在", "提示", MB_OK | MB_ICONWARNING);
		return;
	}
	/*\ 建立连接 \*/
	std::string sUrl = "http://" + gl_opSaveIniInfo->sIp + ":" + gl_opSaveIniInfo->sPort + "/patroluser/deletePatrolUser";
	QString qsUrl = sUrl.c_str();
	/*\ http 请求发送的数据 \*/
	QJsonObject jsonReqData;
	jsonReqData.insert("id", qsUserId);
	QJsonDocument jsonDocument;
	jsonDocument.setObject(jsonReqData);
	QNetworkRequest oRequestInfo;
	/*\ 设置请求url \*/
	oRequestInfo.setUrl(QUrl(qsUrl));
	/*\ 设置请求头信息 \*/
	oRequestInfo.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
	/*\ 设置要发送的数据json \*/
	QByteArray oReqData(jsonDocument.toJson());
	/*\ 发送post请求 \*/
	QNetworkReply* pReply = m_opHttpManager->post(oRequestInfo, oReqData);
	/*\ 设置数据处理函数 \*/
	QObject::connect(pReply, SIGNAL(finished()), this, SLOT(HttpDelUserInfoSuccDataHandle()));
}

/****************************************!
*@brief  根据工号返回用户id
*@author Jinzi
*@date   2019/09/20 16:49:02
*@param[in]
   _qsCardNumber	:	卡号
*@param[out]
*@return
****************************************/
QString EntranceGuard::RetUserIdWithJobNum(QString _qsCardNumber)
{
	if (_qsCardNumber.count() == 0)
	{
		return "";
	}
	if (m_vecSvrRetAllUserInfo.size() == 0)
	{
		return "";
	}
	for (int i = 0; i < m_vecSvrRetAllUserInfo.size(); i++)
	{
		if (_qsCardNumber == m_vecSvrRetAllUserInfo[i].m_qsCardNumber)
		{
			return m_vecSvrRetAllUserInfo[i].m_qsId;
		}
	}
	return "";
}

/****************************************!
*@brief  删除用户服务器返回的数据处理函数
*@author Jinzi
*@date   2019/09/20 16:59:52
*@param[in]
*@param[out]
*@return
****************************************/
void EntranceGuard::HttpDelUserInfoSuccDataHandle()
{
	QNetworkReply* pReplay = qobject_cast<QNetworkReply*>(sender());
	// 保存接受的数据;
	QByteArray replyContent = pReplay->readAll();
	/*\ 将字符数组转换为json对象 \*/
	QJsonObject jsonResData = QJsonDocument::fromJson(replyContent).object();
	/*\ 说明在服务器上 删除成功 \*/
	if (jsonResData.value("code") == 0)
	{
		/*\ 删除一个门禁用户信息（在门禁主机上） \*/
		this->DelMenJinUserInfo();
	}
	else
	{
		MessageBoxA(nullptr, "删除该用户失败", "提示", MB_OK | MB_ICONWARNING);
	}
}

/****************************************!
*@brief  删除一个用户门禁信息
*@author Jinzi
*@date   2019/09/23 8:39:22
*@param[in]
*@param[out]
*@return
****************************************/
void EntranceGuard::DelMenJinUserInfo()
{
	int iIndex = ui.m_listDevInfo->currentIndex().row();
	if (m_vecLoginInfo.empty())
	{
		MessageBoxA(nullptr, "请先登录门禁主机", "提示", MB_OK | MB_ICONWARNING);
		return;
	}
	if (iIndex == -1)
	{
		MessageBoxA(nullptr, "请先查询用户信息", "提示", MB_OK | MB_ICONWARNING);
		return;
	}
	InitSaveLongInfo();

	m_opLongConnInfo->m_iLoginHandle = m_vecLoginInfo[iIndex].m_iLoginHandle;
	/*\ 在门禁主机中删除用户（建立长连接） \*/
	NET_DVR_CARD_CFG_COND* oCardInfo = new NET_DVR_CARD_CFG_COND();
	/*\ 发送长连接数据 \*/
	NET_DVR_CARD_CFG_V50* oDelCardInfo = new NET_DVR_CARD_CFG_V50();

	oCardInfo->dwSize = sizeof(NET_DVR_CARD_CFG_COND);
	oCardInfo->dwCardNum = 1;
	oCardInfo->byCheckCardNo = 1;
	m_opLongConnInfo->m_iLongConnHandle = NET_DVR_StartRemoteConfig(
		m_opLongConnInfo->m_iLoginHandle,
		NET_DVR_SET_CARD_CFG_V50,
		oCardInfo,
		sizeof(NET_DVR_CARD_CFG_COND),
		(fRemoteConfigCallback)RemoteConfigCallBack,
		nullptr
	);
	if (m_opLongConnInfo->m_iLongConnHandle == -1)
	{
		MessageBoxA(nullptr, "删除用户失败", "提示", MB_OK | MB_ICONWARNING);
		goto Free;
		return;
	}
	///////////////////// 2019/09/23 8:45:12 执行删除操作 ==> BEGIN /////////////////////////////////////////////////////
	oDelCardInfo->dwSize = sizeof(NET_DVR_CARD_CFG_V50);
	oDelCardInfo->dwModifyParamType = 0x00000001; //卡是否有效
	oDelCardInfo->byCardValid = 0;//设置为0表示删除

	///////////////////// 2019/09/29 16:16:51 出现错误，内存的错误 ==> BEGIN /////////////////////////////////////////////////////
	if (!NET_DVR_SendRemoteConfig(
		m_opLongConnInfo->m_iLongConnHandle,
		ENUM_ACS_SEND_DATA,
		(char*)oDelCardInfo,
		sizeof(NET_DVR_CARD_CFG_V50)))
	{
		int iError = NET_DVR_GetLastError();
		MessageBoxA(nullptr, "删除用户失败", "提示", MB_OK | MB_ICONWARNING);
		goto Free;
		return;
	}
	else
	{
		int iError = NET_DVR_GetLastError();
		MessageBoxA(nullptr, "删除用户成功", "提示", MB_OK | MB_ICONWARNING);
		NET_DVR_StopRemoteConfig(m_opLongConnInfo->m_iLongConnHandle);
		goto Free;
		return;
	}
	///////////////////// 2019/09/29 16:16:51 出现错误，内存的错误 ==> END /////////////////////////////////////////////////////

	///////////////////// 2019/09/23 8:45:12 执行删除操作 ==> END /////////////////////////////////////////////////////
Free:
	if (oCardInfo != nullptr)
	{
		delete oCardInfo;
		oCardInfo = nullptr;
	}
	if (oDelCardInfo != nullptr)
	{
		delete oDelCardInfo;
		oDelCardInfo = nullptr;
	}
}

/****************************************!
*@brief  显示修改用户信息的窗体
*@author Jinzi
*@date   2019/09/23 14:34:39
*@param[in]
*@param[out]
*@return
****************************************/
void EntranceGuard::ShowFmChangeUserInfo()
{
	int iIndex = ui.m_listDevInfo->currentIndex().row();
	int iTbIndex = ui.m_tbUserInfo->currentIndex().row();
	if (iIndex == -1)
	{
		MessageBoxA(nullptr, "请先登录门禁主机", "提示", MB_OK | MB_ICONWARNING);
		return;
	}
	if (iTbIndex == -1)
	{
		MessageBoxA(nullptr, "请先查询卡号", "提示", MB_OK | MB_ICONWARNING);
		return;
	}
	QAbstractItemModel* oAbModel = ui.m_tbUserInfo->model();
	QModelIndex ModelCardNum = oAbModel->index(iTbIndex, 0);
	/*\ 工号 \*/
	QString qsCardNumber = oAbModel->data(ModelCardNum).toString();
	std::string sUrl = "http://" + gl_opSaveIniInfo->sIp + ":" + gl_opSaveIniInfo->sPort + "/patroluser/getPatrolUserByCardNum";
	QJsonObject jsonReqData;
	jsonReqData.insert("cardNumber", qsCardNumber);
	QJsonDocument jsonDocument(jsonReqData);
	//jsonDocument.setObject(jsonReqData);
	QByteArray reqData(jsonDocument.toJson());
	/*\ 根据工号获得用户的id \*/
	m_opHttpRequest->HttpPostRequest(sUrl.c_str(), reqData,
		std::bind(&EntranceGuard::JuageJobNumResFuncHandle, this, std::placeholders::_1));
}

/****************************************!
*@brief  判断工号是否存在的响应处理函数
*@author Jinzi
*@date   2019/09/24 8:48:35
*@param[in]
*@param[out]
*@return
****************************************/
void EntranceGuard::JuageJobNumResFuncHandle(QNetworkReply* _opReqplay)
{
	if (_opReqplay == nullptr)
	{
		MessageBoxA(nullptr, "服务器响应数据为空", "提示", MB_OK | MB_ICONWARNING);
		return;
	}
	// 保存接受的数据;（图片名称imgName）
	QByteArray replyContent = _opReqplay->readAll();
	QJsonObject jsonResData = QJsonDocument::fromJson(replyContent).object();
	/*\ 用来赋值 \*/
	int iTbIndex = ui.m_tbUserInfo->currentIndex().row();
	int iIndex = ui.m_listDevInfo->currentIndex().row();
	QAbstractItemModel* oAbModel = ui.m_tbUserInfo->model();
	QModelIndex ModelJobNum = oAbModel->index(iTbIndex, 4);
	QModelIndex ModelCardNum = oAbModel->index(iTbIndex, 0);
	/*\ 工号 \*/
	QString qsJobNumber = oAbModel->data(ModelJobNum).toString();
	/*\ 卡号(返回的是一个字符串) \*/
	QString qsCardNumber = oAbModel->data(ModelCardNum).toString();
	/*\ 判断是否回调成功数据\*/
	if (jsonResData.value("code") == 0)
	{
		m_opFmChangeUser->fmChangeUserInit();
		/*\ 显示窗体 \*/
		m_opFmChangeUser->show();
		CSelMenJinInfo* opSelMenJinInfo = new CSelMenJinInfo();
		opSelMenJinInfo->m_qsUserId = jsonResData.value("id").toString();;
		opSelMenJinInfo->m_qsJobNum = qsJobNumber;
		opSelMenJinInfo->m_qsCardNum = qsCardNumber;
		opSelMenJinInfo->m_iSelMenJinHandle = m_vecLoginInfo[iIndex].m_iLoginHandle;
		m_opFmChangeUser->SetSelMenJinInfo(*opSelMenJinInfo);
		if (opSelMenJinInfo != nullptr)
		{
			delete opSelMenJinInfo;
			opSelMenJinInfo = nullptr;
		}
	}
	else
	{
		MessageBoxA(nullptr, "该用户不存在", "提示", MB_OK | MB_ICONWARNING);
		return;
	}
}

/****************************************!
*@brief  返回用户选中行对应的工号（listView）
*@author Jinzi
*@date   2019/09/24 11:23:19
*@param[in]
*@param[out]
*@return     返回用户选中行 失败返回-1
****************************************/
QString EntranceGuard::RetUserSelRowJobNum()
{
	int iIndex = -1;
	iIndex = ui.m_tbUserInfo->currentIndex().row();
	QAbstractItemModel* oAbModel = ui.m_tbUserInfo->model();
	QModelIndex ModelJobNum = oAbModel->index(iIndex, 4);
	/*\ 工号 \*/
	QString qsJobNumber = oAbModel->data(ModelJobNum).toString();
	return qsJobNumber;
}

/****************************************!
*@brief  返回用户选择的门禁登录句柄
*@author Jinzi
*@date   2019/09/24 11:32:00
*@param[in]
*@param[out]
*@return     失败返回-1 成功返回句柄
****************************************/
int EntranceGuard::RetUserSelMenJinLoginHandle()
{
	int iIndex = -1;
	int iSelIndex = ui.m_listDevInfo->currentIndex().row();
	if (iSelIndex != -1)
	{
		iIndex = m_vecLoginInfo[iSelIndex].m_iLoginHandle;
	}
	return iIndex;
}

 /****************************************!
 *@brief  返回用户选中卡号
 *@author Jinzi
 *@date   2019/10/10 11:34:10
 *@param[in]  
 *@param[out] 
 *@return     
 ****************************************/
QString EntranceGuard::RetUserSelRowCardNum()
{
	int iIndex = -1;
	iIndex = ui.m_tbUserInfo->currentIndex().row();
	QAbstractItemModel* oAbModel = ui.m_tbUserInfo->model();
	QModelIndex ModelCardNum = oAbModel->index(iIndex, 0);
	/*\ 工号 \*/
	QString qsCardNumber = oAbModel->data(ModelCardNum).toString();
	return qsCardNumber;
}