#ifndef KEEPERMAINWIDGET_HPP
#define KEEPERMAINWIDGET_HPP

#include <QWidget>
#include <memory>

#include "Core/UserSession/KeeperAccessView.hpp"
#include "Widgets/TrWidget.hpp"

class AnimalsPage;
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

    // Вьюха принадлежит Core (UserSession): виджет только читает её
    // и подписывается на сигналы, но не удаляет.
    void setAccess(KeeperAccessView *view);

private slots:
    void onEnclosureSelected(int enclosureId);
    void onAnimalSelected(int animalId);
    void onEnclosuresRefreshed(const QList<KeeperAccessView::EnclosureInfo> &enclosures);
    void onAnimalsLoaded(const QList<KeeperAccessView::AnimalInfo> &animals);

protected:
    void retranslateUi() override;

private:
    void setupPages();
    void goBack();
    void updateNavigation();

    std::unique_ptr<Ui::KeeperMainWidget> ui;
    EnclosurePage *m_enclosurePage = nullptr;
    AnimalsPage *m_animalsPage = nullptr;
    KeeperAccessView *m_access = nullptr;
};

#endif // KEEPERMAINWIDGET_HPP
