#include "MainWindow.h"
#include <QApplication>
#include "GLMesh.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}
