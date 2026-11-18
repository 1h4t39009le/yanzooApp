#include "MainPage.hpp"
#include "ui_MainPage.h"

#include <QMessageBox>

#include "Login.hpp"
#include "Language/LanguageManager.hpp"
#include "Theme/ThemeManager.hpp"
#include "Core/UserSession/UserSession.hpp"

MainPage::MainPage(QWidget *parent)
    : TrWidget(parent)
    , ui(std::make_unique<Ui::MainPage>())
{
    ui->setupUi(this);

    // Первой в стеке идёт страница входа: элементы сессии (карточка профиля
    // и выход) появляются только после успешной авторизации.
    ui->roleStack->setCurrentWidget(ui->loginPage);
    setSessionUiVisible(false);

    connect(ui->themeButton, &QPushButton::toggled,
            this, &MainPage::onThemeButtonToggled);
    connect(ui->langButton, &QPushButton::clicked,
            this, &MainPage::onLanguageButtonClicked);
    connect(ui->signOutButton, &QPushButton::clicked,
            this, &MainPage::onSignOutClicked);

    // Страница входа сама проверяет ввод и отдаёт логин сигналом.
    connect(ui->loginPage, &Login::loginRequested,
            this, &MainPage::onLoginRequested);
}

MainPage::~MainPage() = default;

void MainPage::setSession(UserSession *session)
{
    if (m_session == session) {
        return;
    }

    if (m_session) {
        disconnect(m_session, nullptr, this, nullptr);
    }

    m_session = session;
    if (!m_session) {
        return;
    }

    // Handle successful authentication notifications
    connect(m_session, &UserSession::loginEnded, this, [this](RoleAccessView *) {
        ui->loginPage->setLoginInProgress(false);
        ui->loginPage->resetForm();
        setSessionUiVisible(true);
        // Пока роль всегда «Кипер». Страницы роли получают данные из
        // фейкового списка KeeperData, без обращения к Core.
        ui->roleStack->setCurrentWidget(ui->keeperPage);
    });

    // Handle active database authorization or networking exceptions
    connect(m_session, &DbAccessController::errorOccurred, this, [this](const QString &message) {
        ui->loginPage->setLoginInProgress(false);
        QMessageBox::critical(this, tr("Authorization Error"), message);
    });
}

void MainPage::retranslateUi()
{
    ui->retranslateUi(this);
}

void MainPage::onThemeButtonToggled(bool dark)
{
    ThemeManager::instance().applyTheme(dark ? ThemeManager::Dark : ThemeManager::Light);
}

void MainPage::onLanguageButtonClicked()
{
    auto &manager = LanguageManager::instance();
    const auto languages = manager.supportedLanguages();

    // Индекс берём из текущего языка, а не из локального счётчика: стартовый
    // язык выставляет applySystemLanguage(), и счётчик с ним расходился.
    const int current = languages.indexOf(manager.currentLanguage());
    manager.applyLanguage(languages[(current + 1) % languages.size()]);
}

void MainPage::onLoginRequested(const QString &login)
{
    if (!m_session) {
        return;
    }

    ui->loginPage->setLoginInProgress(true);
    m_session->requestLogin(login);
}

void MainPage::onSignOutClicked()
{
    // Сессия должна отпустить вьюху и сотрудника, иначе следующий вход
    // получит данные предыдущего пользователя.
    if (m_session) {
        m_session->logout();
    }

    setSessionUiVisible(false);
    ui->loginPage->resetForm();
    ui->roleStack->setCurrentWidget(ui->loginPage);
}

void MainPage::setSessionUiVisible(bool visible)
{
    ui->profileCard->setVisible(visible);
    ui->signOutButton->setVisible(visible);
}
