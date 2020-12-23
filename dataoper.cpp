#pragma execution_character_set("utf-8")  //加了这句话之后，就不用QStringLiteral了
#include "dataoper.h"
#include <QtSql>


DataOper::DataOper(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::database("sql_lite");//连接数据库2
}


//新增用户
void DataOper::saveUser(User u)
{
    QSqlQuery query(db);
    db.transaction();
    query.prepare("INSERT INTO USER(name,password,date,isUse) VALUES (?,?,?,?)");
    query.addBindValue(u.name);
    query.addBindValue(u.password);
    query.addBindValue(u.date);
    query.addBindValue(u.isUse);
    if (query.exec()){ //进行批处理，如果出错就输出错误
        db.commit();
        emit sendDataMessage(tr("保存用户成功!"));
    }else{
         qDebug() << tr("保存用户失败: ") + query.lastError().text();
    }
}
//更新用户
void DataOper::updateUser(User u)
{
    QSqlQuery query(db);
    db.transaction();
    query.prepare("UPDATE USER SET name = ?,password = ? ,isUse = ? where name = ?");
    query.addBindValue(u.name);
    query.addBindValue(u.password);
    query.addBindValue(u.isUse);
    query.addBindValue(u.name);
    if (query.exec()){
        db.commit();
        emit sendDataMessage(tr("修改用户成功!"));
    }else{
         qDebug() << tr("修改用户失败: ") + query.lastError().text();
    }
}
//判断当前新增用户是否存在
bool DataOper::userIsExist(User u)
{
    QSqlQuery query(db);
    query.prepare("select  name from USER where name = ?");
    query.addBindValue(u.name);
    if (query.exec()){
        if(query.next()){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}
//根据用户名删除用户
bool DataOper::deleteUserByName(QString userName)
{
    QSqlQuery query(db);
    QString sql = QString("delete from USER where name = '%1' ").arg(userName);
    if(query.exec(sql)){
//        emit sendDataMessage(tr("删除用户成功! "));
        return true;
    }else{
//        emit sendDataMessage(tr("删除用户失败: ")+query.lastError().text());
        return false;
    }
}
//获取所有用户信息
void DataOper::getAllUsersInfo(vector<User> &users, QString userName)
{
    User  u;
    QSqlQuery query(db);
    QString sql = nullptr;
    if(userName == nullptr){
        sql = "SELECT name,password,date,isUse FROM USER ";
    }else{
        sql = QString("SELECT name,password,date,isUse FROM USER where name = '%1'").arg(userName);
    }

    bool successFlag = query.exec(sql);

    if(successFlag){
        while(query.next()){
            u.name = query.value(0).toString();
            u.password = query.value(1).toString();
            u.date = query.value(2).toString();
            u.isUse = query.value(3).toInt();
            users.push_back(u);
        }
    }else{
        qDebug() << tr("获取用户信息失败: ") + query.lastError().text();
    }
}
//获取所有报警信息
void DataOper::getAllWarningInfo(vector<Warning> &warnings, int falg,QString s_start,QString e_time)
{
    Warning  w;
    QSqlQuery query(db);
    QString sql = nullptr;
    if(falg == 1){
        sql  = "SELECT address,deviceName,info,date FROM Warning  ORDER BY date DESC ";
    }else{
        sql  = "SELECT address,deviceName,info,date FROM Warning where 1 = 1 ";
        if (s_start.compare("1") != 0)//比较大小
        {
            sql += " AND date >= '" + s_start+" 00:00:00' AND date <= '" + e_time +" 23:59:59' ";
        }
        sql += " ORDER BY date DESC";
    }

    bool successFlag = query.exec(sql);
    if(successFlag){
        while(query.next()){
            w.address = query.value(0).toString();
            w.deviceName = query.value(1).toString();
            w.info = query.value(2).toString();
            w.date = query.value(3).toString();
            warnings.push_back(w);
        }
    }else{
        qDebug() << tr("获取报警信息失败: ") + query.lastError().text();
    }
}
//获取所有日志信息
void DataOper::getAllLogInfo(vector<Log> &logs, int falg,QString s_start,QString e_time)
{
    Log  l;
    QSqlQuery query(db);
    QString sql = nullptr;
    if(falg == 1){
        sql  = "SELECT address,device,operType,userName,date FROM operlog  ORDER BY date DESC ";
    }else{
        sql  = "SELECT address,device,operType,userName,date FROM operlog where 1 = 1 ";
        if (s_start.compare("1") != 0)//比较大小
        {
            sql += " AND date >= '" + s_start+" 00:00:00' AND date <= '" + e_time +" 23:59:59' ";
        }
        sql += " ORDER BY date DESC";
    }

    bool successFlag = query.exec(sql);
    if(successFlag){
        while(query.next()){
            l.address = query.value(0).toString();
            l.device = query.value(1).toString();
            l.operType = query.value(2).toString();
            l.userName = query.value(3).toString();
            l.date = query.value(4).toString();
            logs.push_back(l);
        }
    }else{
        qDebug() << tr("获取日志信息失败: ") + query.lastError().text();
    }
}
//保存日志数据
void DataOper::saveLog(Log log)
{
    QSqlQuery query(db);
    db.transaction();
    query.prepare("INSERT INTO operlog(address,device,operType,userName,date) VALUES (?,?,?,?,?)");
    query.addBindValue(log.address);
    query.addBindValue(log.device);
    query.addBindValue(log.operType);
    query.addBindValue(log.userName);
    query.addBindValue(log.date);
    if (!query.exec()){
        qDebug() << tr("保存日志失败: ") + query.lastError().text();
    }else{
        db.commit();
    }
}
//保存日志数据
void DataOper::saveLog(QString address,QString device,QString operType,QString userName,QString date)
{
    QSqlQuery query(db);
    db.transaction();
    query.prepare("INSERT INTO operlog(address,device,operType,userName,date) VALUES (?,?,?,?,?)");
    query.addBindValue(address);
    query.addBindValue(device);
    query.addBindValue(operType);
    query.addBindValue(userName);
    query.addBindValue(date);
    if (!query.exec()){ //进行批处理，如果出错就输出错误
        qDebug() << tr("保存日志失败: ") + query.lastError().text();
    }else{
        db.commit();
    }
}
//保存空压机压力参数设置
void DataOper::saveCompressorSet(CompressorSet compressorSet)
{
    QSqlQuery query(db);
    db.transaction();
    query.prepare("INSERT INTO CompressorSet(uninstallPressure,pressureDiff,date,flag) VALUES (?,?,?,?)");
    query.addBindValue(compressorSet.uninstallPressure);
    query.addBindValue(compressorSet.pressureDiff);
    query.addBindValue(compressorSet.date);
    query.addBindValue(compressorSet.flag);
    if (!query.exec()){ //进行批处理，如果出错就输出错误
        qDebug() << tr("保存空压机压力参数设置失败: ") + query.lastError().text();
    }else{
        db.commit();
    }
}
//获取最近的一次空压机设置参数
void DataOper::getLastCompressorSet(CompressorSet &compressorSet,int compressorNo)
{
    QSqlQuery query(db);
    query.prepare("select uninstallPressure,pressureDiff,date from  CompressorSet where flag = ? order by date DESC limit 1");
    query.addBindValue(compressorNo);
    if (query.exec()){
        if(query.next()){
            compressorSet.uninstallPressure = query.value(0).toFloat();
            compressorSet.pressureDiff = query.value(1).toFloat();
            compressorSet.date = query.value(2).toString();
        }
    }else{
        qDebug() << tr("获取最近的一次空压机设置参数失败: ") + query.lastError().text();
    }
}
//存储读取的空压机数据
void DataOper::saveReadCompressor1(QVector<quint16> compressor, QString storageTime)
{
    QSqlQuery query(db);
    db.transaction();
    query.prepare("INSERT INTO Compressor1(runTimeL,runTimeH,loadTimeL,loadTimeH,electricityType,airDemand,jointControlMode,"
                  "voltageDeviation,hostCurrent,dewPointTemperature,EnvironmentalTemperature,T1,T2,P1,P2,T3,T4,P3,P4,T5,T6,"
                  "runMode1,runMode2,runMode3,dp1,pressureDiff,uninstallPressure,MaxManifoldPressure,MinManifoldPressure,"
                  "MinimalPressure,StartLoadDelayTime,StopTime,OrderTime,RotateTime,TransitionTime,date) VALUES (?,?,?,?,?,?,?,?,?,?,"
                  "?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    query.addBindValue(compressor.at(0));
    query.addBindValue(compressor.at(1));
    query.addBindValue(compressor.at(2));
    query.addBindValue(compressor.at(3));
    query.addBindValue(compressor.at(4));
    query.addBindValue(compressor.at(5));
    query.addBindValue(compressor.at(6));
    query.addBindValue(compressor.at(7));
    query.addBindValue(compressor.at(8));
    query.addBindValue(compressor.at(9));
    query.addBindValue(compressor.at(10));
    query.addBindValue(compressor.at(11));
    query.addBindValue(compressor.at(12));
    query.addBindValue(compressor.at(13));
    query.addBindValue(compressor.at(14));
    query.addBindValue(compressor.at(15));
    query.addBindValue(compressor.at(16));
    query.addBindValue(compressor.at(17));
    query.addBindValue(compressor.at(18));
    query.addBindValue(compressor.at(19));
    query.addBindValue(compressor.at(20));
    query.addBindValue(compressor.at(21));
    query.addBindValue(compressor.at(22));
    query.addBindValue(compressor.at(23));
    query.addBindValue(compressor.at(24));
    query.addBindValue(compressor.at(25));
    query.addBindValue(compressor.at(26));
    query.addBindValue(compressor.at(27));
    query.addBindValue(compressor.at(28));
    query.addBindValue(compressor.at(29));
    query.addBindValue(compressor.at(30));
    query.addBindValue(compressor.at(31));
    query.addBindValue(compressor.at(32));
    query.addBindValue(compressor.at(33));
    query.addBindValue(compressor.at(34));
    query.addBindValue(storageTime);
    if (!query.exec()){
        qDebug() << tr("保存存储读取的空压机数据失败: ") + query.lastError().text();
    }else{
        db.commit();
    }
}
//存储读取的空压机数据
void DataOper::saveReadCompressor2(QVector<quint16> compressor, QString storageTime)
{
    QSqlQuery query(db);
    db.transaction();
    query.prepare("INSERT INTO Compressor2(runTimeL,runTimeH,loadTimeL,loadTimeH,electricityType,airDemand,jointControlMode,"
                  "voltageDeviation,hostCurrent,dewPointTemperature,EnvironmentalTemperature,T1,T2,P1,P2,T3,T4,P3,P4,T5,T6,"
                  "runMode1,runMode2,runMode3,dp1,pressureDiff,uninstallPressure,MaxManifoldPressure,MinManifoldPressure,"
                  "MinimalPressure,StartLoadDelayTime,StopTime,OrderTime,RotateTime,TransitionTime,date) VALUES (?,?,?,?,?,?,?,?,?,?,"
                  "?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    query.addBindValue(compressor.at(0));
    query.addBindValue(compressor.at(1));
    query.addBindValue(compressor.at(2));
    query.addBindValue(compressor.at(3));
    query.addBindValue(compressor.at(4));
    query.addBindValue(compressor.at(5));
    query.addBindValue(compressor.at(6));
    query.addBindValue(compressor.at(7));
    query.addBindValue(compressor.at(8));
    query.addBindValue(compressor.at(9));
    query.addBindValue(compressor.at(10));
    query.addBindValue(compressor.at(11));
    query.addBindValue(compressor.at(12));
    query.addBindValue(compressor.at(13));
    query.addBindValue(compressor.at(14));
    query.addBindValue(compressor.at(15));
    query.addBindValue(compressor.at(16));
    query.addBindValue(compressor.at(17));
    query.addBindValue(compressor.at(18));
    query.addBindValue(compressor.at(19));
    query.addBindValue(compressor.at(20));
    query.addBindValue(compressor.at(21));
    query.addBindValue(compressor.at(22));
    query.addBindValue(compressor.at(23));
    query.addBindValue(compressor.at(24));
    query.addBindValue(compressor.at(25));
    query.addBindValue(compressor.at(26));
    query.addBindValue(compressor.at(27));
    query.addBindValue(compressor.at(28));
    query.addBindValue(compressor.at(29));
    query.addBindValue(compressor.at(30));
    query.addBindValue(compressor.at(31));
    query.addBindValue(compressor.at(32));
    query.addBindValue(compressor.at(33));
    query.addBindValue(compressor.at(34));
    query.addBindValue(storageTime);
    if (!query.exec()){
        qDebug() << tr("保存存储读取的空压机数据失败: ") + query.lastError().text();
    }else{
        db.commit();
    }
}
//存储读取的空压机数据
void DataOper::saveReadCompressor3(QVector<quint16> compressor, QString storageTime)
{
    QSqlQuery query(db);
    db.transaction();
    query.prepare("INSERT INTO Compressor3(runTimeL,runTimeH,loadTimeL,loadTimeH,electricityType,airDemand,jointControlMode,"
                  "voltageDeviation,hostCurrent,dewPointTemperature,EnvironmentalTemperature,T1,T2,P1,P2,T3,T4,P3,P4,T5,T6,"
                  "runMode1,runMode2,runMode3,dp1,pressureDiff,uninstallPressure,MaxManifoldPressure,MinManifoldPressure,"
                  "MinimalPressure,StartLoadDelayTime,StopTime,OrderTime,RotateTime,TransitionTime,date) VALUES (?,?,?,?,?,?,?,?,?,?,"
                  "?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    query.addBindValue(compressor.at(0));
    query.addBindValue(compressor.at(1));
    query.addBindValue(compressor.at(2));
    query.addBindValue(compressor.at(3));
    query.addBindValue(compressor.at(4));
    query.addBindValue(compressor.at(5));
    query.addBindValue(compressor.at(6));
    query.addBindValue(compressor.at(7));
    query.addBindValue(compressor.at(8));
    query.addBindValue(compressor.at(9));
    query.addBindValue(compressor.at(10));
    query.addBindValue(compressor.at(11));
    query.addBindValue(compressor.at(12));
    query.addBindValue(compressor.at(13));
    query.addBindValue(compressor.at(14));
    query.addBindValue(compressor.at(15));
    query.addBindValue(compressor.at(16));
    query.addBindValue(compressor.at(17));
    query.addBindValue(compressor.at(18));
    query.addBindValue(compressor.at(19));
    query.addBindValue(compressor.at(20));
    query.addBindValue(compressor.at(21));
    query.addBindValue(compressor.at(22));
    query.addBindValue(compressor.at(23));
    query.addBindValue(compressor.at(24));
    query.addBindValue(compressor.at(25));
    query.addBindValue(compressor.at(26));
    query.addBindValue(compressor.at(27));
    query.addBindValue(compressor.at(28));
    query.addBindValue(compressor.at(29));
    query.addBindValue(compressor.at(30));
    query.addBindValue(compressor.at(31));
    query.addBindValue(compressor.at(32));
    query.addBindValue(compressor.at(33));
    query.addBindValue(compressor.at(34));
    query.addBindValue(storageTime);
    if (!query.exec()){
        qDebug() << tr("保存存储读取的空压机数据失败: ") + query.lastError().text();
    }else{
        db.commit();
    }
}
//存储读取的空压机数据
void DataOper::saveReadDryer1(QVector<quint16> dryer, QString storageTime)
{
    QSqlQuery query(db);
    db.transaction();
    query.prepare("INSERT INTO dryer1(runHint,faultHint,compressor,drainer,phaseOrderFault,overloadSave,sysHighVoltage,"
                  "sysLowVoltage,dewPointProbeFault,dewPointH,dewPointL,faultWarn,faultStop,countDown,dewPointT,runTimeH,runTimeM,date)"
                  " VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    query.addBindValue(dryer.at(0));
    query.addBindValue(dryer.at(1));
    query.addBindValue(dryer.at(2));
    query.addBindValue(dryer.at(3));
    query.addBindValue(dryer.at(4));
    query.addBindValue(dryer.at(5));
    query.addBindValue(dryer.at(6));
    query.addBindValue(dryer.at(7));
    query.addBindValue(dryer.at(8));
    query.addBindValue(dryer.at(9));
    query.addBindValue(dryer.at(10));
    query.addBindValue(dryer.at(11));
    query.addBindValue(dryer.at(12));
    query.addBindValue(dryer.at(13));
    query.addBindValue(dryer.at(14));
    query.addBindValue(dryer.at(15));
    query.addBindValue(dryer.at(16));
    query.addBindValue(storageTime);
    if (!query.exec()){
        qDebug() << tr("保存存储读取的1#冷干机数据失败: ") + query.lastError().text();
    }else{
        db.commit();
    }
}
//存储读取的空压机数据
void DataOper::saveReadDryer2(QVector<quint16> dryer, QString storageTime)
{
    QSqlQuery query(db);
    db.transaction();
    query.prepare("INSERT INTO dryer2(runHint,faultHint,compressor,drainer,phaseOrderFault,overloadSave,sysHighVoltage,"
                  "sysLowVoltage,dewPointProbeFault,dewPointH,dewPointL,faultWarn,faultStop,countDown,dewPointT,runTimeH,runTimeM,date)"
                  " VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    query.addBindValue(dryer.at(0));
    query.addBindValue(dryer.at(1));
    query.addBindValue(dryer.at(2));
    query.addBindValue(dryer.at(3));
    query.addBindValue(dryer.at(4));
    query.addBindValue(dryer.at(5));
    query.addBindValue(dryer.at(6));
    query.addBindValue(dryer.at(7));
    query.addBindValue(dryer.at(8));
    query.addBindValue(dryer.at(9));
    query.addBindValue(dryer.at(10));
    query.addBindValue(dryer.at(11));
    query.addBindValue(dryer.at(12));
    query.addBindValue(dryer.at(13));
    query.addBindValue(dryer.at(14));
    query.addBindValue(dryer.at(15));
    query.addBindValue(dryer.at(16));
    query.addBindValue(storageTime);
    if (!query.exec()){
        qDebug() << tr("保存存储读取的2#冷干机数据失败: ") + query.lastError().text();
    }else{
        db.commit();
    }
}
//存储读取的空压机数据
void DataOper::saveReadDryer3(QVector<quint16> dryer, QString storageTime)
{
    QSqlQuery query(db);
    db.transaction();
    query.prepare("INSERT INTO dryer3(runHint,faultHint,compressor,drainer,phaseOrderFault,overloadSave,sysHighVoltage,"
                  "sysLowVoltage,dewPointProbeFault,dewPointH,dewPointL,faultWarn,faultStop,countDown,dewPointT,runTimeH,runTimeM,date)"
                  " VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    query.addBindValue(dryer.at(0));
    query.addBindValue(dryer.at(1));
    query.addBindValue(dryer.at(2));
    query.addBindValue(dryer.at(3));
    query.addBindValue(dryer.at(4));
    query.addBindValue(dryer.at(5));
    query.addBindValue(dryer.at(6));
    query.addBindValue(dryer.at(7));
    query.addBindValue(dryer.at(8));
    query.addBindValue(dryer.at(9));
    query.addBindValue(dryer.at(10));
    query.addBindValue(dryer.at(11));
    query.addBindValue(dryer.at(12));
    query.addBindValue(dryer.at(13));
    query.addBindValue(dryer.at(14));
    query.addBindValue(dryer.at(15));
    query.addBindValue(dryer.at(16));
    query.addBindValue(storageTime);
    if (!query.exec()){
        qDebug() << tr("保存存储读取的3#冷干机数据失败: ") + query.lastError().text();
    }else{
        db.commit();
    }
}
//保存报警信息
void DataOper::saveWarningInfo(Warning warning)
{
    QSqlQuery query(db);
    db.transaction();
    query.prepare("INSERT INTO warning(address,DeviceName,Info,Date) VALUES (?,?,?,?)");
    query.addBindValue(warning.address);
    query.addBindValue(warning.deviceName);
    query.addBindValue(warning.info);
    query.addBindValue(warning.date);
    if (!query.exec()){
        qDebug() << tr("保存空压机报警信息失败: ") + query.lastError().text();
    }else{
        db.commit();
    }
}
//根据起止时间查询加载压差和卸载压力
void DataOper::getCompressorInfo(vector<Compressor> &compressors, int CompressorNo, QString s_start, QString e_time)
{
    Compressor  compressor;
    QSqlQuery query(db);
    QString sql = nullptr;
    if(CompressorNo == 1){
        sql  = "SELECT pressureDiff,uninstallPressure,P2,date FROM Compressor1 where 1 = 1 ";
        if (s_start.compare("1") != 0)//比较大小
        {
            sql += " AND date >= '" + s_start+" 00:00:00' AND date <= '" + e_time +" 23:59:59' ";
        }
        sql += " ORDER BY date DESC";
    }else if(CompressorNo == 2){
        sql  = "SELECT pressureDiff,uninstallPressure,P2,date FROM Compressor2 where 1 = 1 ";
        if (s_start.compare("1") != 0)//比较大小
        {
            sql += " AND date >= '" + s_start+" 00:00:00' AND date <= '" + e_time +" 23:59:59' ";
        }
        sql += " ORDER BY date DESC";
    }else if(CompressorNo == 3){
        sql  = "SELECT pressureDiff,uninstallPressure,P2,date FROM Compressor3 where 1 = 1 ";
        if (s_start.compare("1") != 0)//比较大小
        {
            sql += " AND date >= '" + s_start+" 00:00:00' AND date <= '" + e_time +" 23:59:59' ";
        }
        sql += " ORDER BY date DESC";
    }else{
        return;
    }
    bool successFlag = query.exec(sql);
    if(successFlag){
        while(query.next()){
            compressor.pressureDiff = QString::number(query.value(0).toDouble()/145.0,'f',2).toDouble();
            compressor.uninstallPressure = QString::number(query.value(1).toDouble()/145.0,'f',2).toDouble();
            compressor.P2 = QString::number(query.value(2).toDouble()/145.0,'f',2).toDouble();
            compressor.date = query.value(3).toDateTime();
            compressors.push_back(compressor);
        }
    }else{
        qDebug() << tr("查询空压机曲线加载数据失败: ") + query.lastError().text();
    }
}
void DataOper::getDryerInfo(vector<Dryer> &dryers, int DryerNo, QString s_start, QString e_time)
{
    Dryer  dryer;
    QSqlQuery query(db);
    QString sql = nullptr;
    if(DryerNo == 1){
        sql  = "SELECT dewPointT,date FROM dryer1 where 1 = 1 ";
        if (s_start.compare("1") != 0)//比较大小
        {
            sql += " AND date >= '" + s_start+" 00:00:00' AND date <= '" + e_time +" 23:59:59' ";
        }
        sql += " ORDER BY date DESC";
    }else if(DryerNo == 2){
        sql  = "SELECT dewPointT,date FROM dryer2 where 1 = 1 ";
        if (s_start.compare("1") != 0)//比较大小
        {
            sql += " AND date >= '" + s_start+" 00:00:00' AND date <= '" + e_time +" 23:59:59' ";
        }
        sql += " ORDER BY date DESC";
    }else if(DryerNo == 3){
        sql  = "SELECT dewPointT,date FROM dryer3 where 1 = 1 ";
        if (s_start.compare("1") != 0)//比较大小
        {
            sql += " AND date >= '" + s_start+" 00:00:00' AND date <= '" + e_time +" 23:59:59' ";
        }
        sql += " ORDER BY date DESC";
    }else{
        return;
    }
    bool successFlag = query.exec(sql);
    if(successFlag){
        while(query.next()){
            dryer.dewPointT = query.value(0).toDouble();
            dryer.date = query.value(1).toDateTime();
            dryers.push_back(dryer);
        }
    }else{
        qDebug() << tr("查询冷干机曲线加载数据失败: ") + query.lastError().text();
    }
}


//删除给定时间之前的采集的数据，
bool DataOper::deleteGrabDataInfo(QString tableName,QString time)
{
    QSqlQuery query(db);
    QString sql = nullptr;
    sql += QString("delete from %1 where date <= '%2 23:59:59'").arg(tableName).arg(time);
    if(query.exec(sql)){
        return true;
    }else{
        return false;
    }
}
//删除给定时间之前的日志数据，
bool DataOper::deleteLogDataInfo(QString time)
{
    QSqlQuery query(db);
    QString sql = nullptr;
    sql += QString("delete from operlog where date <= '%1 23:59:59'").arg(time);
    if(query.exec(sql)){
        return true;
    }else{
        return false;
    }
}
//删除给定时间之前的警告数据
bool DataOper::deleteWarningDataInfo(QString time)
{
    QSqlQuery query(db);
    QString sql = nullptr;
    sql += QString("delete from warning where date <= '%1 23:59:59'").arg(time);
    if(query.exec(sql)){
        return true;
    }else{
        return false;
    }
}


void DataOper::GetTotalRecordCount(QString tableName, int &totalRecordCount)
{
    QSqlQuery query(db);
    QString sql = QString("select  count(1) as count  from  %1").arg(tableName);
    if(query.exec(sql)){
        if(query.next()){
            totalRecordCount = query.value(0).toInt();
        }
    }else{
        qDebug() << "GetTotalRecordCount fail: " + query.lastError().text();
    }
}

void DataOper::RecordQuery(int limitIndex,int pageRecordCount,QString tableName, vector<Compressor> &compressors,vector<Dryer> &dryers)
{
    Compressor  compressor;
    Dryer  dryer;
    QSqlQuery query(db);
    QString sql = nullptr;
    if(tableName.contains("Compressor")){
        sql = QString("select runTimeL,runTimeH,loadTimeL,loadTimeH,electricityType,airDemand,jointControlMode,voltageDeviation,hostCurrent,"
                      "dewPointTemperature,EnvironmentalTemperature,T1,T2,P1,P2,T3,T4,P3,P4,T5,T6,runMode1,runMode2,"
                      "runMode3,dp1,pressureDiff,uninstallPressure,MaxManifoldPressure,MinManifoldPressure,MinimalPressure,"
                      "StartLoadDelayTime,StopTime,OrderTime,RotateTime,TransitionTime,date from %1 limit %2,%3")
                .arg(tableName).arg(limitIndex).arg(pageRecordCount);
    }
    if(tableName.contains("dryer")){
        sql = QString("select runHint,faultHint,compressor,drainer,phaseOrderFault,overloadSave,sysHighVoltage,sysLowVoltage,dewPointProbeFault,"
                      "dewPointH,dewPointL,faultWarn,faultStop,countDown,dewPointT,runTimeH,runTimeM,date from %1 limit %2,%3")
                .arg(tableName).arg(limitIndex).arg(pageRecordCount);
    }
    bool successFlag = query.exec(sql);
    if(successFlag){
        while(query.next()){
            if(tableName.contains("Compressor")){
                compressor.runTimeL = query.value(0).toDouble();
                compressor.runTimeH = query.value(1).toDouble();
                compressor.loadTimeL = query.value(2).toDouble();
                compressor.loadTimeH = query.value(3).toDouble();
                compressor.electricityType = query.value(4).toDouble();
                compressor.airDemand = query.value(5).toDouble();
                compressor.jointControlMode = query.value(6).toDouble();
                compressor.voltageDeviation = query.value(7).toDouble();
                compressor.hostCurrent = query.value(8).toDouble();
                compressor.dewPointTemperature = query.value(9).toDouble();
                compressor.EnvironmentalTemperature = query.value(10).toDouble();
                compressor.T1 = query.value(11).toDouble();
                compressor.T2 = query.value(12).toDouble();
                compressor.P1 = query.value(13).toDouble();
                compressor.P2 = query.value(14).toDouble();
                compressor.T3 = query.value(15).toDouble();
                compressor.T4 = query.value(16).toDouble();
                compressor.P3 = query.value(17).toDouble();
                compressor.P4 = query.value(18).toDouble();
                compressor.T5 = query.value(19).toDouble();
                compressor.T6 = query.value(20).toDouble();
                compressor.runMode1 = query.value(21).toDouble();
                compressor.runMode2 = query.value(22).toDouble();
                compressor.runMode3 = query.value(23).toDouble();
                compressor.dp1 = query.value(24).toDouble();
                compressor.pressureDiff = QString::number(query.value(25).toDouble()/145.0,'f',2).toDouble();
                compressor.uninstallPressure = QString::number(query.value(26).toDouble()/145.0,'f',2).toDouble();
                compressor.MaxManifoldPressure = query.value(27).toDouble();
                compressor.MinManifoldPressure = query.value(28).toDouble();
                compressor.MinimalPressure = query.value(29).toDouble();
                compressor.StartLoadDelayTime = query.value(30).toDouble();
                compressor.StopTime = query.value(31).toDouble();
                compressor.OrderTime = query.value(32).toDouble();
                compressor.RotateTime = query.value(33).toDouble();
                compressor.TransitionTime = query.value(34).toDouble();
                compressor.date = query.value(35).toDateTime();
                compressors.push_back(compressor);
            }
            if(tableName.contains("dryer")){
                dryer.runHint = query.value(0).toDouble();
                dryer.faultHint = query.value(1).toDouble();
                dryer.compressor = query.value(2).toDouble();
                dryer.drainer = query.value(3).toDouble();
                dryer.phaseOrderFault = query.value(4).toDouble();
                dryer.overloadSave = query.value(5).toDouble();
                dryer.sysHighVoltage = query.value(6).toDouble();
                dryer.sysLowVoltage = query.value(7).toDouble();
                dryer.dewPointProbeFault = query.value(8).toDouble();
                dryer.dewPointH = query.value(9).toDouble();
                dryer.dewPointL = query.value(10).toDouble();
                dryer.faultWarn = query.value(11).toDouble();
                dryer.faultStop = query.value(12).toDouble();
                dryer.countDown = query.value(13).toDouble();
                dryer.dewPointT = query.value(14).toDouble();
                dryer.runTimeH = query.value(15).toDouble();
                dryer.runTimeM = query.value(16).toDouble();
                dryer.date = query.value(17).toDateTime();
                dryers.push_back(dryer);
            }
        }
    }else{
        qDebug() << tr("分页查询数据失败: ") + query.lastError().text();
    }
}

void DataOper::RecordQuery_w(int limitIndex,int pageRecordCount, QString s_start,QString e_time,vector<Warning> &w)
{
    Warning  warning;
    QSqlQuery query(db);

//    QString sql = nullptr;

//    sql  = "SELECT address,deviceName,info,date FROM Warning where 1 = 1 ";
//    if (s_start.compare("1") != 0)//比较大小
//    {
//        sql += " AND date >= '" + s_start+" 00:00:00' AND date <= '" + e_time +" 23:59:59' ";
//    }
//    sql += " ORDER BY date DESC limit "+limitIndex +", "+pageRecordCount;

    QString sql = QString("SELECT address,deviceName,info,date FROM Warning where 1 = 1 AND date >= '%1 00:00:00' AND date <= '%2 23:59:59' "
                          "ORDER BY date DESC limit %3,%4").arg(s_start).arg(e_time).arg(limitIndex).arg(pageRecordCount);

    bool successFlag = query.exec(sql);
    if(successFlag){
        while(query.next()){
            warning.address = query.value(0).toString();
            warning.deviceName = query.value(1).toString();
            warning.info = query.value(2).toString();
            warning.date = query.value(3).toString();
            w.push_back(warning);
        }
    }else{
        qDebug() << tr("分页查询数据失败: ") + query.lastError().text();
    }
}

//批量定时存储采集的数据
bool DataOper::saveData(vector<Compressor> compressors1,vector<Compressor> compressors2,vector<Compressor> compressors3,
                          vector<Dryer> dryers1,vector<Dryer> dryers2,vector<Dryer> dryers3,vector<Warning> warnings)
{
    QSqlQuery query(db);
    int c1Size = compressors1.size();
    int c2Size = compressors2.size();
    int c3Size = compressors3.size();
    int d1Size = dryers1.size();
    int d2Size = dryers2.size();
    int d3Size = dryers3.size();
    int wSize = warnings.size();

    db.transaction();
    for(int i = 0; i < c1Size; i++){
        query.prepare("INSERT INTO Compressor1(runTimeL,runTimeH,loadTimeL,loadTimeH,electricityType,airDemand,jointControlMode,"
                      "voltageDeviation,hostCurrent,dewPointTemperature,EnvironmentalTemperature,T1,T2,P1,P2,T3,T4,P3,P4,T5,T6,"
                      "runMode1,runMode2,runMode3,dp1,pressureDiff,uninstallPressure,MaxManifoldPressure,MinManifoldPressure,"
                      "MinimalPressure,StartLoadDelayTime,StopTime,OrderTime,RotateTime,TransitionTime,date) VALUES (?,?,?,?,?,?,?,?,?,?,"
                      "?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
        query.addBindValue(compressors1.at(i).runTimeL);
        query.addBindValue(compressors1.at(i).runTimeH);
        query.addBindValue(compressors1.at(i).loadTimeL);
        query.addBindValue(compressors1.at(i).loadTimeH);
        query.addBindValue(compressors1.at(i).electricityType);
        query.addBindValue(compressors1.at(i).airDemand);
        query.addBindValue(compressors1.at(i).jointControlMode);
        query.addBindValue(compressors1.at(i).voltageDeviation);
        query.addBindValue(compressors1.at(i).hostCurrent);
        query.addBindValue(compressors1.at(i).dewPointTemperature);
        query.addBindValue(compressors1.at(i).EnvironmentalTemperature);
        query.addBindValue(compressors1.at(i).T1);
        query.addBindValue(compressors1.at(i).T2);
        query.addBindValue(compressors1.at(i).P1);
        query.addBindValue(compressors1.at(i).P2);
        query.addBindValue(compressors1.at(i).T3);
        query.addBindValue(compressors1.at(i).T4);
        query.addBindValue(compressors1.at(i).P3);
        query.addBindValue(compressors1.at(i).P4);
        query.addBindValue(compressors1.at(i).T5);
        query.addBindValue(compressors1.at(i).T6);
        query.addBindValue(compressors1.at(i).runMode1);
        query.addBindValue(compressors1.at(i).runMode2);
        query.addBindValue(compressors1.at(i).runMode3);
        query.addBindValue(compressors1.at(i).dp1);
        query.addBindValue(compressors1.at(i).pressureDiff);
        query.addBindValue(compressors1.at(i).uninstallPressure);
        query.addBindValue(compressors1.at(i).MaxManifoldPressure);
        query.addBindValue(compressors1.at(i).MinManifoldPressure);
        query.addBindValue(compressors1.at(i).MinimalPressure);
        query.addBindValue(compressors1.at(i).StartLoadDelayTime);
        query.addBindValue(compressors1.at(i).StopTime);
        query.addBindValue(compressors1.at(i).OrderTime);
        query.addBindValue(compressors1.at(i).RotateTime);
        query.addBindValue(compressors1.at(i).TransitionTime);
        query.addBindValue(compressors1.at(i).date.toString("yyyy-MM-dd hh:mm:ss"));
        query.exec();
//        query.finish();
    }
    for(int i = 0; i < c2Size; i++){
        query.prepare("INSERT INTO Compressor2(runTimeL,runTimeH,loadTimeL,loadTimeH,electricityType,airDemand,jointControlMode,"
                      "voltageDeviation,hostCurrent,dewPointTemperature,EnvironmentalTemperature,T1,T2,P1,P2,T3,T4,P3,P4,T5,T6,"
                      "runMode1,runMode2,runMode3,dp1,pressureDiff,uninstallPressure,MaxManifoldPressure,MinManifoldPressure,"
                      "MinimalPressure,StartLoadDelayTime,StopTime,OrderTime,RotateTime,TransitionTime,date) VALUES (?,?,?,?,?,?,?,?,?,?,"
                      "?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
        query.addBindValue(compressors2.at(i).runTimeL);
        query.addBindValue(compressors2.at(i).runTimeH);
        query.addBindValue(compressors2.at(i).loadTimeL);
        query.addBindValue(compressors2.at(i).loadTimeH);
        query.addBindValue(compressors2.at(i).electricityType);
        query.addBindValue(compressors2.at(i).airDemand);
        query.addBindValue(compressors2.at(i).jointControlMode);
        query.addBindValue(compressors2.at(i).voltageDeviation);
        query.addBindValue(compressors2.at(i).hostCurrent);
        query.addBindValue(compressors2.at(i).dewPointTemperature);
        query.addBindValue(compressors2.at(i).EnvironmentalTemperature);
        query.addBindValue(compressors2.at(i).T1);
        query.addBindValue(compressors2.at(i).T2);
        query.addBindValue(compressors2.at(i).P1);
        query.addBindValue(compressors2.at(i).P2);
        query.addBindValue(compressors2.at(i).T3);
        query.addBindValue(compressors2.at(i).T4);
        query.addBindValue(compressors2.at(i).P3);
        query.addBindValue(compressors2.at(i).P4);
        query.addBindValue(compressors2.at(i).T5);
        query.addBindValue(compressors2.at(i).T6);
        query.addBindValue(compressors2.at(i).runMode1);
        query.addBindValue(compressors2.at(i).runMode2);
        query.addBindValue(compressors2.at(i).runMode3);
        query.addBindValue(compressors2.at(i).dp1);
        query.addBindValue(compressors2.at(i).pressureDiff);
        query.addBindValue(compressors2.at(i).uninstallPressure);
        query.addBindValue(compressors2.at(i).MaxManifoldPressure);
        query.addBindValue(compressors2.at(i).MinManifoldPressure);
        query.addBindValue(compressors2.at(i).MinimalPressure);
        query.addBindValue(compressors2.at(i).StartLoadDelayTime);
        query.addBindValue(compressors2.at(i).StopTime);
        query.addBindValue(compressors2.at(i).OrderTime);
        query.addBindValue(compressors2.at(i).RotateTime);
        query.addBindValue(compressors2.at(i).TransitionTime);
        query.addBindValue(compressors2.at(i).date.toString("yyyy-MM-dd hh:mm:ss"));
        query.exec();
//        query.finish();
    }
    for(int i = 0; i < c3Size; i++){
        query.prepare("INSERT INTO Compressor3(runTimeL,runTimeH,loadTimeL,loadTimeH,electricityType,airDemand,jointControlMode,"
                      "voltageDeviation,hostCurrent,dewPointTemperature,EnvironmentalTemperature,T1,T2,P1,P2,T3,T4,P3,P4,T5,T6,"
                      "runMode1,runMode2,runMode3,dp1,pressureDiff,uninstallPressure,MaxManifoldPressure,MinManifoldPressure,"
                      "MinimalPressure,StartLoadDelayTime,StopTime,OrderTime,RotateTime,TransitionTime,date) VALUES (?,?,?,?,?,?,?,?,?,?,"
                      "?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
        query.addBindValue(compressors3.at(i).runTimeL);
        query.addBindValue(compressors3.at(i).runTimeH);
        query.addBindValue(compressors3.at(i).loadTimeL);
        query.addBindValue(compressors3.at(i).loadTimeH);
        query.addBindValue(compressors3.at(i).electricityType);
        query.addBindValue(compressors3.at(i).airDemand);
        query.addBindValue(compressors3.at(i).jointControlMode);
        query.addBindValue(compressors3.at(i).voltageDeviation);
        query.addBindValue(compressors3.at(i).hostCurrent);
        query.addBindValue(compressors3.at(i).dewPointTemperature);
        query.addBindValue(compressors3.at(i).EnvironmentalTemperature);
        query.addBindValue(compressors3.at(i).T1);
        query.addBindValue(compressors3.at(i).T2);
        query.addBindValue(compressors3.at(i).P1);
        query.addBindValue(compressors3.at(i).P2);
        query.addBindValue(compressors3.at(i).T3);
        query.addBindValue(compressors3.at(i).T4);
        query.addBindValue(compressors3.at(i).P3);
        query.addBindValue(compressors3.at(i).P4);
        query.addBindValue(compressors3.at(i).T5);
        query.addBindValue(compressors3.at(i).T6);
        query.addBindValue(compressors3.at(i).runMode1);
        query.addBindValue(compressors3.at(i).runMode2);
        query.addBindValue(compressors3.at(i).runMode3);
        query.addBindValue(compressors3.at(i).dp1);
        query.addBindValue(compressors3.at(i).pressureDiff);
        query.addBindValue(compressors3.at(i).uninstallPressure);
        query.addBindValue(compressors3.at(i).MaxManifoldPressure);
        query.addBindValue(compressors3.at(i).MinManifoldPressure);
        query.addBindValue(compressors3.at(i).MinimalPressure);
        query.addBindValue(compressors3.at(i).StartLoadDelayTime);
        query.addBindValue(compressors3.at(i).StopTime);
        query.addBindValue(compressors3.at(i).OrderTime);
        query.addBindValue(compressors3.at(i).RotateTime);
        query.addBindValue(compressors3.at(i).TransitionTime);
        query.addBindValue(compressors3.at(i).date.toString("yyyy-MM-dd hh:mm:ss"));
        query.exec();
//        query.finish();
    }
    for(int i = 0; i < d1Size; i++){
        query.prepare("INSERT INTO dryer1(runHint,faultHint,compressor,drainer,phaseOrderFault,overloadSave,sysHighVoltage,"
                      "sysLowVoltage,dewPointProbeFault,dewPointH,dewPointL,faultWarn,faultStop,countDown,dewPointT,runTimeH,runTimeM,date)"
                      " VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
        query.addBindValue(dryers1.at(i).runHint);
        query.addBindValue(dryers1.at(i).faultHint);
        query.addBindValue(dryers1.at(i).compressor);
        query.addBindValue(dryers1.at(i).drainer);
        query.addBindValue(dryers1.at(i).phaseOrderFault);
        query.addBindValue(dryers1.at(i).overloadSave);
        query.addBindValue(dryers1.at(i).sysHighVoltage);
        query.addBindValue(dryers1.at(i).sysLowVoltage);
        query.addBindValue(dryers1.at(i).dewPointProbeFault);
        query.addBindValue(dryers1.at(i).dewPointH);
        query.addBindValue(dryers1.at(i).dewPointL);
        query.addBindValue(dryers1.at(i).faultWarn);
        query.addBindValue(dryers1.at(i).faultStop);
        query.addBindValue(dryers1.at(i).countDown);
        query.addBindValue(dryers1.at(i).dewPointT);
        query.addBindValue(dryers1.at(i).runTimeH);
        query.addBindValue(dryers1.at(i).runTimeM);
        query.addBindValue(dryers1.at(i).date.toString("yyyy-MM-dd hh:mm:ss"));
        query.exec();
//        query.finish();
    }
    for(int i = 0; i < d2Size; i++){
        query.prepare("INSERT INTO dryer2(runHint,faultHint,compressor,drainer,phaseOrderFault,overloadSave,sysHighVoltage,"
                      "sysLowVoltage,dewPointProbeFault,dewPointH,dewPointL,faultWarn,faultStop,countDown,dewPointT,runTimeH,runTimeM,date)"
                      " VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
        query.addBindValue(dryers2.at(i).runHint);
        query.addBindValue(dryers2.at(i).faultHint);
        query.addBindValue(dryers2.at(i).compressor);
        query.addBindValue(dryers2.at(i).drainer);
        query.addBindValue(dryers2.at(i).phaseOrderFault);
        query.addBindValue(dryers2.at(i).overloadSave);
        query.addBindValue(dryers2.at(i).sysHighVoltage);
        query.addBindValue(dryers2.at(i).sysLowVoltage);
        query.addBindValue(dryers2.at(i).dewPointProbeFault);
        query.addBindValue(dryers2.at(i).dewPointH);
        query.addBindValue(dryers2.at(i).dewPointL);
        query.addBindValue(dryers2.at(i).faultWarn);
        query.addBindValue(dryers2.at(i).faultStop);
        query.addBindValue(dryers2.at(i).countDown);
        query.addBindValue(dryers2.at(i).dewPointT);
        query.addBindValue(dryers2.at(i).runTimeH);
        query.addBindValue(dryers2.at(i).runTimeM);
        query.addBindValue(dryers2.at(i).date.toString("yyyy-MM-dd hh:mm:ss"));
        query.exec();
//        query.finish();
    }
    for(int i = 0; i < d3Size; i++){
        query.prepare("INSERT INTO dryer3(runHint,faultHint,compressor,drainer,phaseOrderFault,overloadSave,sysHighVoltage,"
                      "sysLowVoltage,dewPointProbeFault,dewPointH,dewPointL,faultWarn,faultStop,countDown,dewPointT,runTimeH,runTimeM,date)"
                      " VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
        query.addBindValue(dryers3.at(i).runHint);
        query.addBindValue(dryers3.at(i).faultHint);
        query.addBindValue(dryers3.at(i).compressor);
        query.addBindValue(dryers3.at(i).drainer);
        query.addBindValue(dryers3.at(i).phaseOrderFault);
        query.addBindValue(dryers3.at(i).overloadSave);
        query.addBindValue(dryers3.at(i).sysHighVoltage);
        query.addBindValue(dryers3.at(i).sysLowVoltage);
        query.addBindValue(dryers3.at(i).dewPointProbeFault);
        query.addBindValue(dryers3.at(i).dewPointH);
        query.addBindValue(dryers3.at(i).dewPointL);
        query.addBindValue(dryers3.at(i).faultWarn);
        query.addBindValue(dryers3.at(i).faultStop);
        query.addBindValue(dryers3.at(i).countDown);
        query.addBindValue(dryers3.at(i).dewPointT);
        query.addBindValue(dryers3.at(i).runTimeH);
        query.addBindValue(dryers3.at(i).runTimeM);
        query.addBindValue(dryers3.at(i).date.toString("yyyy-MM-dd hh:mm:ss"));
        query.exec();
//        query.finish();
    }
    for(int i = 0; i < wSize; i++){
        query.prepare("INSERT INTO warning(address,DeviceName,Info,Date) VALUES (?,?,?,?)");
        query.addBindValue(warnings.at(i).address);
        query.addBindValue(warnings.at(i).deviceName);
        query.addBindValue(warnings.at(i).info);
        query.addBindValue(warnings.at(i).date);
        query.exec();
//        query.finish();
    }
    if(db.commit()){
        qDebug() << tr("批量提交采集数据成功: ") + query.lastError().text();
        return true;
    }else{
        qDebug() << tr("批量提交采集数据失败: ") + query.lastError().text();
        return false;
    }
}
