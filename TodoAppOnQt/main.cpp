#include "stdafx.h"
#include "TodoAppOnQt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	TodoAppOnQt window;
    window.show(); 
	return app.exec(); // Event loop start
}
