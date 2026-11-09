#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <memory>

#include "Theme/ThemeManager.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow() override;
private slots:
	void onThemeButtonToggled(bool dark);
	void onLanguageButtonClicked();
private:
	size_t m_langIdx=0;
    std::unique_ptr<Ui::MainWindow> ui;
};

#endif // MAINWINDOW_HPP
