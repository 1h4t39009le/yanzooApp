#include "AnimalsPage.hpp"
#include "ui_AnimalsPage.h"

#include "Widgets/ListCardWidget.hpp"

#include <QKeyEvent>
#include <QListWidgetItem>

AnimalsPage::AnimalsPage(QWidget *parent)
    : TrWidget(parent)
    , ui(std::make_unique<Ui::AnimalsPage>())
{
    ui->setupUi(this);

    ui->animalList->setMouseTracking(true);
    ui->animalList->viewport()->setMouseTracking(true);
    ui->animalList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->animalList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->animalList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->animalList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->animalList->setCursor(Qt::PointingHandCursor);

    connect(ui->animalList, &QListWidget::itemClicked,
            this, [this](QListWidgetItem *item) {
        if (item) {
            emit animalSelected(item->data(Qt::UserRole).toInt());
        }
    });

    ui->animalList->installEventFilter(this);

    rebuild();
}

AnimalsPage::~AnimalsPage() = default;

void AnimalsPage::setAnimals(const QList<KeeperData::AnimalInfo> &animals)
{
    m_animals = animals;
    rebuild();
}

void AnimalsPage::showEvent(QShowEvent *event)
{
    TrWidget::showEvent(event);

    // При возврате на страницу выделение сбрасывается, как у списка вольеров.
    ui->animalList->clearSelection();
    ui->animalList->setCurrentRow(-1);
}

void AnimalsPage::retranslateUi()
{
    ui->retranslateUi(this);
    rebuild();
}

bool AnimalsPage::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->animalList && event->type() == QEvent::KeyPress) {
        auto *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            openCurrentItem();
            return true;
        }
    }

    return TrWidget::eventFilter(watched, event);
}

void AnimalsPage::rebuild()
{
    ui->animalList->clear();

    for (const KeeperData::AnimalInfo &animal : m_animals) {
        // Подпись собираем здесь, при отрисовке: иначе строка переведётся
        // один раз и не обновится при смене языка.
        const QString subtitle = tr("%1, age %2, fed %3 h ago")
                                     .arg(animal.species)
                                     .arg(animal.age)
                                     .arg(animal.hoursSinceFed);

        auto *item = new QListWidgetItem(ui->animalList);
        item->setData(Qt::UserRole, animal.id);
        item->setSizeHint(QSize(0, 78));

        const QString title = animal.nickname.isEmpty() ? animal.species : animal.nickname;
        auto *card = new ListCardWidget(title, subtitle, ui->animalList);
        ui->animalList->setItemWidget(item, card);
    }

    const bool empty = m_animals.isEmpty();
    ui->emptyLabel->setVisible(empty);
    ui->animalList->setVisible(!empty);
}

void AnimalsPage::openCurrentItem()
{
    QListWidgetItem *item = ui->animalList->currentItem();
    if (item) {
        emit animalSelected(item->data(Qt::UserRole).toInt());
    }
}
