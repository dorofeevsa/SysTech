#ifndef DBCONTROLLER_H
#define DBCONTROLLER_H

#include <QtSql>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QMap>
#include "employee.h"
#include "employeeconfig.h"
#include "corecontroller.h"
#include "helperutils.h"
#include <QPair>
#include <QSqlTableModel>
#include <QSharedPointer>

class CoreController;
class Employee;
class UserRoles;
class usersRecord;

struct usersRecord
{
    usersRecord(QString _pass, QString _role, QSharedPointer<UserRoles> _config){pass =_pass, role =_role, roleConfigure = _config;}
    QString pass;
    QString role;
    QSharedPointer<UserRoles> roleConfigure;

};
class dbController
{
public:
     dbController(QString _dbName);
    ~dbController();
    void FillEmployees(QMap<int, QSharedPointer<Employee>> &_employeesMap);
    void FillEmployeeConfig(QMap<QString, QSharedPointer<EmployeeConfig> > &_employeesConfMap);
    void FillStringList(QStringList &_list, QString _table,QString _column);
    const QSqlDatabase& getDb(){return sdb;}
    UserRoles* getUserRole(QPair<QString,QString> passPair);
    bool getSimpleTable(QSqlQueryModel* table);
    bool UpdateDataRecord(QSqlRecord &rec);
    bool AddNewRecord(QSqlRecord &rec);
private:

    QSqlDatabase sdb;
    QString dbName;
    CoreController* ccRef;
    QMap<QString,QSharedPointer<UserRoles>> mUserRolesMap;
    QMap<QString,usersRecord> mUserRecords;
    UserRoles* mCurrentRole;
};

#endif // DBCONTROLLER_H
