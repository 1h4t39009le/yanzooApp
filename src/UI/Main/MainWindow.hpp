#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

class UserSession;

// Тонкая оболочка QMainWindow: всё содержимое окна — в MainPage (TrWidget),
// поэтому здесь нет ни интерфейсных виджетов, ни обработки смены языка.
// Central widget создаётся в конструкторе, .ui для окна не нужен.
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(UserSession *session = nullptr, QWidget *parent = nullptr);
    ~MainWindow() override;
};

#endif // MAINWINDOW_HPP
