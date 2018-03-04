#include "employeeconfig.h"
EmployeeConfig::EmployeeConfig()
{
    mValid = false;
}
EmployeeConfig::EmployeeConfig(const QSqlRecord &_rec,  const QSqlQuery& _query)
{   
    mTypeName = _query.value(_rec.indexOf("employee_type")).toString();
    mYearBonus = _query.value(_rec.indexOf("every_year_bonus")).toFloat();
    mMaxExtraBonus = _query.value(_rec.indexOf("max_extra_bonus")).toFloat();
    mStepSubordinateBonus =_query.value(_rec.indexOf("step_subordinate_bonus")).toFloat();
    mCanHaveSubordinate = _query.value(_rec.indexOf("can_have_subordinate")).toBool();
    mBonusOfAllSubordinate = _query.value(_rec.indexOf("bonus_on_all_subordinate")).toBool();
    mBaseSalary = _query.value(_rec.indexOf("base_salary")).toFloat();
    mValid = true;
}
