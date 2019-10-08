#include "fmChangeUser.h"
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QNetworkReply>

#include "../HIKSDK/HCNetSDK.h"
#include "./CHttpRequest.h"

#ifndef _UTILS_H_
#include "../Utils/utils.h"
#endif // !_UTILS_H_


extern CSaveiniInfo* gl_opSaveIniInfo;			/*\ �洢���������ļ�����Ϣ \*/

unsigned int			gl_uiCallBackType = -1;	/*\ �����ӻص����� \*/
NET_DVR_CARD_CFG_V50	gl_opSelCardInfo;		/*\ ��ѯ�Ŀ���Ϣ \*/

/****************************************!
*@brief  ����Զ��������Ϣ�Ļص������������ӣ�
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
void HikLongConnCallBackFunc(DWORD dwType, void* lpBuffer, DWORD dwBufLen, void* pUserData)
{
	switch (dwType)
	{
		/*\ �ص����������� \*/
	case NET_SDK_CALLBACK_TYPE_DATA:
		switch (gl_uiCallBackType)
		{
			/*\ ��ѯ����Ϣ,�ص����� \*/
		case NET_DVR_GET_CARD_CFG_V50:
			gl_opSelCardInfo = *(static_cast<NET_DVR_CARD_CFG_V50*>(lpBuffer));
			break;
		}
		break;
	}
}

/****************************************!
*@brief  ���캯��
*@author Jinzi
*@date   2019/09/23 14:19:44
*@param[in]
*@param[out]
*@return
****************************************/
fmChangeUser::fmChangeUser(QWidget* parent /* = Q_NULLPTR */)
{
	m_oUi.setupUi(this);
	/*\ ���õ�ǰ���ں���Ĵ��ڲ��ܵ�� \*/
	setWindowModality(Qt::ApplicationModal);
	/*\ ���������С����ť \*/
	setWindowFlags(this->windowFlags() & ~(Qt::WindowMinMaxButtonsHint));
	/*\ ��ʼ������ \*/
	this->InitMemberVar();
	/*\ ���źźͲ� \*/
	this->BindSignalAndSlots();
}

/****************************************!
*@brief  ��������
*@author Jinzi
*@date   2019/09/23 14:20:07
*@param[in]
*@param[out]
*@return
****************************************/
fmChangeUser::~fmChangeUser()
{
	if (m_opSelMenJinInfo != nullptr)
	{
		delete m_opSelMenJinInfo;
		m_opSelMenJinInfo = nullptr;
	}
}

/****************************************!
*@brief  ���źźͲ�
*@author Jinzi
*@date   2019/09/23 14:21:51
*@param[in]
*@param[out]
*@return
****************************************/
void fmChangeUser::BindSignalAndSlots()
{
	/*\ �޸İ�ť����¼� \*/
	QObject::connect(m_oUi.m_btnAdd, SIGNAL(clicked()), this, SLOT(BtnChangeClickEvent()));
	/*\ ���ذ�ť����¼� \*/
	QObject::connect(m_oUi.m_btnQuit, SIGNAL(clicked()), this, SLOT(close()));
	/*\ ��ȡͼƬ·����ť����¼� \*/
	QObject::connect(m_oUi.m_btnPicPath, SIGNAL(clicked()), this, SLOT(BtnGetPicPathClickEvent()));
}

/****************************************!
*@brief  ��ʼ����Ա����
*@author Jinzi
*@date   2019/09/23 14:22:35
*@param[in]
*@param[out]
*@return
****************************************/
void fmChangeUser::InitMemberVar()
{
	this->m_opSelMenJinInfo = new CSelMenJinInfo();
	this->m_iLongConnHandle = -1;
	this->m_opHttpRequest = new CHttpRequest();
	this->m_opModifyUserInfo = new CModifyUserInfo();
}

/****************************************!
*@brief  �޸İ�ť����¼�
*@author Jinzi
*@date   2019/09/23 14:26:50
*@param[in]
*@param[out]
*@return
****************************************/
void fmChangeUser::BtnChangeClickEvent()
{
	/*\ ���ݹ��� �ж��û��Ƿ��ڷ������д���,���ڷ���id \*/
	std::string sUrl = "http://" + gl_opSaveIniInfo->sIp + ":" + gl_opSaveIniInfo->sPort + "/patroluser/getPatrolUserByJobNum";
	if (m_funcRetUserSelMenJinLoginHandle != nullptr)
	{
		m_iLoginHandle = m_funcRetUserSelMenJinLoginHandle();
	}
	/*\ ��ȡ�û�ѡ�еĹ��� \*/
	if (m_funcRetUserSelCardNum != nullptr)
	{
		m_qsJobNum = m_funcRetUserSelCardNum();
	}
	/*\ ����http���� \*/
	QJsonObject jsonObject;
	jsonObject.insert("jobNumber", m_qsJobNum);
	QJsonDocument jsonDocument(jsonObject);
	QByteArray oReqData(jsonDocument.toJson());
	/*\ ����http���� \*/
	m_opHttpRequest->HttpPostRequest(sUrl.c_str(), oReqData,
		std::bind(&fmChangeUser::ModifyUserInfoWithCardNum, this, std::placeholders::_1));
}

/****************************************!
*@brief  ��ȡͼƬ·����ť�ĵ���¼�
*@author Jinzi
*@date   2019/09/23 14:28:19
*@param[in]
*@param[out]
*@return
****************************************/
void fmChangeUser::BtnGetPicPathClickEvent()
{
	//�����ļ��Ի�����
	QFileDialog* fileDialog = new QFileDialog(this);
	//�����ļ��Ի������
	fileDialog->setWindowTitle(QString::fromLocal8Bit("��ͼƬ"));
	//����Ĭ���ļ�·��
	fileDialog->setDirectory(".");
	//�����ļ�������
	fileDialog->setNameFilter(tr("Images(*.png *.jpg *.jpeg *.bmp)"));
	//���ÿ���ѡ�����ļ�,Ĭ��Ϊֻ��ѡ��һ���ļ�QFileDialog::ExistingFiles
	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	//������ͼģʽ
	fileDialog->setViewMode(QFileDialog::Detail);
	//��ӡ����ѡ����ļ���·��
	QStringList fileNames;
	if (fileDialog->exec())
	{
		fileNames = fileDialog->selectedFiles();
	}
	/*\ ����ͼƬ·�� \*/
	if (fileNames.size() > 0)
	{
		m_oUi.m_linePath->setText(fileNames[0]);
	}
}

/****************************************!
*@brief  ����ѡ���Ž�������Ϣ
*@author Jinzi
*@date   2019/09/23 14:52:41
*@param[in]
	_oSelMenJinInfo	:	�洢ѡ�е��Ž������Ͷ�Ӧ���û�id����Ϣ
*@param[out]
*@return
****************************************/
void fmChangeUser::SetSelMenJinInfo(CSelMenJinInfo& _oSelMenJinInfo)
{
	this->m_opSelMenJinInfo->m_qsUserId = _oSelMenJinInfo.m_qsUserId;
	this->m_opSelMenJinInfo->m_iSelMenJinHandle = _oSelMenJinInfo.m_iSelMenJinHandle;
	this->m_opSelMenJinInfo->m_qsCardNum = _oSelMenJinInfo.m_qsCardNum;
	this->m_opSelMenJinInfo->m_qsJobNum = _oSelMenJinInfo.m_qsJobNum;
}

/****************************************!
*@brief  ���ݿ����޸��û���Ϣ
*@author Jinzi
*@date   2019/09/23 15:17:10
*@param[in]
*@param[out]
*@return
****************************************/
void fmChangeUser::ModifyUserInfoWithCardNum(QNetworkReply* _opReplay)
{
	if (_opReplay == nullptr)
	{
		MessageBoxA(nullptr, "��������Ӧ����Ϊ��", "��ʾ", MB_OK | MB_ICONWARNING);
		return;
	}
	// ������ܵ�����;
	QByteArray replyContent = _opReplay->readAll();
	QJsonObject jsonResData = QJsonDocument::fromJson(replyContent).object();
	QJsonValue jsonValue;
	DWORD dwType = 0x00000000;
	if (jsonResData.value("code") == 0)
	{
		if (!jsonResData.contains("data"))
		{
			MessageBoxA(nullptr, "��������Ӧ���ݴ���", "��ʾ", MB_OK | MB_ICONWARNING);
			return;
		}
		jsonValue = jsonResData.take("data");
		if (jsonValue.isArray())
		{
			QJsonArray jsonArray = jsonValue.toArray();
			jsonValue = jsonArray.at(0);
		}
		/*\ �жϸÿ��Ƿ���� \*/
		/*if (!this->JuageCardNumExistWithMenJin(m_opSelMenJinInfo->m_qsCardNum.toLocal8Bit().data()))
		{
			MessageBoxA(nullptr, "���Ų�����", "��ʾ", MB_OK | MB_ICONWARNING);
			return;
		}*/
		/*\ ��ȡ�û��������ݣ��ж��û����޸�ʲô���� \*/
		//m_opModifyUserInfo->m_qsCardNum = m_oUi.m_editCardNum->text();
		m_opModifyUserInfo->m_qsCardPass = m_oUi.m_editCardPass->text();
		m_opModifyUserInfo->m_qsUserName = m_oUi.m_editUserName->text();
		m_opModifyUserInfo->m_qsUserNum = m_oUi.m_editUserNum->text();
		m_opModifyUserInfo->m_qsPicPath = m_oUi.m_linePath->text();
		m_opModifyUserInfo->m_iCardType = m_oUi.m_cbCardType->currentIndex();
		m_opModifyUserInfo->m_iUserType = m_oUi.m_cbUserType->currentIndex();
		/*\ ����������ʹ�õĽṹ��ָ�� \*/
		NET_DVR_CARD_CFG_COND* opCreateLongInfo = new NET_DVR_CARD_CFG_COND();
		/*\ �����޸Ŀ���Ϣ�Ľṹ��ָ�� \*/
		NET_DVR_CARD_CFG_V50* opModifyInfo = new NET_DVR_CARD_CFG_V50();
		opCreateLongInfo->dwSize = sizeof(NET_DVR_CARD_CFG_COND);
		opCreateLongInfo->dwCardNum = 1;
		opCreateLongInfo->byCheckCardNo = 1;
		/*\ ���������� \*/
		m_iLongConnHandle = -1;
		m_iLongConnHandle = NET_DVR_StartRemoteConfig(
			m_iLoginHandle,
			NET_DVR_SET_CARD_CFG_V50,
			opCreateLongInfo,
			sizeof(NET_DVR_CARD_CFG_COND),
			nullptr,
			nullptr
		);
		if (m_iLongConnHandle == -1)
		{
			MessageBoxA(nullptr, "�޸��û���Ϣʧ��", "��ʾ", MB_OK | MB_ICONWARNING);
			goto Free;
			return;
		}
		/*\ �ж��û����������ĸ�,�����޸��� \*/
		if (m_opModifyUserInfo->m_qsCardPass != "")
		{
			dwType |= 1 << 7;
			strcpy((char*)opModifyInfo->byCardPassword, m_opModifyUserInfo->m_qsCardNum.toLocal8Bit().data());
		}
		if (m_opModifyUserInfo->m_qsUserName != "")
		{
			dwType |= 1 << 11;
			strcpy((char*)opModifyInfo->byName, m_opModifyUserInfo->m_qsUserName.toLocal8Bit().data());
		}
		if (m_opModifyUserInfo->m_qsUserNum != "")
		{
			/*\ �жϹ��������Ƿ�Ϸ� \*/
			if (!CUtils::GetInstance()->JuageNumberLegal(m_opModifyUserInfo->m_qsUserNum))
			{
				MessageBoxA(nullptr, "������Ϸ��Ĺ���0-999999", "��ʾ", MB_OK | MB_ICONWARNING);
				return;
			}
			dwType |= 1 << 10;
			opModifyInfo->dwEmployeeNo = m_opModifyUserInfo->m_qsUserNum.toUInt();
		}
		if (m_opModifyUserInfo->m_qsPicPath != "")
		{
			/*\ �ж�ͼƬ�ļ�·���Ƿ�Ϸ� \*/
			if (CUtils::GetInstance()->JuageNumberLegal(m_opModifyUserInfo->m_qsPicPath))
			{
				MessageBoxA(nullptr, "������Ϸ���ͼƬ·��", "��ʾ", MB_OK | MB_ICONWARNING);
				return;
			}
			/*\ �����·� \*/
			this->SendFaceInfoToMenJinHost(m_opModifyUserInfo->m_qsPicPath);
		}
		/*\ ��0��ʼ \*/
		if (gl_opSelCardInfo.byCardType != m_opModifyUserInfo->m_iCardType)
		{
			dwType |= 1 << 2;
			opModifyInfo->byCardType = m_opModifyUserInfo->m_iCardType + 1;
		}
		if (gl_opSelCardInfo.byUserType != m_opModifyUserInfo->m_iUserType)
		{
			dwType |= 1 << 17;
			opModifyInfo->byUserType = m_opModifyUserInfo->m_iUserType;
		}
		/*\ ���޸����ĸ��ֶε��ֶθ�ֵ \*/
		opModifyInfo->dwModifyParamType = dwType;
		opModifyInfo->dwSize = sizeof(NET_DVR_CARD_CFG_V50);
		strcpy((char*)opModifyInfo->byCardNo, jsonValue["cardNumber"].toString().toLocal8Bit().data());
		//strcpy((char*)opModifyInfo->byCardNo, m_opModifyUserInfo->m_qsCardNum.toLocal8Bit().data());
		if (!NET_DVR_SendRemoteConfig(
			m_iLongConnHandle,
			ENUM_ACS_SEND_DATA,
			(char*)opModifyInfo,
			sizeof(NET_DVR_CARD_CFG_V50)
		))
		{
			MessageBoxA(nullptr, "�޸��û���Ϣʧ��", "��ʾ", MB_OK | MB_ICONWARNING);
			goto Free;
			return;
		}
		else
		{
			char chId[32] = { 0 };
			/*\ ͼƬ��url��ַ \*/
			std::string sUrl = "http://" + gl_opSaveIniInfo->sIp + ":" +
				gl_opSaveIniInfo->sPort + "/headinfo" + jsonValue["imgName"].toString().toLocal8Bit().data();
			int sUserId = jsonValue["id"].toInt();
			QString qsImgName = jsonValue["imgName"].toString();
			/*\ ������������޸����� \*/
			this->SendModifyUserToHttpSvr(QString::number(sUserId), QString(sUrl.c_str()), qsImgName);
		}
	Free:
		if (opCreateLongInfo != nullptr)
		{
			delete opCreateLongInfo;
			opCreateLongInfo = nullptr;
		}
		if (opModifyInfo != nullptr)
		{
			delete opModifyInfo;
			opModifyInfo = nullptr;
		}
	}
}

/****************************************!
*@brief  �жϿ����Ƿ����(�Ž�����)
*@author Jinzi
*@date   2019/09/23 15:17:20
*@param[in]
   _cchpCardNum	:	����
*@param[out]
*@return
****************************************/
bool fmChangeUser::JuageCardNumExistWithMenJin(const char* _cchpCardNum)
{
	bool IsSucc = false;
	NET_DVR_CARD_CFG_COND* opGetCardInfo = new	NET_DVR_CARD_CFG_COND();
	NET_DVR_CARD_CFG_SEND_DATA* opSendCardInfo = new NET_DVR_CARD_CFG_SEND_DATA();
	opGetCardInfo->dwSize = sizeof(NET_DVR_CARD_CFG_COND);
	opGetCardInfo->dwCardNum = 1;
	opGetCardInfo->byCheckCardNo = 1;
	m_iLongConnHandle = NET_DVR_StartRemoteConfig(
		m_opSelMenJinInfo->m_iSelMenJinHandle,
		NET_DVR_GET_CARD_CFG_V50,
		opGetCardInfo,
		sizeof(NET_DVR_CARD_CFG_COND),
		(fRemoteConfigCallback)HikLongConnCallBackFunc,
		nullptr
	);
	/*\ ���ûص��������� \*/
	gl_uiCallBackType = NET_DVR_GET_CARD_CFG_V50;
	if (m_iLongConnHandle == -1)
	{
		MessageBoxA(nullptr, "�޸Ŀ���Ϣʧ��", "��ʾ", MB_OK | MB_ICONWARNING);
		goto Free;
	}
	opSendCardInfo->dwSize = sizeof(NET_DVR_CARD_CFG_SEND_DATA);
	strcpy((char*)opSendCardInfo->byCardNo, _cchpCardNum);

	if (!NET_DVR_SendRemoteConfig(
		m_iLongConnHandle,
		ENUM_ACS_SEND_DATA,
		(char*)opSendCardInfo,
		sizeof(NET_DVR_CARD_CFG_SEND_DATA)
	))
	{
		IsSucc = true;
	}
Free:
	if (opSendCardInfo != nullptr)
	{
		delete opSendCardInfo;
		opSendCardInfo = nullptr;
	}
	if (opSendCardInfo != nullptr)
	{
		delete opSendCardInfo;
		opSendCardInfo = nullptr;
	}
	NET_DVR_StopRemoteConfig(m_iLongConnHandle);
	return IsSucc;
}

/****************************************!
*@brief  �����޸��û���Ϣ�����ݵ�������
*@author Jinzi
*@date   2019/09/24 16:38:04
*@param[in]
	_sUserId	:	�û�id
	_sPicPath	:	ҪУ���ͼƬ·��
*@param[out]
*@return
****************************************/
void fmChangeUser::SendModifyUserToHttpSvr(QString& _sUserId, QString& _sPicPath, QString& _qsImgName)
{
	bool IsOwn = false;
	std::string sUrl = "http://" + gl_opSaveIniInfo->sIp + ":" + gl_opSaveIniInfo->sPort + "/patroluser/updatePatrolUser";
	/*\ У��ͼƬ�Ƿ���һ���� \*/
	QByteArray oSvrPicData = m_opHttpRequest->HttpGetPicDataWithUrl(_sPicPath.toLocal8Bit().data());
	oSvrPicData = oSvrPicData.toBase64();
	QByteArray oLocalPicData = CUtils::GetInstance()->LocalImageToBase64(m_opModifyUserInfo->m_qsPicPath);
	QJsonObject json;
	/*\ ����ȵĻ�����Ϊtrue \*/
	if (oSvrPicData == oLocalPicData)
	{
		json.insert("file", "");
	}
	else
	{
		IsOwn = true;
		json.insert("file", QString(oLocalPicData));
	}
	json.insert("id", _sUserId);
	json.insert("cardNumber", m_opModifyUserInfo->m_qsCardNum);
	json.insert("jobNumber", m_opModifyUserInfo->m_qsUserNum);
	json.insert("userName", m_opModifyUserInfo->m_qsUserName);
	json.insert("isOwn", IsOwn);
	json.insert("faceTrait", "");
	json.insert("imgName", _qsImgName);
	QJsonDocument jsonDocument(json);
	QByteArray oReqData = jsonDocument.toJson();
	m_opHttpRequest->HttpPostRequest(sUrl.c_str(), oReqData,
		std::bind(&fmChangeUser::SvrRetModifyUserHandle, this, std::placeholders::_1));
}

/****************************************!
*@brief  �����������û��޸����ݵĽ���Ĵ�����
*@author Jinzi
*@date   2019/09/26 8:39:08
*@param[in]
	_opReplay	:	������ȡ�������ص����ݵĶ���ָ��
*@param[out]
*@return
****************************************/
void fmChangeUser::SvrRetModifyUserHandle(QNetworkReply* _opReplay)
{
	if (_opReplay == nullptr)
	{
		MessageBoxA(nullptr, "��������Ӧ����Ϊ��", "��ʾ", MB_OK | MB_ICONWARNING);
		this->close();
		return;
	}
	// ������ܵ�����;
	QByteArray replyContent = _opReplay->readAll();
	QJsonObject jsonResData = QJsonDocument::fromJson(replyContent).object();
	if (jsonResData.value("code") == 0)
	{
		MessageBoxA(nullptr, "�޸��û���Ϣ�ɹ�", "��ʾ", MB_OK);
		/*\ �رյ�ǰ���� \*/
		this->close();
	}
	else
	{
		MessageBoxA(nullptr, "�޸��û���Ϣʧ��", "��ʾ", MB_OK | MB_ICONWARNING);
		this->close();
	}
}

/****************************************!
*@brief  �޸��û�������Ϣ��ʼ�����������������û����������ΪĬ��
*@author Jinzi
*@date   2019/09/29 9:42:11
*@param[in]
*@param[out]
*@return
****************************************/
void fmChangeUser::fmChangeUserInit()
{
	m_oUi.m_editCardPass->setText(QString::fromLocal8Bit(""));
	m_oUi.m_editUserName->setText(QString::fromLocal8Bit(""));
	m_oUi.m_editUserNum->setText(QString::fromLocal8Bit(""));
	m_oUi.m_linePath->setText(QString::fromLocal8Bit(""));
	m_oUi.m_cbCardType->setCurrentIndex(0);
	m_oUi.m_cbUserType->setCurrentIndex(0);
}

/****************************************!
*@brief  �·�������Ϣ,���Ž�����
*@author Jinzi
*@date   2019/10/08 16:38:09
*@param[in]
   _qsPicPath	:	ͼƬ·��
*@param[out]
*@return
****************************************/
void fmChangeUser::SendFaceInfoToMenJinHost(QString& _qsPicPath)
{
	NET_DVR_FACE_PARAM_COND oStartFaceInfo = { 0 };
	oStartFaceInfo.dwSize = sizeof(NET_DVR_FACE_PARAM_COND);
	oStartFaceInfo.dwFaceNum = 1;
	oStartFaceInfo.byFaceID = 1;
	oStartFaceInfo.byEnableCardReader[0] = 1;
	strcpy((char*)oStartFaceInfo.byCardNo, m_opModifyUserInfo->m_qsCardNum.toLocal8Bit().data());
	/*\ ���������� \*/
	m_iLongConnHandle = NET_DVR_StartRemoteConfig(
		m_iLoginHandle,
		NET_DVR_SET_FACE_PARAM_CFG,
		&oStartFaceInfo,
		static_cast<DWORD>(sizeof(NET_DVR_FACE_PARAM_COND)),
		nullptr,
		nullptr
	);
	if (m_iLongConnHandle == -1)
	{
		int iError = NET_DVR_GetLastError();
		MessageBoxA(nullptr, "���������Ϣʧ��", "��ʾ", MB_OK | MB_ICONWARNING);
		return;
	}
	/*\ �������� \*/
	NET_DVR_FACE_PARAM_CFG oSendFaceInfo = { 0 };
	oSendFaceInfo.dwSize = sizeof(NET_DVR_FACE_PARAM_CFG);
	oSendFaceInfo.byFaceDataType = 1;
	oSendFaceInfo.byFaceID = 1;
	oSendFaceInfo.byEnableCardReader[0] = 1;
	/*\ ���� \*/
	strcpy((char*)oSendFaceInfo.byCardNo, m_opModifyUserInfo->m_qsCardNum.toLocal8Bit().data());
	/*\ ��ȡ�ļ���Ϣ \*/
	//char chpBuf[200 * 1024] = { 0 };
	char* chpBuf = new char[200 * 1024];
	FILE* fileI = fopen(std::string(m_opModifyUserInfo->m_qsPicPath.toLocal8Bit().data()).c_str(), "rb");
	if (!fileI)
	{
		return;
	}
	/*\ ��char*�����ڴ� \*/
	int iLength = fread(chpBuf, 1, 200 * 1024, fileI);
	fclose(fileI);
	/*\ ��ֵ \*/
	oSendFaceInfo.pFaceBuffer = chpBuf;
	oSendFaceInfo.dwFaceLen = iLength;
	/*\ �������� \*/
	if (!NET_DVR_SendRemoteConfig(
		m_iLongConnHandle,
		ENUM_ACS_INTELLIGENT_IDENTITY_DATA,
		(char*)& oSendFaceInfo,
		static_cast<DWORD>(sizeof(NET_DVR_FACE_PARAM_CFG))))
	{
		int iError = -1;
		iError = NET_DVR_GetLastError();
		MessageBoxA(nullptr, "�·�������Ϣʧ��", "��ʾ", MB_OK | MB_ICONWARNING);
		NET_DVR_StopRemoteConfig(m_iLongConnHandle);
	}
	else
	{
		MessageBoxA(nullptr, "�·�������Ϣ�ɹ�", "��ʾ", MB_OK);
		NET_DVR_StopRemoteConfig(m_iLongConnHandle);
	}
}