#ifndef VETDATA_HPP
#define VETDATA_HPP

#include <QDateTime>
#include <QString>

// Представление данных роли «Ветеринарный врач» на стороне UI.
//
// Источник — Core/UserSession/VetAccessView. Страница получает готовый список
// через setEvents() и не зависит от Core напрямую.
namespace VetData {

struct MedicalEventInfo {
    int id = 0;
    QString type;
    QString nickname;
    QDateTime date;
    QString species;
    int age = 0;
};

} // namespace VetData

#endif // VETDATA_HPP
