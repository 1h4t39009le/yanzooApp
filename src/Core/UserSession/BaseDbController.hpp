#ifndef BASEDBCONTROLLER_HPP
#define BASEDBCONTROLLER_HPP

#include <QObject>
#include <QSqlDatabase>

class BaseDbController : public QObject {
    Q_OBJECT
public:
    explicit BaseDbController(QSqlDatabase db, QObject *parent = nullptr);
    virtual ~BaseDbController() = default;

signals:
    void errorOccurred(const QString &message);

protected:
    bool ensureOpen();

    QSqlDatabase m_db;
};

#endif // BASEDBCONTROLLER_HPP
