#include "fmMenJinLogin.h"

#ifndef _UTILS_H_
#include "../Utils/utils.h"
#endif // !_UTILS_H_


fmMenJinLogin::fmMenJinLogin(QWidget* _oWidget)
	:QMainWindow(_oWidget)
{
	/*\ 初始化门禁登录结构体 \*/
	m_opDevInfo = new NET_DVR_DEVICEINFO_V40();
	m_opDevLoginInfo = new NET_DVR_USER_LOGIN_INFO();
	m_funcSetLoginInfo = nullptr;
	m_funcSetLabelInfo = nullptr;
	m_funcGetListViewCheck = nullptr;
	/*\ 初始化ui界面 \*/
	m_oUi.setupUi(this);
	/*\ 设置当前窗口后面的窗口不能点击 \*/
	setWindowModality(Qt::ApplicationModal);
	/*\ 隐藏最大化最小化按钮 \*/
	setWindowFlags(this->windowFlags() &~(Qt::WindowMinMaxButtonsHint));
	/*\ 登录按钮的点击事件 \*/
	QObject::connect(m_oUi.m_btnLogin, SIGNAL(clicked()), this, SLOT(HikMenJinLogin()));
	/*\ 返回按钮点击事件 \*/
	QObject::connect(m_oUi.m_btnQuit, SIGNAL(clicked()), this, SLOT(close()));
}

/****************************************!
*@brief  海康门禁登录的槽函数
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

	/*\ 得到用户输入 \*/
	QString sNodeName = m_oUi.m_editNodeName->text();
	QString sIp = m_oUi.m_editIp->text();
	QString sPort = m_oUi.m_editPort->text();
	QString sUser = m_oUi.m_editUser->text();
	QString sPass = m_oUi.m_editPass->text();
	/*\ 判断有一个不满足,直接退出 \*/
	if (sNodeName.isEmpty() || sIp.isEmpty() || sPort.isEmpty() ||
		sUser.isEmpty() || sPass.isEmpty())
	{
		MessageBoxA(nullptr, "您输入有为空的字段", "提示", MB_OK | MB_ICONWARNING);
		return;
	}
	/*\ 可以进行登录 \*/
	else
	{
		/*\ 校验ip是否合法 \*/
		if (!CUtils::GetInstance()->JuageIpLegal(sIp))
		{
			MessageBoxA(nullptr, "请输入合法的ip地址", "提示", MB_OK | MB_ICONWARNING);
			return;
		}
		/*\ 校验端口是否合法 \*/
		if (!CUtils::GetInstance()->JuagePortLegal(sPort))
		{
			MessageBoxA(nullptr, "请输入合法的端口", "提示", MB_OK | MB_ICONWARNING);
			return;
		}


		/*\ 首先判断用户是否登录（是否是第一个登录） \*/
		if (m_vecLoginInfo.empty())
		{
			/*\ toLatin1()函数将QString转换为字节数组data()转换为char*\*/
			strcpy(m_opDevLoginInfo->sDeviceAddress, sIp.toLatin1().data());
			m_opDevLoginInfo->wPort = sPort.toUShort(&bOk, 10);
			strcpy(m_opDevLoginInfo->sUserName, sUser.toLatin1().data());
			strcpy(m_opDevLoginInfo->sPassword, sPass.toLatin1().data());
			m_opDevLoginInfo->bUseAsynLogin = FALSE;	/*\ 同步登陆方式 \*/
			m_opDevLoginInfo->byLoginMode = 0;
			/*\ 进行登录 \*/
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
				MessageBoxA(nullptr, "门禁主机登录成功", "提示", MB_OK);
				/*\ 将登录成功的数据,存储到主窗体中 \*/
				if (m_funcSetLoginInfo != nullptr)
				{
					m_funcSetLoginInfo(m_vecLoginInfo);
				}
				/*\ 设置主窗口的label显示的信息 \*/
				if (m_funcSetLabelInfo != nullptr)
				{
					m_funcSetLabelInfo("登录成功");
				}
				this->close();
			}
			else
			{
				MessageBoxA(nullptr, "用户名或密码或端口或ip地址错误", "提示", MB_OK | MB_ICONWARNING);
			}
		}
		/*\ 判断是否状态是否登录 \*/
		else
		{
			for (std::vector<CLoginInfo>::iterator it = m_vecLoginInfo.begin();
				it != m_vecLoginInfo.end(); it++)
			{
				/*\ 门禁已经登录 \*/
				if ((*it).m_sIp == sIp)
				{
					/*\ 说明门禁已经登录，连接中 \*/
					if ((*it).m_bIsLogin == true)
					{
						MessageBoxA(nullptr, "该门禁主机已登录,请勿重复登录", "提示", MB_OK | MB_ICONWARNING);
						this->close();
					}
					else
					{
						/*\ toLatin1()函数将QString转换为字节数组data()转换为char*\*/
						strcpy(m_opDevLoginInfo->sDeviceAddress, sIp.toLatin1().data());
						m_opDevLoginInfo->wPort = sPort.toUShort(&bOk, 10);
						strcpy(m_opDevLoginInfo->sUserName, sUser.toLatin1().data());
						strcpy(m_opDevLoginInfo->sPassword, sPass.toLatin1().data());
						m_opDevLoginInfo->bUseAsynLogin = FALSE;	/*\ 同步登陆方式 \*/
						m_opDevLoginInfo->byLoginMode = 0;
						iLoginHandle = NET_DVR_Login_V40(m_opDevLoginInfo, m_opDevInfo);
						if (iLoginHandle >= 0)
						{
							(*it).m_iLoginHandle = iLoginHandle;
							(*it).m_bIsLogin = true;
							MessageBoxA(nullptr, "门禁主机登录成功", "提示", MB_OK);
							/*\ 将登录成功的数据,存储到主窗体中 \*/
							if (m_funcSetLoginInfo != nullptr)
							{
								m_funcSetLoginInfo(m_vecLoginInfo);
							}
							/*\ 设置主窗口的label显示的信息 \*/
							if (m_funcSetLabelInfo != nullptr)
							{
								m_funcSetLabelInfo("登录成功");
							}
							this->close();
						}
						else
						{
							MessageBoxA(nullptr, "用户名或密码或端口或ip地址错误", "提示", MB_OK | MB_ICONWARNING);
						}
					}
				}
				/*\ 门禁未登录 \*/
				else
				{
					/*\ toLatin1()函数将QString转换为字节数组data()转换为char*\*/
					strcpy(m_opDevLoginInfo->sDeviceAddress, sIp.toLatin1().data());
					m_opDevLoginInfo->wPort = sPort.toUShort(&bOk, 10);
					strcpy(m_opDevLoginInfo->sUserName, sUser.toLatin1().data());
					strcpy(m_opDevLoginInfo->sPassword, sPass.toLatin1().data());
					m_opDevLoginInfo->bUseAsynLogin = FALSE;	/*\ 同步登陆方式 \*/
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
						MessageBoxA(nullptr, "门禁主机登录成功", "提示", MB_OK);
						/*\ 将登录成功的数据,存储到主窗体中 \*/
						if (m_funcSetLoginInfo != nullptr)
						{
							m_funcSetLoginInfo(m_vecLoginInfo);
						}
						/*\ 设置主窗口的label显示的信息 \*/
						if (m_funcSetLabelInfo != nullptr)
						{
							m_funcSetLabelInfo("登录成功");
						}
						this->close();
					}
					else
					{
						MessageBoxA(nullptr, "用户名或密码或端口或ip地址错误", "提示", MB_OK | MB_ICONWARNING);
					}
				}
			}
		}
	}
}

/****************************************!
*@brief  门禁主机登出
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
			/*\ 判断是否连接 \*/
			if (m_vecLoginInfo[iIndex].m_bIsLogin)
			{
				NET_DVR_Logout_V30(m_vecLoginInfo[iIndex].m_iLoginHandle);
				m_vecLoginInfo.erase(m_vecLoginInfo.begin() + iIndex);
				MessageBoxA(nullptr, "门禁登出成功", "提示", MB_OK);
				/*\ 设置label显示数据 \*/
				if (m_funcSetLabelInfo != nullptr)
				{
					m_funcSetLabelInfo("门禁登出成功");
				}
				/*\ 设置主窗口登录信息（vector集合） \*/
				if (m_funcSetLoginInfo != nullptr)
				{
					m_funcSetLoginInfo(m_vecLoginInfo);
				}
			}
			/*\ 未连接 删除该元素 重新显示 \*/
			else
			{
				m_vecLoginInfo.erase(m_vecLoginInfo.begin() + iIndex);
				MessageBoxA(nullptr, "门禁登出成功", "提示", MB_OK);
				/*\ 设置label显示数据 \*/
				if (m_funcSetLabelInfo != nullptr)
				{
					m_funcSetLabelInfo("门禁登出成功");
				}
				/*\ 设置主窗口登录信息（vector集合） \*/
				if (m_funcSetLoginInfo != nullptr)
				{
					m_funcSetLoginInfo(m_vecLoginInfo);
				}
			}
		}
	}
	/*\ 没有门禁主机登录 \*/
	else
	{
		MessageBoxA(nullptr, "请先登录门禁主机", "提示", MB_OK | MB_ICONWARNING);
	}

}

/****************************************!
*@brief  析构函数 释放内存
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
*@brief  所有门禁主机登出
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
		MessageBoxA(nullptr, "登出所有门禁成功", "提示", MB_OK);
		/*\ 设置label显示数据 \*/
		if (m_funcSetLabelInfo != nullptr)
		{
			m_funcSetLabelInfo("登出所有门禁成功");
		}
		/*\ 设置主窗口登录信息（vector集合） \*/
		if (m_funcSetLoginInfo != nullptr)
		{
			m_funcSetLoginInfo(m_vecLoginInfo);
		}
	}
	else
	{
		MessageBoxA(nullptr, "请先登录门禁主机", "提示", MB_OK | MB_ICONWARNING);
	}
}