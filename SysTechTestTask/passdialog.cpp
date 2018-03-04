#include "passdialog.h"
#include "ui_passdialog.h"
#include <QDebug>
#include <QPair>
#include "corecontroller.h"
#include <QMessageBox>

PassDialog::PassDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PassDialog)
{
    parentPtr = static_cast<MainWindow*>(parent);
    ui->setupUi(this);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(slotAccept()));
    mAttemtsLeft = 3;
    this->setLayout(ui->verticalLayout);
}

PassDialog::~PassDialog()
{
    delete ui;
}

void PassDialog::slotAccept()
{
   passPair = QPair<QString,QString>(ui->login_txt->text(),ui->password_txt->text());
  if(CoreController::getInstance()->checkAccess(passPair))
  {
      this->accept();
  }
  else
  {
      mAttemtsLeft--;
      if(mAttemtsLeft>0)
      {
          QString message("WARNING! Incorrect login or password!\n Try again. "+ QString::number(mAttemtsLeft) +" attempts");
          QMessageBox msgBox;
          msgBox.setWindowTitle("Incorrect password!");
           // logging incorect password;
          msgBox.setIcon(QMessageBox::Critical);
          msgBox.setText(message);
          msgBox.exec();
      }
      else
      {
          QMessageBox msgBox;
          msgBox.setWindowTitle("Incorrect password!");
          // logging incorect password;
          msgBox.setIcon(QMessageBox::Critical);
          msgBox.setText("Incorrect password typed 3 times. \n Application shutdown!");
          msgBox.exec();
          this->reject();
      }

  }
}
