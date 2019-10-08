#pragma once
#include <qmainwindow.h>
#include <iostream>
#include "ui_fmAddUser.h"
#include <QNetworkAccessManager>
#include <functional>
#include <vector>
#include <windows.h>

#ifndef _PUBLICDEFINE_H_
#include "../Public/PublicDefine.h"
#endif // !_PUBLICDEFINE_H_

/*\ 用来调用父窗口中的函数 \*/
typedef std::function<int(void)> FuncGetListViewCheck;

/*\ 连接信息的回调函数 \*/
void AddUserRemoteConfigCallBack(DWORD _dwType, void* _lpBuffer, DWORD _dwBufLen, void* _pUserData);

class fmAddUser :
	public QMainWindow
{
	Q_OBJECT
public:
	fmAddUser(QWidget* parent = Q_NULLPTR);
	~fmAddUser();
public:
	/*\ 初始化控件状态等信息 \*/
	void InitControlInfo();
	/*\ 初始化成员变量 \*/
	void InitMemberVar();
public:
	/*\ 读取配置文件 \*/
	void ReadIniFile(std::string _sFilePath);
public:
	/*\ 绑定信号和槽 \*/
	void BindSignalAndSlot();
	/*\ qt图片base64编码 \*/
	QByteArray ImageToBase64(QString _qsPicPath);
	/*\ 向门禁设备中添加卡的信息 \*/
	void AddUserInfoToMenJin();
	/*\ 设置登录信息 \*/
	void SetLoginInfo(std::vector<CLoginInfo>& _vecLoginInfo);
	/*\ 初始化m_opLongConnStatus \*/
	void InitLongConnStatus();
	/*\ 发送下发卡的信息 \*/
	void SendCardInfoToMenJin();
	/*\ 判断用户输入lineEdit中的数据是否合法 \*/
	void JuageUserInputData();
	/*\ 录入人脸信息 \*/
	void SendCardSuccInputFaceInfo();
	/*\ 读取图片信息到文件流 \*/
	char* ReadPicInfoToStream();
public slots:
	/*\ 添加用户按钮点击事件 \*/
	void BtnAddUserInfo();
	/*\ 打开文件夹路径 \*/
	void OpenFloderPath();
	/*\ 得到 添加用户的回调数据处理函数 \*/
	void HttpAddUserRetDataHandle();
public:
	FuncGetListViewCheck	m_funcGetListViewCheck; /*\ 用来调用主窗口中的函数 \*/
private:
	Ui::CFmAddUser			m_oUi;				/*\ 用来操作ui界面 \*/
	QNetworkAccessManager*	m_opHttpManager;	/*\ http请求管理 \*/
	CAddUserInfo*			m_opAddUserInfo;	/*\ 存储添加用户信息 \*/
	std::vector<CLoginInfo> m_vecLoginInfo;     /*\ 存储登录信息 \*/
	CLongConnStatus*		m_opLongConnStatus;	/*\ 长连接信息 \*/
};

