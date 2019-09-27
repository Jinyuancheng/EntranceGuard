#include "./utils.h"
#include <Windows.h>
#include <QString>
#include <QCoreApplication>
#include <QDir>
#include "../Public/PublicDefine.h"


typedef bool(*FuncReadPictureInfo)(const char*, std::string&);
/*\ 存储路径 \*/
extern CSaveiniInfo* gl_opSaveIniInfo;

/****************************************!
*@brief  去除掉字符串中的指定字符
*@author Jinzi
*@date   2019/09/19 8:42:56
*@param[in]
   _sStr	:	要去除指定字符的字符串
   _cChar	:	要在字符串中去掉的字符
*@param[out]
*@return
****************************************/
void CUtils::DelCharOnString(std::string& _sStr, char _cChar)
{
	for (std::string::iterator it = _sStr.begin();
		it != _sStr.end(); it++)
	{
		if (it == _sStr.end() - 1)
		{
			continue;
		}
		if (*it == _cChar || *it == ' ')
		{
			_sStr.erase(it);
		}
	}
}

/****************************************!
*@brief  使用C#的方式进行图片的读取
*@author Jinzi
*@date   2019/09/19 17:57:42
*@param[in]
   _cchpPicJpgPath	:	jpg文件路径
*@param[out]
*@return
****************************************/
std::string CUtils::ReadJpgInfoWithCSharp(const char* _cchpPicJpgPath)
{
	if (_cchpPicJpgPath == nullptr)
	{
		return "";
	}
	FuncReadPictureInfo ReadPictureInfo = nullptr;
	std::string sSavePicInfo;
	std::string qsDllPath = gl_opSaveIniInfo->sReadPicDllPath;
	QString sPicPath(_cchpPicJpgPath);
	//qsDllPath = QDir::toNativeSeparators(qsDllPath);
	sPicPath = sPicPath.replace("/", "\\\\");
	HMODULE pInstance = LoadLibraryA(qsDllPath.c_str());
	if (pInstance != nullptr)
	{
		ReadPictureInfo = (FuncReadPictureInfo)GetProcAddress(pInstance, "ReadPictureInfo");
		if (ReadPictureInfo != nullptr)
		{
			if (ReadPictureInfo(sPicPath.toLocal8Bit().data(), sSavePicInfo))
			{
				QString qsStr(sSavePicInfo.c_str());
				QStringList qsList = qsStr.split("-");
				for (int i = 0; i < qsList.count(); i++)
				{
					qsList[i] = "0x" + qsList[i];
					QString str(qsList[i]);
					int dec = str.toInt(0,16);
					char st = dec;
					sSavePicInfo.clear();
					sSavePicInfo.append(&st);
				}
				return sSavePicInfo;
			}
		}
	}
	return "";
}

 /****************************************!
 *@brief  将本地图片图片base64编码
 *@author Jinzi
 *@date   2019/09/26 9:44:44
 *@param[in]  
	_qsPicPath	:	本地图片的路径
 *@param[out] 
 *@return     
 ****************************************/
QByteArray CUtils::LocalImageToBase64(QString _qsPicPath)
{
	QImage image(_qsPicPath);
	QByteArray oByteArray;
	QBuffer oBuf(&oByteArray);
	image.save(&oBuf, "PNG", 20);
	QByteArray oPicBase64 = oByteArray.toBase64();
	oBuf.close();
	return oPicBase64;
}