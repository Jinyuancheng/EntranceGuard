/**************************************************************
 * @file fmMenJinLogin.h										  
 * @date 2019/09/10 11:25		
 * @author Jinzi                                              
 * @Contact: 2493349541@qq.com							      
 * @brief 登录窗口				  
***************************************************************/
#pragma once
#ifndef _FMMENJINLOGIN_H_
#define _FMMENJINLOGIN_H_

#include <qmainwindow.h>
#include <QWidget>
#include <vector>
#include <QString>
#include <windows.h>
#include <functional>

#include "ui_fmMenJinLogin.h"
#include "../Public/PublicDefine.h"
#include "../HIKSDK/HCNetSDK.h"

/*\ 用于子窗口调用父窗口中的函数的指针，用来设置登录数据 \*/
typedef std::function<void(std::vector<CLoginInfo>&)>	FuncSetLoginInfo;
/*\ 用于设置主窗口中的label信息 \*/
typedef std::function<void(const char*)>				FuncSetLabelInfo;
/*\ 用来返回listView选中的index \*/
typedef std::function<int(void)>						FuncGetListViewCheck;

/*\ 子窗口类 \*/
class fmMenJinLogin :
	public QMainWindow
{
	Q_OBJECT
public:
	fmMenJinLogin(QWidget* _oWidget);
	~fmMenJinLogin();
public slots:
	/*\ 门禁主机登录 \*/
	void HikMenJinLogin();
	/*\ 门禁主机登出 \*/
	void HikMenJinLoginOut();
	/*\ 所有门禁主机登出 \*/
	void HikMenJinLoginOutAll();
public:
	FuncSetLoginInfo			m_funcSetLoginInfo;	/*\ 用来设置主页面的登录信息 \*/
	FuncSetLabelInfo			m_funcSetLabelInfo;	/*\ 用来设置主窗体中的label信息 \*/
	FuncGetListViewCheck		m_funcGetListViewCheck;/*\ 用来返回listview的index \*/
private:
	Ui::MainWindow				m_oUi;				/*\ 门禁登录的ui界面 \*/
	std::vector<CLoginInfo>		m_vecLoginInfo;		/*\ 用来存储登录信息 \*/
	NET_DVR_USER_LOGIN_INFO*	m_opDevLoginInfo;	/*\ 用来门禁登录 \*/
	NET_DVR_DEVICEINFO_V40*		m_opDevInfo;		/*\ 用来接收登录成功后的数据 \*/
};
#endif
