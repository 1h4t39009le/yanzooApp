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

void VetAccessView::refreshAnimals()
{
    if (!ensureOpen()) return;

    QList<AnimalInfo> newList;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM UserAnimView ORDER BY species, nickname");

    if (query.exec()) {
        while (query.next()) {
            newList.append(readAnimal(query));
        }
        m_animals = newList;
        emit animalsLoaded(m_animals);
    } else {
        emit errorOccurred(tr("Failed to load animals list: %1")
                               .arg(query.lastError().text()));
    }
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

        // Для карточки ветеринара сразу подтягиваем журнал и историю животного.
        loadMedicalEvents(animalId);
        loadMedicalHistory(animalId);
    } else {
        emit errorOccurred(tr("Failed to load animal details: %1")
                               .arg(query.lastError().text()));
    }
}

void VetAccessView::refreshMedicalEvents()
{
    if (!ensureOpen()) return;

    QList<MedicalEventInfo> newList;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM UserMedEvtView ORDER BY date DESC, event_id DESC");

    if (query.exec()) {
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
            newList.append(event);
        }
        m_medicalEvents = newList;
        emit medicalEventsLoaded(m_medicalEvents);
    } else {
        emit errorOccurred(tr("Failed to load medical events: %1")
                               .arg(query.lastError().text()));
    }
}

void VetAccessView::loadMedicalEvents(int animalId)
{
    if (!ensureOpen()) return;

    QList<MedicalEventInfo> newList;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM UserMedEvtView "
                  "WHERE animal_id = :animId "
                  "ORDER BY date DESC, event_id DESC");
    query.bindValue(":animId", animalId);

    if (query.exec()) {
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
            newList.append(event);
        }
        m_medicalEvents = newList;
        emit medicalEventsLoaded(m_medicalEvents);
    } else {
        emit errorOccurred(tr("Failed to load animal medical events: %1")
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

void VetAccessView::registerMedicalEvent(int animalId, const QString &type, const QString &result)
{
    if (!ensureOpen()) return;

    QSqlQuery query(m_db);
    query.prepare("SET NOCOUNT ON; "
                  "INSERT INTO MedicalEvent (animal_id, employee_id, type, date, result) "
                  "VALUES (:animalId, :empId, :type, GETDATE(), :result); "
                  "SELECT CAST(SCOPE_IDENTITY() AS INT) AS event_id;");

    query.bindValue(":animalId", animalId);
    query.bindValue(":empId", static_cast<qlonglong>(employeeId()));
    query.bindValue(":type", type);
    query.bindValue(":result", result.isEmpty()
                                  ? QVariant(QMetaType(QMetaType::QString))
                                  : QVariant(result));

    if (query.exec()) {
        const int eventId = query.next() ? query.value("event_id").toInt() : -1;
        emit medicalEventRegistered(eventId);

        // Синхронно обновляем карточку животного, если открыто именно оно.
        if (m_currentAnimal.id == animalId) {
            loadMedicalEvents(animalId);
            loadMedicalHistory(animalId);
        }
    } else {
        emit errorOccurred(tr("Failed to register medical event: %1")
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
    loadMedicalEvents(animalId);
    loadMedicalHistory(animalId);
}
