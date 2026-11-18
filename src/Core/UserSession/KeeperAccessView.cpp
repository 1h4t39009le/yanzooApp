#include "KeeperAccessView.hpp"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDateTime>
#include <QDebug>

KeeperAccessView::KeeperAccessView(QObject *parent, EmployeeID empid, QSqlDatabase db)
    : RoleAccessView(parent, empid, db)
    , m_selectedEnclosureId(-1)
{
}

void KeeperAccessView::refreshData() {
    if (!ensureOpen()) return;

    QList<EnclosureInfo> newList;
    QSqlQuery query(m_db);

    // Точная SQL-логика связывания вольеров с конкретным сотрудником
    QString sqlQuery =
        "SELECT v.* "
        "FROM UserEnclosureView v "
        "JOIN KeeperToEnclView k ON v.enclosure_id = k.enclosure_id "
        "WHERE k.employee_id = :empId";

    query.prepare(sqlQuery);
    query.bindValue(":empId", static_cast<qlonglong>(employeeId()));

    if (query.exec()) {
        while (query.next()) {
            EnclosureInfo enc;
            enc.id = query.value("enclosure_id").toInt();
            enc.name = query.value("name").toString();
            // Логика форматирования строки с количеством животных
            enc.countText = tr("Animals: %1").arg(query.value("animal_count").toInt());
            enc.maxHoursSinceFed = query.value("max_hours_since_fed").toInt();
            newList.append(enc);
        }
        
        m_enclosures = newList;
        emit enclosuresRefreshed(m_enclosures);
    } else {
        emit errorOccurred(tr("Failed to load enclosures list: %1")
                               .arg(query.lastError().text()));
    }
}

void KeeperAccessView::selectEnclosure(int enclosureId) {
    m_selectedEnclosureId = enclosureId;
    if (!ensureOpen()) return;

    QList<AnimalInfo> newAnimalsList;
    QSqlQuery query(m_db);
    QString sqlQuery = "SELECT * FROM UserAnimView WHERE enclosure_id = :encId";

    query.prepare(sqlQuery);
    query.bindValue(":encId", enclosureId);

    if (query.exec()) {
        while (query.next()) {
            AnimalInfo anim;
            anim.id = query.value("animal_id").toInt();
            anim.species = query.value("species").toString();
            anim.nickname = query.value("nickname").toString();
            anim.gender = query.value("gender").toString();
            anim.age = query.value("age").toInt();
            anim.hoursSinceFed = query.value("hours_since_fed").toInt();
            anim.rationId = query.value("ration_id").toInt();
            newAnimalsList.append(anim);
        }
        m_animals = newAnimalsList;
        emit animalsLoaded(m_animals);
    } else {
        emit errorOccurred(tr("Failed to load enclosure animals: %1")
                               .arg(query.lastError().text()));
    }
}

void KeeperAccessView::selectAnimal(int animalId) {
    if (!ensureOpen()) return;

    QSqlQuery query(m_db);
    // MS SQL подзапрос для подсчета фактов кормления за текущие сутки
    QString sqlQuery =
        "SELECT v.*, r.name AS ration_name, r.frequency AS feeding_frequency, "
        "(SELECT COUNT(*) FROM Feeding f WHERE f.animal_id = v.animal_id AND CAST(f.date AS DATE) = CAST(GETDATE() AS DATE)) AS fed_today_count "
        "FROM UserAnimView v "
        "JOIN Ration r ON v.ration_id = r.ration_id "
        "WHERE v.animal_id = :animId";

    query.prepare(sqlQuery);
    query.bindValue(":animId", animalId);

    if (query.exec() && query.next()) {
        m_currentAnimal.base.id = query.value("animal_id").toInt();
        m_currentAnimal.base.species = query.value("species").toString();
        m_currentAnimal.base.nickname = query.value("nickname").toString();
        m_currentAnimal.base.gender = query.value("gender").toString();
        m_currentAnimal.base.age = query.value("age").toInt();
        m_currentAnimal.base.hoursSinceFed = query.value("hours_since_fed").toInt();
        m_currentAnimal.base.rationId = query.value("ration_id").toInt();
        
        m_currentAnimal.feedingFrequency = query.value("feeding_frequency").toString();
        m_currentAnimal.fedTodayCount = query.value("fed_today_count").toInt();
        m_currentAnimal.rationName = query.value("ration_name").toString();

        // Подгружаем состав рациона через приватный хелпер-метод
        m_currentRationDetails = getRationDetails(m_currentAnimal.base.rationId);
        
        emit animalDetailsLoaded(m_currentAnimal, m_currentRationDetails);
    } else {
        emit errorOccurred(tr("Failed to load animal details: %1")
                               .arg(query.lastError().text()));
    }
}

QList<KeeperAccessView::RationIngredient> KeeperAccessView::getRationDetails(int rationId) {
    QList<RationIngredient> list;
    QSqlQuery query(m_db);
    QString sqlQuery = "SELECT feed_name, amount, unit FROM RationDetail WHERE ration_id = :ratId";

    query.prepare(sqlQuery);
    query.bindValue(":ratId", rationId);

    if (query.exec()) {
        while (query.next()) {
            RationIngredient ingredient;
            ingredient.name = query.value("feed_name").toString();
            // Склеиваем число и единицу измерения (например, "5" + " " + "кг")
            ingredient.amountWithUnit = query.value("amount").toString() + " " + query.value("unit").toString();
            list.append(ingredient);
        }
    } else {
        emit errorOccurred(tr("Failed to load ration composition: %1")
                               .arg(query.lastError().text()));
    }
    return list;
}

void KeeperAccessView::registerFeeding(int animalId, const QString &notes) {
    if (!ensureOpen()) return;

    QSqlQuery query(m_db);
    query.prepare("INSERT INTO Feeding (animal_id, employee_id, date, note) "
                  "VALUES (:animalId, :empId, GETDATE(), :notes)");

    query.bindValue(":animalId", animalId);
    query.bindValue(":empId", static_cast<qlonglong>(employeeId())); // Связываем с текущим залогиненным сотрудником
    query.bindValue(":notes", notes.isEmpty()
                                 ? QVariant(QMetaType(QMetaType::QString))
                                 : QVariant(notes));

    if (query.exec()) {
        emit feedingRegistered();
        
        // Автоматически обновляем UI, чтобы сбросились таймеры и обновились счетчики кормлений
        if (m_selectedEnclosureId != -1) {
            selectEnclosure(m_selectedEnclosureId);
        }
        selectAnimal(animalId);
    } else {
        emit errorOccurred(tr("Failed to register feeding: %1")
                               .arg(query.lastError().text()));
    }
}
