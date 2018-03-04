#include "employee.h"


#include <QDebug>

Employee::Employee(const QSqlRecord &_rec, const QSqlQuery& _query)
{
    mSelfId = _query.value(_rec.indexOf("id")).toInt();
    mSurname = _query.value(_rec.indexOf("surname")).toString();
    mName = _query.value(_rec.indexOf("name")).toString();
    mPatronymic = _query.value(_rec.indexOf("patronymic")).toString();
    mEmployeeType = _query.value(_rec.indexOf("employee_type")).toString();
    //mAdoptionDdate = QDate::fromString(_query.value(_rec.indexOf("date_of_adoption")).toString(),"dd.mm.yy");
    QString dateS = _query.value(_rec.indexOf("date_of_adoption")).toString();
    mAdoptionDdate = QDate::fromString(dateS,"d.M.yyyy");
    mBaseSalary = _query.value(_rec.indexOf("base_salary")).toFloat();
    mChiefId  = _query.value(_rec.indexOf("chief")).toInt();
    qDebug()<<"Employee(rec query)";

}
Employee::~Employee()
{
    qDebug()<<"~Employee()";
}
Employee::Employee()
{
    qDebug()<<"Employee()";
}

double Employee::CalculateSalary(QDate _dateToCalculate, double * SalaryHierarchyAccumulated, salaryControllerAccum *SalaryGlobalCollector)
{
    // TODO: параметр расчета иерархичности передавать вручную, следить за тем, будет ли иерархичный расчет влиять на зарплату конкретного сотрудника
    double yearExp = 0.0;
    if(_dateToCalculate > mAdoptionDdate)
    { // расчет зарплаты за стаж

        double fullWorkingYears = (_dateToCalculate.year() - mAdoptionDdate.year());
        if(mAdoptionDdate.daysTo(_dateToCalculate)<365) fullWorkingYears = 0.0;
        double salaryCoef(fullWorkingYears*mYearBonus);

        if(salaryCoef>mMaxExtraBonus) salaryCoef=mMaxExtraBonus;
        yearExp = salaryCoef*mBaseSalary; //надбавка за стаж
    }
    yearExp +=mBaseSalary;
    if(!mCanHaveSubordinate || mSubordinates.isEmpty() )
       {//возвращаются надбавка за стаж + оклад, и эта рассчитанная зарплата плюсуется в иерархуческий аккумулятор зарплат
         if(SalaryHierarchyAccumulated) *SalaryHierarchyAccumulated+=yearExp;
         if(SalaryGlobalCollector) SalaryGlobalCollector->push(this,yearExp);
          return yearExp;
       }
    //==========================
        else
        {
            double accumHierarchySalar = 0.0; // для аккумулирования
            double SubOrdinarySalar = 0.0;

            if(mBonusOfAllSubordinate)
            {
                // собираем по всей нижеследующей иерархии
               salaryControllerAccum AllHierarchyCollect;
                for(int i = 0; i<mSubordinates.size();i++)
                {
                  mSubordinates[i]->CalculateSalary(_dateToCalculate,&accumHierarchySalar,&AllHierarchyCollect);
                }
                yearExp += (StepSubordinateBonus*(AllHierarchyCollect.getAllSum()));                
                if(SalaryHierarchyAccumulated) *SalaryHierarchyAccumulated+=yearExp;
                if(SalaryGlobalCollector)
                {
                    SalaryGlobalCollector->push(AllHierarchyCollect);
                    SalaryGlobalCollector->push(this,yearExp);
                }
                return yearExp;
            }
            else
            {
                //собираем только по первому шагу подчинения
                for(int i = 0; i<mSubordinates.size();i++)
                {
                  SubOrdinarySalar+= mSubordinates[i]->CalculateSalary(_dateToCalculate,NULL,SalaryGlobalCollector);
                }
                yearExp += (StepSubordinateBonus*SubOrdinarySalar);
                if(SalaryHierarchyAccumulated) *SalaryHierarchyAccumulated+=yearExp;
                if(SalaryGlobalCollector) SalaryGlobalCollector->push(this,yearExp);
                return yearExp;
            }
        }
}

QString Employee::getFullName()
{
    QString fullName(mSurname +" "+mName+" "+mPatronymic);
    return fullName;
}

void Employee::configure(EmployeeConfig *config)
{
    if(config->isValid())
    {
        mYearBonus = config->getYearBonus();
        StepSubordinateBonus = config->getStepSubBonus();
        mMaxExtraBonus = config->getMaxYearsBonus();
        mCanHaveSubordinate = config->isCanHaveSub();
        mBonusOfAllSubordinate = config->isBonusOfAllSub();
    }
    else
    {
        // CoreController::Logging("Invalid config for type " +mEmployeeType+" with ID:"+mSelfId+ "!");
    }

}
