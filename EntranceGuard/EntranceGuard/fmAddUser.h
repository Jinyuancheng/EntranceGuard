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

/*\ �������ø������еĺ��� \*/
typedef std::function<int(void)> FuncGetListViewCheck;

/*\ ������Ϣ�Ļص����� \*/
void AddUserRemoteConfigCallBack(DWORD _dwType, void* _lpBuffer, DWORD _dwBufLen, void* _pUserData);

class fmAddUser :
	public QMainWindow
{
	Q_OBJECT
public:
	fmAddUser(QWidget* parent = Q_NULLPTR);
	~fmAddUser();
public:
	/*\ ��ʼ���ؼ�״̬����Ϣ \*/
	void InitControlInfo();
	/*\ ��ʼ����Ա���� \*/
	void InitMemberVar();
public:
	/*\ ��ȡ�����ļ� \*/
	void ReadIniFile(std::string _sFilePath);
public:
	/*\ ���źźͲ� \*/
	void BindSignalAndSlot();
	/*\ qtͼƬbase64���� \*/
	QByteArray ImageToBase64(QString _qsPicPath);
	/*\ ���Ž��豸����ӿ�����Ϣ \*/
	void AddUserInfoToMenJin();
	/*\ ���õ�¼��Ϣ \*/
	void SetLoginInfo(std::vector<CLoginInfo>& _vecLoginInfo);
	/*\ ��ʼ��m_opLongConnStatus \*/
	void InitLongConnStatus();
	/*\ �����·�������Ϣ \*/
	void SendCardInfoToMenJin();
	/*\ �ж��û�����lineEdit�е������Ƿ�Ϸ� \*/
	void JuageUserInputData();
	/*\ ¼��������Ϣ \*/
	void SendCardSuccInputFaceInfo();
	/*\ ��ȡͼƬ��Ϣ���ļ��� \*/
	char* ReadPicInfoToStream();
public slots:
	/*\ ����û���ť����¼� \*/
	void BtnAddUserInfo();
	/*\ ���ļ���·�� \*/
	void OpenFloderPath();
	/*\ �õ� ����û��Ļص����ݴ����� \*/
	void HttpAddUserRetDataHandle();
public:
	FuncGetListViewCheck	m_funcGetListViewCheck; /*\ ���������������еĺ��� \*/
private:
	Ui::CFmAddUser			m_oUi;				/*\ ��������ui���� \*/
	QNetworkAccessManager*	m_opHttpManager;	/*\ http������� \*/
	CAddUserInfo*			m_opAddUserInfo;	/*\ �洢����û���Ϣ \*/
	std::vector<CLoginInfo> m_vecLoginInfo;     /*\ �洢��¼��Ϣ \*/
	CLongConnStatus*		m_opLongConnStatus;	/*\ ��������Ϣ \*/
};

