#include "UserSession.hpp"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "KeeperAccessView.hpp"
#include "VetAccessView.hpp"

UserSession::UserSession(QObject *parent) : 
    DbAccessController(QSqlDatabase::addDatabase("QODBC", "YanZooDbConnection"), parent),
    m_employeeId{-1}
{
    QString connectionString =
        "DRIVER={ODBC Driver 17 for SQL Server};"
        "SERVER=.\\SQLEXPRESS;"
        "DATABASE=YanZoo;"
        "Trusted_Connection=yes;"
        "TrustServerCertificate=yes;";
    m_db.setDatabaseName(connectionString);
    if (!m_db.open()) {
        qWarning() << "[UserSession] SQL Server connection error:" << m_db.lastError().text();
    }
}

void UserSession::requestLogin(const QString &login) {
    if (!ensureOpen()) return;

    QSqlQuery query(m_db);
    QString sqlQuery = "SELECT employee_id FROM LoginTable WHERE login = :login";
    
    query.prepare(sqlQuery);
    query.bindValue(":login", login);
    
    if (query.exec()) {
        if (query.next()) {
            m_employeeId = query.value("employee_id").toLongLong();
            m_view = createViewForEmployee(m_employeeId);

            if (!m_view) {
                // createViewForEmployee() уже объяснил причину через errorOccurred.
                m_employeeId = -1;
                return;
            }

            emit loginEnded(m_view);
        } else {
            emit errorOccurred(tr("User not found"));
        }
    } else {
        emit errorOccurred(tr("Authorization error: %1")
                               .arg(query.lastError().text()));
    }
}

RoleAccessView *UserSession::createViewForEmployee(EmployeeID empId) {
    QSqlQuery query(m_db);
    query.prepare("SELECT position FROM Employee "
                  "WHERE employee_id = :empId AND condition = N'Действующий'");
    query.bindValue(":empId", static_cast<qlonglong>(empId));

    if (!query.exec()) {
        emit errorOccurred(tr("Failed to determine employee position: %1")
                               .arg(query.lastError().text()));
        return nullptr;
    }

    if (!query.next()) {
        emit errorOccurred(tr("Active employee %1 was not found.").arg(empId));
        return nullptr;
    }

    const QString position = query.value("position").toString();

    if (position == QStringLiteral("Кипер")) {
        return new KeeperAccessView(this, empId, m_db);
    }
    if (position == QStringLiteral("Ветеринарный врач")) {
        return new VetAccessView(this, empId, m_db);
    }

    // Роли «Зоотехник» и «Заведующий зоологическим отделом» пока без вьюх.
    emit errorOccurred(tr("Access for position \"%1\" is not implemented yet.")
                           .arg(position));
    return nullptr;
}

void UserSession::logout() {
    if (m_view) {
        // Родитель — эта сессия, поэтому удаляем отложенно: UI мог ещё
        // держать указатель до переключения на страницу входа.
        m_view->deleteLater();
        m_view = nullptr;
    }
    m_employeeId = -1;
}
