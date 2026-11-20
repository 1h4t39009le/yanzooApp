#pragma once

#include <QStyledItemDelegate>
#include "ListCardWidget.hpp"

class ListCardDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    enum Roles {
        SubtitleRole = Qt::UserRole + 1,
        BadgeRole    = Qt::UserRole + 2
    };

    explicit ListCardDelegate(QWidget *parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

private:
    mutable ListCardWidget m_stamp;
};
