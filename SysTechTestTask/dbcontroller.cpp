#include "dbcontroller.h"
#include "helperutils.h"
dbController::dbController(QString _dbName)
{
    sdb = QSqlDatabase::addDatabase("QSQLITE");
    dbName = _dbName;
    sdb.setDatabaseName(dbName);
    if(sdb.open())
    {
        qDebug()<<"DB "+ _dbName +" is connect and open!";
        QSqlQuery roleQuery(sdb);
        if (!roleQuery.exec("SELECT * FROM roles")) qDebug()<<roleQuery.lastError().text();
        else
        {
            QSqlRecord rec = roleQuery.record();
            while (roleQuery.next())
            {
                mUserRolesMap.insert( roleQuery.value(rec.indexOf("role")).toString(),
                                      QSharedPointer<UserRoles>(new UserRoles(
                                    roleQuery.value(rec.indexOf("read")).toBool(),
                                    roleQuery.value(rec.indexOf("write")).toBool(),
                                    roleQuery.value(rec.indexOf("all_access")).toBool(),
                                    roleQuery.value(rec.indexOf("deleting")).toBool())
                                     ));
            }
        }


        QSqlQuery query(sdb);
        if (!query.exec("SELECT * FROM passwords")) qDebug()<<query.lastError().text();
        else
        {
            QSqlRecord rec = query.record();
            while (query.next())
            {
                QSharedPointer<UserRoles>sharedPtrRole(new UserRoles(*mUserRolesMap.find( query.value(rec.indexOf("role")).toString() ).value().data()));
                QMap<QString,usersRecord>::iterator it = mUserRecords.insert(query.value(rec.indexOf("login")).toString(),
                                    usersRecord(query.value(rec.indexOf("password")).toString(),
                                                    query.value(rec.indexOf("role")).toString(),
                                              sharedPtrRole )
                                    );               
                it.value().roleConfigure->setId(query.value(rec.indexOf("id")).toInt());
            }

        }
    }

}

dbController::~dbController()
{


}
void dbController::FillEmployees(QMap<int, QSharedPointer<Employee>> &_employeesMap)
{
    ccRef =  CoreController::getInstance(); //todo: replace on some post-init section
    QSqlQuery query(sdb);
    if (!query.exec("SELECT * FROM employee_all")) qDebug()<<query.lastError().text();
    QSqlRecord rec = query.record();
    _employeesMap.clear();    
    _employeesMap.insert(0,QSharedPointer<Employee>(new Employee())); //zeroDummy
    while (query.next())
    {
         // базовое заполнение
       // Employee* empTemp = new Employee(rec, query);
        QSharedPointer<Employee> ptr (new Employee(rec, query));
        _employeesMap.insert(ptr->getId(),ptr );

    }
    // точечная донастройка
   QMap<int, QSharedPointer<Employee>>::iterator it = _employeesMap.begin();
    for(;it!= _employeesMap.end();it++)
    {
        if(it.value()->getId() != 0 && it.key()!=0)
        {
           int id = it.value()->getChiefId();
           if(id>0) it.value()->setChief(_employeesMap.find(id).value().data());
           it.value()->configure(ccRef->getConfig( it.value()->getType()));

        }

    }

}

void dbController::FillEmployeeConfig(QMap<QString,QSharedPointer<EmployeeConfig>> &_employeesConfMap)
{
    _employeesConfMap.clear();
    QSqlQuery query(sdb);
    if (!query.exec("SELECT * FROM employee_type_config")) qDebug()<<query.lastError().text();
    QSqlRecord rec = query.record();
    _employeesConfMap.insert("invalid", QSharedPointer<EmployeeConfig> (new EmployeeConfig())); //TODO: rewrite with shared_ptr
    while (query.next())
    {
        QSharedPointer<EmployeeConfig>confTemp(new EmployeeConfig(rec, query));
        _employeesConfMap.insert(confTemp->getName(),confTemp);
    }

}

 void dbController::FillStringList(QStringList &_list, QString _table,QString _column)
 {
      QSqlQuery query(sdb);
      if (!query.exec("SELECT " +_column+ " FROM " +_table+ "")) qDebug()<<query.lastError().text();
      QSqlRecord rec;

      while (query.next())
      {
          rec = query.record();         
        _list<< rec.value(_column).toString();

      }
 }

 UserRoles* dbController::getUserRole(QPair<QString,QString> passPair)
 {
     QMap<QString,usersRecord>::iterator it = mUserRecords.find(passPair.first);
     if(it!= mUserRecords.end())
     {
         if(it.value().pass == passPair.second)
         {
             //CorrectedPass
             mCurrentRole = mUserRecords.find(passPair.first).value().roleConfigure.data();
             return mCurrentRole;
         }

     }

     {
        //IncorrectedPass
        mCurrentRole = mUserRolesMap.find("UNAUTHORIZED").value().data();
        return mCurrentRole;
     }
 }

 bool dbController::getSimpleTable(QSqlQueryModel* table)
  {


     bool allAccess = mCurrentRole->canAllAccess();

     if(mCurrentRole->canRead())
     {
          if(allAccess)
          {
             table->setQuery("SELECT id, surname, name, patronymic, employee_type FROM employee_all ORDER by id ASC ", sdb); //
          }
          else
          {
              table->setQuery("SELECT id, surname, name, patronymic, employee_type FROM employee_all WHERE id=" + QString::number(mCurrentRole->idAvailable()) +" ORDER by id ASC " , sdb); //id, surname, name, patronymic, employee_type
          }

          table->setHeaderData(0, Qt::Horizontal, QString("ID"));
          table->setHeaderData(1, Qt::Horizontal, QString("Surname"));
          table->setHeaderData(2, Qt::Horizontal, QString("Name"));
          table->setHeaderData(3, Qt::Horizontal, QString("Patronymic"));
          table->setHeaderData(4, Qt::Horizontal, QString("Employee type"));

          return true;
     }
     else return false;


  }

  bool dbController::UpdateDataRecord(QSqlRecord &rec)
  {
      if(mCurrentRole->canWrite())
      {
          QString strArgsPrepare ="UPDATE employee_all SET "
                                  "name='%1', "
                                  "surname='%2', "
                                  "patronymic='%3', "
                                  "employee_type='%4', "
                                  "date_of_adoption='%5', "
                                  "chief='%6' "
                                  "WHERE id=%7";
          QString finalQuery = strArgsPrepare
                  .arg(rec.value("name").toString())
                  .arg(rec.value("surname").toString())
                  .arg(rec.value("patronymic").toString())
                  .arg(rec.value("employee_type").toString())
                  .arg(rec.value("date_of_adoption").toString())
                  .arg(rec.value("chief").toString())
                  .arg(rec.value("id").toString());

        QSqlQuery query(finalQuery,sdb);
        // logging qDebug()<<"prepared query: "<< finalQuery;
        if(!query.exec())
        {
            qDebug()<<query.lastError();  // logging
        }
        return true;
          
      }
      else return false;
}

bool dbController::AddNewRecord(QSqlRecord &rec)
{
     if(mCurrentRole->canWrite())
     {
         QString strArgsPrepare ="INSERT INTO employee_all "
                                 "(id,surname,name,patronymic,employee_type,date_of_adoption,base_salary,chief) "
                                 "VALUES("
                                 "%1, "
                                 "'%2', "
                                 "'%3', "
                                 "'%4', "
                                 "'%5', "
                                 "'%6', "
                                 "%7, "
                                 "'%8') ";
         QString finalQuery = strArgsPrepare
                 .arg(rec.value("id").toString())
                 .arg(rec.value("surname").toString())
                 .arg(rec.value("name").toString())
                 .arg(rec.value("patronymic").toString())
                 .arg(rec.value("employee_type").toString())
                 .arg(rec.value("date_of_adoption").toString())
                 .arg(rec.value("base_salary").toString())
                 .arg(rec.value("chief").toString());

         QSqlQuery query(finalQuery,sdb);

         //qDebug()<<"prepared query: "<< finalQuery;
         if(!query.exec())
         {
             qDebug()<<query.lastError().databaseText();
         }
         return true;

     }
      else return false;
}


