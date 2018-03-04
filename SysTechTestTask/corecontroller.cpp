#include "corecontroller.h"

CoreController* CoreController::selfRef = NULL;

CoreController *CoreController::getInstance()
{
    if(!selfRef)
    {
        selfRef = new CoreController();
        selfRef->postInit();
        return selfRef;
    }
    else return selfRef;
}

CoreController::CoreController()
{


}

void CoreController::postInit()
{
    QString dbName ("testDb.db");
#ifdef UNIT_TEST
    qDebug()<<"UnitTesting in progress!";
    dbName = "dbForUnitTest.db";
#endif
    dbc = new dbController(dbName);
    dbc->FillStringList(employeeTypeList,"employee_type_config","employee_type");
  // slotPassRequest();

    updateData();
}

double CoreController::CalculateSalary(qint64 employeeId,QDate _calculateDate)
{
    double none;
    Employee* CurrentEmployee = employeesMap.find(employeeId).value().data();
    double result = CurrentEmployee->CalculateSalary(_calculateDate, &none);    
    return result;
}

void CoreController::updateData()
{
    // refill some containers    
    whoCanHaveSub.clear();
    dbc->FillEmployeeConfig(employeesConfig);
    dbc->FillEmployees(employeesMap);

    QMap<int, QSharedPointer<Employee>>::iterator it = employeesMap.begin();
    for(;it!= employeesMap.end();++it ) {        
        if(it.value()->CanHaveSub())
        {            
            whoCanHaveSub.insert( whoCanHaveSub.end(),it.value()->getFullName(),it.value().data());

        }
    }
}

void CoreController::LogOutCleanUp()
{
    employeesMap.clear();
    employeesConfig.clear();
    employeeTypeList.clear();

}

void CoreController::checkBossHierarchy(recursionCheckResult &_result, int _checkedId, int _rootId)
{
    Employee* empl =  employeesMap.find(_checkedId).value().data();
    if(empl)
    {
       int id =empl->getChiefId() ;
        _result.hierarchy.push_back(empl->getFullName());

        if(id == 0) return;
        if(id == _rootId || id == 0)
        {
             _result.hierarchy.push_back(employeesMap.find(id).value().data()->getFullName());
            _result.mValid = false;
           return;
        }
        else checkBossHierarchy(_result,id,_rootId);

    }


}

bool CoreController::checkAccess(QPair<QString, QString> _pass)
{
     mCurrentUserRole = dbc->getUserRole(_pass);
     if(mCurrentUserRole->canRead()) return true;
}

CoreController::~CoreController()
{
    delete dbc;
}

EmployeeConfig *CoreController::getConfig(QString EmployeeType)
{    
   EmployeeConfig* ref;
   QMap<QString, QSharedPointer<EmployeeConfig> >::iterator it= employeesConfig.find(EmployeeType);
   if(it!=employeesConfig.end()) return ref = it.value().data();
   else return employeesConfig.find("invalid").value().data();
}




