#include "UserSession.hpp"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "UserKeeperView.hpp" 

UserSession::UserSession(QObject *parent) : 
    BaseDbController(QSqlDatabase::addDatabase("QODBC", "YanZooDbConnection"), parent),
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
            
            emit loginEnded(m_view);
        } else {
            emit errorOccurred(tr("User not found"));
        }
    } else {
        emit errorOccurred(tr("Authorization error: %1")
                               .arg(query.lastError().text()));
    }
}

QVariant UserSession::createViewForEmployee(EmployeeID empId) {
    UserKeeperView* keeperView = new UserKeeperView(this, empId, m_db);
    return QVariant::fromValue(keeperView);
}
