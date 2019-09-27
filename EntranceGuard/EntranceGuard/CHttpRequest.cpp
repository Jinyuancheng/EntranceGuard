#include "./CHttpRequest.h"
#include <QNetworkRequest>
#include <QString>
#include <QEventLoop>

/****************************************!
*@brief  构造函数
*@author Jinzi
*@date   2019/09/23 16:57:13
*@param[in]
*@param[out]
*@return
****************************************/
CHttpRequest::CHttpRequest()
{
	this->InitMemBerVar();
}

/****************************************!
*@brief  析构函数
*@author Jinzi
*@date   2019/09/23 16:58:03
*@param[in]
*@param[out]
*@return
****************************************/
CHttpRequest::~CHttpRequest()
{
	if (m_opHttpManager != nullptr)
	{
		delete m_opHttpManager;
		m_opHttpManager = nullptr;
	}
}

/****************************************!
*@brief  发送post请求
*@author Jinzi
*@date   2019/09/23 16:51:09
*@param[in]
   _cchpUrl		:	http请求的路由
   _byArrReqData	:	请求数据
   _funcHandle		:	槽函数(请求结束后执行的函数)
*@param[out]
*@return
****************************************/
void CHttpRequest::HttpPostRequest(const char* _cchpUrl, QByteArray& _byArrReqData, FuncHandle _funcHandle)
{
	QString qsUrl(_cchpUrl);
	//QString qsReqData = "{\"user\":\"jinzi\"}";
	QNetworkRequest oRequestInfo;
	/*\ 设置请求url \*/
	oRequestInfo.setUrl(QUrl(qsUrl));
	/*\ 设置请求头信息 \*/
	oRequestInfo.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
	/*\ 发送post请求 \*/
	QNetworkReply* pReply = m_opHttpManager->post(oRequestInfo, _byArrReqData);
	QObject::connect(pReply, &QNetworkReply::finished, [=]() {
		if (_funcHandle != nullptr)
		{
			_funcHandle(pReply);
		}
		});
}

/****************************************!
*@brief  初始化成员变量
*@author Jinzi
*@date   2019/09/23 16:40:53
*@param[in]
*@param[out]
*@return
****************************************/
void CHttpRequest::InitMemBerVar()
{
	m_opHttpManager = new QNetworkAccessManager(this);
}

/****************************************!
*@brief  根据图片url ，http得到图片数据
*@author Jinzi
*@date   2019/09/26 9:18:44
*@param[in]
   _cchpUrl	:	图片的url
*@param[out]
*@return
****************************************/
QByteArray CHttpRequest::HttpGetPicDataWithUrl(const char* _cchpUrl)
{
	QString qsUrl(_cchpUrl);
	QNetworkRequest oRequestInfo;
	oRequestInfo.setUrl(qsUrl);
	oRequestInfo.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	QNetworkReply* replay = m_opHttpManager->get(oRequestInfo);
	QEventLoop oLoop;
	QObject::connect(replay, SIGNAL(finished()), &oLoop, SLOT(quit()));
	oLoop.exec();
	return replay->readAll();
}

 /****************************************!
 *@brief  发送http Get请求
 *@author Jinzi
 *@date   2019/09/26 9:41:55
 *@param[in]  
	_cchpUrl	:	get请求的url
	_funcHandle	:	请求回调数据的处理函数
 *@param[out] 
 *@return     
 ****************************************/
void CHttpRequest::HttpGetRequest(const char* _cchpUrl, FuncHandle _funcHandle)
{
	QString qsUrl(_cchpUrl);
	QNetworkRequest oRequestInfo;
	oRequestInfo.setUrl(qsUrl);
	oRequestInfo.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	QNetworkReply* replay = m_opHttpManager->get(oRequestInfo);
	QObject::connect(replay, &QNetworkReply::finished, [=]() {
		if (_funcHandle != nullptr)
		{
			_funcHandle(replay);
		}
		});
}