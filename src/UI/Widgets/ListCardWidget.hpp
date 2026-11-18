#ifndef LISTCARDWIDGET_HPP
#define LISTCARDWIDGET_HPP

#include <QString>
#include <QWidget>
#include <memory>

namespace Ui {
class ListCardWidget;
}

// Содержимое одной карточки списка. Сама карточка (фон, рамка, hover и
// выделение) рисуется QListWidget через QSS, а этот виджет только показывает
// заголовок, подзаголовок и иконку на правом лейбле.
//
// Виджет презентационный: своих переводимых строк у него нет, готовые тексты
// приходят снаружи (страница собирает их через tr() в момент отрисовки).
//
// Иконка задаётся динамическим свойством iconName: конкретный url подставляет
// QSS темы (light.qss/dark.qss), поэтому при смене темы картинка меняется
// автоматически, без участия C++.
class ListCardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ListCardWidget(const QString &title,
                            const QString &subtitle,
                            QWidget *parent = nullptr);
    ~ListCardWidget() override;

    void setSubtitle(const QString &subtitle);
	//TODO: Remove completely if we don't use it
    //void setIcon(const QString &iconName);

private:
    std::unique_ptr<Ui::ListCardWidget> ui;
};

#endif // LISTCARDWIDGET_HPP
