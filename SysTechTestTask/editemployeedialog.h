#ifndef EDITEMPLOYEEDIALOG_H
#define EDITEMPLOYEEDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlRecord>
#include "employee.h"
namespace Ui {
class EditEmployeeDialog;
}

class EditEmployeeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditEmployeeDialog(QWidget *parent = 0);
    ~EditEmployeeDialog();

    void setRecord(QSqlRecord& rec);
    void toggleToAddNew();
    void fillBossList(QSqlRecord const &rec = QSqlRecord());

signals:
   void updateRecord(QSqlRecord& rec);
    void createNewEmployee(QSqlRecord& rec);
private slots:
    void on_updButton_clicked();

    void on_createButton_clicked();

    void on_salaryButton_clicked();

private:
    Ui::EditEmployeeDialog *ui;
    QSqlRecord* employeeRec;    
    QMap<QString, Employee*> * canHaveSubMap;
};

#endif // EDITEMPLOYEEDIALOG_H
