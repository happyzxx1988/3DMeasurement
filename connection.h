#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtSql>
#include <QDebug>
//#include <QSettings>


#define SETTINGS_VERSION                    "1.8"
#define DEFAULT_SETTINGS_FILENAME           "mysql.ini"
#define DEFAULT_PLUGINS_PATH                "plugins"


QSettings *aa;

static bool db_sqllitecreateConnection()
{

    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE","sql_lite");
    database.setDatabaseName("ch.db");
    if (!database.open()){
        qDebug() << "Error: Failed to connect database." << database.lastError();
        return false;
    } else {
        qDebug() << "Succeed to connect QSQLITE database." ;
        return true;
    }
}


static bool db_mysqlcreateConnection2()
{
    QSqlDatabase db_mysql = QSqlDatabase::addDatabase("QMYSQL");
    db_mysql.setHostName("127.0.0.1");
    db_mysql.setDatabaseName("ch");
    db_mysql.setUserName("root");
    db_mysql.setPassword("123456");
    if (!db_mysql.open()) {
        qDebug() << "MYSQL数据库链接失败：" << db_mysql.lastError();
        return false;
    }
    return true;
}


static QSettings * init_mysql()
{
    // Initialize settings
    aa = new QSettings(DEFAULT_SETTINGS_FILENAME, QSettings::IniFormat);
    if (aa->value("SettingsVersion").toString() == SETTINGS_VERSION){
        return aa;
    }

    QFileInfo fi(DEFAULT_SETTINGS_FILENAME);
    if (fi.exists())
    {
        return aa;
    }

    aa->setValue("SettingsVersion", SETTINGS_VERSION);

    aa->setValue("SQL/HostName", "127.0.0.1");
    aa->setValue("SQL/port", 3306);
    aa->setValue("SQL/DataBaseName", "ch");
    aa->setValue("SQL/UserName", "root");
    aa->setValue("SQL/PWD", "123456");

    return aa;
}

static bool db_mysqlcreateConnection()
{
    QSqlDatabase db_mysql = QSqlDatabase::addDatabase("QMYSQL","sql_lite");

    db_mysql.setHostName(aa->value("SQL/HostName").toString());
    db_mysql.setPort(aa->value("SQL/port").toInt());
    db_mysql.setDatabaseName(aa->value("SQL/DataBaseName").toString());
    db_mysql.setUserName(aa->value("SQL/UserName").toString());
    db_mysql.setPassword(aa->value("SQL/PWD").toString());

    if (!db_mysql.open()) {
        qDebug() << QStringLiteral("MYSQL数据库链接失败：") << db_mysql.lastError();
        return false;
    }
    return true;
}




//创建数据库表
static bool createTable()
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    bool success = query.exec("create table automobil(id int primary key,attribute varchar,"
                              "type varchar,kind varchar,nation int,carnumber int,elevaltor int,"
                              "distance int,oil int,temperature int)");
    if(success)
    {
        qDebug() << QObject::tr("数据库表创建成功！\n");
        return true;
    }
    else
    {
        qDebug() << QObject::tr("数据库表创建失败！\n");
        return false;
    }
}

#endif // CONNECTION_H
