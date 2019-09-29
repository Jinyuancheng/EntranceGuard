#include "fmMenJinLogin.h"

#ifndef _UTILS_H_
#include "../Utils/utils.h"
#endif // !_UTILS_H_


fmMenJinLogin::fmMenJinLogin(QWidget* _oWidget)
	:QMainWindow(_oWidget)
{
	/*\ ��ʼ���Ž���¼�ṹ�� \*/
	m_opDevInfo = new NET_DVR_DEVICEINFO_V40();
	m_opDevLoginInfo = new NET_DVR_USER_LOGIN_INFO();
	m_funcSetLoginInfo = nullptr;
	m_funcSetLabelInfo = nullptr;
	m_funcGetListViewCheck = nullptr;
	/*\ ��ʼ��ui���� \*/
	m_oUi.setupUi(this);
	/*\ ���õ�ǰ���ں���Ĵ��ڲ��ܵ�� \*/
	setWindowModality(Qt::ApplicationModal);
	/*\ ���������С����ť \*/
	setWindowFlags(this->windowFlags() &~(Qt::WindowMinMaxButtonsHint));
	/*\ ��¼��ť�ĵ���¼� \*/
	QObject::connect(m_oUi.m_btnLogin, SIGNAL(clicked()), this, SLOT(HikMenJinLogin()));
	/*\ ���ذ�ť����¼� \*/
	QObject::connect(m_oUi.m_btnQuit, SIGNAL(clicked()), this, SLOT(close()));
}

/****************************************!
*@brief  �����Ž���¼�Ĳۺ���
*@author Jinzi
*@date   2019/09/02 16:15:52
*@param[in]
*@param[out]
*@return
****************************************/
void fmMenJinLogin::HikMenJinLogin()
{
	bool bOk;
	int iLoginHandle = -1;

	/*\ �õ��û����� \*/
	QString sNodeName = m_oUi.m_editNodeName->text();
	QString sIp = m_oUi.m_editIp->text();
	QString sPort = m_oUi.m_editPort->text();
	QString sUser = m_oUi.m_editUser->text();
	QString sPass = m_oUi.m_editPass->text();
	/*\ �ж���һ��������,ֱ���˳� \*/
	if (sNodeName.isEmpty() || sIp.isEmpty() || sPort.isEmpty() ||
		sUser.isEmpty() || sPass.isEmpty())
	{
		MessageBoxA(nullptr, "��������Ϊ�յ��ֶ�", "��ʾ", MB_OK | MB_ICONWARNING);
		return;
	}
	/*\ ���Խ��е�¼ \*/
	else
	{
		/*\ У��ip�Ƿ�Ϸ� \*/
		if (!CUtils::GetInstance()->JuageIpLegal(sIp))
		{
			MessageBoxA(nullptr, "������Ϸ���ip��ַ", "��ʾ", MB_OK | MB_ICONWARNING);
			return;
		}
		/*\ У��˿��Ƿ�Ϸ� \*/
		if (!CUtils::GetInstance()->JuagePortLegal(sPort))
		{
			MessageBoxA(nullptr, "������Ϸ��Ķ˿�", "��ʾ", MB_OK | MB_ICONWARNING);
			return;
		}


		/*\ �����ж��û��Ƿ��¼���Ƿ��ǵ�һ����¼�� \*/
		if (m_vecLoginInfo.empty())
		{
			/*\ toLatin1()������QStringת��Ϊ�ֽ�����data()ת��Ϊchar*\*/
			strcpy(m_opDevLoginInfo->sDeviceAddress, sIp.toLatin1().data());
			m_opDevLoginInfo->wPort = sPort.toUShort(&bOk, 10);
			strcpy(m_opDevLoginInfo->sUserName, sUser.toLatin1().data());
			strcpy(m_opDevLoginInfo->sPassword, sPass.toLatin1().data());
			m_opDevLoginInfo->bUseAsynLogin = FALSE;	/*\ ͬ����½��ʽ \*/
			m_opDevLoginInfo->byLoginMode = 0;
			/*\ ���е�¼ \*/
			iLoginHandle = NET_DVR_Login_V40(m_opDevLoginInfo, m_opDevInfo);
			if (iLoginHandle >= 0)
			{
				CLoginInfo oLoginInfo;
				oLoginInfo.m_sNodeName = sNodeName;
				oLoginInfo.m_sIp = sIp;
				oLoginInfo.m_sPort = sPort;
				oLoginInfo.m_sUser = sUser;
				oLoginInfo.m_sPass = sPass;
				oLoginInfo.m_iLoginHandle = iLoginHandle;
				oLoginInfo.m_bIsLogin = true;
				m_vecLoginInfo.push_back(oLoginInfo);
				MessageBoxA(nullptr, "�Ž�������¼�ɹ�", "��ʾ", MB_OK);
				/*\ ����¼�ɹ�������,�洢���������� \*/
				if (m_funcSetLoginInfo != nullptr)
				{
					m_funcSetLoginInfo(m_vecLoginInfo);
				}
				/*\ ���������ڵ�label��ʾ����Ϣ \*/
				if (m_funcSetLabelInfo != nullptr)
				{
					m_funcSetLabelInfo("��¼�ɹ�");
				}
				this->close();
			}
			else
			{
				MessageBoxA(nullptr, "�û����������˿ڻ�ip��ַ����", "��ʾ", MB_OK | MB_ICONWARNING);
			}
		}
		/*\ �ж��Ƿ�״̬�Ƿ��¼ \*/
		else
		{
			for (std::vector<CLoginInfo>::iterator it = m_vecLoginInfo.begin();
				it != m_vecLoginInfo.end(); it++)
			{
				/*\ �Ž��Ѿ���¼ \*/
				if ((*it).m_sIp == sIp)
				{
					/*\ ˵���Ž��Ѿ���¼�������� \*/
					if ((*it).m_bIsLogin == true)
					{
						MessageBoxA(nullptr, "���Ž������ѵ�¼,�����ظ���¼", "��ʾ", MB_OK | MB_ICONWARNING);
						this->close();
					}
					else
					{
						/*\ toLatin1()������QStringת��Ϊ�ֽ�����data()ת��Ϊchar*\*/
						strcpy(m_opDevLoginInfo->sDeviceAddress, sIp.toLatin1().data());
						m_opDevLoginInfo->wPort = sPort.toUShort(&bOk, 10);
						strcpy(m_opDevLoginInfo->sUserName, sUser.toLatin1().data());
						strcpy(m_opDevLoginInfo->sPassword, sPass.toLatin1().data());
						m_opDevLoginInfo->bUseAsynLogin = FALSE;	/*\ ͬ����½��ʽ \*/
						m_opDevLoginInfo->byLoginMode = 0;
						iLoginHandle = NET_DVR_Login_V40(m_opDevLoginInfo, m_opDevInfo);
						if (iLoginHandle >= 0)
						{
							(*it).m_iLoginHandle = iLoginHandle;
							(*it).m_bIsLogin = true;
							MessageBoxA(nullptr, "�Ž�������¼�ɹ�", "��ʾ", MB_OK);
							/*\ ����¼�ɹ�������,�洢���������� \*/
							if (m_funcSetLoginInfo != nullptr)
							{
								m_funcSetLoginInfo(m_vecLoginInfo);
							}
							/*\ ���������ڵ�label��ʾ����Ϣ \*/
							if (m_funcSetLabelInfo != nullptr)
							{
								m_funcSetLabelInfo("��¼�ɹ�");
							}
							this->close();
						}
						else
						{
							MessageBoxA(nullptr, "�û����������˿ڻ�ip��ַ����", "��ʾ", MB_OK | MB_ICONWARNING);
						}
					}
				}
				/*\ �Ž�δ��¼ \*/
				else
				{
					/*\ toLatin1()������QStringת��Ϊ�ֽ�����data()ת��Ϊchar*\*/
					strcpy(m_opDevLoginInfo->sDeviceAddress, sIp.toLatin1().data());
					m_opDevLoginInfo->wPort = sPort.toUShort(&bOk, 10);
					strcpy(m_opDevLoginInfo->sUserName, sUser.toLatin1().data());
					strcpy(m_opDevLoginInfo->sPassword, sPass.toLatin1().data());
					m_opDevLoginInfo->bUseAsynLogin = FALSE;	/*\ ͬ����½��ʽ \*/
					m_opDevLoginInfo->byLoginMode = 0;
					iLoginHandle = NET_DVR_Login_V40(m_opDevLoginInfo, m_opDevInfo);
					if (iLoginHandle >= 0)
					{
						CLoginInfo oLoginInfo;
						oLoginInfo.m_sNodeName = sNodeName;
						oLoginInfo.m_sIp = sIp;
						oLoginInfo.m_sPort = sPort;
						oLoginInfo.m_sUser = sUser;
						oLoginInfo.m_sPass = sPass;
						oLoginInfo.m_iLoginHandle = iLoginHandle;
						oLoginInfo.m_bIsLogin = true;
						m_vecLoginInfo.push_back(oLoginInfo);
						MessageBoxA(nullptr, "�Ž�������¼�ɹ�", "��ʾ", MB_OK);
						/*\ ����¼�ɹ�������,�洢���������� \*/
						if (m_funcSetLoginInfo != nullptr)
						{
							m_funcSetLoginInfo(m_vecLoginInfo);
						}
						/*\ ���������ڵ�label��ʾ����Ϣ \*/
						if (m_funcSetLabelInfo != nullptr)
						{
							m_funcSetLabelInfo("��¼�ɹ�");
						}
						this->close();
					}
					else
					{
						MessageBoxA(nullptr, "�û����������˿ڻ�ip��ַ����", "��ʾ", MB_OK | MB_ICONWARNING);
					}
				}
			}
		}
	}
}

/****************************************!
*@brief  �Ž������ǳ�
*@author Jinzi
*@date   2019/09/03 8:28:19
*@param[in]
*@param[out]
*@return
****************************************/
void fmMenJinLogin::HikMenJinLoginOut()
{
	if (!m_vecLoginInfo.empty())
	{
		if (m_funcGetListViewCheck != nullptr)
		{
			int iIndex = m_funcGetListViewCheck();
			/*\ �ж��Ƿ����� \*/
			if (m_vecLoginInfo[iIndex].m_bIsLogin)
			{
				NET_DVR_Logout_V30(m_vecLoginInfo[iIndex].m_iLoginHandle);
				m_vecLoginInfo.erase(m_vecLoginInfo.begin() + iIndex);
				MessageBoxA(nullptr, "�Ž��ǳ��ɹ�", "��ʾ", MB_OK);
				/*\ ����label��ʾ���� \*/
				if (m_funcSetLabelInfo != nullptr)
				{
					m_funcSetLabelInfo("�Ž��ǳ��ɹ�");
				}
				/*\ ���������ڵ�¼��Ϣ��vector���ϣ� \*/
				if (m_funcSetLoginInfo != nullptr)
				{
					m_funcSetLoginInfo(m_vecLoginInfo);
				}
			}
			/*\ δ���� ɾ����Ԫ�� ������ʾ \*/
			else
			{
				m_vecLoginInfo.erase(m_vecLoginInfo.begin() + iIndex);
				MessageBoxA(nullptr, "�Ž��ǳ��ɹ�", "��ʾ", MB_OK);
				/*\ ����label��ʾ���� \*/
				if (m_funcSetLabelInfo != nullptr)
				{
					m_funcSetLabelInfo("�Ž��ǳ��ɹ�");
				}
				/*\ ���������ڵ�¼��Ϣ��vector���ϣ� \*/
				if (m_funcSetLoginInfo != nullptr)
				{
					m_funcSetLoginInfo(m_vecLoginInfo);
				}
			}
		}
	}
	/*\ û���Ž�������¼ \*/
	else
	{
		MessageBoxA(nullptr, "���ȵ�¼�Ž�����", "��ʾ", MB_OK | MB_ICONWARNING);
	}

}

/****************************************!
*@brief  �������� �ͷ��ڴ�
*@author Jinzi
*@date   2019/09/03 9:52:53
*@param[in]
*@param[out]
*@return
****************************************/
fmMenJinLogin::~fmMenJinLogin()
{
	if (m_opDevInfo != nullptr)
	{
		delete[] m_opDevLoginInfo;
	}
	if (m_opDevInfo != nullptr)
	{
		delete[] m_opDevInfo;
	}
}

/****************************************!
*@brief  �����Ž������ǳ�
*@author Jinzi
*@date   2019/09/03 14:40:30
*@param[in]
*@param[out]
*@return
****************************************/
void fmMenJinLogin::HikMenJinLoginOutAll()
{
	if (!m_vecLoginInfo.empty())
	{
		for (std::vector<CLoginInfo>::iterator it = m_vecLoginInfo.begin();
			it != m_vecLoginInfo.end(); it++)
		{
			NET_DVR_Logout_V30(it->m_iLoginHandle);
		}
		m_vecLoginInfo.clear();
		MessageBoxA(nullptr, "�ǳ������Ž��ɹ�", "��ʾ", MB_OK);
		/*\ ����label��ʾ���� \*/
		if (m_funcSetLabelInfo != nullptr)
		{
			m_funcSetLabelInfo("�ǳ������Ž��ɹ�");
		}
		/*\ ���������ڵ�¼��Ϣ��vector���ϣ� \*/
		if (m_funcSetLoginInfo != nullptr)
		{
			m_funcSetLoginInfo(m_vecLoginInfo);
		}
	}
	else
	{
		MessageBoxA(nullptr, "���ȵ�¼�Ž�����", "��ʾ", MB_OK | MB_ICONWARNING);
	}
}