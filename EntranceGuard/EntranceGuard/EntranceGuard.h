/**************************************************************
 * @file EntranceGuard.h
 * @date 2019/09/10 11:25
 * @author Jinzi
 * @Contact: 2493349541@qq.com
 * @brief ������
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


/*\ �����ص����� \*/
BOOL MsgHikCallBack(LONG lCommand, NET_DVR_ALARMER* pAlarmer,
	char* pAlarmInfo, DWORD dwBufLen, void* pUser);
/*\ ������Ϣ�Ļص����� \*/
void RemoteConfigCallBack(DWORD _dwType, void* _lpBuffer, DWORD _dwBufLen, void* _pUserData);

class EntranceGuard : public QMainWindow
{
	Q_OBJECT

public:
	EntranceGuard(QWidget* parent = Q_NULLPTR);
	~EntranceGuard();
protected:
	/*\ ������¼�,������ʾ�˵����� \*/
	void mousePressEvent(QMouseEvent* _oEvent);
public:
	/*\ ��ʼ������sdk \*/
	void InitHikSDK();
	/*\ �������sdk \*/
	void CleanHikSDK();
	/*\ ��ʾtreeView(�Ž�������¼�ɹ������) \*/
	void ShowListView();
	/*\ ���źźͲ� \*/
	void InitSignalAndSlot();
	/*\ ��ʼ���洢��������Ϣ�Ľṹ�� \*/
	void InitSaveLongInfo();
	/*\ ���󲢷��������û���Ϣ��http����������� \*/
	void ShowAllUserInfoWithListView();
	/*\ ���Ӵ��ڵĺ���ָ�븳ֵ \*/
	void InitChildWinFuncAssign();
public:
	/*\ ���õ�¼��Ϣ \*/
	void SetListLoinInfo(std::vector<CLoginInfo>& _lstListLoginInfo);
	/*\ ����label��ʾ��Ϣ \*/
	void SetLabelShowInfo(const char* _cchpShowInfo);
	/*\ �õ�listView��ѡ��Ľڵ� \*/
	int GetListViewCheck();
	/*\ ��ʾ�Ž��û���Ϣ \*/
	void ShowMenJinUserInfo();
	/*\ ���ݿ��� Ѱ�ҵ��û���id \*/
	QString RetUserIdWithJobNum(QString _qsCardNumber);
	/*\ ɾ��һ���û�����Ϣ \*/
	void DelMenJinUserInfo();
	/*\ ִ�������ȡ�����û���Ϣ�����ݴ����� \*/
	void HttpGetAllUserSuccDataHandle(QNetworkReply* _opReply);
public: //fmChangeUser ������õĺ���
	/*\ �����û�ѡ�еĹ�����Ϣ��tableView�ؼ������� \*/
	QString RetUserSelRowJobNum();
	/*\ �����û�ѡ���Ž������ĵ�¼��� \*/
	int RetUserSelMenJinLoginHandle();
	/*\ �����û�ѡ�еĿ�����Ϣ \*/
	QString RetUserSelRowCardNum();
public:
	/*\ ���������� \*/
	int CreateLongConn(int _iLoginHandle, DWORD _uiType, LPVOID _lpBuffer,
		DWORD _dwLength, LPVOID _lpUserData = nullptr);
	/*\ �رճ����� \*/
	bool CloseLongConn(int _iLongConnHandle);
private slots:
	/*\ ��ѯ�Ž���Ϣ \*/
	void SelectMenJinInfo();
	/*\ ��ȡ�豸������ \*/
	void GetDevAbilty();
	/*\ ɾ��һ���Ž���������Ա��Ϣ \*/
	void DelMenJinOnAllUserInfo();
	/*\ �����Ž�������¼���ж��Ƿ������ʾ����û����� \*/
	void JuageShowAddUserFm();
	/*\ �·��û���Ϣ \*/
	void BeSentDownMenJin();
	/*\ ɾ���û���Ϣ \*/
	void DelUserInfoWithMenJin();
	/*\ ɾ���û���Ϣ�ص����ݵĴ����� \*/
	void HttpDelUserInfoSuccDataHandle();
	/*\ ��ʾ�����û����� \*/
	void ShowFmChangeUserInfo();
	/*\ �жϹ����Ƿ���ڵ�http��Ӧ�������� \*/
	void JuageJobNumResFuncHandle(QNetworkReply* _opReqplay);
private:
	Ui::EntranceGuardClass				ui;                     /*\ ui���� \*/
	fmMenJinLogin*                      m_oFmMenJinLogin;       /*\ �Ž���¼���� \*/
	fmAddUser*                          m_oFmAddUser;			/*\ ����û���Ϣ���� \*/
	fmChangeUser*						m_opFmChangeUser;		/*\ �޸��û���Ϣ���� \*/
	QMenu*                              m_oMenu;	            /*\ �Ҽ��˵� \*/
	QAction*                            m_oAction1;	            /*\ �˵����µĵ�һ�� \*/
	QAction*                            m_oAction2;	            /*\ �˵����µĵڶ��� \*/
	QAction*                            m_oAction3;	            /*\ �˵����µĵ����� \*/
	MSGCallBack_V31	                    m_funcMsgHikCallBack;   /*\ �����ص����� \*/
	std::vector<CLoginInfo>             m_vecLoginInfo;         /*\ �洢��¼��Ϣ \*/
	fRemoteConfigCallback	            m_funcMsgRemoteConfig;  /*\ ����������Ϣ�Ļص����� \*/
	CLongConnStatus*                    m_opLongConnInfo;		/*\ ��������Ϣ�������״̬ \*/
	NET_DVR_CARD_CFG_COND*              m_opCardInfo;			/*\ ��ѯ��ʹ�õĽṹ�� \*/
	NET_DVR_CARD_CFG_SEND_DATA*         m_opSendDataInfo;		/*\ ����������Ϣ��ʱ��ʹ�õĽṹ�� \*/
	QNetworkAccessManager*				m_opHttpManager;		/*\ http���� \*/
	std::vector<CSvrRetAllUserInfo>		m_vecSvrRetAllUserInfo;	/*\ ��ʼ���ӷ������õ��������û����� \*/
	CHttpRequest*						m_opHttpRequest;		/*\ ��������http���� \*/
};
