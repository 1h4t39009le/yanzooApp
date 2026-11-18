#include "DbAccessController.hpp"
#include <QSqlError>

DbAccessController::DbAccessController(QSqlDatabase db, QObject *parent)
    : QObject(parent)
    , m_db(db)
{
}

bool DbAccessController::ensureOpen() {
    if (m_db.isOpen()) {
        return true;
    }
    if (m_db.open()) {
        return true;
    }
    
    emit errorOccurred(tr("Database connection lost and could not be restored."));
    return false;
}
