#include "./EntranceGuard/EntranceGuard.h"
#include <QtWidgets/QApplication>
#include "./Utils/json.h"

extern CSaveiniInfo* gl_opSaveIniInfo;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	boost::property_tree::ptree oParentPtree;
	/*\ ¶ÁÈ¡ÅäÖÃÎÄ¼þ \*/
	read_ini("../bin/Config/config.ini", oParentPtree);
	gl_opSaveIniInfo->sIp = oParentPtree.get<std::string>("IP.sIp");
	gl_opSaveIniInfo->sPort = oParentPtree.get<std::string>("PORT.sPort");
	gl_opSaveIniInfo->sReadPicDllPath = oParentPtree.get<std::string>("PATH.sReadPicDllPath");
	EntranceGuard w;
	w.show();
	return a.exec();
}
