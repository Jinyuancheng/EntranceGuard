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


extern CSaveiniInfo* gl_opSaveIniInfo;			/*\ 存储的是配置文件的信息 \*/

unsigned int			gl_uiCallBackType = -1;	/*\ 长连接回调类型 \*/
NET_DVR_CARD_CFG_V50	gl_opSelCardInfo;		/*\ 查询的卡信息 \*/

/****************************************!
*@brief  配置远程连接信息的回调函数（长连接）
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
void HikLongConnCallBackFunc(DWORD dwType, void* lpBuffer, DWORD dwBufLen, void* pUserData)
{
	switch (dwType)
	{
		/*\ 回调回来的数据 \*/
	case NET_SDK_CALLBACK_TYPE_DATA:
		switch (gl_uiCallBackType)
		{
			/*\ 查询卡信息,回调数据 \*/
		case NET_DVR_GET_CARD_CFG_V50:
			gl_opSelCardInfo = *(static_cast<NET_DVR_CARD_CFG_V50*>(lpBuffer));
			break;
		}
		break;
	}
}

/****************************************!
*@brief  构造函数
*@author Jinzi
*@date   2019/09/23 14:19:44
*@param[in]
*@param[out]
*@return
****************************************/
fmChangeUser::fmChangeUser(QWidget* parent /* = Q_NULLPTR */)
{
	m_oUi.setupUi(this);
	/*\ 设置当前窗口后面的窗口不能点击 \*/
	setWindowModality(Qt::ApplicationModal);
	/*\ 隐藏最大化最小化按钮 \*/
	setWindowFlags(this->windowFlags() & ~(Qt::WindowMinMaxButtonsHint));
	/*\ 初始化变量 \*/
	this->InitMemberVar();
	/*\ 绑定信号和槽 \*/
	this->BindSignalAndSlots();
}

/****************************************!
*@brief  析构函数
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
*@brief  绑定信号和槽
*@author Jinzi
*@date   2019/09/23 14:21:51
*@param[in]
*@param[out]
*@return
****************************************/
void fmChangeUser::BindSignalAndSlots()
{
	/*\ 修改按钮点击事件 \*/
	QObject::connect(m_oUi.m_btnAdd, SIGNAL(clicked()), this, SLOT(BtnChangeClickEvent()));
	/*\ 返回按钮点击事件 \*/
	QObject::connect(m_oUi.m_btnQuit, SIGNAL(clicked()), this, SLOT(close()));
	/*\ 获取图片路径按钮点击事件 \*/
	QObject::connect(m_oUi.m_btnPicPath, SIGNAL(clicked()), this, SLOT(BtnGetPicPathClickEvent()));
}

/****************************************!
*@brief  初始化成员变量
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
*@brief  修改按钮点击事件
*@author Jinzi
*@date   2019/09/23 14:26:50
*@param[in]
*@param[out]
*@return
****************************************/
void fmChangeUser::BtnChangeClickEvent()
{
	/*\ 根据工号 判断用户是否在服务器中存在,存在返回id \*/
	std::string sUrl = "http://" + gl_opSaveIniInfo->sIp + ":" + gl_opSaveIniInfo->sPort + "/patroluser/getPatrolUserByJobNum";
	if (m_funcRetUserSelMenJinLoginHandle != nullptr)
	{
		m_iLoginHandle = m_funcRetUserSelMenJinLoginHandle();
	}
	/*\ 获取用户选中的工号 \*/
	if (m_funcRetUserSelCardNum != nullptr)
	{
		m_qsJobNum = m_funcRetUserSelCardNum();
	}
	/*\ 发送http请求 \*/
	QJsonObject jsonObject;
	jsonObject.insert("jobNumber", m_qsJobNum);
	QJsonDocument jsonDocument(jsonObject);
	QByteArray oReqData(jsonDocument.toJson());
	/*\ 发送http请求 \*/
	m_opHttpRequest->HttpPostRequest(sUrl.c_str(), oReqData,
		std::bind(&fmChangeUser::ModifyUserInfoWithCardNum, this, std::placeholders::_1));
}

/****************************************!
*@brief  获取图片路径按钮的点击事件
*@author Jinzi
*@date   2019/09/23 14:28:19
*@param[in]
*@param[out]
*@return
****************************************/
void fmChangeUser::BtnGetPicPathClickEvent()
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
	if (fileNames.size() > 0)
	{
		m_oUi.m_linePath->setText(fileNames[0]);
	}
}

/****************************************!
*@brief  设置选中门禁主机信息
*@author Jinzi
*@date   2019/09/23 14:52:41
*@param[in]
	_oSelMenJinInfo	:	存储选中的门禁主机和对应的用户id等信息
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
*@brief  根据卡号修改用户信息
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
		MessageBoxA(nullptr, "服务器响应数据为空", "提示", MB_OK | MB_ICONWARNING);
		return;
	}
	// 保存接受的数据;
	QByteArray replyContent = _opReplay->readAll();
	QJsonObject jsonResData = QJsonDocument::fromJson(replyContent).object();
	QJsonValue jsonValue;
	DWORD dwType = 0x00000000;
	if (jsonResData.value("code") == 0)
	{
		if (!jsonResData.contains("data"))
		{
			MessageBoxA(nullptr, "服务器响应数据错误", "提示", MB_OK | MB_ICONWARNING);
			return;
		}
		jsonValue = jsonResData.take("data");
		if (jsonValue.isArray())
		{
			QJsonArray jsonArray = jsonValue.toArray();
			jsonValue = jsonArray.at(0);
		}
		/*\ 判断该卡是否存在 \*/
		/*if (!this->JuageCardNumExistWithMenJin(m_opSelMenJinInfo->m_qsCardNum.toLocal8Bit().data()))
		{
			MessageBoxA(nullptr, "卡号不存在", "提示", MB_OK | MB_ICONWARNING);
			return;
		}*/
		/*\ 获取用户输入数据，判断用户都修改什么数据 \*/
		//m_opModifyUserInfo->m_qsCardNum = m_oUi.m_editCardNum->text();
		m_opModifyUserInfo->m_qsCardPass = m_oUi.m_editCardPass->text();
		m_opModifyUserInfo->m_qsUserName = m_oUi.m_editUserName->text();
		m_opModifyUserInfo->m_qsUserNum = m_oUi.m_editUserNum->text();
		m_opModifyUserInfo->m_qsPicPath = m_oUi.m_linePath->text();
		m_opModifyUserInfo->m_iCardType = m_oUi.m_cbCardType->currentIndex();
		m_opModifyUserInfo->m_iUserType = m_oUi.m_cbUserType->currentIndex();
		/*\ 建立长连接使用的结构体指针 \*/
		NET_DVR_CARD_CFG_COND* opCreateLongInfo = new NET_DVR_CARD_CFG_COND();
		/*\ 用来修改卡信息的结构体指针 \*/
		NET_DVR_CARD_CFG_V50* opModifyInfo = new NET_DVR_CARD_CFG_V50();
		opCreateLongInfo->dwSize = sizeof(NET_DVR_CARD_CFG_COND);
		opCreateLongInfo->dwCardNum = 1;
		opCreateLongInfo->byCheckCardNo = 1;
		/*\ 建立长连接 \*/
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
			MessageBoxA(nullptr, "修改用户信息失败", "提示", MB_OK | MB_ICONWARNING);
			goto Free;
			return;
		}
		/*\ 判断用户都更改了哪个,返回修改项 \*/
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
			/*\ 判断工号输入是否合法 \*/
			if (!CUtils::GetInstance()->JuageNumberLegal(m_opModifyUserInfo->m_qsUserNum))
			{
				MessageBoxA(nullptr, "请输入合法的工号0-999999", "提示", MB_OK | MB_ICONWARNING);
				return;
			}
			dwType |= 1 << 10;
			opModifyInfo->dwEmployeeNo = m_opModifyUserInfo->m_qsUserNum.toUInt();
		}
		if (m_opModifyUserInfo->m_qsPicPath != "")
		{
			/*\ 判断图片文件路径是否合法 \*/
			if (CUtils::GetInstance()->JuageNumberLegal(m_opModifyUserInfo->m_qsPicPath))
			{
				MessageBoxA(nullptr, "请输入合法的图片路径", "提示", MB_OK | MB_ICONWARNING);
				return;
			}
			/*\ 人脸下发 \*/
			this->SendFaceInfoToMenJinHost(m_opModifyUserInfo->m_qsPicPath);
		}
		/*\ 从0开始 \*/
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
		/*\ 给修改了哪个字段的字段赋值 \*/
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
			MessageBoxA(nullptr, "修改用户信息失败", "提示", MB_OK | MB_ICONWARNING);
			goto Free;
			return;
		}
		else
		{
			char chId[32] = { 0 };
			/*\ 图片的url地址 \*/
			std::string sUrl = "http://" + gl_opSaveIniInfo->sIp + ":" +
				gl_opSaveIniInfo->sPort + "/headinfo" + jsonValue["imgName"].toString().toLocal8Bit().data();
			int sUserId = jsonValue["id"].toInt();
			QString qsImgName = jsonValue["imgName"].toString();
			/*\ 像服务器发送修改数据 \*/
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
*@brief  判断卡号是否存在(门禁主机)
*@author Jinzi
*@date   2019/09/23 15:17:20
*@param[in]
   _cchpCardNum	:	卡号
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
	/*\ 设置回调函数类型 \*/
	gl_uiCallBackType = NET_DVR_GET_CARD_CFG_V50;
	if (m_iLongConnHandle == -1)
	{
		MessageBoxA(nullptr, "修改卡信息失败", "提示", MB_OK | MB_ICONWARNING);
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
*@brief  发送修改用户信息的数据到服务器
*@author Jinzi
*@date   2019/09/24 16:38:04
*@param[in]
	_sUserId	:	用户id
	_sPicPath	:	要校验的图片路径
*@param[out]
*@return
****************************************/
void fmChangeUser::SendModifyUserToHttpSvr(QString& _sUserId, QString& _sPicPath, QString& _qsImgName)
{
	bool IsOwn = false;
	std::string sUrl = "http://" + gl_opSaveIniInfo->sIp + ":" + gl_opSaveIniInfo->sPort + "/patroluser/updatePatrolUser";
	/*\ 校验图片是否是一样的 \*/
	QByteArray oSvrPicData = m_opHttpRequest->HttpGetPicDataWithUrl(_sPicPath.toLocal8Bit().data());
	oSvrPicData = oSvrPicData.toBase64();
	QByteArray oLocalPicData = CUtils::GetInstance()->LocalImageToBase64(m_opModifyUserInfo->m_qsPicPath);
	QJsonObject json;
	/*\ 不相等的话设置为true \*/
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
*@brief  服务器返回用户修改数据的结果的处理函数
*@author Jinzi
*@date   2019/09/26 8:39:08
*@param[in]
	_opReplay	:	用来获取服务器回调数据的对象指针
*@param[out]
*@return
****************************************/
void fmChangeUser::SvrRetModifyUserHandle(QNetworkReply* _opReplay)
{
	if (_opReplay == nullptr)
	{
		MessageBoxA(nullptr, "服务器响应数据为空", "提示", MB_OK | MB_ICONWARNING);
		this->close();
		return;
	}
	// 保存接受的数据;
	QByteArray replyContent = _opReplay->readAll();
	QJsonObject jsonResData = QJsonDocument::fromJson(replyContent).object();
	if (jsonResData.value("code") == 0)
	{
		MessageBoxA(nullptr, "修改用户信息成功", "提示", MB_OK);
		/*\ 关闭当前窗口 \*/
		this->close();
	}
	else
	{
		MessageBoxA(nullptr, "修改用户信息失败", "提示", MB_OK | MB_ICONWARNING);
		this->close();
	}
}

/****************************************!
*@brief  修改用户窗体信息初始化。用来将窗口中用户输入的数据为默认
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
*@brief  下发人脸信息,到门禁主机
*@author Jinzi
*@date   2019/10/08 16:38:09
*@param[in]
   _qsPicPath	:	图片路径
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
	/*\ 建立长连接 \*/
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
		MessageBoxA(nullptr, "添加人脸信息失败", "提示", MB_OK | MB_ICONWARNING);
		return;
	}
	/*\ 发送数据 \*/
	NET_DVR_FACE_PARAM_CFG oSendFaceInfo = { 0 };
	oSendFaceInfo.dwSize = sizeof(NET_DVR_FACE_PARAM_CFG);
	oSendFaceInfo.byFaceDataType = 1;
	oSendFaceInfo.byFaceID = 1;
	oSendFaceInfo.byEnableCardReader[0] = 1;
	/*\ 卡号 \*/
	strcpy((char*)oSendFaceInfo.byCardNo, m_opModifyUserInfo->m_qsCardNum.toLocal8Bit().data());
	/*\ 读取文件信息 \*/
	//char chpBuf[200 * 1024] = { 0 };
	char* chpBuf = new char[200 * 1024];
	FILE* fileI = fopen(std::string(m_opModifyUserInfo->m_qsPicPath.toLocal8Bit().data()).c_str(), "rb");
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
		m_iLongConnHandle,
		ENUM_ACS_INTELLIGENT_IDENTITY_DATA,
		(char*)& oSendFaceInfo,
		static_cast<DWORD>(sizeof(NET_DVR_FACE_PARAM_CFG))))
	{
		int iError = -1;
		iError = NET_DVR_GetLastError();
		MessageBoxA(nullptr, "下发人脸信息失败", "提示", MB_OK | MB_ICONWARNING);
		NET_DVR_StopRemoteConfig(m_iLongConnHandle);
	}
	else
	{
		MessageBoxA(nullptr, "下发人脸信息成功", "提示", MB_OK);
		NET_DVR_StopRemoteConfig(m_iLongConnHandle);
	}
}