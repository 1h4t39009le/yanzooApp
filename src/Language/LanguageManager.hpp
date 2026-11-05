#ifndef LANGUAGEMANAGER_HPP
#define LANGUAGEMANAGER_HPP

#include <QList>
#include <QObject>
#include <QString>
#include <QTranslator>

class LanguageManager : public QObject
{
    Q_OBJECT

public:
    enum Language {
        English,
        Russian
    };
    Q_ENUM(Language)

    static LanguageManager& instance();

    LanguageManager(const LanguageManager&) = delete;
    LanguageManager& operator=(const LanguageManager&) = delete;

    // Устанавливает язык интерфейса. Возвращает false, если .qm не найден.
    bool applyLanguage(Language language);
    bool applyLanguage(const QString &code); // "ru", "ru_RU", "en_US", ...
    bool applySystemLanguage();

    Language currentLanguage() const { return m_currentLanguage; }
    QString currentLocaleCode() const { return localeCode(m_currentLanguage); }

    static QString localeCode(Language language);
    static QString displayName(Language language);
    static QList<Language> supportedLanguages();

signals:
    void languageChanged(Language newLanguage);

private:
    explicit LanguageManager(QObject *parent = nullptr);

    bool installTranslation(Language language);
    static Language languageForLocale(const QString &localeCode);

    Language m_currentLanguage = English;
    QTranslator m_translator;
};

#endif // LANGUAGEMANAGER_HPP
