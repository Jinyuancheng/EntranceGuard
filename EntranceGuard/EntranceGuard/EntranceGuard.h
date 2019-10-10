/**************************************************************
 * @file EntranceGuard.h
 * @date 2019/09/10 11:25
 * @author Jinzi
 * @Contact: 2493349541@qq.com
 * @brief 主窗口
***************************************************************/
#pragma once
#include <QtWidgets/QMainWindow>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QRect>
#include <vector>
#include <QNetworkAccessManager>
#include "ui_EntranceGuard.h"
#include "./fmMenJinLogin.h"
#include "../HIKSDK/HCNetSDK.h"
#include "../Public/PublicDefine.h"
#include "./fmAddUser.h"
#include "./fmChangeUser.h"

#ifndef _CHTTPREQUEST_H_
#include "./CHttpRequest.h"
#endif // !_CHTTPREQUEST_H_


/*\ 海康回调函数 \*/
BOOL MsgHikCallBack(LONG lCommand, NET_DVR_ALARMER* pAlarmer,
	char* pAlarmInfo, DWORD dwBufLen, void* pUser);
/*\ 连接信息的回调函数 \*/
void RemoteConfigCallBack(DWORD _dwType, void* _lpBuffer, DWORD _dwBufLen, void* _pUserData);

class EntranceGuard : public QMainWindow
{
	Q_OBJECT

public:
	EntranceGuard(QWidget* parent = Q_NULLPTR);
	~EntranceGuard();
protected:
	/*\ 鼠标点击事件,用来显示菜单栏等 \*/
	void mousePressEvent(QMouseEvent* _oEvent);
public:
	/*\ 初始化海康sdk \*/
	void InitHikSDK();
	/*\ 清除海康sdk \*/
	void CleanHikSDK();
	/*\ 显示treeView(门禁主机登录成功后调用) \*/
	void ShowListView();
	/*\ 绑定信号和槽 \*/
	void InitSignalAndSlot();
	/*\ 初始化存储长连接信息的结构体 \*/
	void InitSaveLongInfo();
	/*\ 请求并返回所有用户信息（http请求服务器） \*/
	void ShowAllUserInfoWithListView();
	/*\ 给子窗口的函数指针赋值 \*/
	void InitChildWinFuncAssign();
public:
	/*\ 设置登录信息 \*/
	void SetListLoinInfo(std::vector<CLoginInfo>& _lstListLoginInfo);
	/*\ 设置label显示信息 \*/
	void SetLabelShowInfo(const char* _cchpShowInfo);
	/*\ 得到listView中选择的节点 \*/
	int GetListViewCheck();
	/*\ 显示门禁用户信息 \*/
	void ShowMenJinUserInfo();
	/*\ 根据卡号 寻找到用户的id \*/
	QString RetUserIdWithJobNum(QString _qsCardNumber);
	/*\ 删除一个用户的信息 \*/
	void DelMenJinUserInfo();
	/*\ 执行请求获取所有用户信息的数据处理函数 \*/
	void HttpGetAllUserSuccDataHandle(QNetworkReply* _opReply);
public: //fmChangeUser 窗体调用的函数
	/*\ 返回用户选中的工号信息（tableView控件操作） \*/
	QString RetUserSelRowJobNum();
	/*\ 返回用户选择门禁主机的登录句柄 \*/
	int RetUserSelMenJinLoginHandle();
	/*\ 返回用户选中的卡号信息 \*/
	QString RetUserSelRowCardNum();
public:
	/*\ 建立长连接 \*/
	int CreateLongConn(int _iLoginHandle, DWORD _uiType, LPVOID _lpBuffer,
		DWORD _dwLength, LPVOID _lpUserData = nullptr);
	/*\ 关闭长连接 \*/
	bool CloseLongConn(int _iLongConnHandle);
private slots:
	/*\ 查询门禁信息 \*/
	void SelectMenJinInfo();
	/*\ 获取设备能力集 \*/
	void GetDevAbilty();
	/*\ 删除一个门禁的所有人员信息 \*/
	void DelMenJinOnAllUserInfo();
	/*\ 根据门禁主机登录，判断是否可以显示添加用户窗口 \*/
	void JuageShowAddUserFm();
	/*\ 下发用户信息 \*/
	void BeSentDownMenJin();
	/*\ 删除用户信息 \*/
	void DelUserInfoWithMenJin();
	/*\ 删除用户信息回调数据的处理函数 \*/
	void HttpDelUserInfoSuccDataHandle();
	/*\ 显示更改用户窗体 \*/
	void ShowFmChangeUserInfo();
	/*\ 判断工号是否存在的http响应函数处理 \*/
	void JuageJobNumResFuncHandle(QNetworkReply* _opReqplay);
private:
	Ui::EntranceGuardClass				ui;                     /*\ ui界面 \*/
	fmMenJinLogin*                      m_oFmMenJinLogin;       /*\ 门禁登录窗体 \*/
	fmAddUser*                          m_oFmAddUser;			/*\ 添加用户信息窗体 \*/
	fmChangeUser*						m_opFmChangeUser;		/*\ 修改用户信息窗体 \*/
	QMenu*                              m_oMenu;	            /*\ 右键菜单 \*/
	QAction*                            m_oAction1;	            /*\ 菜单栏下的第一列 \*/
	QAction*                            m_oAction2;	            /*\ 菜单栏下的第二列 \*/
	QAction*                            m_oAction3;	            /*\ 菜单栏下的第三列 \*/
	MSGCallBack_V31	                    m_funcMsgHikCallBack;   /*\ 海康回调函数 \*/
	std::vector<CLoginInfo>             m_vecLoginInfo;         /*\ 存储登录信息 \*/
	fRemoteConfigCallback	            m_funcMsgRemoteConfig;  /*\ 配置连接信息的回调函数 \*/
	CLongConnStatus*                    m_opLongConnInfo;		/*\ 长连接信息，句柄和状态 \*/
	NET_DVR_CARD_CFG_COND*              m_opCardInfo;			/*\ 查询卡使用的结构体 \*/
	NET_DVR_CARD_CFG_SEND_DATA*         m_opSendDataInfo;		/*\ 发送数据信息的时候使用的结构体 \*/
	QNetworkAccessManager*				m_opHttpManager;		/*\ http管理 \*/
	std::vector<CSvrRetAllUserInfo>		m_vecSvrRetAllUserInfo;	/*\ 初始化从服务器拿到的所有用户数据 \*/
	CHttpRequest*						m_opHttpRequest;		/*\ 用来发送http请求 \*/
};
