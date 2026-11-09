#include <QApplication>
#include <QStyleFactory>
#include <QString>

#include "Language/LanguageManager.hpp"
#include "Theme/ThemeManager.hpp"
#include "UI/MainWindow.hpp"

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(assetsLib_icons);
    Q_INIT_RESOURCE(assetsLib_themes);
    Q_INIT_RESOURCE(assetsLib_fonts);
	
	Q_INIT_RESOURCE(assetsLib_translations);
	
    QApplication app(argc, argv);
	
    QApplication::setApplicationName(QStringLiteral("yanzoo App"));
    QApplication::setOrganizationName(QStringLiteral("yanek755"));
	
    QApplication::setStyle(QStyleFactory::create(QStringLiteral("Fusion")));
	
    ThemeManager::instance().applyTheme(ThemeManager::Light);
    LanguageManager::instance().applySystemLanguage();
	
    MainWindow window;
    window.show();
	
    return app.exec();
}
