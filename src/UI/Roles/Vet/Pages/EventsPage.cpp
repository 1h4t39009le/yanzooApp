#include "EventsPage.hpp"
#include "ui_EventsPage.h"

#include "Common/WidgetUpdateGuard.hpp"
#include "Widgets/ListCardDelegate.hpp"
#include "Widgets/ListCardWidget.hpp"

#include <QListWidgetItem>

EventsPage::EventsPage(QWidget *parent)
    : TrWidget(parent)
    , ui(std::make_unique<Ui::EventsPage>())
{
    ui->setupUi(this);

    ui->eventList->setMouseTracking(true);
    ui->eventList->viewport()->setMouseTracking(true);
    ui->eventList->setSelectionMode(QAbstractItemView::NoSelection);
    ui->eventList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->eventList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->eventList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	
	ui->eventList->setItemDelegate(new ListCardDelegate(ui->eventList));
    ui->eventList->setUniformItemSizes(true);
	
    rebuild();
}

EventsPage::~EventsPage() = default;

void EventsPage::setEvents(const QList<VetData::MedicalEventInfo> &events)
{
    m_events = events;
    rebuild();
}

void EventsPage::retranslateUi()
{
    ui->retranslateUi(this);
    rebuild();
}

void EventsPage::rebuild()
{
    WidgetUpdateGuard guard(ui->eventList);
    ui->eventList->clear();

    for (const VetData::MedicalEventInfo &event : m_events) {
        // Подписи собираем при отрисовке, чтобы они обновились при смене языка.
        const QString title = tr("%1: %2").arg(event.type, event.nickname);
        const QString subtitle = tr("%1, %2, %3 years")
                                     .arg(event.date.isValid()
                                              ? event.date.toString(QStringLiteral("dd.MM.yyyy"))
                                              : QString())
                                     .arg(event.species)
                                     .arg(event.age);
		auto *item = new QListWidgetItem();
        item->setData(Qt::UserRole, event.id);
        item->setData(Qt::DisplayRole, title);
        item->setData(ListCardDelegate::SubtitleRole, subtitle);
		item->setData(ListCardDelegate::BadgeRole, event.date.toString(QStringLiteral("HH:mm")));
        ui->eventList->addItem(item);
    }

    const bool empty = m_events.isEmpty();
    ui->emptyLabel->setVisible(empty);
    ui->eventList->setVisible(!empty);
}