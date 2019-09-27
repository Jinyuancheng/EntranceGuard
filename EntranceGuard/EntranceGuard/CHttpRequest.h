#pragma once
#ifndef _CHTTPREQUEST_H_
#define _CHTTPREQUEST_H_

#ifndef _TEMPLATE_H_
#include "../Utils/template.h"
#endif

#include <QObject>
#include <QtWidgets/QMainWindow>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <functional>
#include <QNetworkReply>

typedef std::function<void(QNetworkReply*)>		FuncHandle;		/*\ 用来处理回调数据 \*/

class CHttpRequest :public CSingleton<CHttpRequest>, QMainWindow
{
public:
	CHttpRequest();
	~CHttpRequest();
	/*\ 初始化成员变量 \*/
	void InitMemBerVar();
public:
	/*\ 发送http post请求 \*/
	void HttpPostRequest(const char* _cchpUrl, QByteArray& _byArrReqData, FuncHandle _funcHandle);
	/*\ 根据url获取图片的数据,返回图片数据(未base64编码) \*/
	QByteArray HttpGetPicDataWithUrl(const char* _cchpUrl);
	/*\ 发送http get请求 \*/
	void HttpGetRequest(const char* _cchpUrl, FuncHandle _funcHandle);
private:
	QNetworkAccessManager* m_opHttpManager;	/*\ http管理对象 \*/
};



#endif // !_CHTTPREQUEST_H_



