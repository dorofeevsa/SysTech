#ifndef HELPERUTILS_H
#define HELPERUTILS_H
#include <QMap>
#include <QSet>
#include "employee.h"
class Employee;

struct salaryControllerAccum
{
    QMap<Employee*,double>accumulated;
    void  push(Employee* _key,double _salary)
    {
        accumulated.insert(accumulated.end(),_key,_salary );

    }
    void push(salaryControllerAccum& _another)
    {
        accumulated.unite(_another.accumulated);
    }

    double getAllSum()
    {
        double sum = 0.0;
        for (QMap<Employee*,double>::iterator it =accumulated.begin();it!=accumulated.end();it++)
        {
           sum += it.value();
        }
        return sum;
    }

};
struct UserRoles
{
public:
    enum ROLES{SUPER,EMPLOYEE,SUPER_RO,HR};
    ROLES getRole(){return mCurrentRole;}
    bool canRead(){return mCanRead;}
    bool canWrite(){return mCanWrite;}
    bool canAllAccess(){return mAllAccess;}
    bool canDelete(){return mCanDelete;}
    int idAvailable(){return mCurrentEmployeeId; }
    void setId(int _id){mCurrentEmployeeId = _id;}
    UserRoles(bool _read,bool _write,bool _allAccess,bool _canDelete, int _id =0):
        mCanRead(_read),
        mCanWrite(_write),
        mAllAccess(_allAccess),
        mCanDelete(_canDelete),
        mCurrentEmployeeId(_id){}

    UserRoles():
        mCanRead(false),
        mCanWrite(false),
        mAllAccess(false),
        mCanDelete(false),
        mCurrentEmployeeId(0){}

    UserRoles(UserRoles& another):
        mCanRead(another.mCanRead),
        mCanWrite(another.mCanWrite),
        mAllAccess(another.mAllAccess),
        mCanDelete(another.mCanDelete),
        mCurrentEmployeeId(0){}

private:
    ROLES mCurrentRole;
    bool mCanRead;
    bool mCanWrite;
    bool mAllAccess;
    bool mCanDelete;
    int mCurrentEmployeeId;

};
#endif // HELPERUTILS_H
