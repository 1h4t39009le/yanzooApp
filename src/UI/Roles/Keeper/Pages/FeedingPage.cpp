#include "FeedingPage.hpp"
#include "ui_FeedingPage.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLayoutItem>

FeedingPage::FeedingPage(QWidget *parent)
    : TrWidget(parent)
    , ui(std::make_unique<Ui::FeedingPage>())
{
    ui->setupUi(this);

    ui->rationLayout->setAlignment(Qt::AlignTop);
    ui->feedButton->setEnabled(false);

    connect(ui->feedButton, &QPushButton::clicked,
            this, &FeedingPage::onFeedClicked);

    rebuild();
}

FeedingPage::~FeedingPage() = default;

void FeedingPage::setAnimalDetails(const KeeperData::AnimalDetails &animal,
                                   const QList<KeeperData::RationIngredient> &ration)
{
    m_animal = animal;
    m_ration = ration;
    m_animalId = animal.base.id;

    rebuild();
}

void FeedingPage::showEvent(QShowEvent *event)
{
    TrWidget::showEvent(event);

    ui->notesEdit->clear();
    ui->notesEdit->setFocus();
}

void FeedingPage::retranslateUi()
{
    ui->retranslateUi(this);
    rebuild();
}

void FeedingPage::onFeedClicked()
{
    if (m_animalId <= 0) {
        return;
    }

    const QString notes = ui->notesEdit->toPlainText();
    ui->notesEdit->clear();
    emit feedingRequested(m_animalId, notes);
}

void FeedingPage::rebuild()
{
    const bool hasAnimal = m_animalId > 0;
    ui->feedButton->setEnabled(hasAnimal);

    const QString nickname = m_animal.base.nickname.isEmpty()
        ? tr("Animal")
        : m_animal.base.nickname;
    ui->subtitleLabel->setText(tr("%1 — feeding registration").arg(nickname));

    if (hasAnimal) {
        const QString frequency = m_animal.feedingFrequency.isEmpty()
            ? QStringLiteral("—")
            : m_animal.feedingFrequency;
        ui->statusValue->setText(
            tr("%1 h • Schedule: %2").arg(m_animal.base.hoursSinceFed).arg(frequency));
    } else {
        ui->statusValue->setText(QStringLiteral("—"));
    }

    ui->speciesValue->setText(m_animal.base.species.isEmpty()
                                  ? QStringLiteral("—")
                                  : m_animal.base.species);
    ui->genderValue->setText(m_animal.base.gender.isEmpty()
                                 ? QStringLiteral("—")
                                 : m_animal.base.gender);
    ui->ageValue->setText(hasAnimal
                              ? tr("%1 years").arg(m_animal.base.age)
                              : QStringLiteral("—"));

    const QString rationName = m_animal.rationName.isEmpty()
        ? tr("Standard")
        : m_animal.rationName;
    ui->rationTitle->setText(tr("Ration: %1").arg(rationName));

    populateRation();
}

void FeedingPage::clearRationRows()
{
    while (QLayoutItem *item = ui->rationLayout->takeAt(0)) {
        if (QWidget *widget = item->widget()) {
            delete widget;
        }
        delete item;
    }
}

void FeedingPage::populateRation()
{
    clearRationRows();

    if (m_ration.isEmpty()) {
        auto *empty = new QLabel(tr("No ration data"), ui->rationContainer);
        empty->setProperty("role", QStringLiteral("secondary"));
        empty->setWordWrap(true);
        ui->rationLayout->addWidget(empty);
        return;
    }

    for (const KeeperData::RationIngredient &ingredient : m_ration) {
        auto *row = new QWidget(ui->rationContainer);
        auto *rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(0, 0, 0, 0);
        rowLayout->setSpacing(8);

        auto *name = new QLabel(QStringLiteral("• %1").arg(ingredient.name), row);
        name->setWordWrap(true);

        auto *amount = new QLabel(ingredient.amountWithUnit, row);
        amount->setProperty("role", QStringLiteral("secondary"));
        amount->setAlignment(Qt::AlignRight | Qt::AlignTop);

        rowLayout->addWidget(name, 1);
        rowLayout->addWidget(amount, 0);
        ui->rationLayout->addWidget(row);
    }
}
