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
	std::string ReadJpgInfoWithCSharp(const char* _cchpPicJpgPath);
	/*\ qt图片base64编码 \*/
	QByteArray LocalImageToBase64(QString _qsPicPath);
private:
};

#endif