#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QString>
#include <memory>

#include "Theme/ThemeManager.hpp"

namespace Ui {
class MainWindow;
}

class UserSession; 

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(UserSession *session = nullptr, QWidget *parent = nullptr);
	~MainWindow() override;
private slots:
	void onThemeButtonToggled(bool dark);
	void onLanguageButtonClicked();
	void onLoginRequested(const QString &login);
	void onSignOutClicked();
private:
	void setSessionUiVisible(bool visible);

	size_t m_langIdx=0;
	UserSession *m_session;
    std::unique_ptr<Ui::MainWindow> ui;
};

#endif // MAINWINDOW_HPP
