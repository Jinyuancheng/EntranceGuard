#pragma once
#ifndef	_UTILS_H_
#define _UTILS_H_

#include <iostream>

#include "./template.h"
#include <QImage>
#include <QBuffer>

class CUtils :public CSingleton<CUtils>
{
public:
	CUtils() {}
public:
	/*\ ȥ�����ַ����е�ָ���ַ� \*/
	void DelCharOnString(std::string& _sStr, char _cChr);
	/*\ ʹ��c#��ʽ��ȡ�ļ�(ʹ��C#) \*/
	char* ReadJpgInfoWithCSharp(const char* _cchpPicJpgPath);
	/*\ qtͼƬbase64���� \*/
	QByteArray LocalImageToBase64(QString _qsPicPath);
	/*\ �ж�ip�Ƿ�Ϸ�(������ʽ) \*/
	bool JuageIpLegal(QString& _qsIp);
	/*\ �ж϶˿��Ƿ�Ϸ�(������ʽ)\*/
	bool JuagePortLegal(QString& _qsPort);
	/*\ �ж��������ַ�0���������Ϊ9999(������ʽ) \*/
	bool JuageNumberLegal(QString& _qsNumber);
	/*\ �ж�·���Ƿ�Ϸ�(������ʽ) \*/
	bool JuageFilePathLegal(QString& _qsFilePath);
private:
};

#endif