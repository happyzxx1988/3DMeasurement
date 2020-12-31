#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>
#include <QSettings>
#include <QTimer>
#include "DM_Modbus.h"
#include <QVariant>

class AppCore : public QObject
{
    Q_OBJECT
public:
    explicit AppCore(QObject *parent = nullptr);

signals:
    void deviceDisconnected502();
    void deviceConnected502();
    void deviceError502(const QString& error);
    void deviceDisconnected503();
    void deviceConnected503();
    void deviceError503(const QString& error);
    void deviceDisconnected504();
    void deviceConnected504();
    void deviceError504(const QString& error);
    void deviceDisconnected505();
    void deviceConnected505();
    void deviceError505(const QString& error);
    void deviceDisconnected506();
    void deviceConnected506();
    void deviceError506(const QString& error);
    void infoMessage(const QString& info);

public slots:

public:
    QSettings*          appSettings()       { return settings; }
    bool                deviceIsConnected502();
    void                disconnectDevice502();
    bool                deviceIsConnected503();
    void                disconnectDevice503();
    bool                deviceIsConnected504();
    void                disconnectDevice504();
    bool                deviceIsConnected505();
    void                disconnectDevice505();
    bool                deviceIsConnected506();
    void                disconnectDevice506();
    QSettings*          initSettings();
    void                initDevice();
    void connectPLC502();
    void connectPLC503();
    void connectPLC504();
    void connectPLC505();
    void connectPLC506();

    void readUint16(int address_, int count_, std::vector<unsigned short> &buffer_);

    void readFloat32(int address_, float &buffer_);
    void startReadData506(int address_, unsigned short &buffer_);

    void readFloat32(int plcFlag, int address_, int count_, std::vector<float> &buffer_);//起始地址必须为偶数

    void writeBool(int address, int offset, bool val);
    void writeFloat32(int address, float value);

    void sleep(unsigned int msec);
private:
    QSettings*          settings;
    DM_Modbus           dc502;
    DM_Modbus           dc503;
    DM_Modbus           dc504;
    DM_Modbus           dc505;
    DM_Modbus           dc506;

    QVariant ip502;
    int port502;
    int timeout502;
    int retries502;
    int serverId502;

    QVariant ip503;
    int port503;
    int timeout503;
    int retries503;
    int serverId503;

    QVariant ip504;
    int port504;
    int timeout504;
    int retries504;
    int serverId504;

    QVariant ip505;
    int port505;
    int timeout505;
    int retries505;
    int serverId505;

    QVariant ip506;
    int port506;
    int timeout506;
    int retries506;
    int serverId506;


};

#endif // APPCORE_H
