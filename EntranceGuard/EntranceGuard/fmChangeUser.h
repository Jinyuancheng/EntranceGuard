/**************************************************************
 * @file fmChangeUser.h
 * @date 2019/09/23 14:06
 * @author Jinzi
 * @Contact: 2493349541@qq.com
 * @brief 该文件是修改卡信息窗口处理
***************************************************************/
#pragma once
#ifndef _FMCHANGEUSER_H_
#define _FMCHANGEUSER_H_
#include <qmainwindow.h>
#include <QString>
#include <windows.h>
#include <functional>
#include <QNetworkReply>

#include "ui_fmChangeUser.h"

#include "../Public/PublicDefine.h"

#ifndef _CHTTPREQUEST_H_
#include "./CHttpRequest.h"
#endif // !_CHTTPREQUEST_H_
#include "./CHttpRequest.h"

/*\ 用来调用主窗口中的函数,用来返回用户选中行(tableView控件) \*/
typedef std::function<QString(void)>	FuncRetUserSelCardNum;
/*\ 获取用户选择的门禁主机的登录句柄 \*/
typedef std::function<int(void)>		FuncRetUserSelMenJinLoginHandle;

/*\ 长连接回调函数 \*/
void HikLongConnCallBackFunc(DWORD dwType, void* lpBuffer, DWORD dwBufLen, void* pUserData);

class fmChangeUser :
	public QMainWindow
{
	Q_OBJECT
public:
	fmChangeUser(QWidget* parent = Q_NULLPTR);
	~fmChangeUser();
public:
	/*\ 绑定信号和槽 \*/
	void BindSignalAndSlots();
	/*\ 初始化成员变量 \*/
	void InitMemberVar();
public:
	/*\ 判断卡号是否存在(门禁主机) \*/
	bool JuageCardNumExistWithMenJin(const char* _cchpCardNum);
	/*\ 向服务器发送修改用户信息的请求 \*/
	void SendModifyUserToHttpSvr(QString& _sUserId, QString& _sPicPath, QString& _qsImgName);
public:
	/*\ 设置存储门禁信息结构体数据 \*/
	void SetSelMenJinInfo(CSelMenJinInfo& _oSelMenJinInfo);
public slots:
	/*\ 修改按钮点击事件 \*/
	void BtnChangeClickEvent();
	/*\ 获取图片路径点击事件 \*/
	void BtnGetPicPathClickEvent();
	/*\ 根据卡号修改用户信息 \*/
	void ModifyUserInfoWithCardNum(QNetworkReply* _opReplay);
	/*\ 请求修改用户信息的回调数据处理 \*/
	void SvrRetModifyUserHandle(QNetworkReply* _opReplay);
public:
	/*\ 用来调用主窗口中的函数，用来获取选中行 \*/
	FuncRetUserSelCardNum				m_funcRetUserSelCardNum;
	/*\ 用来调用主窗口中的函数,用啦获取用户选中门禁主机的登录句柄 \*/
	FuncRetUserSelMenJinLoginHandle		m_funcRetUserSelMenJinLoginHandle;
private:
	Ui::CFmChangeUser		m_oUi;				/*\ 用来操作ui界面 \*/
	CSelMenJinInfo* m_opSelMenJinInfo;	/*\ 存储门禁信息结构体 \*/
	CHttpRequest* m_opHttpRequest;	/*\ http请求对象 \*/
	int						m_iLongConnHandle;	/*\ 长连接连接句柄 \*/
	CModifyUserInfo* m_opModifyUserInfo;	/*\ 存储修改用户信息的结构体 \*/
	int						m_iLoginHandle;		/*\ 门禁主机登录句柄(用户选中) \*/
	QString					m_qsJobNum;		/*\ 用户选中数据的工号 \*/
};

#endif



