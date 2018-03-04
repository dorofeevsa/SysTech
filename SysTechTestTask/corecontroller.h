#ifndef CORECONTROLLER_H
#define CORECONTROLLER_H

#include "employee.h"
#include "dbController.h"
#include "employeeconfig.h"
#include <QMap>
#include <QStringList>
#include "helperutils.h"
#include <QSharedPointer>

class dbController;
class Employee;
class CoreController
{
public:

    struct recursionCheckResult
    {
        bool mValid;
        QStringList hierarchy;
    };
    
    // get's:
     static CoreController* getInstance();
    EmployeeConfig* getConfig(QString EmployeeType);
    dbController* getDbController(){return dbc;}
    QStringList getEmployeesTypeList(){return employeeTypeList; }
    QMap<QString,Employee*>& getWhoCanSub(){return whoCanHaveSub;}
    Employee* getEmployee(int _id){return employeesMap.find(_id).value().data() ;}

    int getNewId(){ return employeesMap.size();}

    // sets and calls:
    double CalculateSalary(qint64 employeeId, QDate _calculateDate);
    //Logging(QString);
    void updateData();
    void LogOutCleanUp();
    void checkBossHierarchy(recursionCheckResult &_result, int _checkedId, int _rootId);
    bool testingDummy(){return true;}
    UserRoles* getUserRole(){return mCurrentUserRole;}

    bool checkAccess(QPair<QString,QString> _pass);
    ~CoreController();
private:
   CoreController();
   void postInit();
   static CoreController* selfRef;

    dbController* dbc;
    QMap<int, QSharedPointer<Employee>> employeesMap;
    QMap<QString, QSharedPointer<EmployeeConfig>> employeesConfig;
    QStringList employeeTypeList;
    QMap <QString, Employee*> whoCanHaveSub;

    UserRoles* mCurrentUserRole;

};

#endif // CORECONTROLLER_H
