#include "ListCardDelegate.hpp"

#include <QApplication>
#include <QPainter>
#include <QStyle>

ListCardDelegate::ListCardDelegate(QWidget *parent)
    : QStyledItemDelegate(parent)
    , m_stamp("", "", parent)
{
    // Прячем сам виджет-штамп, но оставляем его дочерним для QListWidget,
    // чтобы при переключении light.qss <-> dark.qss его стили обновлялись сами.
    m_stamp.hide();
    m_stamp.setBadge("");
}

void ListCardDelegate::paint(QPainter *painter,
                             const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    // 1. Рисуем фон карточки, градиент, рамку, :hover и :selected из QListWidget::item в QSS
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    opt.text.clear();

    const QWidget *widget = option.widget;
    QStyle *style = widget ? widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

    // 2. Заполняем наш единственный виджет-штамп данными текущей строки
    m_stamp.setTitle(index.data(Qt::DisplayRole).toString());
    m_stamp.setSubtitle(index.data(SubtitleRole).toString());
    m_stamp.setBadge(index.data(BadgeRole).toString());

    // 3. Подгоняем размер карточки (учитываем margin-bottom: 4px из QListWidget::item)
    const QRect contentRect = option.rect.adjusted(0, 0, 0, -4);
    m_stamp.resize(contentRect.size());

    // 4. Отпечатываем дочерние элементы карточки (текст, бейдж, стрелку) поверх плашки
    painter->save();
    painter->translate(contentRect.topLeft());
    m_stamp.render(painter, QPoint(), QRegion(), QWidget::DrawChildren);
    painter->restore();
}

QSize ListCardDelegate::sizeHint(const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QSize(option.rect.width(), 78);
}
