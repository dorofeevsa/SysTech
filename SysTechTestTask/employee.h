#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <QString>
#include <QDate>
#include <QVector>
#include <QtSql>
#include "employeeconfig.h"
#include "corecontroller.h"
#include "helperutils.h"

class CoreController;
class salaryControllerAccum;
class Employee
{
protected:
    QString mSurname, mName, mPatronymic,mEmployeeType,mLoginLink; // нужно ли?
    QVector<Employee*> mSubordinates;
    Employee* mChiefRef;
    int mChiefId, mSelfId;
    float mBaseSalary,mYearBonus, mMaxExtraBonus,StepSubordinateBonus;
    bool mCanHaveSubordinate,mBonusOfAllSubordinate;
    QDate mAdoptionDdate;
public:
    Employee(const QSqlRecord &_rec, const QSqlQuery& _query);
    Employee();
    virtual ~Employee();
    //const bool operator < ( const Employee &EmplR) {return mSelfId < EmplR.mSelfId;}
    // const bool operator < ( const Employee *EmplR) {return mSelfId < EmplR->mSelfId;}
    double CalculateSalary(QDate _dateToCalculate, double *SalaryHierarchyAccumulated = NULL, salaryControllerAccum *SalaryGlobalCollector = NULL);

    //gets
    int getId(){return mSelfId;}
    int getChiefId(){return mChiefId;}
    QString getType(){return mEmployeeType;}
    QString getFullName();
    bool CanHaveSub(){return mCanHaveSubordinate;}

    //sets
    void setChief(Employee* _chiefRef){mChiefRef = _chiefRef; _chiefRef->addSubordinates(this);}
    void addSubordinates(Employee* subOrdinateRef){mSubordinates.push_back(subOrdinateRef);}
    void configure(EmployeeConfig* config);
    const QVector<Employee*>& getSubs(){return mSubordinates;}
};

#endif // EMPLOYEE_H
