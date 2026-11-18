#ifndef USERSESSION_HPP
#define USERSESSION_HPP

#include "DbAccessController.hpp" // Наследуемся от базового класса
#include <cstdint>

class RoleAccessView;

class UserSession : public DbAccessController {
    Q_OBJECT
public:
    using EmployeeID = int64_t;
    
    // Передаем созданное соединение в базовый конструктор
    explicit UserSession(QObject *parent = nullptr);

signals:
    // Отдаём базовый тип роли: конкретную вьюху подписчик достаёт dynamic_cast'ом.
    void loginEnded(RoleAccessView *view);
    // errorOccurred удален, он уже есть в DbAccessController

public slots:
    void requestLogin(const QString &login);
    void logout();

private:
    RoleAccessView *createViewForEmployee(EmployeeID empId); 

    EmployeeID m_employeeId;
    RoleAccessView *m_view = nullptr;
    // m_db и ensureOpen() удалены, они унаследованы из базы
};

#endif // USERSESSION_HPP
