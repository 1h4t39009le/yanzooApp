#include "LanguageManager.hpp"

#include <QCoreApplication>
#include <QDebug>
#include <QLocale>
#include <QStringList>

LanguageManager::LanguageManager(QObject *parent)
    : QObject(parent)
{
}

LanguageManager& LanguageManager::instance()
{
    static LanguageManager instance;
    return instance;
}

QString LanguageManager::localeCode(Language language)
{
    switch (language) {
    case Russian:
        return QStringLiteral("ru");
    case English:
    default:
        return QStringLiteral("en");
    }
}

QString LanguageManager::displayName(Language language)
{
    switch (language) {
    case Russian:
        return QStringLiteral("Русский");
    case English:
    default:
        return QStringLiteral("English");
    }
}

QList<LanguageManager::Language> LanguageManager::supportedLanguages()
{
    return { English, Russian };
}

LanguageManager::Language LanguageManager::languageForLocale(const QString &code)
{
    const QString language = code.left(2).toLower();
    if (language == QStringLiteral("ru")) {
        return Russian;
    }
    return English;
}

bool LanguageManager::applyLanguage(Language language)
{
    const bool loaded = installTranslation(language);
    const bool changed = language != m_currentLanguage;

    QLocale::setDefault(QLocale(localeCode(language)));
    m_currentLanguage = language;

    if (changed) {
        emit languageChanged(language);
    }

    qDebug() << QStringLiteral("[LanguageManager] Language changed to %1")
                    .arg(localeCode(language));

    return loaded;
}

bool LanguageManager::applyLanguage(const QString &code)
{
    return applyLanguage(languageForLocale(code));
}

bool LanguageManager::applySystemLanguage()
{
    return applyLanguage(QLocale::system().name());
}

bool LanguageManager::installTranslation(Language language)
{
    QCoreApplication::removeTranslator(&m_translator);

    // Английский — исходный язык приложения, отдельный перевод не нужен.
    if (language == English) {
        return true;
    }

    const QString code = localeCode(language);
    const QStringList candidates = {
        QStringLiteral(":/i18n/yanzoo_%1.qm").arg(code),
        QCoreApplication::applicationDirPath()
            + QStringLiteral("/translations/yanzoo_%1.qm").arg(code)
    };

    for (const QString &path : candidates) {
        if (m_translator.load(path)) {
            QCoreApplication::installTranslator(&m_translator);
            return true;
        }
    }

    qWarning() << "[LanguageManager] Перевод не найден:" << code
               << "Искали:" << candidates;
    return false;
}
