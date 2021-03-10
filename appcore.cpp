#include "appcore.h"
#include <QDir>
#include <QFileInfo>
#include <QEventLoop>
#include <QCoreApplication>
#include <QTime>

#define SETTINGS_VERSION                    "1.8"
#define DEFAULT_SETTINGS_FILENAME           "Settings.ini"
#define DEFAULT_PLUGINS_PATH                "plugins"

AppCore::AppCore(QObject *parent) : QObject(parent)
{
    settings = nullptr;
}

QSettings *AppCore::initSettings()
{
    // Initialize settings
    settings = new QSettings(DEFAULT_SETTINGS_FILENAME, QSettings::IniFormat, this);
    if (settings->value("SettingsVersion").toString() == SETTINGS_VERSION){
        return settings;
    }
    QFileInfo fi(DEFAULT_SETTINGS_FILENAME);
    if (fi.exists()){
        QDir dir;
        dir.remove(DEFAULT_SETTINGS_FILENAME);
    }
    settings->setValue("SettingsVersion", SETTINGS_VERSION);
    settings->setValue("Device502/IP", "127.0.0.1");
    settings->setValue("Device502/port", 502);
    settings->setValue("Device502/Timeout", 5000);
    settings->setValue("Device502/Retries", 3);
    settings->setValue("Device502/ServerId", 1);

    settings->setValue("Device503/IP", "127.0.0.1");
    settings->setValue("Device503/port", 503);
    settings->setValue("Device503/Timeout", 5000);
    settings->setValue("Device503/Retries", 3);
    settings->setValue("Device503/ServerId", 1);

    settings->setValue("Device504/IP", "127.0.0.1");
    settings->setValue("Device504/port", 504);
    settings->setValue("Device504/Timeout", 5000);
    settings->setValue("Device504/Retries", 3);
    settings->setValue("Device504/ServerId", 1);

    settings->setValue("Device505/IP", "127.0.0.1");
    settings->setValue("Device505/port", 505);
    settings->setValue("Device505/Timeout", 5000);
    settings->setValue("Device505/Retries", 3);
    settings->setValue("Device505/ServerId", 1);

    settings->setValue("Device506/IP", "127.0.0.1");
    settings->setValue("Device506/port", 506);
    settings->setValue("Device506/Timeout", 5000);
    settings->setValue("Device506/Retries", 3);
    settings->setValue("Device506/ServerId", 1);
    return settings;
}
void AppCore::initDevice()
{
    if (!settings) return;

    emit infoMessage(tr("Initializing device..."));

    connect(&dc502, &DM_Modbus::deviceConnected, this, [this](){
        emit deviceConnected502();
    });
    connect(&dc502, &DM_Modbus::deviceDisconnected, this, [this](){
        emit deviceDisconnected502();
    });
    connect(&dc502, &DM_Modbus::deviceError, this, [this](const QString& error){
        emit deviceError502(error);
    });

    connect(&dc503, &DM_Modbus::deviceConnected, this, [this](){
        emit deviceConnected503();
    });
    connect(&dc503, &DM_Modbus::deviceDisconnected, this, [this](){
        emit deviceDisconnected503();
    });
    connect(&dc503, &DM_Modbus::deviceError, this, [this](const QString& error){
        emit deviceError503(error);
    });

    connect(&dc504, &DM_Modbus::deviceConnected, this, [this](){
        emit deviceConnected504();
    });
    connect(&dc504, &DM_Modbus::deviceDisconnected, this, [this](){
        emit deviceDisconnected504();
    });
    connect(&dc504, &DM_Modbus::deviceError, this, [this](const QString& error){
        emit deviceError504(error);
    });

    connect(&dc505, &DM_Modbus::deviceConnected, this, [this](){
        emit deviceConnected505();
    });
    connect(&dc505, &DM_Modbus::deviceDisconnected, this, [this](){
        emit deviceDisconnected505();
    });
    connect(&dc505, &DM_Modbus::deviceError, this, [this](const QString& error){
        emit deviceError505(error);
    });

    connect(&dc506, &DM_Modbus::deviceConnected, this, [this](){
        emit deviceConnected506();
    });
    connect(&dc506, &DM_Modbus::deviceDisconnected, this, [this](){
        emit deviceDisconnected506();
    });
    connect(&dc506, &DM_Modbus::deviceError, this, [this](const QString& error){
        emit deviceError506(error);
    });

    ip502 = settings->value("Device502/IP");
    port502 = settings->value("Device502/port").toInt();
    timeout502 = settings->value("Device502/Timeout").toInt();
    retries502 = settings->value("Device502/Retries").toInt();
    serverId502 = settings->value("Device502/ServerId").toInt();

    ip503 = settings->value("Device503/IP");
    port503 = settings->value("Device503/port").toInt();
    timeout503 = settings->value("Device503/Timeout").toInt();
    retries503 = settings->value("Device503/Retries").toInt();
    serverId503 = settings->value("Device503/ServerId").toInt();

    ip504 = settings->value("Device504/IP");
    port504 = settings->value("Device504/port").toInt();
    timeout504 = settings->value("Device504/Timeout").toInt();
    retries504 = settings->value("Device504/Retries").toInt();
    serverId504 = settings->value("Device504/ServerId").toInt();

    ip505 = settings->value("Device505/IP");
    port505 = settings->value("Device505/port").toInt();
    timeout505 = settings->value("Device505/Timeout").toInt();
    retries505 = settings->value("Device505/Retries").toInt();
    serverId505 = settings->value("Device505/ServerId").toInt();

    ip506 = settings->value("Device506/IP");
    port506 = settings->value("Device506/port").toInt();
    timeout506 = settings->value("Device506/Timeout").toInt();
    retries506 = settings->value("Device506/Retries").toInt();
    serverId506 = settings->value("Device506/ServerId").toInt();

    connectPLC502();
    connectPLC503();
    connectPLC504();
    connectPLC505();
    connectPLC506();

    emit infoMessage(tr("Device initialized."));
}

void AppCore::connectPLC502()
{
    dc502.connectDevice(ip502,port502,serverId502,timeout502,retries502);
}
void AppCore::connectPLC503()
{
    dc503.connectDevice(ip503,port503,serverId503,timeout503,retries503);
}
void AppCore::connectPLC504()
{
    dc504.connectDevice(ip504,port504,serverId504,timeout504,retries504);
}
void AppCore::connectPLC505()
{
    dc505.connectDevice(ip505,port505,serverId505,timeout505,retries505);
}
void AppCore::connectPLC506()
{
    dc506.connectDevice(ip506,port506,serverId506,timeout506,retries506);
}

bool AppCore::deviceIsConnected502()
{
    return dc502.isConnected();
}
void AppCore::disconnectDevice502()
{
    dc502.disconnectDevice();
}

bool AppCore::deviceIsConnected503()
{
    return dc503.isConnected();
}
void AppCore::disconnectDevice503()
{
    dc503.disconnectDevice();
}

bool AppCore::deviceIsConnected504()
{
    return dc504.isConnected();
}
void AppCore::disconnectDevice504()
{
    dc504.disconnectDevice();
}

bool AppCore::deviceIsConnected505()
{
    return dc505.isConnected();
}
void AppCore::disconnectDevice505()
{
    dc505.disconnectDevice();
}

bool AppCore::deviceIsConnected506()
{
    return dc506.isConnected();
}
void AppCore::disconnectDevice506()
{
    dc506.disconnectDevice();
}


void AppCore::readUint16(int address_, int count_, std::vector<unsigned short> &buffer_)
{
    dc506.readUint16(address_,count_,buffer_);
}
void AppCore::readFloat32(int address_,float &buffer_)
{
//    dc.readFloat32(address_,buffer_);
}
void AppCore::readFloat32(int plcFlag,int address_,int count_,std::vector<float> &buffer_)
{
    switch (plcFlag) {
    case 502:
        dc502.readFloat32(address_,count_,buffer_);
        break;
    case 503:
        dc503.readFloat32(address_,count_,buffer_);
        break;
    case 504:
        dc504.readFloat32(address_,count_,buffer_);
        break;
    case 505:
        dc505.readFloat32(address_,count_,buffer_);
        break;
    default:
        break;
    }
}

void AppCore::startReadData506(int address_, unsigned short &buffer_)
{
    dc506.readUint16(address_,buffer_);
}

void AppCore::writeBool(int address, int offset, bool val)
{
    dc506.writeBool(address,offset,val);
}
void AppCore::writeFloat32(int address, float value)
{
    dc506.writeFloat32(address,value);
}
void AppCore::writeFloat32(int address, std::vector<float> values)
{
    dc506.writeFloat32(address,values);
}

//以毫秒为单位的延时函数
void AppCore::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
       QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}




