/**************************************************************
 * @file fmChangeUser.h
 * @date 2019/09/23 14:06
 * @author Jinzi
 * @Contact: 2493349541@qq.com
 * @brief ���ļ����޸Ŀ���Ϣ���ڴ���
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

/*\ ���������������еĺ���,���������û�ѡ����(tableView�ؼ�) \*/
typedef std::function<QString(void)>	FuncRetUserSelCardNum;
/*\ ��ȡ�û�ѡ����Ž������ĵ�¼��� \*/
typedef std::function<int(void)>		FuncRetUserSelMenJinLoginHandle;

/*\ �����ӻص����� \*/
void HikLongConnCallBackFunc(DWORD dwType, void* lpBuffer, DWORD dwBufLen, void* pUserData);

class fmChangeUser :
	public QMainWindow
{
	Q_OBJECT
public:
	fmChangeUser(QWidget* parent = Q_NULLPTR);
	~fmChangeUser();
public:
	/*\ ���źźͲ� \*/
	void BindSignalAndSlots();
	/*\ ��ʼ����Ա���� \*/
	void InitMemberVar();
public:
	/*\ �жϿ����Ƿ����(�Ž�����) \*/
	bool JuageCardNumExistWithMenJin(const char* _cchpCardNum);
	/*\ ������������޸��û���Ϣ������ \*/
	void SendModifyUserToHttpSvr(QString& _sUserId, QString& _sPicPath, QString& _qsImgName);
public:
	/*\ ���ô洢�Ž���Ϣ�ṹ������ \*/
	void SetSelMenJinInfo(CSelMenJinInfo& _oSelMenJinInfo);
public slots:
	/*\ �޸İ�ť����¼� \*/
	void BtnChangeClickEvent();
	/*\ ��ȡͼƬ·������¼� \*/
	void BtnGetPicPathClickEvent();
	/*\ ���ݿ����޸��û���Ϣ \*/
	void ModifyUserInfoWithCardNum(QNetworkReply* _opReplay);
	/*\ �����޸��û���Ϣ�Ļص����ݴ��� \*/
	void SvrRetModifyUserHandle(QNetworkReply* _opReplay);
public:
	/*\ ���������������еĺ�����������ȡѡ���� \*/
	FuncRetUserSelCardNum				m_funcRetUserSelCardNum;
	/*\ ���������������еĺ���,������ȡ�û�ѡ���Ž������ĵ�¼��� \*/
	FuncRetUserSelMenJinLoginHandle		m_funcRetUserSelMenJinLoginHandle;
private:
	Ui::CFmChangeUser		m_oUi;				/*\ ��������ui���� \*/
	CSelMenJinInfo* m_opSelMenJinInfo;	/*\ �洢�Ž���Ϣ�ṹ�� \*/
	CHttpRequest* m_opHttpRequest;	/*\ http������� \*/
	int						m_iLongConnHandle;	/*\ ���������Ӿ�� \*/
	CModifyUserInfo* m_opModifyUserInfo;	/*\ �洢�޸��û���Ϣ�Ľṹ�� \*/
	int						m_iLoginHandle;		/*\ �Ž�������¼���(�û�ѡ��) \*/
	QString					m_qsJobNum;		/*\ �û�ѡ�����ݵĹ��� \*/
};

#endif



