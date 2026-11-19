#ifndef EVENTSPAGE_HPP
#define EVENTSPAGE_HPP

#include <QList>
#include <memory>

#include "VetData.hpp"
#include "Widgets/TrWidget.hpp"

namespace Ui {
class EventsPage;
}

// Список медицинских мероприятий, назначенных текущему ветеринарному врачу.
class EventsPage : public TrWidget
{
    Q_OBJECT

public:
    explicit EventsPage(QWidget *parent = nullptr);
    ~EventsPage() override;

    void setEvents(const QList<VetData::MedicalEventInfo> &events);

protected:
    void retranslateUi() override;

private:
    void rebuild();

    std::unique_ptr<Ui::EventsPage> ui;
    QList<VetData::MedicalEventInfo> m_events;
};

#endif // EVENTSPAGE_HPP
