#pragma once
#include <QWidget>
#include "Language/LanguageManager.hpp"

class TrWidget : public QWidget {
    Q_OBJECT
public:
    explicit TrWidget(QWidget *parent = nullptr) : QWidget(parent) {
        connect(&LanguageManager::instance(), &LanguageManager::languageChanged,
                this, [this](LanguageManager::Language) {
                    retranslateUi();
                });
    }

protected:
    virtual void retranslateUi() = 0;
};