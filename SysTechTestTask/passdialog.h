#ifndef PASSDIALOG_H
#define PASSDIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class PassDialog;
}

class MainWindow;
class PassDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PassDialog(QWidget *parent = 0);
    ~PassDialog();


 QPair<QString,QString> passPair;


    private slots:
    void slotAccept();


private:
    Ui::PassDialog *ui;
    MainWindow* parentPtr;
    int mAttemtsLeft;

};

#endif // PASSDIALOG_H
