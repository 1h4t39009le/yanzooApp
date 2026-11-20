#include "MainWidget.hpp"
#include "ui_VetMainWidget.h"

#include "VetData.hpp"
#include "Pages/EventsPage.hpp"

#include <QMessageBox>

VetMainWidget::VetMainWidget(QWidget *parent)
    : TrWidget(parent)
    , ui(std::make_unique<Ui::VetMainWidget>())
{
    ui->setupUi(this);

    setupPages();

    connect(ui->backButton, &QPushButton::clicked, this, &VetMainWidget::goBack);

    updateNavigation();
}

VetMainWidget::~VetMainWidget() = default;

void VetMainWidget::setAccess(VetAccessView *view)
{
    if (m_access == view) {
        return;
    }
    if (m_access) {
        disconnect(m_access, nullptr, this, nullptr);
    }

    m_access = view;

    // Сессия всегда начинается со списка назначенных мероприятий.
    ui->pageStack->setCurrentWidget(m_eventsPage);
    updateNavigation();

    if (!m_access) {
        // Отцепились от Core: не оставляем данные прошлой сессии на экране.
        m_eventsPage->setEvents({});
        return;
    }

    connect(m_access, &VetAccessView::assignedMedicalEventsLoaded,
            this, &VetMainWidget::onAssignedMedicalEventsLoaded);
    connect(m_access, &VetAccessView::errorOccurred,
            this, [this](const QString &message) {
                QMessageBox::critical(this, tr("Data Error"), message);
            });

    // Данные придут обратно синхронно сигналом assignedMedicalEventsLoaded().
    m_access->refreshAssignedMedicalEvents();
}

void VetMainWidget::setupPages()
{
    m_eventsPage = new EventsPage(ui->pageStack);

    ui->pageStack->addWidget(m_eventsPage);
    ui->pageStack->setCurrentWidget(m_eventsPage);
}

void VetMainWidget::onAssignedMedicalEventsLoaded(const QList<VetAccessView::MedicalEventInfo> &events)
{
    // Адаптер Core -> UI: страница не знает про вьюху и работает
    // с собственным представлением данных.
    QList<VetData::MedicalEventInfo> items;
    items.reserve(events.size());
    for (const VetAccessView::MedicalEventInfo &event : events) {
        items.append({event.eventId, event.type, event.nickname, event.date,
                      event.species, event.age});
    }

    m_eventsPage->setEvents(items);
}

void VetMainWidget::goBack()
{
    const int index = ui->pageStack->currentIndex();
    if (index > 0) {
        ui->pageStack->setCurrentIndex(index - 1);
    }

    updateNavigation();
}

void VetMainWidget::updateNavigation()
{
    // На первой странице возвращаться некуда.
    const bool canGoBack = ui->pageStack->currentIndex() > 0;
    ui->backButton->setVisible(canGoBack);
    ui->topBar->setVisible(canGoBack);
}

void VetMainWidget::retranslateUi()
{
    ui->retranslateUi(this);
}
