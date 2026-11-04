#include "ThemeManager.hpp"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QFontDatabase>
#include <QTextStream>

ThemeManager::ThemeManager(QObject *parent)
    : QObject(parent)
{
	QFontDatabase::addApplicationFont(QStringLiteral(":/fonts/roboto"));
    QFontDatabase::addApplicationFont(QStringLiteral(":/fonts/roboto-italic"));
    QFontDatabase::addApplicationFont(QStringLiteral(":/fonts/fredoka"));
}

ThemeManager& ThemeManager::instance()
{
    static ThemeManager instance;
    return instance;
}

QString ThemeManager::stylesheetFor(Theme theme)
{
    QString &cached = theme == Theme::Dark ? m_darkStyle : m_lightStyle;
    if (!cached.isNull()) {
        return cached;
    }

    const QString qssPath = theme == Theme::Dark
        ? QStringLiteral(":/themes/dark")
        : QStringLiteral(":/themes/light");

    QFile file(qssPath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qWarning() << "[ThemeManager] Не удалось открыть файл ресурсов темы:" << qssPath;
        cached = QString();
        return cached;
    }

    QTextStream stream(&file);
    cached = stream.readAll();
    file.close();
    return cached;
}

void ThemeManager::applyTheme(Theme theme)
{
    const QString stylesheet = stylesheetFor(theme);
    if (stylesheet.isEmpty()) {
        return;
    }
    qApp->setStyleSheet(stylesheet);
    m_currentTheme = theme;
    emit themeChanged(m_currentTheme);
	
	qDebug() << QString("[ThemeManager] Theme changed %1").arg(isDark() ? "Dark" : "Light");
}
