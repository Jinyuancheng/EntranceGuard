#include "fmAddUser.h"
#include <QDateTimeEdit>
#include <QNetworkRequest>
#include <QString>
#include <QByteArray>
#include <windows.h>
#include <QDesktopServices>
#include <QFileDialog>
#include <QBuffer>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QRegExp>
#include <QFile>
#include <QDataStream>
#include <QDir>

#ifndef _JSON_H_
#include "../Utils/json.h"
#endif

#ifndef _UTILS_H_
#include "../Utils/utils.h"
#endif

#ifndef _HC_NET_SDK_H_
#include "../HIKSDK/HCNetSDK.h"
#endif


/*\ 引用外部的配置文件信息 \*/
extern CSaveiniInfo* gl_opSaveIniInfo;

/****************************************!
*@brief  回调函数
*@author Jinzi
*@date   2019/09/18 10:09:39
*@param[in]
*@param[out]
*@return
****************************************/
void AddUserRemoteConfigCallBack(DWORD _dwType, void* _lpBuffer, DWORD _dwBufLen, void* _pUserData)
{

}

fmAddUser::fmAddUser(QWidget* parent)
{
	m_oUi.setupUi(this);
	/*\ 设置当前窗口后面的窗口不能点击 \*/
	setWindowModality(Qt::ApplicationModal);
	/*\ 隐藏最大化最小化按钮 \*/
	setWindowFlags(this->windowFlags() & ~(Qt::WindowMinMaxButtonsHint));
	/*\ 绑定信号和槽 \*/
	this->BindSignalAndSlot();
	/*\ 初始化控件信息 \*/
	this->InitControlInfo();
	/*\ 初始化成员变量信息 \*/
	this->InitMemberVar();
	/*\ 读取配置文件 \*/
	//this->ReadIniFile("./Config/config.ini");
}


fmAddUser::~fmAddUser()
{
	if (m_opHttpManager != nullptr)
	{
		delete m_opHttpManager;
		m_opHttpManager = nullptr;
	}
	if (m_opAddUserInfo != nullptr)
	{
		delete m_opAddUserInfo;
		m_opAddUserInfo = nullptr;
	}
	if (m_opLongConnStatus != nullptr)
	{
		delete m_opLongConnStatus;
		m_opLongConnStatus = nullptr;
	}
}

/****************************************!
*@brief  绑定信号和槽
*@author Jinzi
*@date   2019/09/16 9:21:20
*@param[in]
*@param[out]
*@return
****************************************/
void fmAddUser::BindSignalAndSlot()
{
	/*\ 添加按钮点击事件 \*/
	connect(m_oUi.m_btnAdd, SIGNAL(clicked()), this, SLOT(BtnAddUserInfo()));
	/*\ 返回按钮点击事件 \*/
	connect(m_oUi.m_btnQuit, SIGNAL(clicked()), this, SLOT(close()));
	/*\ 选择图片路径的点击事件 \*/
	connect(m_oUi.m_btnPicPath, SIGNAL(clicked()), this, SLOT(OpenFloderPath()));
}

/****************************************!
*@brief  添加用户信息按钮点击事件
*@author Jinzi
*@date   2019/09/16 17:39:00
*@param[in]
*@param[out]
*@return
****************************************/
void fmAddUser::BtnAddUserInfo()
{
	/*\ 判断用户是否所有edit都编辑了 \*/
	if (m_oUi.m_editCardNum->text() == "" ||
		m_oUi.m_editCardPass->text() == "" ||
		m_oUi.m_editUserName->text() == "" ||
		m_oUi.m_editUserNum->text() == "" ||
		m_oUi.m_linePath->text() == "")
	{
		MessageBoxA(nullptr, "请输入必要字段", "提示", MB_OK | MB_ICONWARNING);
		return;
	}
	else
	{
		/*\ 判断用户输入是否合法 \*/
		if (!CUtils::GetInstance()->JuageNumberLegal(m_oUi.m_editUserNum->text()))
		{
			MessageBoxA(nullptr, "请输入合法的工号0-999999", "提示", MB_OK | MB_ICONWARNING);
			return;
		}
		/*\ 判断图片路径是否合法 \*/
		if (!CUtils::GetInstance()->JuageFilePathLegal(m_oUi.m_linePath->text()))
		{
			MessageBoxA(nullptr, "请输入合法的文件路径", "提示", MB_OK | MB_ICONWARNING);
			return;
		}
		/*\ 得到用户输入数据 \*/
		m_opAddUserInfo->m_qsCardNum = m_oUi.m_editCardNum->text();
		m_opAddUserInfo->m_qsCardPass = m_oUi.m_editCardPass->text();
		m_opAddUserInfo->m_qsUserName = m_oUi.m_editUserName->text();
		m_opAddUserInfo->m_qsUserNum = m_oUi.m_editUserNum->text();
		m_opAddUserInfo->m_qsPicPath = m_oUi.m_linePath->text();
		/*\ 图片的数据 \*/
		QByteArray oPicBase64 = ImageToBase64(m_opAddUserInfo->m_qsPicPath);
		/*\ 进行http请求 \*/
		std::string sUrl = "http://" + gl_opSaveIniInfo->sIp + ":" + gl_opSaveIniInfo->sPort + "/patroluser/addPatrolUser";
		QString qsUrl = sUrl.c_str();

		/*\ 组织json \*/
		QString qsReqData = "{\"file\":\"" + oPicBase64 + "\"" + ","
			+ "\"userName\":\"" + m_opAddUserInfo->m_qsUserName + "\"" + ","
			+ "\"faceTrait\":\"\"" + ","
			+ "\"cardNumber\":\"" + m_opAddUserInfo->m_qsCardNum + "\"" + ","
			+ "\"jobNumber\":\"" + m_opAddUserInfo->m_qsUserName + "\"" +
			"}";
		//QString qsReqData = "{\"user\":\"jinzi\"}";
		QNetworkRequest oRequestInfo;
		/*\ 设置请求url \*/
		oRequestInfo.setUrl(QUrl(qsUrl));
		/*\ 设置请求头信息 \*/
		oRequestInfo.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
		/*\ 设置要发送的数据json \*/
		QByteArray oReqData(qsReqData.toLocal8Bit().data());
		/*\ 发送post请求 \*/
		QNetworkReply* pReply = m_opHttpManager->post(oRequestInfo, oReqData);
		QObject::connect(pReply, SIGNAL(finished()), this, SLOT(HttpAddUserRetDataHandle()));
	}
}

/****************************************!
*@brief  初始化控件信息
*@author Jinzi
*@date   2019/09/16 17:41:15
*@param[in]
*@param[out]
*@return
****************************************/
void fmAddUser::InitControlInfo()
{

}

/****************************************!
*@brief  初始化成员变量
*@author Jinzi
*@date   2019/09/17 8:26:49
*@param[in]
*@param[out]
*@return
****************************************/
void fmAddUser::InitMemberVar()
{
	m_opHttpManager = new QNetworkAccessManager(this);
	m_opAddUserInfo = new CAddUserInfo();
	m_opLongConnStatus = new CLongConnStatus();
	/*\ 初始化m_opLongConnStatus \*/
	this->InitLongConnStatus();
}

/****************************************!
*@brief  读取配置文件
*@author Jinzi
*@date   2019/09/17 8:30:42
*@param[in]
*@param[out]
*@return
****************************************/
void fmAddUser::ReadIniFile(std::string _sFilePath)
{
	boost::property_tree::ptree oParentPtree;
	read_ini(_sFilePath, oParentPtree);
	gl_opSaveIniInfo->sIp = oParentPtree.get<std::string>("IP.sIp");
	gl_opSaveIniInfo->sPort = oParentPtree.get<std::string>("PORT.sPort");
}

/****************************************!
*@brief  打开文件路径
*@author Jinzi
*@date   2019/09/17 9:38:27
*@param[in]
*@param[out]
*@return
****************************************/
void fmAddUser::OpenFloderPath()
{
	//定义文件对话框类
	QFileDialog* fileDialog = new QFileDialog(this);
	//定义文件对话框标题
	fileDialog->setWindowTitle(QString::fromLocal8Bit("打开图片"));
	//设置默认文件路径
	fileDialog->setDirectory(".");
	//设置文件过滤器
	fileDialog->setNameFilter(tr("Images(*.png *.jpg *.jpeg *.bmp)"));
	//设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	//设置视图模式
	fileDialog->setViewMode(QFileDialog::Detail);
	//打印所有选择的文件的路径
	QStringList fileNames;
	if (fileDialog->exec())
	{
		fileNames = fileDialog->selectedFiles();
	}
	/*\ 设置图片路径 \*/
	if (fileNames[0] != nullptr)
	{
		m_oUi.m_linePath->setText(fileNames[0]);
	}
}

/****************************************!
*@brief  图片进行base64编码
*@author Jinzi
*@date   2019/09/17 10:11:32
*@param[in]
   _qsPicPath		:	图片路径
*@param[out]
*@return     返回base64编码后的图片数据
****************************************/
QByteArray fmAddUser::ImageToBase64(QString _qsPicPath)
{
	QImage image(_qsPicPath);
	QByteArray oByteArray;
	QBuffer oBuf(&oByteArray);
	image.save(&oBuf, "PNG", 20);
	QByteArray oPicBase64 = oByteArray.toBase64();
	oBuf.close();
	return oPicBase64;
}

/****************************************!
*@brief  http请求添加用户的回调数据处理函数
*@author Jinzi
*@date   2019/09/17 11:35:59
*@param[in]
*@param[out]
*@return
****************************************/
void fmAddUser::HttpAddUserRetDataHandle()
{
	QNetworkReply* pReplay = qobject_cast<QNetworkReply*>(sender());
	// 保存接受的数据;
	QByteArray replyContent = pReplay->readAll();
	QJsonObject jsonResData = QJsonDocument::fromJson(replyContent).object();
	if (jsonResData.value("code").toString() == 0)
	{
		/*\ 执行添加卡的信息 \*/
		AddUserInfoToMenJin();
	}
	else
	{
		MessageBoxA(nullptr, "添加用户失败", "提示", MB_OK | MB_ICONWARNING);
	}
}

/****************************************!
*@brief  向门禁中添加卡的信息
*@author Jinzi
*@date   2019/09/17 14:54:18
*@param[in]
*@param[out]
*@return
****************************************/
void fmAddUser::AddUserInfoToMenJin()
{
	int iIndex = -1;
	if (m_funcGetListViewCheck != nullptr)
	{
		iIndex = m_funcGetListViewCheck();
		if (iIndex == -1)
		{
			MessageBoxA(nullptr, "请选择门禁主机", "提示", MB_OK | MB_ICONWARNING);
			this->close();
			return;
		}
	}
	/*\ 判断是否有门禁主机登录 \*/
	if (m_vecLoginInfo.empty())
	{
		MessageBoxA(nullptr, "请先登录门禁主机", "提示", MB_OK | MB_ICONWARNING);
		this->close();
		return;
	}
	m_opLongConnStatus->m_iLoginHandle = m_vecLoginInfo[iIndex].m_iLoginHandle;
	NET_DVR_CARD_CFG_COND* opInputBuf = new NET_DVR_CARD_CFG_COND();
	opInputBuf->dwSize = sizeof(*opInputBuf);
	opInputBuf->dwCardNum = 1;
	opInputBuf->byCheckCardNo = 1;
	opInputBuf->wLocalControllerID = 0; /*\ 0表示门禁主机 \*/
	/*\ 建立长连接 \*/
	m_opLongConnStatus->m_iLongConnHandle = NET_DVR_StartRemoteConfig(
		m_opLongConnStatus->m_iLoginHandle,
		NET_DVR_SET_CARD_CFG_V50,
		opInputBuf,
		static_cast<DWORD>(sizeof(*opInputBuf)),
		(fRemoteConfigCallback)AddUserRemoteConfigCallBack,
		nullptr
	);
	if (m_opLongConnStatus->m_iLongConnHandle == -1)
	{
		this->InitLongConnStatus();
		goto Free;
		return;
	}
	m_opLongConnStatus->m_bIsLongConn = true;
	this->SendCardInfoToMenJin();
Free:
	if (opInputBuf != nullptr)
	{
		delete opInputBuf;
		opInputBuf = nullptr;
	}
}

/****************************************!
*@brief  设置登录信息
*@author Jinzi
*@date   2019/09/17 15:07:57
*@param[in]
   _vecLoginInfo	:	所有门禁主机登录信息
*@param[out]
*@return
****************************************/
void fmAddUser::SetLoginInfo(std::vector<CLoginInfo>& _vecLoginInfo)
{
	if (!m_vecLoginInfo.empty())
	{
		m_vecLoginInfo.clear();
	}
	m_vecLoginInfo = _vecLoginInfo;
}

/****************************************!
*@brief  初始化长连接状态
*@author Jinzi
*@date   2019/09/17 15:28:32
*@param[in]
*@param[out]
*@return
****************************************/
void fmAddUser::InitLongConnStatus()
{
	m_opLongConnStatus->m_bIsLongConn = false;
	m_opLongConnStatus->m_iLoginHandle = -1;
	m_opLongConnStatus->m_iLongConnHandle = -1;
}

/****************************************!
*@brief  发送(下发)卡信息
*@author Jinzi
*@date   2019/09/17 16:27:10
*@param[in]
*@param[out]
*@return
****************************************/
void fmAddUser::SendCardInfoToMenJin()
{
	int iCardType = m_oUi.m_cbCardType->currentIndex(); //从1开始
	int iUserType = m_oUi.m_cbUserType->currentIndex();//0，普通用户 1，管理员
	/*\ 下发卡信息 \*/
	NET_DVR_CARD_CFG_V50* opSendCardInfo = new NET_DVR_CARD_CFG_V50();
	if (opSendCardInfo == nullptr)
	{
		return;
	}
	opSendCardInfo->dwSize = sizeof(NET_DVR_CARD_CFG_V50);
	opSendCardInfo->dwModifyParamType = 0x00040d8d; //0x000c80;//工号 姓名 密码
	/*\ 门权限模板 开启魔板 \*/
	opSendCardInfo->dwPlanTemplate = 1;//1.全天开启
	/*\ 门权限 \*/
	opSendCardInfo->byDoorRight[0] = 1;
	/*\ 卡模板参数 \*/
	opSendCardInfo->wCardRightPlan[0][0] = 1;
	opSendCardInfo->wCardRightPlan[0][1] = 2;
	/*\ 卡号 \*/
	strcpy((char*)(opSendCardInfo->byCardNo), m_opAddUserInfo->m_qsCardNum.toLocal8Bit().data());
	/*\ 卡密码 \*/
	strcpy((char*)opSendCardInfo->byCardPassword, m_opAddUserInfo->m_qsCardPass.toLocal8Bit().data());
	/*\ 工号,不能以0开头，且不能重复 \*/
	opSendCardInfo->dwEmployeeNo = m_opAddUserInfo->m_qsUserNum.toInt();
	/*\ 用户名 \*/
	strcpy((char*)opSendCardInfo->byName, m_opAddUserInfo->m_qsUserName.toLocal8Bit().data());
	/*\ 卡类型 \*/
	opSendCardInfo->byCardType = iCardType + 1;
	/*\ 卡是否有效 \*/
	opSendCardInfo->byCardValid = 1;
	/*\ 用户类型 \*/
	switch (iUserType)
	{
	case 0:
		opSendCardInfo->byUserType = 0;
		break;
	default:
		opSendCardInfo->byUserType = 1;
		break;
	}
	/*\ 发送数据 \*/
	if (!NET_DVR_SendRemoteConfig(
		m_opLongConnStatus->m_iLongConnHandle,
		ENUM_ACS_SEND_DATA,
		(char*)opSendCardInfo,
		sizeof(NET_DVR_CARD_CFG_V50)))
	{
		if (opSendCardInfo != nullptr)
		{
			delete opSendCardInfo;
			opSendCardInfo = nullptr;
		}
		int iError = NET_DVR_GetLastError();
		MessageBoxA(nullptr, "添加用户失败", "提示", MB_OK | MB_ICONWARNING);
		if (opSendCardInfo != nullptr)
		{
			delete opSendCardInfo;
			opSendCardInfo = nullptr;
		}
		this->close();
		return;
	}
	else
	{
		MessageBoxA(nullptr, "添加用户成功", "提示", MB_OK);
		NET_DVR_StopRemoteConfig(m_opLongConnStatus->m_iLongConnHandle);
		this->InitLongConnStatus();
		if (opSendCardInfo != nullptr)
		{
			delete opSendCardInfo;
			opSendCardInfo = nullptr;
		}
		/*\ 下发人脸信息 \*/
		this->SendCardSuccInputFaceInfo();
		this->close();
	}
}

/****************************************!
*@brief  判断用户输入的数据是否合法
*@author Jinzi
*@date   2019/09/18 10:48:12
*@param[in]
*@param[out]
*@return
****************************************/
void fmAddUser::JuageUserInputData()
{

}

/****************************************!
*@brief  设置卡的人脸信息
*@author Jinzi
*@date   2019/09/18 11:20:16
*@param[in]
*@param[out]
*@return
****************************************/
void fmAddUser::SendCardSuccInputFaceInfo()
{
	int iIndex = -1;
	/*\ 判断用户选择的是哪个门禁主机 \*/
	if (m_funcGetListViewCheck != nullptr)
	{
		iIndex = m_funcGetListViewCheck();
		if (iIndex == -1)
		{
			MessageBoxA(nullptr, "请选择门禁主机", "提示", MB_OK | MB_ICONWARNING);
			this->close();
			return;
		}
	}
	if (m_vecLoginInfo.empty())
	{
		MessageBoxA(nullptr, "请先登录门禁主机", "提示", MB_OK | MB_ICONWARNING);
		this->close();
		return;
	}
	/*\ 初始化长连接数据 \*/
	this->InitLongConnStatus();
	m_opLongConnStatus->m_iLoginHandle = m_vecLoginInfo[iIndex].m_iLoginHandle;
	NET_DVR_FACE_PARAM_COND oStartFaceInfo = { 0 };
	oStartFaceInfo.dwSize = sizeof(NET_DVR_FACE_PARAM_COND);
	oStartFaceInfo.dwFaceNum = 1;
	oStartFaceInfo.byFaceID = 1;
	oStartFaceInfo.byEnableCardReader[0] = 1;
	strcpy((char*)oStartFaceInfo.byCardNo, m_opAddUserInfo->m_qsCardNum.toLocal8Bit().data());
	/*\ 建立长连接 \*/
	m_opLongConnStatus->m_iLongConnHandle = NET_DVR_StartRemoteConfig(
		m_opLongConnStatus->m_iLoginHandle,
		NET_DVR_SET_FACE_PARAM_CFG,
		&oStartFaceInfo,
		static_cast<DWORD>(sizeof(NET_DVR_FACE_PARAM_COND)),
		(fRemoteConfigCallback)AddUserRemoteConfigCallBack,
		nullptr
	);
	if (m_opLongConnStatus->m_iLongConnHandle == -1)
	{
		int iError = NET_DVR_GetLastError();
		MessageBoxA(nullptr, "添加人脸信息失败", "提示", MB_OK | MB_ICONWARNING);
		return;
	}
	m_opLongConnStatus->m_bIsLongConn = true;
	/*\ 发送数据 \*/
	NET_DVR_FACE_PARAM_CFG oSendFaceInfo = { 0 };
	oSendFaceInfo.dwSize = sizeof(NET_DVR_FACE_PARAM_CFG);
	oSendFaceInfo.byFaceDataType = 1;
	oSendFaceInfo.byFaceID = 1;
	oSendFaceInfo.byEnableCardReader[0] = 1;
	/*\ 卡号 \*/
	strcpy((char*)oSendFaceInfo.byCardNo, m_opAddUserInfo->m_qsCardNum.toLocal8Bit().data());
	/*\ 读取文件信息 \*/
	//char chpBuf[200 * 1024] = { 0 };
	char* chpBuf = new char[200 * 1024];
	FILE* fileI = fopen(std::string(m_opAddUserInfo->m_qsPicPath.toLocal8Bit().data()).c_str(), "rb");
	if (!fileI)
	{
		return;
	}
	/*\ 给char*分配内存 \*/
	int iLength = fread(chpBuf, 1, 200 * 1024, fileI);
	fclose(fileI);
	/*\ 赋值 \*/
	oSendFaceInfo.pFaceBuffer = chpBuf;
	oSendFaceInfo.dwFaceLen = iLength;
	/*\ 发送数据 \*/
	if (!NET_DVR_SendRemoteConfig(
		m_opLongConnStatus->m_iLongConnHandle,
		ENUM_ACS_INTELLIGENT_IDENTITY_DATA,
		(char*)& oSendFaceInfo,
		static_cast<DWORD>(sizeof(NET_DVR_FACE_PARAM_CFG))))
	{
		int iError = -1;
		iError = NET_DVR_GetLastError();
		MessageBoxA(nullptr, "下发人脸信息失败", "提示", MB_OK | MB_ICONWARNING);
		NET_DVR_StopRemoteConfig(m_opLongConnStatus->m_iLongConnHandle);
	}
	else
	{
		MessageBoxA(nullptr, "下发人脸信息成功", "提示", MB_OK);
		NET_DVR_StopRemoteConfig(m_opLongConnStatus->m_iLongConnHandle);
	}
}

/****************************************!
*@brief  读取图片信息到文件流
*@author Jinzi
*@date   2019/09/18 11:49:03
*@param[in]
*@param[out]
*@return     返回读取的信息
****************************************/
char* fmAddUser::ReadPicInfoToStream()
{
	//QByteArray oByteArray;
	//QString qsPicPath = QString::fromLocal8Bit(std::string(m_opAddUserInfo->m_qsPicPath.toLocal8Bit().data()).c_str());
	//qsPicPath = QDir::toNativeSeparators(qsPicPath);
	//QFile oFile(qsPicPath);
	//if (!oFile.exists())
	//{
	//	MessageBoxA(nullptr, "该文件不存在", "提示", MB_OK | MB_ICONWARNING);
	//	return "";
	//}
	//QImage img(qsPicPath);
	//img.load(qsPicPath);
	//QBuffer buffer(&oByteArray);
	//buffer.open(QIODevice::WriteOnly);
	//img.save(&buffer, "PNG");
	///*QFile file("./1.txt");
	//if (file.open(QIODevice::WriteOnly))
	//{
	//	file.write(oByteArray);
	//	file.close();
	//}*/
	//buffer.close();
	//oFile.close();
	//return buffer.data();
	return CUtils::GetInstance()->ReadJpgInfoWithCSharp(
		std::string(m_opAddUserInfo->m_qsPicPath.toLocal8Bit().data()).c_str()
	);
}