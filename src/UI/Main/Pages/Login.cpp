#include "Login.hpp"
#include "ui_Login.h"

#include <QMessageBox>

Login::Login(QWidget *parent)
    : TrWidget(parent)
    , ui(std::make_unique<Ui::Login>())
{
    ui->setupUi(this);

    connect(ui->loginButton, &QPushButton::clicked,
            this, &Login::onLoginClicked);
    connect(ui->loginLineEdit, &QLineEdit::returnPressed,
            this, &Login::onLoginClicked);
}

void Login::retranslateUi()
{
    ui->retranslateUi(this);
}

Login::~Login() = default;

void Login::setLoginInProgress(bool inProgress)
{
    ui->loginButton->setEnabled(!inProgress);
    ui->loginLineEdit->setReadOnly(inProgress);
}

void Login::resetForm()
{
    ui->loginLineEdit->clear();
    setLoginInProgress(false);
    ui->loginLineEdit->setFocus();
}

void Login::onLoginClicked()
{
    const QString login = ui->loginLineEdit->text().trimmed();
    if (login.isEmpty()) {
        QMessageBox::warning(this, tr("Input Error"),
                             tr("Please enter a valid login identifier."));
        return;
    }

    emit loginRequested(login);
}
