#ifndef KEEPERDATA_HPP
#define KEEPERDATA_HPP

#include <QList>
#include <QString>

// Представление данных роли «Кипер» на стороне UI.
//
// Источник — Core/UserSession/KeeperAccessView. Страницы получают готовый
// список через setEnclosures() и не зависят от Core напрямую.
//
// Данные намеренно не содержат готовых подписей: количество животных — это
// число, а "Animals: %1" собирает страница в момент отрисовки. Иначе строка
// переводится один раз при запросе данных и не обновится при смене языка.
namespace KeeperData {

struct EnclosureInfo {
    int id = 0;
    QString name;
    int animalCount = 0;
};

struct AnimalInfo {
    int id = 0;
    QString species;
    QString nickname;
    QString gender;
    int age = 0;
    int hoursSinceFed = 0;
};

struct AnimalDetails {
    AnimalInfo base;
    QString feedingFrequency;
    int fedTodayCount = 0;
    QString rationName;
};

struct RationIngredient {
    QString name;
    QString amountWithUnit;
};

} // namespace KeeperData

#endif // KEEPERDATA_HPP
