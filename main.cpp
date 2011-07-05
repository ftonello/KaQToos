#include <QApplication>

#include "kaqtoos.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QApplication::setOrganizationName("Felipe Ferreri Tonello");
	QApplication::setApplicationName("KaQToos");

	Kaqtoos k;
	k.show();

	return app.exec();
}
