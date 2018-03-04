#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include "passdialog.h"
#include "dbController.h"
#include "employee.h"
#include "corecontroller.h"
#include <QMap>
#include<QPair>

namespace Ui {
class MainWindow;
}
class PassDialog;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

 public:
    void CheckPassCorrect();

    void UpdateMainTable();
    
    void CommonSalaryCalc(int id, Employee *empl = NULL);

signals:
    void PasswordIncorrect();
public slots:
    void UpdateDataRecord(QSqlRecord &rec);
    void AddNewEmployee(QSqlRecord &rec);
    void slotPassRequest();
    void slotLogOut();

private slots:
    void slotTerminateApp();



    void on_bEmployeeDetails_clicked();

    void on_bEmployeeNew_clicked();

    void on_bAllSalaryCalc_clicked();

    void on_pushButton_clicked();

    void on_bEmployeeList_clicked();

private:
    Ui::MainWindow *ui;
    PassDialog* passDlg;
    QSqlTableModel* mainTableModel;
    QSqlQueryModel* queryModel;

};

#endif // MAINWINDOW_H
