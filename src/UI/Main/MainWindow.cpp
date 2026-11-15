#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include "Pages/Login.hpp"
#include "Language/LanguageManager.hpp"
#include "Theme/ThemeManager.hpp"
#include "Core/UserSession/UserSession.hpp"

MainWindow::MainWindow(UserSession *session, QWidget *parent)
    : QMainWindow(parent)
    , m_session(session)
    , ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);

    setMinimumSize(900, 600);
    resize(1100, 700);

    // Первой в стеке идёт страница входа: элементы сессии (карточка профиля
    // и выход) появляются только после успешной авторизации.
    ui->roleStack->setCurrentWidget(ui->loginPage);
    setSessionUiVisible(false);

    connect(ui->themeButton, &QPushButton::toggled,
            this, &MainWindow::onThemeButtonToggled);
    connect(ui->langButton, &QPushButton::clicked,
            this, &MainWindow::onLanguageButtonClicked);
    connect(ui->signOutButton, &QPushButton::clicked,
            this, &MainWindow::onSignOutClicked);

    connect(&LanguageManager::instance(), &LanguageManager::languageChanged,
            this, [this](LanguageManager::Language) {
                ui->retranslateUi(this);
            });

    // Страница входа сама проверяет ввод и отдаёт логин сигналом.
    connect(ui->loginPage, &Login::loginRequested,
            this, &MainWindow::onLoginRequested);

    // Hook up the Core UserSession integration layers
    if (m_session) {
        // Handle successful authentication notifications
        connect(m_session, &UserSession::loginEnded, this, [this](const QVariant &) {
            ui->loginPage->setLoginInProgress(false);
            ui->loginPage->resetForm();
            setSessionUiVisible(true);
            // Заглушка рабочей области: позже здесь будет страница нужной роли.
            ui->roleStack->setCurrentWidget(ui->welcomePage);
        });

        // Handle active database authorization or networking exceptions
        connect(m_session, &UserSession::errorOccurred, this, [this](const QString &message) {
            ui->loginPage->setLoginInProgress(false);
            QMessageBox::critical(this, tr("Authorization Error"), message);
        });
    }
}

MainWindow::~MainWindow() = default;

void MainWindow::onThemeButtonToggled(bool dark){
    ThemeManager::instance().applyTheme(dark ? ThemeManager::Dark : ThemeManager::Light);
}

void MainWindow::onLanguageButtonClicked(){
    const auto& langs = LanguageManager::instance().supportedLanguages();
    m_langIdx = (m_langIdx + 1) % langs.size();
    LanguageManager::instance().applyLanguage(langs[m_langIdx]);
}

void MainWindow::onLoginRequested(const QString &login){
    if (!m_session) {
        return;
    }
    ui->loginPage->setLoginInProgress(true);
    m_session->requestLogin(login);
}

void MainWindow::onSignOutClicked(){
    setSessionUiVisible(false);
    ui->loginPage->resetForm();
    ui->roleStack->setCurrentWidget(ui->loginPage);
}

void MainWindow::setSessionUiVisible(bool visible){
    ui->profileCard->setVisible(visible);
    ui->signOutButton->setVisible(visible);
}
