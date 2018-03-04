#include <QString>
#include <QtTest>
#include "../../SysTechTestTast/corecontroller.h"
#include "../../SysTechTestTast/dbcontroller.h"
#include "../../SysTechTestTast/employee.h"
#include "../../SysTechTestTast/employeeconfig.h"


class Test_CoreController : public QObject
{
    Q_OBJECT

public:
    Test_CoreController();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCaseSalaryCalc();
};

Test_CoreController::Test_CoreController()
{
}

void Test_CoreController::initTestCase()
{
}

void Test_CoreController::cleanupTestCase()
{
}

void Test_CoreController::testCaseSalaryCalc()
{
    CoreController* ccRef = CoreController::getInstance();
    QVERIFY2(true, "Failure");
   // QCOMPARE(ccRef->testingDummy(),true);
    QDate date(QDate::fromString("02.02.2004","dd.MM.yyyy"));

     salaryControllerAccum accum;
     Employee* empl = ccRef->getEmployee(1);
     double emplResult = empl->CalculateSalary(date,NULL,&accum);
     double allCompanySalary = accum.getAllSum();
    qDebug()<<"Check big boss salary: ";
     QCOMPARE(QString::number(emplResult,'d',2),QString("120109.35")); //проверить расчеты
     qDebug()<<"Check all company salary: ";
     QCOMPARE(QString::number(allCompanySalary,'d',2),QString("165013.60")); //проверить расчеты
}

QTEST_APPLESS_MAIN(Test_CoreController)

#include "tst_test_corecontroller.moc"
