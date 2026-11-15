#ifndef LOGIN_HPP
#define LOGIN_HPP

#include "Widgets/TrWidget.hpp"
#include <memory>

namespace Ui {
class Login;
}

// Панель входа для стека страниц MainWindow (roleStack).
// К БД не обращается: проверяет, что логин не пустой, и сообщает его сигналом.
class Login : public TrWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login() override;

    // Пока Core ищет сотрудника, кнопка и поле блокируются.
    void setLoginInProgress(bool inProgress);
    // Возврат к вводу после выхода из роли.
    void resetForm();

signals:
    void loginRequested(const QString &login);
protected:
    void retranslateUi() override;
private slots:
    void onLoginClicked();

private:
    std::unique_ptr<Ui::Login> ui;
};

#endif // LOGIN_HPP
