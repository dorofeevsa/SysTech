#include "commonsalary.h"
#include "ui_commonsalary.h"
#include <QtAlgorithms>

commonSalary::commonSalary(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::commonSalary)
{
    ui->setupUi(this);
     this->setLayout(ui->verticalLayout);
    ui->tableAllSalary->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

commonSalary::~commonSalary()
{
    delete ui;
}

void commonSalary::setData(salaryControllerAccum *_data, Employee *empl)
{
    if(!empl)
    {
        int i=0;
        for(QMap<Employee*,double>::iterator it = _data->accumulated.begin();it!=_data->accumulated.end();it++)
        {

            ui->tableAllSalary->insertRow(i);
            ui->tableAllSalary->setItem(i,0,new QTableWidgetItem(it.key()->getFullName()));
            ui->tableAllSalary->setItem(i,1,new QTableWidgetItem(QString::number(it.value())));
            i++;
        }

         ui->tableAllSalary->insertRow(i);
         ui->tableAllSalary->setItem(i,0,new QTableWidgetItem(QString("Общая зарплата")));
         ui->tableAllSalary->setItem(i,1,new QTableWidgetItem(QString::number(_data->getAllSum())));

    }
    else
    {
        QVector<Employee*>subs = empl->getSubs();
         int i=0;
         QMap<Employee*,double>::iterator it = _data->accumulated.find(empl);
         ui->tableAllSalary->insertRow(i);
         ui->tableAllSalary->setItem(i,0,new QTableWidgetItem(it.key()->getFullName()));
         ui->tableAllSalary->setItem(i,1,new QTableWidgetItem(QString::number(it.value())));
         i++;
         for(int j = 0; j<subs.size();j++)
         {
             it = _data->accumulated.find(subs[j]);
             ui->tableAllSalary->insertRow(i);
             ui->tableAllSalary->setItem(i,0,new QTableWidgetItem(it.key()->getFullName()));
             ui->tableAllSalary->setItem(i,1,new QTableWidgetItem(QString::number(it.value())));

            i++;
         }
    }

}

