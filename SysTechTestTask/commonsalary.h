#ifndef COMMONSALARY_H
#define COMMONSALARY_H

#include <QDialog>
#include "helperutils.h"

namespace Ui {
class commonSalary;
}

class commonSalary : public QDialog
{
    Q_OBJECT

public:
    explicit commonSalary(QWidget *parent = 0);
    ~commonSalary();

    void setData(salaryControllerAccum* _data, Employee*  empl = NULL);

private:
    Ui::commonSalary *ui;
};

#endif // COMMONSALARY_H
