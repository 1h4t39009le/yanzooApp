#ifndef KEEPERDATA_HPP
#define KEEPERDATA_HPP

#include <QList>
#include <QString>

// Временный источник данных роли «Кипер».
//
// Сейчас это обычный фейковый список, который MainWidget отдаёт страницам.
// Когда появится реальная работа с БД, KeeperData заменит
// Core/UserSession/KeeperAccessView, а интерфейс страниц останется прежним:
// им по-прежнему будет приходить готовый список через setEnclosures().
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

// Список вольеров, закреплённых за текущим сотрудником.
QList<EnclosureInfo> enclosures();

} // namespace KeeperData

#endif // KEEPERDATA_HPP
