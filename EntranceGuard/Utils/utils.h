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
	/*\ 去除掉字符串中的指定字符 \*/
	void DelCharOnString(std::string& _sStr, char _cChr);
	/*\ 使用c#方式读取文件(使用C#) \*/
	char* ReadJpgInfoWithCSharp(const char* _cchpPicJpgPath);
	/*\ qt图片base64编码 \*/
	QByteArray LocalImageToBase64(QString _qsPicPath);
	/*\ 判断ip是否合法(正则表达式) \*/
	bool JuageIpLegal(QString& _qsIp);
	/*\ 判断端口是否合法(正则表达式)\*/
	bool JuagePortLegal(QString& _qsPort);
	/*\ 判断输入数字非0，并且最大为9999(正则表达式) \*/
	bool JuageNumberLegal(QString& _qsNumber);
	/*\ 判断路径是否合法(正则表达式) \*/
	bool JuageFilePathLegal(QString& _qsFilePath);
private:
};

#endif