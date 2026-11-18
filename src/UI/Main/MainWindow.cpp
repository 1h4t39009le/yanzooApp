#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "Pages/MainPage.hpp"

MainWindow::MainWindow(UserSession *session, QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);

    setMinimumSize(900, 600);
    resize(1100, 700);

    // Central widget создаётся uic по promotion из MainWindow.ui.
    ui->centralwidget->setSession(session);
}

MainWindow::~MainWindow() = default;
