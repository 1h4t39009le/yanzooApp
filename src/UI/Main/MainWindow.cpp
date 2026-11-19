#include "MainWindow.hpp"

#include "Pages/MainPage.hpp"

MainWindow::MainWindow(UserSession *session, QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("yanzoo"));
    setMinimumSize(900, 600);
    resize(1100, 700);

    auto *mainPage = new MainPage(this);
    setCentralWidget(mainPage);
    mainPage->setSession(session);
}

MainWindow::~MainWindow() = default;
