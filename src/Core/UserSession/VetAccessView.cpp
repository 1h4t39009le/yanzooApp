#include "VetAccessView.hpp"

#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

VetAccessView::VetAccessView(QObject *parent, EmployeeID empId, QSqlDatabase db)
    : RoleAccessView(parent, empId, db)
{
}

VetAccessView::AnimalInfo VetAccessView::readAnimal(const QSqlQuery &query) const
{
    AnimalInfo animal;
    animal.id = query.value("animal_id").toInt();
    animal.enclosureId = query.value("enclosure_id").toInt();
    animal.species = query.value("species").toString();
    animal.nickname = query.value("nickname").toString();
    animal.gender = query.value("gender").toString();
    animal.age = query.value("age").toInt();
    animal.hoursSinceFed = query.value("hours_since_fed").toInt();
    animal.rationId = query.value("ration_id").toInt();
    return animal;
}

QList<VetAccessView::MedicalEventInfo> VetAccessView::readMedicalEvents(QSqlQuery &query) const
{
    QList<MedicalEventInfo> events;
    while (query.next()) {
        MedicalEventInfo event;
        event.eventId = query.value("event_id").toInt();
        event.employeeId = query.value("employee_id").toInt();
        event.animalId = query.value("animal_id").toInt();
        event.date = query.value("date").toDateTime();
        event.type = query.value("type").toString();
        event.nickname = query.value("nickname").toString();
        event.species = query.value("species").toString();
        event.age = query.value("age").toInt();
        events.append(event);
    }
    return events;
}

void VetAccessView::selectAnimal(int animalId)
{
    if (!ensureOpen()) return;

    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM UserAnimView WHERE animal_id = :animId");
    query.bindValue(":animId", animalId);

    if (query.exec()) {
        if (!query.next()) {
            emit errorOccurred(tr("Animal %1 was not found.").arg(animalId));
            return;
        }

        m_currentAnimal = readAnimal(query);
        emit animalLoaded(m_currentAnimal);

        // Карточка ветеринара сразу подтягивает историю болезни животного.
        loadMedicalHistory(animalId);
    } else {
        emit errorOccurred(tr("Failed to load animal details: %1")
                               .arg(query.lastError().text()));
    }
}

void VetAccessView::refreshAssignedMedicalEvents()
{
    if (!ensureOpen()) return;

    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM UserMedEvtView "
                  "WHERE employee_id = :empId "
                  "ORDER BY date DESC, event_id DESC");
    query.bindValue(":empId", static_cast<qlonglong>(employeeId()));

    if (query.exec()) {
        m_assignedMedicalEvents = readMedicalEvents(query);
        emit assignedMedicalEventsLoaded(m_assignedMedicalEvents);
    } else {
        emit errorOccurred(tr("Failed to load assigned medical events: %1")
                               .arg(query.lastError().text()));
    }
}

void VetAccessView::loadMedicalHistory(int animalId)
{
    if (!ensureOpen()) return;

    QList<MedicalHistoryEntry> newList;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM UserMedHistoryView "
                  "WHERE animal_id = :animId "
                  "ORDER BY date DESC");
    query.bindValue(":animId", animalId);

    if (query.exec()) {
        while (query.next()) {
            MedicalHistoryEntry entry;
            entry.animalId = query.value("animal_id").toInt();
            entry.date = query.value("date").toDateTime();
            entry.type = query.value("type").toString();
            entry.result = query.value("result").toString();
            newList.append(entry);
        }
        m_currentAnimalHistory = newList;
        emit medicalHistoryLoaded(animalId, m_currentAnimalHistory);
    } else {
        emit errorOccurred(tr("Failed to load animal medical history: %1")
                               .arg(query.lastError().text()));
    }
}

void VetAccessView::updateMedicalEventResult(int eventId, const QString &result)
{
    if (!ensureOpen()) return;

    if (result.trimmed().isEmpty()) {
        emit errorOccurred(tr("Medical event result cannot be empty."));
        return;
    }

    // animal_id неизменяем (запрещено триггером), поэтому определяем его до
    // обновления: заодно проверяем, что мероприятие существует.
    int animalId = -1;
    QSqlQuery lookup(m_db);
    lookup.prepare("SELECT animal_id FROM UserMedEvtView WHERE event_id = :eventId");
    lookup.bindValue(":eventId", eventId);
    if (!lookup.exec() || !lookup.next()) {
        emit errorOccurred(tr("Medical event %1 was not found: %2")
                               .arg(eventId)
                               .arg(lookup.lastError().text()));
        return;
    }
    animalId = lookup.value("animal_id").toInt();

    QSqlQuery query(m_db);
    query.prepare("UPDATE MedicalEvent SET result = :result WHERE event_id = :eventId");
    query.bindValue(":result", result);
    query.bindValue(":eventId", eventId);

    if (!query.exec()) {
        emit errorOccurred(tr("Failed to update medical event result: %1")
                               .arg(query.lastError().text()));
        return;
    }

    emit medicalEventResultUpdated(eventId);

    // Результат входит в историю болезни, а журнал назначенных мероприятий
    // мог измениться — обновляем оба представления.
    refreshAssignedMedicalEvents();
    loadMedicalHistory(animalId);
}
