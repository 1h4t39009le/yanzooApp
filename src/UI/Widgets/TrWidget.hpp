#pragma once

#include <QEvent>
#include <QWidget>

// Базовый класс для виджетов с переводимым интерфейсом.
//
// Подписка на сигнал LanguageManager здесь не нужна: Qt сам рассылает
// QEvent::LanguageChange всем виджетам при установке и снятии QTranslator,
// а QWidget::event() дополнительно пробрасывает событие дочерним объектам.
// Поэтому любой наследник просто реализует retranslateUi().
class TrWidget : public QWidget {
    Q_OBJECT
public:
    explicit TrWidget(QWidget *parent = nullptr) : QWidget(parent) {}

protected:
    void changeEvent(QEvent *event) override {
        if (event->type() == QEvent::LanguageChange) {
            retranslateUi();
        }

        QWidget::changeEvent(event);
    }

    // Вызывается при смене языка: обновить тексты из .ui и связанные данные.
    virtual void retranslateUi() = 0;
};
