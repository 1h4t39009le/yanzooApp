#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QMessageBox>
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
	
    connect(ui->themeButton, &QPushButton::toggled, 
            this, &MainWindow::onThemeButtonToggled);
    connect(ui->langButton, &QPushButton::clicked,
            this, &MainWindow::onLanguageButtonClicked);

    connect(&LanguageManager::instance(), &LanguageManager::languageChanged,
            this, [this](LanguageManager::Language) {
                ui->retranslateUi(this);
            });

    // Hook up the Core UserSession integration layers
    if (m_session) {
        // Trigger the authentication database request when clicking the Login button
        connect(ui->loginButton, &QPushButton::clicked, this, [this]() {
            QString inputLogin = ui->loginLineEdit->text().trimmed();
            if (inputLogin.isEmpty()) {
                QMessageBox::warning(this, tr("Input Error"), tr("Please enter a valid login identifier."));
                return;
            }
            m_session->requestLogin(inputLogin);
        });

        // Handle successful authentication notifications
        connect(m_session, &UserSession::loginEnded, this, [this](const QVariant &view) {
            qDebug() << "[MainWindow] Login successful! Session payload received.";
            // Future dashboard initialization or stack widget layouts can be handled here
            QMessageBox::information(this, tr("Success"), tr("Login successful. View layer registered."));
        });

        // Handle active database authorization or networking exceptions
        connect(m_session, &UserSession::errorOccurred, this, [this](const QString &message) {
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
