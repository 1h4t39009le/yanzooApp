#ifndef ROLEACCESSVIEW_HPP
#define ROLEACCESSVIEW_HPP

#include "DbAccessController.hpp"

#include <QSqlDatabase>
#include <cstdint>

// Базовый класс ролевых вьюх (Keeper, позже Zootech/Vet/Manager).
//
// Добавляет к доступу к БД сотрудника, от имени которого идут запросы.
// Смена роли — это создание другого наследника, а не подмена employee_id.
class RoleAccessView : public DbAccessController {
    Q_OBJECT
public:
    using EmployeeID = int64_t;

    explicit RoleAccessView(QObject *parent, EmployeeID empId, QSqlDatabase db)
        : DbAccessController(db, parent)
        , m_employeeId(empId)
    {
    }

    EmployeeID employeeId() const { return m_employeeId; }

private:
    EmployeeID m_employeeId;
};

#endif // ROLEACCESSVIEW_HPP
