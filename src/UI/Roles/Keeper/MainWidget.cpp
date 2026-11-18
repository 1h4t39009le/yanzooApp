#include "MainWidget.hpp"
#include "ui_MainWidget.h"

#include "KeeperData.hpp"
#include "Pages/AnimalsPage.hpp"
#include "Pages/EnclosurePage.hpp"
#include "Pages/FeedingPage.hpp"

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

    // Любая сессия начинается со списка вольеров: после logout() стек не
    // должен оставаться на странице животного или кормления прошлого пользователя.
    ui->pageStack->setCurrentWidget(m_enclosurePage);
    updateNavigation();

    if (!m_access) {
        // Отцепились от Core: не оставляем данные прошлой сессии на экране.
        m_enclosurePage->setEnclosures({});
        m_animalsPage->setAnimals({});
        m_feedingPage->setAnimalDetails({}, {});
        return;
    }

    connect(m_access, &KeeperAccessView::enclosuresRefreshed,
            this, &KeeperMainWidget::onEnclosuresRefreshed);
    connect(m_access, &KeeperAccessView::animalsLoaded,
            this, &KeeperMainWidget::onAnimalsLoaded);
    connect(m_access, &KeeperAccessView::animalDetailsLoaded,
            this, &KeeperMainWidget::onAnimalDetailsLoaded);
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
    m_feedingPage = new FeedingPage(ui->pageStack);

    ui->pageStack->addWidget(m_enclosurePage);
    ui->pageStack->addWidget(m_animalsPage);
    ui->pageStack->addWidget(m_feedingPage);
    ui->pageStack->setCurrentWidget(m_enclosurePage);

    connect(m_enclosurePage, &EnclosurePage::enclosureSelected,
            this, &KeeperMainWidget::onEnclosureSelected);
    connect(m_animalsPage, &AnimalsPage::animalSelected,
            this, &KeeperMainWidget::onAnimalSelected);
    connect(m_feedingPage, &FeedingPage::feedingRequested,
            this, &KeeperMainWidget::onFeedingRequested);
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
    if (!m_access) {
        return;
    }

    // selectAnimal() синхронно вернёт карточку и рацион сигналом
    // animalDetailsLoaded(), поэтому страница открывается с данными.
    m_access->selectAnimal(animalId);
    ui->pageStack->setCurrentWidget(m_feedingPage);
    updateNavigation();
}

void KeeperMainWidget::onFeedingRequested(int animalId, const QString &notes)
{
    if (!m_access) {
        return;
    }

    m_access->registerFeeding(animalId, notes);
    goBack();
}

void KeeperMainWidget::onAnimalsLoaded(const QList<KeeperAccessView::AnimalInfo> &animals)
{
    QList<KeeperData::AnimalInfo> items;
    items.reserve(animals.size());
    for (const KeeperAccessView::AnimalInfo &animal : animals) {
        items.append({animal.id, animal.species, animal.nickname, animal.gender,
                      animal.age, animal.hoursSinceFed});
    }

    m_animalsPage->setAnimals(items);
}

void KeeperMainWidget::onAnimalDetailsLoaded(
    const KeeperAccessView::AnimalFullDetails &animal,
    const QList<KeeperAccessView::RationIngredient> &ration)
{
    KeeperData::AnimalDetails details;
    details.base = {animal.base.id, animal.base.species, animal.base.nickname,
                    animal.base.gender, animal.base.age, animal.base.hoursSinceFed};
    details.feedingFrequency = animal.feedingFrequency;
    details.fedTodayCount = animal.fedTodayCount;
    details.rationName = animal.rationName;

    QList<KeeperData::RationIngredient> ingredients;
    ingredients.reserve(ration.size());
    for (const KeeperAccessView::RationIngredient &ingredient : ration) {
        ingredients.append({ingredient.name, ingredient.amountWithUnit});
    }

    m_feedingPage->setAnimalDetails(details, ingredients);
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
