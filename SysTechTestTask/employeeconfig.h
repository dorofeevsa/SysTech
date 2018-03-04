#ifndef EMPLOYEECONFIG_H
#define EMPLOYEECONFIG_H
/*
simple config class
*/

#include <QtSql>

class EmployeeConfig
{
public:
    EmployeeConfig();
    EmployeeConfig(const QSqlRecord &_rec, const QSqlQuery &_query);

    QString getName(){return mTypeName;}
    float getYearBonus(){return mYearBonus;}
    float getMaxYearsBonus(){return mMaxExtraBonus;}
    float getStepSubBonus(){return mStepSubordinateBonus;}
    float getBaseSalary(){return mBaseSalary;}
    bool isCanHaveSub(){return mCanHaveSubordinate;}
    bool isBonusOfAllSub(){return mBonusOfAllSubordinate;}
    bool isValid(){return mValid;}

private:
    QString mTypeName;
    float mYearBonus,mMaxExtraBonus,mStepSubordinateBonus, mBaseSalary ;
    bool mCanHaveSubordinate, mBonusOfAllSubordinate;
    bool mValid;
};

#endif // EMPLOYEECONFIG_H
