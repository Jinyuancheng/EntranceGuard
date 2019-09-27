#include "./CHttpRequest.h"
#include <QNetworkRequest>
#include <QString>
#include <QEventLoop>

/****************************************!
*@brief  ���캯��
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
*@brief  ��������
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
*@brief  ����post����
*@author Jinzi
*@date   2019/09/23 16:51:09
*@param[in]
   _cchpUrl		:	http�����·��
   _byArrReqData	:	��������
   _funcHandle		:	�ۺ���(���������ִ�еĺ���)
*@param[out]
*@return
****************************************/
void CHttpRequest::HttpPostRequest(const char* _cchpUrl, QByteArray& _byArrReqData, FuncHandle _funcHandle)
{
	QString qsUrl(_cchpUrl);
	//QString qsReqData = "{\"user\":\"jinzi\"}";
	QNetworkRequest oRequestInfo;
	/*\ ��������url \*/
	oRequestInfo.setUrl(QUrl(qsUrl));
	/*\ ��������ͷ��Ϣ \*/
	oRequestInfo.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
	/*\ ����post���� \*/
	QNetworkReply* pReply = m_opHttpManager->post(oRequestInfo, _byArrReqData);
	QObject::connect(pReply, &QNetworkReply::finished, [=]() {
		if (_funcHandle != nullptr)
		{
			_funcHandle(pReply);
		}
		});
}

/****************************************!
*@brief  ��ʼ����Ա����
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
*@brief  ����ͼƬurl ��http�õ�ͼƬ����
*@author Jinzi
*@date   2019/09/26 9:18:44
*@param[in]
   _cchpUrl	:	ͼƬ��url
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
 *@brief  ����http Get����
 *@author Jinzi
 *@date   2019/09/26 9:41:55
 *@param[in]  
	_cchpUrl	:	get�����url
	_funcHandle	:	����ص����ݵĴ�����
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