#include "MainWidget.hpp"
#include "ui_MainWidget.h"

#include "KeeperData.hpp"
#include "Pages/EnclosurePage.hpp"


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

void KeeperMainWidget::setupPages()
{
    m_enclosurePage = new EnclosurePage(ui->pageStack);

    ui->pageStack->addWidget(m_enclosurePage);
    ui->pageStack->setCurrentWidget(m_enclosurePage);

    connect(m_enclosurePage, &EnclosurePage::enclosureSelected,
            this, &KeeperMainWidget::onEnclosureSelected);

    loadData();
}

void KeeperMainWidget::loadData()
{
    // Заглушка вместо Core/UserSession: страница получает готовый список,
    // поэтому позже здесь достаточно будет вызвать KeeperAccessView.
    m_enclosurePage->setEnclosures(KeeperData::enclosures());
}

void KeeperMainWidget::onEnclosureSelected(int enclosureId)
{
    Q_UNUSED(enclosureId)

    // Следующая страница («Животные вольера») появится позже.
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
