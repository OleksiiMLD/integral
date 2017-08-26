#include <iostream>
#include <QtGui>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QApplication::setStyle(new QPlastiqueStyle);
	QTranslator apptr;

//	apptr.load(":/translations/translations/integral_ua.qm");
	app.installTranslator(&apptr);

	MainWindow *main_window = new MainWindow;
	main_window->show();

	return app.exec();
}
