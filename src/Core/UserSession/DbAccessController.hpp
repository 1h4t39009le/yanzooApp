#ifndef DBACCESSCONTROLLER_HPP
#define DBACCESSCONTROLLER_HPP

#include <QObject>
#include <QSqlDatabase>

class DbAccessController : public QObject {
    Q_OBJECT
public:
    explicit DbAccessController(QSqlDatabase db, QObject *parent = nullptr);
    virtual ~DbAccessController() = default;

signals:
    void errorOccurred(const QString &message);

protected:
    bool ensureOpen();

    QSqlDatabase m_db;
};

#endif // DBACCESSCONTROLLER_HPP
