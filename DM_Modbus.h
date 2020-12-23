#ifndef DM_Modbus_H
#define DM_Modbus_H

#include <QObject>
#include <QModbusDataUnit>
#include <QModbusClient>
#include <functional>
#include <QSemaphore>
#include <QVariant>
#include <QDebug>
#include <QBitArray>
#include <iostream>
#include <vector>

class DM_Modbus : public QObject
{
        Q_OBJECT
    public:
        explicit DM_Modbus(QObject *parent = nullptr);
        ~DM_Modbus();
    public://连接modbus及相关状态
        void connectDevice(QString port, int parity, int baud, int dataBits, int stopBits,int srvID, int timeout = 3000, int retries = 2);//com
        void connectDevice(QVariant ip_addr, int port, int srvID,int timeout = 3000, int retries = 3);//net
        void disconnectDevice();
        bool isConnected();
    public://modbus的相关读写操作
        void readWriteFloat32(int readAddress_,int readCount_, std::vector<float>&readValue_,int writeAddress_,
                                 std::vector<float>writeValue_,QString analyticalModel_ = "ABCD",
                                 std::function<void(void)> func = nullptr);
        void readWriteFloat32(int readAddress_, float &readValue_, int writeAddress_, float writeValue_,
                              QString analyticalModel_ = "ABCD", std::function<void(void)> func = nullptr);
        void readWriteUint16(int readAddress_,int readCount_, std::vector<unsigned short>&readValue_,
                             int writeAddress_, std::vector<unsigned short>writeValue_,
                             std::function<void(void)> func = nullptr);
        void readWriteUint16(int readAddress_, unsigned short &readValue_,
                             int writeAddress_, unsigned short writeValue_,
                             std::function<void(void)> func = nullptr);

        void writeFloat32(int address_, float value_,QString analyticalModel_ = "ABCD",std::function<void ()> func = nullptr);
        void writeFloat32(int address_, std::vector<float> value_,QString analyticalModel_ = "ABCD",std::function<void ()> func = nullptr);
        void writeUint16(int address_, std::vector<unsigned short>value_, std::function<void ()> func = nullptr);
        void writeUint16(int address_, unsigned short value_, std::function<void ()> func = nullptr);
        void writePulse(int address_, std::function<void ()> func);
        void writeBool(int address_,int offset_,bool b_,std::function<void()> func=nullptr);

        void readFloat32(int address_,float &buffer_,QString analyticalModel_ = "ABCD");
        void readFloat32(int address_,int count_,std::vector<float> &buffer_,QString analyticalModel_ = "ABCD");//起始地址必须为偶数
        void readUint16(int address_,unsigned short &buffer_);
        void readUint16(int address_,int count_,std::vector<unsigned short> &buffer_);
        bool readBool(int address_,int offset_);
        void readBool(int address_,int count_,std::vector<std::vector<bool>> &buffer_);//读出寄存器的每一位
    private:
        bool readRequest(QModbusDataUnit unit, std::function<void(QModbusDataUnit)> func);
        bool writeRequest(QModbusDataUnit unit, std::function<void(void)> func);
        bool readWriteRequest(QModbusDataUnit readUnit, QModbusDataUnit writeUnit,std::function<void(QModbusDataUnit)> func);
    signals:
        void deviceError(const QString& error);
        void deviceMessage(const QString& msg);
        void deviceConnected(int srvId);//默认就是该设备自身的serverID
        void deviceDisconnected(int srvId);

    signals://以下信号位modbus读写过程中产生的信号，并不对外发送，使用代码的时候，不需要关注
        void readWriteFloat32Signal_D();//读、写多个32位float
        void readWriteFloat32Signal_S();//读、写单个32位float
        void readWriteUint16Signal_D();//读、写单个16位的整数
        void readWriteUint16Signal_S();//读、写单个16位的整数
        void readUint16Signal_S();//读单个16位的整数
        void readUint16Signal_D();//读多个16位的整数
        void readFloat32Signal_D();//读取多个float
        void readFloat32Signal_S();//读取单个float
    protected slots:
        void onConnectionStateChanged(int state);
    protected:
        QModbusClient   *ModbusDevice;
        int              ServerID;
};
#endif // DM_Modbus_H
