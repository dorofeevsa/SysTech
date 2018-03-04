#include "editemployeedialog.h"
#include "ui_editemployeedialog.h"
#include "dbcontroller.h"
#include "corecontroller.h"
#include <QMessageBox>

EditEmployeeDialog::EditEmployeeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditEmployeeDialog)
{
    ui->setupUi(this);
    ui->dateEdit_salary->setDate(QDate::currentDate());
     this->setLayout(ui->verticalLayout);
    if(CoreController::getInstance()->getUserRole()->canWrite())
    {
        ui->updButton->show();
    }
    else
    {
        ui->updButton->hide();
    }
}

EditEmployeeDialog::~EditEmployeeDialog()
{
    delete ui;
    delete employeeRec;
}

void EditEmployeeDialog::fillBossList(const QSqlRecord &rec)
{
    canHaveSubMap = &CoreController::getInstance()->getWhoCanSub();
    QMap<QString,Employee*>::iterator it =  canHaveSubMap->begin();   
    for(;it!= canHaveSubMap->end();++it) {
       ui->combo_chief->addItem(it.value()->getFullName());

       if( !rec.isEmpty() && rec.value("chief").toString() == QString::number(it.value()->getId())) ui->combo_chief->setCurrentIndex(ui->combo_chief->count()-1);
    }
}

void EditEmployeeDialog::setRecord(QSqlRecord &rec)
{
     ui->createButton->hide();
    employeeRec = &rec;
     //(("SELECT * from employee_all WHERE id " +rec.value("id").toString()+ " ORDER by id ASC"));
    ui->line_id->setText(rec.value("id").toString());
    ui->line_name->setText(rec.value("name").toString());
    ui->line_surname->setText(rec.value("surname").toString());
    ui->line_patronymic ->setText(rec.value("patronymic").toString());
    ui->line_salary->setText(rec.value("base_salary").toString());
    QStringList typeList = CoreController::getInstance()->getEmployeesTypeList();
    ui->combo_type->addItems(typeList);

    auto indexSearch = [&](){
            for(int it = 0;it<typeList.size();++it)
            {
                if(typeList[it] == rec.value("employee_type").toString()) return it;
            }

            return int(0);};

     ui->combo_type->setCurrentIndex(indexSearch());
    ui->dateEdit->setDate(QDate::fromString(rec.value("date_of_adoption").toString(),"d.M.yyyy"));



   fillBossList(rec);
}


void EditEmployeeDialog::on_updButton_clicked()
{
   employeeRec->setValue("name",ui->line_name->text());
   employeeRec->setValue("surname",ui->line_surname->text());
   employeeRec->setValue("patronymic",ui->line_patronymic->text());
   employeeRec->setValue("employee_type",ui->combo_type->currentText());

   QString dateStr(QString::number(ui->dateEdit->date().day())+"."+
                 QString::number(ui->dateEdit->date().month())+"."+
                 QString::number(ui->dateEdit->date().year()));

   employeeRec->setValue("date_of_adoption",dateStr);
   employeeRec->setValue("base_salary",ui->line_salary->text());

   // проверка на рекурсивность боссов
    int selectedBossID = 0;


    QMap<QString, Employee*>::iterator it = canHaveSubMap->find(ui->combo_chief->currentText());

   if(ui->combo_chief->currentIndex()!=0 && it!=canHaveSubMap->end())
   {
       selectedBossID = it.value()->getId();
       int currentEmployeeID = ui->line_id->text().toInt();
       CoreController::recursionCheckResult result;
       CoreController::getInstance()->checkBossHierarchy(result,selectedBossID,currentEmployeeID);

      if(selectedBossID==currentEmployeeID)
      {
           QMessageBox msgBox;
           msgBox.setWindowTitle("Recursed hierarchy!");
           msgBox.setIcon(QMessageBox::Critical);
           msgBox.setText("Warning!Hierarchy self-recursion!");
           msgBox.exec();
           ui->combo_chief->setCurrentIndex(0);
       }
       if(result.mValid )
       {
           employeeRec->setValue("chief",QString::number(selectedBossID));
           emit updateRecord(*employeeRec);
       }
       else
       {   QString message("WARNING! Recursed hierarchy!\n");
           message+=ui->line_surname->text()+"->\n";
            for(int i=0;i<result.hierarchy.size();i++)
            {
                message+=result.hierarchy[i]+ "->\n";

            }
           QMessageBox msgBox;
           msgBox.setWindowTitle("Recursed hierarchy!");
           msgBox.setIcon(QMessageBox::Critical);
           msgBox.setText(message);
           msgBox.exec();
           ui->combo_chief->setCurrentIndex(0);
       }
       
   }
   else
   {
       employeeRec->setValue("chief",QString::number(selectedBossID));
       emit updateRecord(*employeeRec);
   }
       

}

void EditEmployeeDialog::toggleToAddNew()
{
    ui->dateEdit->setDate(QDate::currentDate());    
    ui->line_id->setText(QString::number(CoreController::getInstance()->getNewId()));
    ui->updButton->hide();
    ui->combo_type->addItems(CoreController::getInstance()->getEmployeesTypeList());
    fillBossList();
}



void EditEmployeeDialog::on_createButton_clicked()
{   delete employeeRec;
    employeeRec = new QSqlRecord();
    employeeRec->append(QSqlField("id"));
    employeeRec->append(QSqlField("name"));
    employeeRec->append(QSqlField("surname"));
    employeeRec->append(QSqlField("patronymic"));
    employeeRec->append(QSqlField("employee_type"));
    employeeRec->append(QSqlField("date_of_adoption"));
    employeeRec->append(QSqlField("base_salary"));
    // todo refactoring
         employeeRec->setValue("id",ui->line_id->text());
       employeeRec->setValue("name",ui->line_name->text());
       employeeRec->setValue("surname",ui->line_surname->text());
       employeeRec->setValue("patronymic",ui->line_patronymic->text());
       employeeRec->setValue("employee_type",ui->combo_type->currentText());

       QString dateStr(QString::number(ui->dateEdit->date().day())+"."+
                     QString::number(ui->dateEdit->date().month())+"."+
                     QString::number(ui->dateEdit->date().year()));

       employeeRec->setValue("date_of_adoption",dateStr);
       employeeRec->setValue("base_salary",ui->line_salary->text());
       employeeRec->setValue("chief",0);
       // refactoring
       emit createNewEmployee(*employeeRec);
}

void EditEmployeeDialog::on_salaryButton_clicked()
{
    qint32 id = ui->line_id->text().toInt();
   ui->txt_log->append("Calculated salary for "+
                       ui->line_surname->text()+" "
                       +ui->line_name->text()+" "
                       +ui->line_patronymic->text()+
                       " to date: "+ ui->dateEdit_salary->date().toString()+":\n");
   ui->txt_log->append(QString::number( CoreController::getInstance()->CalculateSalary(id,ui->dateEdit_salary->date())));

}
