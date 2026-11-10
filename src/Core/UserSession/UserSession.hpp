#ifndef USERSESSION_HPP
#define USERSESSION_HPP

#include "BaseDbController.hpp" // Наследуемся от базового класса
#include <QVariant>
#include <cstdint>

class UserSession : public BaseDbController {
    Q_OBJECT
public:
    using EmployeeID = int64_t;
    
    // Передаем созданное соединение в базовый конструктор
    explicit UserSession(QObject *parent = nullptr);

signals:
    void loginEnded(const QVariant &view);
    // errorOccurred удален, он уже есть в BaseDbController

public slots:
    void requestLogin(const QString &login);

private:
    QVariant createViewForEmployee(EmployeeID empId); 

    EmployeeID m_employeeId;
    QVariant m_view;
    // m_db и ensureOpen() удалены, они унаследованы из базы
};

#endif // USERSESSION_HPP
