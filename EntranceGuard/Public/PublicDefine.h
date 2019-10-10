#pragma once
#ifndef _PUBLICDEFINE_H_
#define _PUBLICDEFINE_H_
#include <iostream>
#include <QString>

/*\ �����洢�Ž����� \*/
typedef struct
{
	QString m_sNodeName;	/*\ �ڵ����� \*/
	QString m_sIp;			/*\ �Ž���¼ip \*/
	QString m_sPort;		/*\ �Ž���¼�˿� \*/
	QString m_sUser;		/*\ �Ž���¼�û��� \*/
	QString m_sPass;		/*\ �Ž���¼���� \*/
	int		m_iLoginHandle;	/*\ �Ž���¼��� \*/
	bool	m_bIsLogin;		/*\ �Ž��Ƿ��¼ \*/
}CLoginInfo, * PCLoginInfo;

/*\ �����洢 �����Ӿ��������״̬ \*/
typedef struct
{
	int m_iLongConnHandle;	/*\ ���������Ӿ�� \*/
	bool m_bIsLongConn;		/*\ �������Ƿ����� \*/
	int m_iLoginHandle;		/*\ ��¼��� \*/
}CLongConnStatus, * PCLongConnStatus;

/*\ �����洢�����ļ��е���Ϣ \*/
typedef struct
{
	std::string sPort;		     /*\ ��˶˿� \*/
	std::string sfPort;			 /*\ ǰ�˶˿� \*/
	std::string	sIp;		     /*\ ip \*/
	std::string sReadPicDllPath; /*\ ReadPic.dll·�� \*/
}CSaveiniInfo, * PCSaveiniInfo;

/*\ �����Ա��Ϣ�Ľṹ�� \*/
typedef struct
{
	QString m_qsCardNum;	/*\ ���� \*/
	QString m_qsCardPass;	/*\ ������ \*/
	QString m_qsUserName;	/*\ �û��� \*/
	QString m_qsUserNum;	/*\ ���� \*/
	QString m_qsPicPath;	/*\ ͼƬ·�� \*/
}CAddUserInfo, * PCAddUserInfo;

/*\ �����洢�ӷ�������ȡ���û���Ϣ \*/
typedef struct
{
	QString			m_qsId;			/*\ �û�id \*/
	QString			m_qsUserName;	/*\ �û����� \*/
	QString			m_qsJobNumber;	/*\ �û����� \*/
	QString			m_qsCardNumber;	/*\ �û����� \*/
}CSvrRetAllUserInfo, * PCSvrRetAllUserInfo;

/*\ �����洢�û��޸ĵ����ĸ��Ž������е��ĸ�������Ϣ \*/
typedef struct
{
	int		m_iSelMenJinHandle;	/*\ ��ǰѡ�е��Ž����� \*/
	QString m_qsCardNum;		/*\ ��ǰѡ�еĿ��� \*/
	QString m_qsUserId;			/*\ ��ǰ���Ŷ�Ӧ���û�id \*/
	QString m_qsJobNum;			/*\ ��ǰѡ�еĹ��� \*/
}CSelMenJinInfo, * PCSelMenJinInfo;

/*\ �޸���Ա��Ϣ�Ľṹ�� \*/
typedef struct
{
	QString m_qsCardNum;	/*\ ���� \*/
	QString m_qsCardPass;	/*\ ������ \*/
	QString m_qsUserName;	/*\ �û��� \*/
	QString m_qsUserNum;	/*\ ���� \*/
	QString m_qsPicPath;	/*\ ͼƬ·�� \*/
	int		m_iUserType;	/*\ �û����� 0,��ͨ�û�1,����Ա \*/
	int		m_iCardType;	/*\ ������ \*/
}CModifyUserInfo, * PCModifyUserInfo;

#endif
