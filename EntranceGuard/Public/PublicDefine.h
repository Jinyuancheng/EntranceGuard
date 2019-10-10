#pragma once
#ifndef _PUBLICDEFINE_H_
#define _PUBLICDEFINE_H_
#include <iostream>
#include <QString>

/*\ 用来存储门禁数据 \*/
typedef struct
{
	QString m_sNodeName;	/*\ 节点名称 \*/
	QString m_sIp;			/*\ 门禁登录ip \*/
	QString m_sPort;		/*\ 门禁登录端口 \*/
	QString m_sUser;		/*\ 门禁登录用户名 \*/
	QString m_sPass;		/*\ 门禁登录密码 \*/
	int		m_iLoginHandle;	/*\ 门禁登录句柄 \*/
	bool	m_bIsLogin;		/*\ 门禁是否登录 \*/
}CLoginInfo, * PCLoginInfo;

/*\ 用来存储 长连接句柄和连接状态 \*/
typedef struct
{
	int m_iLongConnHandle;	/*\ 长连接连接句柄 \*/
	bool m_bIsLongConn;		/*\ 长连接是否连接 \*/
	int m_iLoginHandle;		/*\ 登录句柄 \*/
}CLongConnStatus, * PCLongConnStatus;

/*\ 用来存储配置文件中的信息 \*/
typedef struct
{
	std::string sPort;		     /*\ 后端端口 \*/
	std::string sfPort;			 /*\ 前端端口 \*/
	std::string	sIp;		     /*\ ip \*/
	std::string sReadPicDllPath; /*\ ReadPic.dll路径 \*/
}CSaveiniInfo, * PCSaveiniInfo;

/*\ 添加人员信息的结构体 \*/
typedef struct
{
	QString m_qsCardNum;	/*\ 卡号 \*/
	QString m_qsCardPass;	/*\ 卡密码 \*/
	QString m_qsUserName;	/*\ 用户名 \*/
	QString m_qsUserNum;	/*\ 工号 \*/
	QString m_qsPicPath;	/*\ 图片路径 \*/
}CAddUserInfo, * PCAddUserInfo;

/*\ 用来存储从服务器获取的用户信息 \*/
typedef struct
{
	QString			m_qsId;			/*\ 用户id \*/
	QString			m_qsUserName;	/*\ 用户名称 \*/
	QString			m_qsJobNumber;	/*\ 用户工号 \*/
	QString			m_qsCardNumber;	/*\ 用户卡号 \*/
}CSvrRetAllUserInfo, * PCSvrRetAllUserInfo;

/*\ 用来存储用户修改的是哪个门禁主机中的哪个卡号信息 \*/
typedef struct
{
	int		m_iSelMenJinHandle;	/*\ 当前选中的门禁主机 \*/
	QString m_qsCardNum;		/*\ 当前选中的卡号 \*/
	QString m_qsUserId;			/*\ 当前卡号对应的用户id \*/
	QString m_qsJobNum;			/*\ 当前选中的工号 \*/
}CSelMenJinInfo, * PCSelMenJinInfo;

/*\ 修改人员信息的结构体 \*/
typedef struct
{
	QString m_qsCardNum;	/*\ 卡号 \*/
	QString m_qsCardPass;	/*\ 卡密码 \*/
	QString m_qsUserName;	/*\ 用户名 \*/
	QString m_qsUserNum;	/*\ 工号 \*/
	QString m_qsPicPath;	/*\ 图片路径 \*/
	int		m_iUserType;	/*\ 用户类型 0,普通用户1,管理员 \*/
	int		m_iCardType;	/*\ 卡类型 \*/
}CModifyUserInfo, * PCModifyUserInfo;

#endif
