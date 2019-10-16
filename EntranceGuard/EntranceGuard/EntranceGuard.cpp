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

/*\ �����ⲿ�������ļ���Ϣ \*/
extern CSaveiniInfo* gl_opSaveIniInfo;

std::vector<NET_DVR_CARD_CFG_V50>	gl_vecCardInfo;			/*\ �洢������Ϣ \*/
DWORD								gl_dwOptType = -1;		/*\ ��������,�����������ݵ������ж� \*/
std::function<void(void)>			gl_funcShowUserInfo;	/*\ ������ʾ�û���Ϣ \*/
QString								gl_qsUserId = -1;		/*\ ���������û�id \*/

/****************************************!
*@brief  ���캯��
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
	/*\ �����ڴ� \*/
	m_oMenu = new QMenu();
	m_oFmMenJinLogin = new fmMenJinLogin(this);
	m_oFmAddUser = new fmAddUser(this);
	m_opFmChangeUser = new fmChangeUser(this);
	m_opLongConnInfo = new CLongConnStatus();
	/*\ ��ʼ��m_opLongConnInfo \*/
	InitSaveLongInfo();

	m_opCardInfo = new NET_DVR_CARD_CFG_COND();
	m_opSendDataInfo = new NET_DVR_CARD_CFG_SEND_DATA();
	m_opHttpManager = new QNetworkAccessManager(this);
	m_opHttpRequest = new CHttpRequest();

	/*\ ��ʼ������,�ص����� \*/
	m_funcMsgHikCallBack = (MSGCallBack_V31)MsgHikCallBack;
	m_funcMsgRemoteConfig = (fRemoteConfigCallback)RemoteConfigCallBack;
	gl_funcShowUserInfo = std::bind(&EntranceGuard::ShowMenJinUserInfo, this);
	/*\ ���Ӵ��ں���ָ�븳ֵ \*/
	InitChildWinFuncAssign();
	/*\ ��ʼ������sdk \*/
	InitHikSDK();
	/*\ ���ı���ʹ��fromLocal8Bit����,�������Ļ����� \*/
	m_oAction1 = new QAction(QString::fromLocal8Bit("��¼�Ž�"), this);
	m_oAction2 = new QAction(QString::fromLocal8Bit("�ǳ��Ž�"), this);
	m_oAction3 = new QAction(QString::fromLocal8Bit("�ǳ���������"), this);
	/*\ ���Ӳ˵���ӵ��˵��� \*/
	m_oMenu->addAction(m_oAction1);
	m_oMenu->addAction(m_oAction2);
	m_oMenu->addAction(m_oAction3);
	/*\ ���źźͲ� \*/
	InitSignalAndSlot();
	/*\ ��ʾ�����û���Ϣ \*/
	ShowAllUserInfoWithListView();
}

/****************************************!
*@brief  ���Ӵ��ڸ�ֵ
*@author Jinzi
*@date   2019/09/24 11:35:30
*@param[in]
*@param[out]
*@return
****************************************/
void EntranceGuard::InitChildWinFuncAssign()
{
	///////////////////// 2019/09/24 11:36:38 ��¼���� ==> BEGIN /////////////////////////////////////////////////////
	m_oFmMenJinLogin->m_funcSetLoginInfo = std::bind(&EntranceGuard::SetListLoinInfo, this, std::placeholders::_1);
	m_oFmMenJinLogin->m_funcSetLabelInfo = std::bind(&EntranceGuard::SetLabelShowInfo, this, std::placeholders::_1);
	m_oFmMenJinLogin->m_funcGetListViewCheck = std::bind(&EntranceGuard::GetListViewCheck, this);
	///////////////////// 2019/09/24 11:36:38 ��¼���� ==> END /////////////////////////////////////////////////////

	///////////////////// 2019/09/24 11:36:56 �޸��û����� ==> BEGIN /////////////////////////////////////////////////////
	m_opFmChangeUser->m_funcRetUserSelMenJinLoginHandle =
		std::bind(&EntranceGuard::RetUserSelMenJinLoginHandle, this);
	m_opFmChangeUser->m_funcRetUserSelJobNum =
		std::bind(&EntranceGuard::RetUserSelRowJobNum, this);
	m_opFmChangeUser->m_funcRetUserSelCardNum =
		std::bind(&EntranceGuard::RetUserSelRowCardNum, this);
	///////////////////// 2019/09/24 11:36:56 �޸��û����� ==> END /////////////////////////////////////////////////////

	///////////////////// 2019/09/24 11:37:23 ����û����� ==> BEGIN /////////////////////////////////////////////////////
	m_oFmAddUser->m_funcGetListViewCheck = std::bind(&EntranceGuard::GetListViewCheck, this);
	///////////////////// 2019/09/24 11:37:23 ����û����� ==> END /////////////////////////////////////////////////////
}

/****************************************!
*@brief  ���źźͲ�
*@author Jinzi
*@date   2019/09/10 10:51:27
*@param[in]
*@param[out]
*@return
****************************************/
void EntranceGuard::InitSignalAndSlot()
{
	/*\ ��ʾ��¼ҳ��,ʹ��triggered()�źź���,������ \*/
	connect(m_oAction1, SIGNAL(triggered()), m_oFmMenJinLogin, SLOT(show()));
	/*\ �Ž������ǳ� \*/
	connect(m_oAction2, SIGNAL(triggered()), m_oFmMenJinLogin, SLOT(HikMenJinLoginOut()));
	/*\ �ǳ������Ž����� \*/
	connect(m_oAction3, SIGNAL(triggered()), m_oFmMenJinLogin, SLOT(HikMenJinLoginOutAll()));
	/*\ ��ѯ��ť \*/
	connect(ui.m_btnSelect, SIGNAL(clicked()), this, SLOT(SelectMenJinInfo()));
	/*\ ��ȡ�豸��������ť \*/
	connect(ui.m_btnDevAblity, SIGNAL(clicked()), this, SLOT(GetDevAbilty()));
	/*\ �����Ա��Ϣ��ť \*/
	connect(ui.m_btnDelAllUser, SIGNAL(clicked()), this, SLOT(DelMenJinOnAllUserInfo()));
	/*\ ����û���ť \*/
	connect(ui.m_btnAddUser, SIGNAL(clicked()), this, SLOT(JuageShowAddUserFm()));
	/*\ �·���Ϣ \*/
	connect(ui.m_btnSend, SIGNAL(clicked()), this, SLOT(BeSentDownMenJin()));
	/*\ ɾ����Ա��Ϣ��ť \*/
	connect(ui.m_btnDelUser, SIGNAL(clicked()), this, SLOT(DelUserInfoWithMenJin()));
	/*\ �޸���Ա��Ϣ��ť \*/
	connect(ui.m_btnModifUser, SIGNAL(clicked()), this, SLOT(ShowFmChangeUserInfo()));
	/*\ ҳ��ر��˳���¼ \*/
	connect(this, SIGNAL(close()), this, SLOT(HikMenJinLoginOutAll()));
}

/****************************************!
*@brief  ��������
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
*@brief  ����Ҽ�����¼�
*@author Jinzi
*@date   2019/09/02 13:39:32
*@param[in]
   _oEvent		:	����¼�
*@param[out]
*@return
****************************************/
void EntranceGuard::mousePressEvent(QMouseEvent* _oEvent)
{
	if (_oEvent->buttons() == Qt::RightButton)
	{
		QRect oLblRect = ui.m_lblOptInfo->rect();
		/*\ �жϽ����Ƿ��ھ����� \*/
		if (oLblRect.contains(_oEvent->pos()))
		{
			m_oMenu->exec(_oEvent->globalPos());
		}
	}
}

/****************************************!
*@brief  ��ʼ���洢��������Ϣ�Ľṹ��
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
*@brief  ��ʼ������sdk
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
*@brief  �����ص�����
*@author Jinzi
*@date   2019/09/02 16:27:42
*@param[in]
   _lCommand		:	�ϴ�����Ϣ���ͣ���ͬ�ı�����Ϣ��Ӧ��ͬ�����ͣ�ͨ������������ʲô������Ϣ
   _pAlarmer		:	�����豸��Ϣ�������豸���кš�IP��ַ����¼IUserID�����
   _pAlarmInfo		:	������Ϣ��ͨ��lCommandֵ�ж�pAlarmer��Ӧ�Ľṹ��
   _dwBufLen		:	������Ϣ�����С
   _pUser			:	�û�����
*@param[out]
*@return
****************************************/
BOOL MsgHikCallBack(LONG _lCommand, NET_DVR_ALARMER* _pAlarmer,
	char* _pAlarmInfo, DWORD _dwBufLen, void* _pUser)
{
	return TRUE;
}

/****************************************!
*@brief  �������sdk
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
*@brief  ��ʾtreeView
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
*@brief  �Ӵ��ڵ��øú�����Ϊ�����帳ֵ(�Ž���¼�ɹ�)
*@author Jinzi
*@date   2019/09/03 10:47:51
*@param[in]
   _lstListLoginInfo :	�Ž��ĵ�¼��Ϣ
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
	/*\ ��ʾ��¼��Ϣ \*/
	this->ShowListView();
}

/****************************************!
*@brief  ����label��ʾ����
*@author Jinzi
*@date   2019/09/03 13:47:54
*@param[in]
   _sShowInfo : Ҫ��ʾ����Ϣ
*@param[out]
*@return
****************************************/
void EntranceGuard::SetLabelShowInfo(const char* _cchpShowInfo)
{
	/*\ ������ʾ��Ϣ \*/
	ui.m_lblOptInfo->setText(QString::fromLocal8Bit(_cchpShowInfo));
}

/****************************************!
*@brief  ����treeVeiw�ؼ�ԭ�������
*@author Jinzi
*@date   2019/09/03 14:16:50
*@param[in]
*@param[out]
*@return �ɹ�����ѡ���λ�� ʧ�ܷ���-1
****************************************/
int EntranceGuard::GetListViewCheck()
{
	int iIndex = -1;
	iIndex = ui.m_listDevInfo->currentIndex().row();
	return iIndex;
}

/****************************************!
*@brief  ��ѯ�Ž�������Ϣ
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
		MessageBoxA(nullptr, "��ѡ��һ���Ž�����", "��ʾ", MB_OK | MB_ICONWARNING);
	}
	else
	{
		/*\ �ڴ˴���Ӳ���Ա��Ϣ�Ĵ��� \*/
		if (!m_vecLoginInfo.empty())
		{
			m_opCardInfo->dwSize = sizeof(NET_DVR_CARD_CFG_COND);
			m_opCardInfo->dwCardNum = 0xffffffff;
			m_opCardInfo->byCheckCardNo = 1;

			/*\ ��¼�����ֵ\*/
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
			/*\ ���ò������� \*/
			gl_dwOptType = NET_DVR_GET_CARD_CFG_V50;
			/*\ ���ͻ�ȡ����Ϣ������ \*/
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
			MessageBoxA(nullptr, "���¼�Ž��������ٽ��иò���", "��ʾ", MB_OK | MB_ICONWARNING);
		}
	}
}

/****************************************!
*@brief  ����Զ��������Ϣ�Ļص�����
*@author Jinzi
*@date   2019/09/04 8:40:07
*@param[in]
   _dwType		:	�ص�������
   _lpBuffer	:	�洢���ݻ�������ָ��
   _dwBufLen	:	�������Ĵ�С
   _pUserData	:	�û�����
*@param[out]
*@return
****************************************/
void RemoteConfigCallBack(DWORD _dwType, void* _lpBuffer,
	DWORD _dwBufLen, void* _pUserData)
{
	switch (_dwType)
	{
		/*\ �ص�״̬,��ʾ����ȫ��������� \*/
	case NET_SDK_CALLBACK_TYPE_STATUS:
		if (*static_cast<int*>(_lpBuffer) == NET_SDK_CALLBACK_STATUS_SUCCESS)
		{
			switch (gl_dwOptType)
			{
				/*\ ��ȡ�Ž���Ա��Ϣ \*/
			case NET_DVR_GET_CARD_CFG_V50:
				/*\ ��ʾ���� \*/
				if (gl_funcShowUserInfo != nullptr)
				{
					gl_funcShowUserInfo();
				}
				break;
				/*\ �·��Ž���Ա��Ϣ \*/
			case NET_DVR_SET_CARD_CFG_V50:
				break;


			}

		}
		break;
		/*\ �ص����� \*/
	case NET_SDK_CALLBACK_TYPE_DATA:
		switch (gl_dwOptType)
		{
			/*\ ��ȡ�Ž���Ա��Ϣ \*/
		case NET_DVR_GET_CARD_CFG_V50:
			NET_DVR_CARD_CFG_V50* opData = new NET_DVR_CARD_CFG_V50();
			opData = static_cast<NET_DVR_CARD_CFG_V50*>(_lpBuffer);
			gl_vecCardInfo.push_back(*opData);
			break;
		}

	}
}

/****************************************!
*@brief  �õ��豸������ ����ȡ���Ĳ�����
*@author Jinzi
*@date   2019/09/04 8:53:38
*@param[in]
*@param[out]
*@return
****************************************/
void EntranceGuard::GetDevAbilty()
{
	/*\ ���ж����ĸ��豸 \*/
	int iIndex = ui.m_listDevInfo->currentIndex().row();
	if (!m_vecLoginInfo.empty())
	{
		MessageBoxA(nullptr, "δ����", "��ʾ", MB_OK);
	}
	else
	{
		MessageBoxA(nullptr, "����ѡ��һ���豸", "��ʾ", MB_OK | MB_ICONWARNING);
	}
}

/****************************************!
*@brief  ���һ���Ž��ϵ������û���Ϣ
*@author Jinzi
*@date   2019/09/04 9:40:14
*@param[in]
*@param[out]
*@return
****************************************/
void EntranceGuard::DelMenJinOnAllUserInfo()
{
	/*\ �ж����ĸ��Ž� \*/
	int iIndex = ui.m_listDevInfo->currentIndex().row();
	if (iIndex == -1)
	{
		MessageBoxA(nullptr, "��ѡ��һ���Ž�", "��ʾ", MB_OK | MB_ICONWARNING);
	}
	else
	{
		std::string sStart = "ȷ��Ҫ����Ž�: ";
		std::string sEnd = " �������û���Ϣ��?";
		std::string sInfo = sStart + m_vecLoginInfo[iIndex].m_sNodeName.toLocal8Bit().data() + sEnd;
		int iBtnType = MessageBoxA(nullptr, sInfo.c_str(), "��ʾ", MB_OKCANCEL);
		/*\ 1.��ʾ�û������ȷ�� \*/
		if (iBtnType == 1)
		{
			if (m_vecLoginInfo.empty())
			{
				MessageBoxA(nullptr, "���ȵ�¼�Ž�����", "��ʾ", MB_OK | MB_ICONWARNING);
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
				MessageBoxA(nullptr, "����Ž�����ʧ��", "��ʾ", MB_OK | MB_ICONWARNING);
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
				MessageBoxA(nullptr, "����Ž����ݳɹ�", "��ʾ", MB_OK);
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
*@brief  �ж��Ƿ������ʾ����û�����
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
		MessageBoxA(nullptr, "���ȵ�¼�Ž�����", "��ʾ", MB_OK | MB_ICONWARNING);
	}
}

/****************************************!
*@brief  ����������
*@author Jinzi
*@date   2019/09/16 13:12:14
*@param[in]
   _iLoginHandle	:	��¼���
   _uiType			:	��������
   _lpBuffer		:	�������͵Ĳ�ͬ�����ݽṹҲ��ͬ
   _dwLength		:	��������ݵĳ���
   _lpUserData		:	�û����ݣ�Ĭ��Ϊnull
*@param[out]
*@return     ʧ�ܷ���-1,�ɹ��������Ӿ��
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
*@brief  �رճ�����
*@author Jinzi
*@date   2019/09/16 13:18:33
*@param[in]
   _iLongConnHandle	:	�����Ӿ��
*@param[out]
*@return     �ɹ�����true��ʧ�ܷ���false
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
*@brief  ��ʾ�Ž��������û���Ϣ
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
	opModel->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("����"));
	opModel->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("������"));
	opModel->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("����"));
	opModel->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("�û�����"));
	opModel->setHeaderData(4, Qt::Horizontal, QString::fromLocal8Bit("����"));
	/*\ ���ñ������ \*/
	ui.m_tbUserInfo->setModel(opModel);
	/*\ ��������� \*/
	ui.m_tbUserInfo->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	//�������ű��仯������Ӧ�仯
	ui.m_tbUserInfo->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);
	//�������ű��仯������Ӧ�仯
	ui.m_tbUserInfo->verticalHeader()->sectionResizeMode(QHeaderView::Stretch);
	for (int i = 0; i < gl_vecCardInfo.size(); i++)
	{
		/*\ ���� \*/
		opModel->setItem(i, 0, new QStandardItem((const char*)gl_vecCardInfo[i].byCardNo));
		/*\ ������ \*/
		//�����ͣ�1- ��ͨ����Ĭ�ϣ���2- �м��˿���3- ����������4- Ѳ������5- в�ȿ���6- ��������7- ��������8- �������
		//9- Ա������10- Ӧ������11- Ӧ��������������Ȩ��ʱ��Ȩ�ޣ������ܿ��ţ���Ĭ����ͨ�� 
		switch (gl_vecCardInfo[i].byCardType)
		{
		case 1:
			opModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("��ͨ��")));
			break;
		case 2:
			opModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("�м��˿�")));
			break;
		case 3:
			opModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("��������")));
			break;
		case 4:
			opModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("Ѳ����")));
			break;
		case 5:
			opModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("в�ȿ�")));
			break;
		case 6:
			opModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("������")));
			break;
		case 7:
			opModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("������")));
			break;
		case 8:
			opModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("�����")));
			break;
		case 9:
			opModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("Ա����")));
			break;
		case 10:
			opModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("Ӧ����")));
			break;
		case 11:
			opModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("Ӧ������")));
			break;
		}
		/*\ ���� \*/
		opModel->setItem(i, 2, new QStandardItem(QString::fromLocal8Bit((const char*)gl_vecCardInfo[i].byName)));
		/*\ �û����� \*/
		switch (gl_vecCardInfo[i].byUserType)
		{
		case 0://��ͨ
			opModel->setItem(i, 3, new QStandardItem(QString::fromLocal8Bit("��ͨ�û�")));
			break;
		case 1://����Ա
			opModel->setItem(i, 3, new QStandardItem(QString::fromLocal8Bit("����Ա")));
			break;
		}
		char chbuf[16] = { 0 };
		_ultoa((unsigned long)gl_vecCardInfo[i].dwEmployeeNo, chbuf, 10);
		/*\ ���� \*/
		opModel->setItem(i, 4, new QStandardItem(chbuf));
		//for (int j = 0; j < 5; j++)
		//{
		//	//�����ַ�λ�� 
		//	opModel->item(i, j)->setTextAlignment(Qt::AlignCenter);
		//}
		//�����ַ���ɫ 
		opModel->item(i, 0)->setForeground(QBrush(QColor(255, 0, 0)));
	}
	this->SetLabelShowInfo("��ѯ�ɹ�");
	/*\ ��ʼ��m_opLongConnInfo \*/
	InitSaveLongInfo();
	/*\ ��gl_vecCardInfo�ṹ����� \*/
	gl_vecCardInfo.clear();
	this->CloseLongConn(m_opLongConnInfo->m_iLongConnHandle);
}

/****************************************!
*@brief  ��ʾ�����û���Ϣ
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
	/*\ ��������url \*/
	oRequestInfo.setUrl(QUrl(qsUrl));
	/*\ ��������ͷ��Ϣ \*/
	oRequestInfo.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
	/*\ ����Ҫ���͵�����json \*/
	QByteArray oReqData;
	/*\ ����post���� \*/
	QNetworkReply* pReply = m_opHttpManager->post(oRequestInfo, oReqData);
	/*\ ʹ��ͬ���ķ�ʽ���еȴ��������ص����� \*/
	QEventLoop oLoop;
	QObject::connect(pReply, SIGNAL(finished()), &oLoop, SLOT(quit()));
	oLoop.exec();
	this->HttpGetAllUserSuccDataHandle(pReply);
	/*\ �첽��ʽ�ȴ��������ص� \*/
	//QObject::connect(pReply, SIGNAL(finished()), this, SLOT(HttpGetAllUserSuccDataHandle()));
}

/****************************************!
*@brief  �·��Ž���Ϣ
*@author Jinzi
*@date   2019/09/16 16:19:05
*@param[in]
*@param[out]
*@return
****************************************/
void EntranceGuard::BeSentDownMenJin()
{
	MessageBoxA(nullptr, "�·�", "��ʾ", MB_OK);
}

/****************************************!
*@brief  �����ȡ�����û���Ϣ��ķ�������
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
		MessageBoxA(nullptr, "����������", "��ʾ", MB_OK | MB_ICONWARNING);
		return;
	}
	// ������ܵ�����;
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
		/*\ ���û���Ϣ�洢���� \*/
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
*@brief  ɾ���Ž���ָ����Ա����Ϣ
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
		MessageBoxA(nullptr, "���ȵ�¼�Ž�����", "��ʾ", MB_OK | MB_ICONWARNING);
		return;
	}
	if (iIndex == -1)
	{
		MessageBoxA(nullptr, "���Ȳ�ѯ�û���Ϣ", "��ʾ", MB_OK | MB_ICONWARNING);
		return;
	}

	/*\ ������ȡ���е��û�����,�����浽m_vecSvrRetAllUserInfo�� \*/
	this->ShowAllUserInfoWithListView();

	QAbstractItemModel* oAbModel = ui.m_tbUserInfo->model();
	QModelIndex ModelIndex = oAbModel->index(iIndex, 0);
	/*\ ���� \*/
	QString qsCardNumber = oAbModel->data(ModelIndex).toString();

	/*\ �õ����Ŷ�Ӧ���û�id \*/
	QString qsUserId = RetUserIdWithJobNum(qsCardNumber);
	/*\ �ж��Ƿ�Ѱ�ҵ��˸�id \*/
	if (qsUserId == "")
	{
		MessageBoxA(nullptr, "���û�������", "��ʾ", MB_OK | MB_ICONWARNING);
		return;
	}
	/*\ �������� \*/
	std::string sUrl = "http://" + gl_opSaveIniInfo->sIp + ":" + gl_opSaveIniInfo->sPort + "/patroluser/deletePatrolUser";
	QString qsUrl = sUrl.c_str();
	/*\ http �����͵����� \*/
	QJsonObject jsonReqData;
	jsonReqData.insert("id", qsUserId);
	QJsonDocument jsonDocument;
	jsonDocument.setObject(jsonReqData);
	QNetworkRequest oRequestInfo;
	/*\ ��������url \*/
	oRequestInfo.setUrl(QUrl(qsUrl));
	/*\ ��������ͷ��Ϣ \*/
	oRequestInfo.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
	/*\ ����Ҫ���͵�����json \*/
	QByteArray oReqData(jsonDocument.toJson());
	/*\ ����post���� \*/
	QNetworkReply* pReply = m_opHttpManager->post(oRequestInfo, oReqData);
	/*\ �������ݴ����� \*/
	QObject::connect(pReply, SIGNAL(finished()), this, SLOT(HttpDelUserInfoSuccDataHandle()));
}

/****************************************!
*@brief  ���ݹ��ŷ����û�id
*@author Jinzi
*@date   2019/09/20 16:49:02
*@param[in]
   _qsCardNumber	:	����
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
*@brief  ɾ���û����������ص����ݴ�����
*@author Jinzi
*@date   2019/09/20 16:59:52
*@param[in]
*@param[out]
*@return
****************************************/
void EntranceGuard::HttpDelUserInfoSuccDataHandle()
{
	QNetworkReply* pReplay = qobject_cast<QNetworkReply*>(sender());
	// ������ܵ�����;
	QByteArray replyContent = pReplay->readAll();
	/*\ ���ַ�����ת��Ϊjson���� \*/
	QJsonObject jsonResData = QJsonDocument::fromJson(replyContent).object();
	/*\ ˵���ڷ������� ɾ���ɹ� \*/
	if (jsonResData.value("code") == 0)
	{
		/*\ ɾ��һ���Ž��û���Ϣ�����Ž������ϣ� \*/
		this->DelMenJinUserInfo();
	}
	else
	{
		MessageBoxA(nullptr, "ɾ�����û�ʧ��", "��ʾ", MB_OK | MB_ICONWARNING);
	}
}

/****************************************!
*@brief  ɾ��һ���û��Ž���Ϣ
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
		MessageBoxA(nullptr, "���ȵ�¼�Ž�����", "��ʾ", MB_OK | MB_ICONWARNING);
		return;
	}
	if (iIndex == -1)
	{
		MessageBoxA(nullptr, "���Ȳ�ѯ�û���Ϣ", "��ʾ", MB_OK | MB_ICONWARNING);
		return;
	}
	InitSaveLongInfo();

	m_opLongConnInfo->m_iLoginHandle = m_vecLoginInfo[iIndex].m_iLoginHandle;
	/*\ ���Ž�������ɾ���û������������ӣ� \*/
	NET_DVR_CARD_CFG_COND* oCardInfo = new NET_DVR_CARD_CFG_COND();
	/*\ ���ͳ��������� \*/
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
		MessageBoxA(nullptr, "ɾ���û�ʧ��", "��ʾ", MB_OK | MB_ICONWARNING);
		goto Free;
		return;
	}
	///////////////////// 2019/09/23 8:45:12 ִ��ɾ������ ==> BEGIN /////////////////////////////////////////////////////
	oDelCardInfo->dwSize = sizeof(NET_DVR_CARD_CFG_V50);
	oDelCardInfo->dwModifyParamType = 0x00000001; //���Ƿ���Ч
	oDelCardInfo->byCardValid = 0;//����Ϊ0��ʾɾ��

	///////////////////// 2019/09/29 16:16:51 ���ִ����ڴ�Ĵ��� ==> BEGIN /////////////////////////////////////////////////////
	if (!NET_DVR_SendRemoteConfig(
		m_opLongConnInfo->m_iLongConnHandle,
		ENUM_ACS_SEND_DATA,
		(char*)oDelCardInfo,
		sizeof(NET_DVR_CARD_CFG_V50)))
	{
		int iError = NET_DVR_GetLastError();
		MessageBoxA(nullptr, "ɾ���û�ʧ��", "��ʾ", MB_OK | MB_ICONWARNING);
		goto Free;
		return;
	}
	else
	{
		int iError = NET_DVR_GetLastError();
		MessageBoxA(nullptr, "ɾ���û��ɹ�", "��ʾ", MB_OK | MB_ICONWARNING);
		NET_DVR_StopRemoteConfig(m_opLongConnInfo->m_iLongConnHandle);
		goto Free;
		return;
	}
	///////////////////// 2019/09/29 16:16:51 ���ִ����ڴ�Ĵ��� ==> END /////////////////////////////////////////////////////

	///////////////////// 2019/09/23 8:45:12 ִ��ɾ������ ==> END /////////////////////////////////////////////////////
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
*@brief  ��ʾ�޸��û���Ϣ�Ĵ���
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
		MessageBoxA(nullptr, "���ȵ�¼�Ž�����", "��ʾ", MB_OK | MB_ICONWARNING);
		return;
	}
	if (iTbIndex == -1)
	{
		MessageBoxA(nullptr, "���Ȳ�ѯ����", "��ʾ", MB_OK | MB_ICONWARNING);
		return;
	}
	QAbstractItemModel* oAbModel = ui.m_tbUserInfo->model();
	QModelIndex ModelCardNum = oAbModel->index(iTbIndex, 0);
	/*\ ���� \*/
	QString qsCardNumber = oAbModel->data(ModelCardNum).toString();
	std::string sUrl = "http://" + gl_opSaveIniInfo->sIp + ":" + gl_opSaveIniInfo->sPort + "/patroluser/getPatrolUserByCardNum";
	QJsonObject jsonReqData;
	jsonReqData.insert("cardNumber", qsCardNumber);
	QJsonDocument jsonDocument(jsonReqData);
	//jsonDocument.setObject(jsonReqData);
	QByteArray reqData(jsonDocument.toJson());
	/*\ ���ݹ��Ż���û���id \*/
	m_opHttpRequest->HttpPostRequest(sUrl.c_str(), reqData,
		std::bind(&EntranceGuard::JuageJobNumResFuncHandle, this, std::placeholders::_1));
}

/****************************************!
*@brief  �жϹ����Ƿ���ڵ���Ӧ������
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
		MessageBoxA(nullptr, "��������Ӧ����Ϊ��", "��ʾ", MB_OK | MB_ICONWARNING);
		return;
	}
	// ������ܵ�����;��ͼƬ����imgName��
	QByteArray replyContent = _opReqplay->readAll();
	QJsonObject jsonResData = QJsonDocument::fromJson(replyContent).object();
	/*\ ������ֵ \*/
	int iTbIndex = ui.m_tbUserInfo->currentIndex().row();
	int iIndex = ui.m_listDevInfo->currentIndex().row();
	QAbstractItemModel* oAbModel = ui.m_tbUserInfo->model();
	QModelIndex ModelJobNum = oAbModel->index(iTbIndex, 4);
	QModelIndex ModelCardNum = oAbModel->index(iTbIndex, 0);
	/*\ ���� \*/
	QString qsJobNumber = oAbModel->data(ModelJobNum).toString();
	/*\ ����(���ص���һ���ַ���) \*/
	QString qsCardNumber = oAbModel->data(ModelCardNum).toString();
	/*\ �ж��Ƿ�ص��ɹ�����\*/
	if (jsonResData.value("code") == 0)
	{
		m_opFmChangeUser->fmChangeUserInit();
		/*\ ��ʾ���� \*/
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
		MessageBoxA(nullptr, "���û�������", "��ʾ", MB_OK | MB_ICONWARNING);
		return;
	}
}

/****************************************!
*@brief  �����û�ѡ���ж�Ӧ�Ĺ��ţ�listView��
*@author Jinzi
*@date   2019/09/24 11:23:19
*@param[in]
*@param[out]
*@return     �����û�ѡ���� ʧ�ܷ���-1
****************************************/
QString EntranceGuard::RetUserSelRowJobNum()
{
	int iIndex = -1;
	iIndex = ui.m_tbUserInfo->currentIndex().row();
	QAbstractItemModel* oAbModel = ui.m_tbUserInfo->model();
	QModelIndex ModelJobNum = oAbModel->index(iIndex, 4);
	/*\ ���� \*/
	QString qsJobNumber = oAbModel->data(ModelJobNum).toString();
	return qsJobNumber;
}

/****************************************!
*@brief  �����û�ѡ����Ž���¼���
*@author Jinzi
*@date   2019/09/24 11:32:00
*@param[in]
*@param[out]
*@return     ʧ�ܷ���-1 �ɹ����ؾ��
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
 *@brief  �����û�ѡ�п���
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
	/*\ ���� \*/
	QString qsCardNumber = oAbModel->data(ModelCardNum).toString();
	return qsCardNumber;
}