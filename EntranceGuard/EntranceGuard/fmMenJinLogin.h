/**************************************************************
 * @file fmMenJinLogin.h										  
 * @date 2019/09/10 11:25		
 * @author Jinzi                                              
 * @Contact: 2493349541@qq.com							      
 * @brief ��¼����				  
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

/*\ �����Ӵ��ڵ��ø������еĺ�����ָ�룬�������õ�¼���� \*/
typedef std::function<void(std::vector<CLoginInfo>&)>	FuncSetLoginInfo;
/*\ ���������������е�label��Ϣ \*/
typedef std::function<void(const char*)>				FuncSetLabelInfo;
/*\ ��������listViewѡ�е�index \*/
typedef std::function<int(void)>						FuncGetListViewCheck;

/*\ �Ӵ����� \*/
class fmMenJinLogin :
	public QMainWindow
{
	Q_OBJECT
public:
	fmMenJinLogin(QWidget* _oWidget);
	~fmMenJinLogin();
public slots:
	/*\ �Ž�������¼ \*/
	void HikMenJinLogin();
	/*\ �Ž������ǳ� \*/
	void HikMenJinLoginOut();
	/*\ �����Ž������ǳ� \*/
	void HikMenJinLoginOutAll();
public:
	FuncSetLoginInfo			m_funcSetLoginInfo;	/*\ ����������ҳ��ĵ�¼��Ϣ \*/
	FuncSetLabelInfo			m_funcSetLabelInfo;	/*\ ���������������е�label��Ϣ \*/
	FuncGetListViewCheck		m_funcGetListViewCheck;/*\ ��������listview��index \*/
private:
	Ui::MainWindow				m_oUi;				/*\ �Ž���¼��ui���� \*/
	std::vector<CLoginInfo>		m_vecLoginInfo;		/*\ �����洢��¼��Ϣ \*/
	NET_DVR_USER_LOGIN_INFO*	m_opDevLoginInfo;	/*\ �����Ž���¼ \*/
	NET_DVR_DEVICEINFO_V40*		m_opDevInfo;		/*\ �������յ�¼�ɹ�������� \*/
};
#endif
