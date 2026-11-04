#ifndef THEMEMANAGER_HPP
#define THEMEMANAGER_HPP

#include <QObject>
#include <QString>

class ThemeManager : public QObject
{
    Q_OBJECT

public:
    enum Theme {
        Light,
        Dark
    };
    Q_ENUM(Theme)

    static ThemeManager& instance();

    ThemeManager(const ThemeManager&) = delete;
    ThemeManager& operator=(const ThemeManager&) = delete;

    void applyTheme(Theme theme);

    Theme currentTheme() const { return m_currentTheme; }
    bool isDark() const { return m_currentTheme == Theme::Dark; }

signals:
    void themeChanged(Theme newTheme);

private:
    explicit ThemeManager(QObject *parent = nullptr);

    QString stylesheetFor(Theme theme);

    Theme m_currentTheme = Theme::Light;
    QString m_lightStyle;
    QString m_darkStyle;
};

#endif // THEMEMANAGER_HPP
