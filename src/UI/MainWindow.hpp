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
private:
    std::unique_ptr<Ui::MainWindow> ui;
};

#endif // MAINWINDOW_HPP
