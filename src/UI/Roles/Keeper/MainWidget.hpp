#ifndef KEEPERMAINWIDGET_HPP
#define KEEPERMAINWIDGET_HPP

#include <QWidget>
#include <memory>
#include "Widgets/TrWidget.hpp"

class EnclosurePage;

namespace Ui {
class KeeperMainWidget;
}
class KeeperMainWidget : public TrWidget
{
    Q_OBJECT

public:
    explicit KeeperMainWidget(QWidget *parent = nullptr);
    ~KeeperMainWidget() override;
private slots:
    void onEnclosureSelected(int enclosureId);
	
protected:
    void retranslateUi() override;
private:
    void setupPages();
    void loadData();
    void goBack();
    void updateNavigation();


    std::unique_ptr<Ui::KeeperMainWidget> ui;
    EnclosurePage *m_enclosurePage = nullptr;
};

#endif // KEEPERMAINWIDGET_HPP
