#include "EnclosurePage.hpp"
#include "ui_EnclosurePage.h"

#include "Widgets/ListCardWidget.hpp"

#include <QKeyEvent>
#include <QListWidgetItem>

EnclosurePage::EnclosurePage(QWidget *parent)
    : TrWidget(parent)
    , ui(std::make_unique<Ui::EnclosurePage>())
{
    ui->setupUi(this);

    ui->enclosureList->setMouseTracking(true);
    ui->enclosureList->viewport()->setMouseTracking(true);
    ui->enclosureList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->enclosureList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->enclosureList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->enclosureList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->enclosureList->setCursor(Qt::PointingHandCursor);

    connect(ui->enclosureList, &QListWidget::itemClicked,
            this, [this](QListWidgetItem *item) {
        if (item) {
            emit enclosureSelected(item->data(Qt::UserRole).toInt());
        }
    });

    ui->enclosureList->installEventFilter(this);

    rebuild();
}

EnclosurePage::~EnclosurePage() = default;

void EnclosurePage::setEnclosures(const QList<KeeperData::EnclosureInfo> &enclosures)
{
    m_enclosures = enclosures;
    rebuild();
}

void EnclosurePage::showEvent(QShowEvent *event)
{
    TrWidget::showEvent(event);

    // Как и в QML-версии, при возврате на страницу выделение сбрасывается.
    ui->enclosureList->clearSelection();
    ui->enclosureList->setCurrentRow(-1);
}

void EnclosurePage::retranslateUi()
{
    ui->retranslateUi(this);
    rebuild();
}

bool EnclosurePage::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->enclosureList && event->type() == QEvent::KeyPress) {
        auto *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            openCurrentItem();
            return true;
        }
    }

    return TrWidget::eventFilter(watched, event);
}

void EnclosurePage::rebuild()
{
    ui->enclosureList->clear();

    for (const KeeperData::EnclosureInfo &enclosure : m_enclosures) {
        auto *item = new QListWidgetItem(ui->enclosureList);
        item->setData(Qt::UserRole, enclosure.id);
        item->setSizeHint(QSize(0, 78));

        auto *card = new ListCardWidget(enclosure.name,
                                        tr("Animals: %1").arg(enclosure.animalCount),
                                        ui->enclosureList);
        ui->enclosureList->setItemWidget(item, card);
    }

    const bool empty = m_enclosures.isEmpty();
    ui->emptyLabel->setText(tr("No enclosures assigned"));
    ui->emptyLabel->setVisible(empty);
    ui->enclosureList->setVisible(!empty);
}

void EnclosurePage::openCurrentItem()
{
    QListWidgetItem *item = ui->enclosureList->currentItem();
    if (item) {
        emit enclosureSelected(item->data(Qt::UserRole).toInt());
    }
}
