#ifndef VETMAINWIDGET_HPP
#define VETMAINWIDGET_HPP

#include <QWidget>
#include <memory>

#include "Core/UserSession/VetAccessView.hpp"
#include "Widgets/TrWidget.hpp"

class EventsPage;

namespace Ui {
class VetMainWidget;
}

class VetMainWidget : public TrWidget
{
    Q_OBJECT

public:
    explicit VetMainWidget(QWidget *parent = nullptr);
    ~VetMainWidget() override;

    // Вьюха принадлежит Core (UserSession): виджет только читает её
    // и подписывается на сигналы, но не удаляет.
    void setAccess(VetAccessView *view);

private slots:
    void onAssignedMedicalEventsLoaded(const QList<VetAccessView::MedicalEventInfo> &events);

protected:
    void retranslateUi() override;

private:
    void setupPages();
    void goBack();
    void updateNavigation();

    std::unique_ptr<Ui::VetMainWidget> ui;
    EventsPage *m_eventsPage = nullptr;
    VetAccessView *m_access = nullptr;
};

#endif // VETMAINWIDGET_HPP
