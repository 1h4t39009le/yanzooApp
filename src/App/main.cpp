#include <QApplication>
#include <QStyleFactory>
#include <QString>

#include "Theme/ThemeManager.hpp"
#include "UI/MainWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	
    QApplication::setApplicationName(QStringLiteral("yanzoo App"));
    QApplication::setOrganizationName(QStringLiteral("yanek755"));
	
    QApplication::setStyle(QStyleFactory::create(QStringLiteral("Fusion")));
	
    ThemeManager::instance().applyTheme(ThemeManager::Light);
	
    MainWindow window;
    window.show();
	
    return app.exec();
}
