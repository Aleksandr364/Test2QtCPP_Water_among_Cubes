#include "WelcomeWindow.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	WelcomeWindow welcomeWindow;
	welcomeWindow.show();
	return app.exec();
}