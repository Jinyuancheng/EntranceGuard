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
	std::string ReadJpgInfoWithCSharp(const char* _cchpPicJpgPath);
	/*\ qtͼƬbase64���� \*/
	QByteArray LocalImageToBase64(QString _qsPicPath);
	/*\ �ж�ip�Ƿ�Ϸ� \*/
	bool JuageIpLegal(QString& _qsIp);
	/*\ �ж϶˿��Ƿ�Ϸ� \*/
	bool JuagePortLegal(QString& _qsPort);
private:
};

#endif