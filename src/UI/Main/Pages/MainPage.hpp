#ifndef MAINPAGE_HPP
#define MAINPAGE_HPP

#include <QString>
#include <memory>

#include "Widgets/TrWidget.hpp"

namespace Ui {
class MainPage;
}

class UserSession;

// Вся клиентская часть окна: стек страниц ролей и нижняя панель.
//
// Живёт внутри MainWindow как central widget, поэтому наследуется от TrWidget
// и переводится штатным QEvent::LanguageChange — отдельная подписка на
// LanguageManager для интерфейса не нужна.
class MainPage : public TrWidget
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = nullptr);
    ~MainPage() override;

    // Подключает Core: страницы роли получают данные через сессию.
    void setSession(UserSession *session);

protected:
    void retranslateUi() override;

private slots:
    void onThemeButtonToggled(bool dark);
    void onLanguageButtonClicked();
    void onLoginRequested(const QString &login);
    void onSignOutClicked();

private:
    void setSessionUiVisible(bool visible);

    UserSession *m_session = nullptr;
    std::unique_ptr<Ui::MainPage> ui;
};

#endif // MAINPAGE_HPP
