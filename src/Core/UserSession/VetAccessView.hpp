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
    struct AnimalInfo {
        int id = 0;
        int enclosureId = 0;
        QString species;
        QString nickname;
        QString gender;
        int age = 0;
        int hoursSinceFed = 0;
        int rationId = 0;
    };

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

    QList<AnimalInfo> animals() const { return m_animals; }
    AnimalInfo currentAnimal() const { return m_currentAnimal; }
    QList<MedicalEventInfo> medicalEvents() const { return m_medicalEvents; }
    QList<MedicalHistoryEntry> currentAnimalHistory() const { return m_currentAnimalHistory; }

signals:
    void animalsLoaded(const QList<VetAccessView::AnimalInfo> &animals);
    void animalLoaded(const VetAccessView::AnimalInfo &animal);
    void medicalEventsLoaded(const QList<VetAccessView::MedicalEventInfo> &events);
    void medicalHistoryLoaded(int animalId, const QList<VetAccessView::MedicalHistoryEntry> &history);
    void medicalEventRegistered(int eventId);
    void medicalEventResultUpdated(int eventId);

public slots:
    // Полный список животных из UserAnimView.
    void refreshAnimals();
    // Карточка животного из UserAnimView + его мероприятия и история.
    void selectAnimal(int animalId);
    // Весь журнал мероприятий из UserMedEvtView.
    void refreshMedicalEvents();
    // Мероприятия, назначенные текущему ветеринарному врачу.
    void refreshAssignedMedicalEvents();
    // Мероприятия конкретного животного из UserMedEvtView.
    void loadMedicalEvents(int animalId);
    // История болезни конкретного животного из UserMedHistoryView.
    void loadMedicalHistory(int animalId);
    // Новое мероприятие; пустой result записывается как NULL.
    void registerMedicalEvent(int animalId, const QString &type, const QString &result = QString());
    // Заполнение результата уже созданного мероприятия (правила — в триггере).
    void updateMedicalEventResult(int eventId, const QString &result);

private:
    AnimalInfo readAnimal(const QSqlQuery &query) const;
    QList<MedicalEventInfo> readMedicalEvents(QSqlQuery &query) const;

    QList<AnimalInfo> m_animals;
    AnimalInfo m_currentAnimal;
    QList<MedicalEventInfo> m_medicalEvents;
    QList<MedicalHistoryEntry> m_currentAnimalHistory;
};

#endif // VETACCESSVIEW_HPP
