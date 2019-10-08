#include "./utils.h"
#include <Windows.h>
#include <QString>
#include <QCoreApplication>
#include <QDir>
#include <QRegExp>

#include "../Public/PublicDefine.h"


typedef bool(*FuncReadPictureInfo)(const char*, std::string&);
/*\ �洢·�� \*/
extern CSaveiniInfo* gl_opSaveIniInfo;

/****************************************!
*@brief  ȥ�����ַ����е�ָ���ַ�
*@author Jinzi
*@date   2019/09/19 8:42:56
*@param[in]
   _sStr	:	Ҫȥ��ָ���ַ����ַ���
   _cChar	:	Ҫ���ַ�����ȥ�����ַ�
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
*@brief  ʹ��C#�ķ�ʽ����ͼƬ�Ķ�ȡ
*@author Jinzi
*@date   2019/09/19 17:57:42
*@param[in]
   _cchpPicJpgPath	:	jpg�ļ�·��
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
 *@brief  ������ͼƬͼƬbase64����
 *@author Jinzi
 *@date   2019/09/26 9:44:44
 *@param[in]  
	_qsPicPath	:	����ͼƬ��·��
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
 *@brief  �ж�ip�Ƿ�Ϸ�
 *@author Jinzi
 *@date   2019/09/29 11:42:44
 *@param[in]  
	_qsIp		:	��ҪУ���ip��ַ
 *@param[out] 
 *@return  true �Ϸ� false ���Ϸ�   
 ****************************************/
bool CUtils::JuageIpLegal(QString& _qsIp)
{
	bool bIsSucc = false;
	/*\ \dƥ������ \?ƥ����λ�һ�� {3}�ظ����� \*/
	QRegExp oRegExpIp("((25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))\\.){3}(25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))");
	if (oRegExpIp.exactMatch(_qsIp))
	{
		bIsSucc = true;
	}
	return bIsSucc;
}


 /****************************************!
 *@brief  �ж϶˿��Ƿ�Ϸ�
 *@author Jinzi
 *@date   2019/09/29 11:45:01
 *@param[in]  
	_qsPort		:	��ҪУ��Ķ˿�
 *@param[out] 
 *@return     true �Ϸ� false ���Ϸ�   
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
 *@brief  �ж������Ƿ��Ǵ�1��9999���ڵ�
 *@author Jinzi
 *@date   2019/09/29 14:03:37
 *@param[in]  
	_qsNumber	:	Ҫ�ȶԵ�����
 *@param[out] 
 *@return     true �Ϸ� false ���Ϸ�
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
 *@brief  �ж��ļ�·���Ƿ�Ϸ�
 *@author Jinzi
 *@date   2019/09/29 14:35:46
 *@param[in]  
	_qsFilePath		:	�ļ�·��
 *@param[out] 
 *@return     true �Ϸ� false ���Ϸ�
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