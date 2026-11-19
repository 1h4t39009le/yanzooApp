#include "ListCardWidget.hpp"
#include "ui_ListCardWidget.h"

#include <QStyle>
#include <QVariant>

ListCardWidget::ListCardWidget(const QString &title,
                               const QString &subtitle,
                               QWidget *parent)
    : QWidget(parent)
    , ui(std::make_unique<Ui::ListCardWidget>())
{
    ui->setupUi(this);

    ui->cardTitleLabel->setText(title);
    setSubtitle(subtitle);

    // Клик должен доходить до QListWidget, а не теряться в карточке.
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    ui->cardTitleLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    ui->cardBadgeFrame->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    ui->cardBadgeLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    ui->cardSubtitleLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    ui->cardArrowLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

ListCardWidget::~ListCardWidget() = default;

void ListCardWidget::setSubtitle(const QString &subtitle)
{
    ui->cardSubtitleLabel->setText(subtitle);
    ui->cardSubtitleLabel->setVisible(!subtitle.isEmpty());
}

void ListCardWidget::setBadge(const QString &badge)
{
    ui->cardBadgeLabel->setText(badge);
    ui->cardBadgeFrame->setVisible(!badge.isEmpty());
}
/*
TODO: Remove if we don't use it
void ListCardWidget::setIcon(const QString &iconName)
{
    QLabel *iconLabel = ui->cardArrowLabel;

    if (iconName.isEmpty()) {
        iconLabel->setProperty("iconName", QVariant());
        iconLabel->clear();
        iconLabel->setVisible(false);
        return;
    }

    iconLabel->setProperty("iconName", iconName);
    iconLabel->style()->unpolish(iconLabel);
    iconLabel->style()->polish(iconLabel);
    iconLabel->setVisible(true);
    iconLabel->update();
}
*/