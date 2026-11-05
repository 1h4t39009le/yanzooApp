#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QDebug>
#include "Language/LanguageManager.hpp"
#include "Theme/ThemeManager.hpp"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);

    setMinimumSize(900, 600);
    resize(1100, 700);
	
	connect(ui->themeButton, &QPushButton::toggled, 
		this, &MainWindow::onThemeButtonToggled);

	connect(&LanguageManager::instance(), &LanguageManager::languageChanged,
		this, [this](LanguageManager::Language) {
			ui->retranslateUi(this);
		});
}

MainWindow::~MainWindow() = default;

void MainWindow::onThemeButtonToggled(bool dark){
	ThemeManager::instance().applyTheme(dark? ThemeManager::Dark : ThemeManager::Light);
}