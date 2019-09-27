/**************************************************************
 * @file template.h										  
 * @date 2019/09/06 9:20		
 * @author Jinzi                                              
 * @Contact: 2493349541@qq.com							      
 * @brief 单例模式模板基类				  
***************************************************************/
#pragma once
#ifndef _TEMPLATE_H_
#define _TEMPLATE_H_
/*\ 实现单例模式模板类 \*/
template <class CName>
class CSingleton
{
public:
	/*\ 返回一个对象 \*/
	static CName* GetInstance()
	{
		if (m_opInstance == nullptr)
		{
			m_opInstance = new CName();
		}
		return m_opInstance;
	}
	/*\ 释放掉对象 \*/
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
