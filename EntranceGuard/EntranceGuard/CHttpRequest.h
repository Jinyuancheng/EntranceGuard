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

typedef std::function<void(QNetworkReply*)>		FuncHandle;		/*\ ��������ص����� \*/

class CHttpRequest :public CSingleton<CHttpRequest>, QMainWindow
{
public:
	CHttpRequest();
	~CHttpRequest();
	/*\ ��ʼ����Ա���� \*/
	void InitMemBerVar();
public:
	/*\ ����http post���� \*/
	void HttpPostRequest(const char* _cchpUrl, QByteArray& _byArrReqData, FuncHandle _funcHandle);
	/*\ ����url��ȡͼƬ������,����ͼƬ����(δbase64����) \*/
	QByteArray HttpGetPicDataWithUrl(const char* _cchpUrl);
	/*\ ����http get���� \*/
	void HttpGetRequest(const char* _cchpUrl, FuncHandle _funcHandle);
private:
	QNetworkAccessManager* m_opHttpManager;	/*\ http������� \*/
};



#endif // !_CHTTPREQUEST_H_



