#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "editemployeedialog.h"
#include <QSharedPointer>
#include "helperutils.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include "commonsalary.h"
#include "passdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    /* new style connecting
    connect(sender, &Sender::valueChanged, [=](const QString &newValue) {
     receiver->updateValue("senderValue", newValue);
     } );
    */
    ui->setupUi(this);
    queryModel = new QSqlQueryModel();
    connect(this,SIGNAL(PasswordIncorrect()),this,SLOT(slotTerminateApp()));
    connect(ui->authorizationAction,SIGNAL(triggered(bool)),this,SLOT(slotPassRequest()));
    slotLogOut();
    connect(ui->simpleQueryView,&QTableView::clicked,[=](){ui->bEmployeeDetails->setEnabled(true);});
    connect(ui->simpleQueryView,&QTableView::clicked,[=](){ui->bEmployeeList->setEnabled(true);});
    QTimer::singleShot(50,this,SLOT(slotPassRequest()));
    ui->bEmployeeList->setEnabled(false);


}

MainWindow::~MainWindow()
{
    delete ui;
    delete queryModel;
}

void MainWindow::UpdateMainTable()
{
    CoreController* ccRef = CoreController::getInstance();
    dbController*  const dbCtrlRef = ccRef->getDbController();

    if(dbCtrlRef->getSimpleTable(queryModel))
    {
        ui->simpleQueryView->setModel(queryModel);
        ui->simpleQueryView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
    else
    {   // logging not access
        delete queryModel;
    }
}

void MainWindow::CheckPassCorrect()
{
    UpdateMainTable();
    UserRoles* currentRole = CoreController::getInstance()->getUserRole();

    if(currentRole->canRead()) ui->bEmployeeDetails->show();
    else ui->bEmployeeDetails->hide();


    if(currentRole->canWrite()) ui->bEmployeeNew->show();
    else ui->bEmployeeNew->hide();

    if(currentRole->canAllAccess()) ui->bAllSalaryCalc->show();
    else  ui->bAllSalaryCalc->hide();

    ui->bEmployeeDetails->setEnabled(false);

}

void MainWindow::UpdateDataRecord(QSqlRecord &rec)
{
   dbController*  const dbCtrlRef = CoreController::getInstance()->getDbController();
   dbCtrlRef->UpdateDataRecord(rec);
   CoreController::getInstance()->updateData();
   UpdateMainTable();
}

void MainWindow::AddNewEmployee(QSqlRecord &rec)
{
    dbController*  const dbCtrlRef = CoreController::getInstance()->getDbController();
    if(!dbCtrlRef->AddNewRecord(rec))
    {
       // logging qDebug()<<"error while add new info";
    }

}

void MainWindow::slotTerminateApp()
{
    qApp->quit();
    close();
    QApplication::closeAllWindows();

}

void MainWindow::slotPassRequest()
{
   passDlg = new PassDialog(this);
   if( passDlg->exec()== QDialog::Accepted )
   {
       CheckPassCorrect();
       delete passDlg;
   }
   else
   {
      delete passDlg;
       slotTerminateApp();

   }

}

void MainWindow::slotLogOut()
{
    ui->bAllSalaryCalc->hide();
    ui->bEmployeeDetails->hide();
    ui->bEmployeeNew->hide();
}

void MainWindow::on_bEmployeeDetails_clicked()
{
    int currentRow = ui->simpleQueryView->selectionModel()->currentIndex().row();

    CoreController* ccRef = CoreController::getInstance();
    dbController*  const dbCtrlRef = ccRef->getDbController();
    QSqlQuery employeeQuery("SELECT * FROM employee_all WHERE id=" +queryModel->record(currentRow).value("id").toString()+ "",dbCtrlRef->getDb());

    employeeQuery.next();
    QSqlRecord* rec = new QSqlRecord(employeeQuery.record());
    EditEmployeeDialog* emplDlg = new EditEmployeeDialog(this);
    emplDlg->setRecord(*rec);
    emplDlg->show();
    connect(emplDlg,SIGNAL(updateRecord(QSqlRecord&)),this,SLOT(UpdateDataRecord(QSqlRecord&)));
}

void MainWindow::on_bEmployeeNew_clicked()
{
    EditEmployeeDialog* emplDlg = new EditEmployeeDialog(this);
    emplDlg->toggleToAddNew();
    emplDlg->show();
    connect(emplDlg,SIGNAL(createNewEmployee(QSqlRecord&)),this,SLOT(AddNewEmployee(QSqlRecord&)));
}

void MainWindow::CommonSalaryCalc(int id, Employee* empl)
{
    salaryControllerAccum accum;
    CoreController* ccRef =CoreController::getInstance();
    UserRoles* currentRole = ccRef->getUserRole();
    ccRef->getEmployee(id)->CalculateSalary(QDate::currentDate(), NULL,&accum);

    commonSalary* calculatedSalary = new commonSalary(this);
    calculatedSalary->setData(&accum,empl);
    calculatedSalary->show();
}

void MainWindow::on_bAllSalaryCalc_clicked()
{
    CommonSalaryCalc(1);
}

void MainWindow::on_pushButton_clicked()
{
    slotPassRequest();
}

void MainWindow::on_bEmployeeList_clicked()
{
    int currentRow =  ui->simpleQueryView->selectionModel()->currentIndex().row();
    int id = queryModel->record(currentRow).value("id").toInt();
    CommonSalaryCalc(id,CoreController::getInstance()->getEmployee(id));
}
