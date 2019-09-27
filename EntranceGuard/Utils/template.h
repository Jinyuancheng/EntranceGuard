/**************************************************************
 * @file template.h										  
 * @date 2019/09/06 9:20		
 * @author Jinzi                                              
 * @Contact: 2493349541@qq.com							      
 * @brief ����ģʽģ�����				  
***************************************************************/
#pragma once
#ifndef _TEMPLATE_H_
#define _TEMPLATE_H_
/*\ ʵ�ֵ���ģʽģ���� \*/
template <class CName>
class CSingleton
{
public:
	/*\ ����һ������ \*/
	static CName* GetInstance()
	{
		if (m_opInstance == nullptr)
		{
			m_opInstance = new CName();
		}
		return m_opInstance;
	}
	/*\ �ͷŵ����� \*/
	static void DelInstance()
	{
		if (m_opInstance != nullptr)
		{
			delete[] m_opInstance;
		}
	}
private:
	static CName* m_opInstance;
};

template<class CName> 
CName* CSingleton<CName>::m_opInstance = nullptr;

#endif // !1
