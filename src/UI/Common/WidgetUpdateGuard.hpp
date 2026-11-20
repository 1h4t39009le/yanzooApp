#pragma once

#include <QPointer>
#include <QWidget>

class WidgetUpdateGuard
{
public:
    explicit WidgetUpdateGuard(QWidget *widget)
        : m_widget(widget)
        , m_wasEnabled(widget ? widget->updatesEnabled() : false)
    {
        if (m_widget) {
            m_widget->setUpdatesEnabled(false);
        }
    }

    ~WidgetUpdateGuard()
    {
        if (m_widget) {
            m_widget->setUpdatesEnabled(m_wasEnabled);
        }
    }

    WidgetUpdateGuard(const WidgetUpdateGuard &) = delete;
    WidgetUpdateGuard &operator=(const WidgetUpdateGuard &) = delete;

private:
    QPointer<QWidget> m_widget;
    bool m_wasEnabled = false;
};
