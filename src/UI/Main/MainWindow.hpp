#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <memory>

namespace Ui {
class MainWindow;
}

class UserSession;

// Тонкая оболочка QMainWindow: всё содержимое окна — в MainPage (TrWidget),
// поэтому здесь нет ни интерфейсных виджетов, ни обработки смены языка.
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(UserSession *session = nullptr, QWidget *parent = nullptr);
	~MainWindow() override;
private:
    std::unique_ptr<Ui::MainWindow> ui;
};

#endif // MAINWINDOW_HPP
