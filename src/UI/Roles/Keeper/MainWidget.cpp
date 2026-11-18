#include "MainWidget.hpp"
#include "ui_MainWidget.h"

#include "KeeperData.hpp"
#include "Pages/AnimalsPage.hpp"
#include "Pages/EnclosurePage.hpp"

#include <QMessageBox>

KeeperMainWidget::KeeperMainWidget(QWidget *parent)
    : TrWidget(parent)
    , ui(std::make_unique<Ui::KeeperMainWidget>())
{
    ui->setupUi(this);

    setupPages();

    connect(ui->backButton, &QPushButton::clicked, this, &KeeperMainWidget::goBack);

    updateNavigation();
}

KeeperMainWidget::~KeeperMainWidget() = default;

void KeeperMainWidget::setAccess(KeeperAccessView *view)
{
    if (m_access == view) {
        return;
    }
    if (m_access) {
        disconnect(m_access, nullptr, this, nullptr);
    }

    m_access = view;
    if (!m_access) {
        // Отцепились от Core: не оставляем данные прошлой сессии на экране.
        m_enclosurePage->setEnclosures({});
        m_animalsPage->setAnimals({});
        return;
    }

    connect(m_access, &KeeperAccessView::enclosuresRefreshed,
            this, &KeeperMainWidget::onEnclosuresRefreshed);
    connect(m_access, &KeeperAccessView::animalsLoaded,
            this, &KeeperMainWidget::onAnimalsLoaded);
    connect(m_access, &KeeperAccessView::errorOccurred,
            this, [this](const QString &message) {
                QMessageBox::critical(this, tr("Data Error"), message);
            });

    // Данные придут обратно синхронно сигналом enclosuresRefreshed().
    m_access->refreshData();
}

void KeeperMainWidget::setupPages()
{
    m_enclosurePage = new EnclosurePage(ui->pageStack);
    m_animalsPage = new AnimalsPage(ui->pageStack);

    ui->pageStack->addWidget(m_enclosurePage);
    ui->pageStack->addWidget(m_animalsPage);
    ui->pageStack->setCurrentWidget(m_enclosurePage);

    connect(m_enclosurePage, &EnclosurePage::enclosureSelected,
            this, &KeeperMainWidget::onEnclosureSelected);
    connect(m_animalsPage, &AnimalsPage::animalSelected,
            this, &KeeperMainWidget::onAnimalSelected);
}

void KeeperMainWidget::onEnclosuresRefreshed(const QList<KeeperAccessView::EnclosureInfo> &enclosures)
{
    // Адаптер Core -> UI: страница не знает про вьюху и работает
    // с собственным представлением данных.
    QList<KeeperData::EnclosureInfo> items;
    items.reserve(enclosures.size());
    for (const KeeperAccessView::EnclosureInfo &enclosure : enclosures) {
        items.append({enclosure.id, enclosure.name, enclosure.count});
    }

    m_enclosurePage->setEnclosures(items);
}

void KeeperMainWidget::onEnclosureSelected(int enclosureId)
{
    if (!m_access) {
        return;
    }

    // selectEnclosure() синхронно вернёт список животных сигналом
    // animalsLoaded(), поэтому страница открывается уже с данными.
    m_access->selectEnclosure(enclosureId);
    ui->pageStack->setCurrentWidget(m_animalsPage);
    updateNavigation();
}

void KeeperMainWidget::onAnimalSelected(int animalId)
{
    Q_UNUSED(animalId)

    // Страница подробностей животного появится позже.
}

void KeeperMainWidget::onAnimalsLoaded(const QList<KeeperAccessView::AnimalInfo> &animals)
{
    QList<KeeperData::AnimalInfo> items;
    items.reserve(animals.size());
    for (const KeeperAccessView::AnimalInfo &animal : animals) {
        items.append({animal.id, animal.species, animal.nickname,
                      animal.age, animal.hoursSinceFed});
    }

    m_animalsPage->setAnimals(items);
}

void KeeperMainWidget::goBack()
{
    const int index = ui->pageStack->currentIndex();
    if (index > 0) {
        ui->pageStack->setCurrentIndex(index - 1);
    }

    updateNavigation();
} 

void KeeperMainWidget::updateNavigation()
{
    // На первой странице возвращаться некуда: прячем всю верхнюю панель,
    // иначе над списком вольеров остаётся пустая полоса.
    const bool canGoBack = ui->pageStack->currentIndex() > 0;
    ui->backButton->setVisible(canGoBack);
    ui->topBar->setVisible(canGoBack);
}

void KeeperMainWidget::retranslateUi()
{
    ui->retranslateUi(this);
}
