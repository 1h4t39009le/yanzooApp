#ifndef VETACCESSVIEW_HPP
#define VETACCESSVIEW_HPP

#include "RoleAccessView.hpp"

#include <QDateTime>
#include <QList>
#include <QString>

class QSqlQuery;

// Доступ к данным роли «Ветеринарный врач».
//
// Данные о животных берутся из UserAnimView, журнал мероприятий — из
// UserMedEvtView, история болезни конкретного животного — из UserMedHistoryView.
// Права на запись (должность и отсутствие exit_date) проверяют триггеры БД,
// здесь дублировать их не нужно.
class VetAccessView : public RoleAccessView {
    Q_OBJECT
public:
    struct MedicalEventInfo {
        int eventId = 0;
        int employeeId = 0;
        int animalId = 0;
        QDateTime date;
        QString type;
        QString nickname;
        QString species;
        int age = 0;
    };

    struct MedicalHistoryEntry {
        int animalId = 0;
        QDateTime date;
        QString type;
        QString result;
    };

    explicit VetAccessView(QObject *parent, EmployeeID empId, QSqlDatabase db);

    QList<MedicalEventInfo> assignedMedicalEvents() const { return m_assignedMedicalEvents; }
    AnimalInfo currentAnimal() const { return m_currentAnimal; }
    QList<MedicalHistoryEntry> currentAnimalHistory() const { return m_currentAnimalHistory; }

signals:
    void animalLoaded(const VetAccessView::AnimalInfo &animal);
    void assignedMedicalEventsLoaded(const QList<VetAccessView::MedicalEventInfo> &events);
    void medicalHistoryLoaded(int animalId, const QList<VetAccessView::MedicalHistoryEntry> &history);
    void medicalEventResultUpdated(int eventId);

public slots:
    void selectAnimal(int animalId);
    void refreshAssignedMedicalEvents();
    void loadMedicalHistory(int animalId);
    void updateMedicalEventResult(int eventId, const QString &result);

private:
    AnimalInfo readAnimal(const QSqlQuery &query) const;
    QList<MedicalEventInfo> readMedicalEvents(QSqlQuery &query) const;

    AnimalInfo m_currentAnimal;
    QList<MedicalHistoryEntry> m_currentAnimalHistory;
    QList<MedicalEventInfo> m_assignedMedicalEvents;
};

#endif // VETACCESSVIEW_HPP
