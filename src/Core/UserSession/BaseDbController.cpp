#include "BaseDbController.hpp"
#include <QSqlError>

BaseDbController::BaseDbController(QSqlDatabase db, QObject *parent)
    : QObject(parent)
    , m_db(db)
{
}

bool BaseDbController::ensureOpen() {
    if (m_db.isOpen()) {
        return true;
    }
    if (m_db.open()) {
        return true;
    }
    
    emit errorOccurred(tr("Database connection lost and could not be restored."));
    return false;
}
