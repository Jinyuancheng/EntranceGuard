#include "./utils.h"
#include <Windows.h>
#include <QString>
#include <QCoreApplication>
#include <QDir>
#include <QRegExp>

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
char* CUtils::ReadJpgInfoWithCSharp(const char* _cchpPicJpgPath)
{
	if (_cchpPicJpgPath == nullptr)
	{
		return nullptr;
	}
	char chpBuf[200 * 1024] = { 0 };
	if (chpBuf == nullptr)
	{
		return nullptr;
	}
	memset(chpBuf, 0, sizeof(chpBuf));
	FILE* fileI = fopen(_cchpPicJpgPath, "rb");
	if (!fileI)
	{
		return nullptr;
	}
	int iLength = fread(chpBuf, 1, 200 * 1024, fileI);
	if (iLength > 0)
	{
		char* chpRetBuf = (char*)malloc(iLength);
		strncpy(chpRetBuf, chpBuf, iLength);
		return chpRetBuf;
	}
	else
	{
		return nullptr;
	}

	//FuncReadPictureInfo ReadPictureInfo = nullptr;
	//std::string sSavePicInfo;
	//std::string qsDllPath = gl_opSaveIniInfo->sReadPicDllPath;
	//QString sPicPath(_cchpPicJpgPath);
	////qsDllPath = QDir::toNativeSeparators(qsDllPath);
	//sPicPath = sPicPath.replace("/", "\\\\");
	//HMODULE pInstance = LoadLibraryA(qsDllPath.c_str());
	//if (pInstance != nullptr)
	//{
	//	ReadPictureInfo = (FuncReadPictureInfo)GetProcAddress(pInstance, "ReadPictureInfo");
	//	if (ReadPictureInfo != nullptr)
	//	{
	//		if (ReadPictureInfo(sPicPath.toLocal8Bit().data(), sSavePicInfo))
	//		{
	//			QString qsStr(sSavePicInfo.c_str());
	//			QStringList qsList = qsStr.split("-");
	//			for (int i = 0; i < qsList.count(); i++)
	//			{
	//				qsList[i] = "0x" + qsList[i];
	//				QString str(qsList[i]);
	//				int dec = str.toInt(0,16);
	//				char st = dec;
	//				sSavePicInfo.clear();
	//				sSavePicInfo.append(&st);
	//			}
	//			return sSavePicInfo;
	//		}
	//	}
	//}
	//return "";
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

 /****************************************!
 *@brief  判断ip是否合法
 *@author Jinzi
 *@date   2019/09/29 11:42:44
 *@param[in]  
	_qsIp		:	需要校验的ip地址
 *@param[out] 
 *@return  true 合法 false 不合法   
 ****************************************/
bool CUtils::JuageIpLegal(QString& _qsIp)
{
	bool bIsSucc = false;
	/*\ \d匹配数字 \?匹配零次活一次 {3}重复三次 \*/
	QRegExp oRegExpIp("((25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))\\.){3}(25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))");
	if (oRegExpIp.exactMatch(_qsIp))
	{
		bIsSucc = true;
	}
	return bIsSucc;
}


 /****************************************!
 *@brief  判断端口是否合法
 *@author Jinzi
 *@date   2019/09/29 11:45:01
 *@param[in]  
	_qsPort		:	需要校验的端口
 *@param[out] 
 *@return     true 合法 false 不合法   
 ****************************************/
bool CUtils::JuagePortLegal(QString& _qsPort)
{
	bool bIsSucc = false;
	QRegExp oRegExpPort("^[1-9]$|(^[1-9][0-9]$)|(^[1-9][0-9][0-9]$)|(^[1-9][0-9][0-9][0-9]$)|(^[1-6][0-5][0-5][0-3][0-5]$)");
	if (oRegExpPort.exactMatch(_qsPort))
	{
		bIsSucc = true;
	}
	return bIsSucc;
}

 /****************************************!
 *@brief  判断数字是否是从1到9999以内的
 *@author Jinzi
 *@date   2019/09/29 14:03:37
 *@param[in]  
	_qsNumber	:	要比对的数字
 *@param[out] 
 *@return     true 合法 false 不合法
 ****************************************/
bool CUtils::JuageNumberLegal(QString& _qsNumber)
{
	bool bIsSucc = false;
	QRegExp oRegExpNum("(^[1-9]$)|(^[1-9][0-9]$)|(^[1-9][0-9][0-9]$)|(^[1-9][0-9][0-9][0-9]$)|(^[1-9][0-9][0-9][0-9][0-9]$)|(^[1-9][0-9][0-9][0-9][0-9][0-9]$)");
	if (oRegExpNum.exactMatch(_qsNumber))
	{
		bIsSucc = true;
	}
	return bIsSucc;
}

 /****************************************!
 *@brief  判断文件路径是否合法
 *@author Jinzi
 *@date   2019/09/29 14:35:46
 *@param[in]  
	_qsFilePath		:	文件路径
 *@param[out] 
 *@return     true 合法 false 不合法
 ****************************************/
bool CUtils::JuageFilePathLegal(QString& _qsFilePath)
{
	//"(^[a-zA-Z]:(/[a-zA-Z0-9]+)+).([a-zA-Z0-9]+)$|(^[a-zA-Z]:(\\\\[a-zA-Z0-9]+)+).([a-zA-Z0-9]+)$"
	bool bIsSucc = false;
	QRegExp oRegExpFilePath("(^[a-zA-Z]:(/[a-zA-Z0-9]+)+).([a-zA-Z0-9]+)$|(^[a-zA-Z]:(\\\\[a-zA-Z0-9]+)+).([a-zA-Z0-9]+)$");
	if (oRegExpFilePath.exactMatch(_qsFilePath))
	{
		bIsSucc = true;
	}
	return bIsSucc;
}