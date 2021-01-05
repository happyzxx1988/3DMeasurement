#include <QModbusTcpClient>
#include <QModbusRtuSerialMaster>
#include <Windows.h>
#include <QEventLoop>
#include "DM_Modbus.h"


const static QModbusDataUnit::RegisterType registerType1 = QModbusDataUnit::Coils;
const static QModbusDataUnit::RegisterType registerType2 = QModbusDataUnit::DiscreteInputs;
const static QModbusDataUnit::RegisterType registerType3 = QModbusDataUnit::InputRegisters;
const static QModbusDataUnit::RegisterType registerType4 = QModbusDataUnit::HoldingRegisters;


DM_Modbus::DM_Modbus(QObject* parent):
    ModbusDevice(nullptr),
    ServerID(0)
{
    this->setParent(parent);
}

DM_Modbus::~DM_Modbus()
{
    if(this->isConnected())
    {
        this->disconnectDevice();
    }
}

void DM_Modbus::onConnectionStateChanged(int state)
{
    if (state == QModbusDevice::UnconnectedState)
    {
        emit deviceDisconnected(ServerID);
        emit deviceMessage(tr("Device disonnected!"));
    }
    else if (state == QModbusDevice::ConnectedState)
    {
        emit deviceConnected(ServerID);
        emit deviceMessage(tr("Device connected."));
    }
}

void DM_Modbus::connectDevice(QString port, int parity, int baud, int dataBits,
                           int stopBits, int srvID, int timeout, int retries)
{
    if(!this->isConnected())
    {
        if (ModbusDevice)
        {
            ModbusDevice->disconnectDevice();
            delete ModbusDevice;
            ModbusDevice = nullptr;
        }

        ModbusDevice = new QModbusRtuSerialMaster(this);

        connect(ModbusDevice, &QModbusClient::errorOccurred, this, [this](QModbusDevice::Error) {
            emit deviceError(ModbusDevice->errorString());
        });

        if (!ModbusDevice)
        {
            emit deviceDisconnected(ServerID);
            emit deviceError(tr("Could not create connection."));
            return;
        }
        else
        {
            connect(ModbusDevice, &QModbusClient::stateChanged,
                    this, &DM_Modbus::onConnectionStateChanged);
        }

        ServerID = srvID;

        ModbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter, port);
        ModbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter, parity);
        ModbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, baud);
        ModbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, dataBits);
        ModbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, stopBits);
        ModbusDevice->setTimeout(timeout);
        ModbusDevice->setNumberOfRetries(retries);
        if (!ModbusDevice->connectDevice())
            emit deviceError(tr("Connect failed: ") + ModbusDevice->errorString());
    }
}

void DM_Modbus::connectDevice(QVariant ip_addr, int port, int srvID, int timeout, int retries)
{
    if(ModbusDevice)
    {
        ModbusDevice->disconnectDevice();
        delete ModbusDevice;
        ModbusDevice = nullptr;
    }

    ModbusDevice = new QModbusTcpClient(this);

    connect(ModbusDevice, &QModbusClient::errorOccurred,
            this, [this](QModbusDevice::Error)
    {
        emit deviceError(ModbusDevice->errorString());
    });

    if (!ModbusDevice)
    {
        emit deviceDisconnected(ServerID);
        emit deviceError(tr("Could not create connection."));
        return;
    }
    else
    {
        connect(ModbusDevice, &QModbusClient::stateChanged,
                this, &DM_Modbus::onConnectionStateChanged);
    }

    ServerID = srvID;
    ModbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, ip_addr);
    ModbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);
    ModbusDevice->setTimeout(timeout);
    ModbusDevice->setNumberOfRetries(retries);
    /*ModbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, "192.168.2.1");
    ModbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, 502);
    ModbusDevice->setTimeout(3000);
    ModbusDevice->setNumberOfRetries(2);*/
    if (!ModbusDevice->connectDevice())
        emit deviceError(tr("Connect failed: ") + ModbusDevice->errorString());
}
void DM_Modbus::disconnectDevice()
{
    if (ModbusDevice)
        ModbusDevice->disconnectDevice();

    if( ModbusDevice != nullptr)
    {
        delete ModbusDevice;
        ModbusDevice = nullptr;
    }

}

bool DM_Modbus::isConnected()
{
    return ModbusDevice?(ModbusDevice->state()==QModbusDevice::ConnectedState):false;
}

void DM_Modbus::readWriteFloat32(int readAddress_, int readCount_, std::vector<float> &readValue_,
                                 int writeAddress_, std::vector<float> writeValue_,
                                 QString analyticalModel_, std::function<void ()> func)
{
    readValue_.clear();
    //每次读写60个float，即120个寄存器
    int Rnum = readCount_ / 61;
    int Wnum =  writeValue_.size()/ 61;
    int num = Wnum > Rnum ? Wnum : Rnum;
    int k = 0;

    int writeCount_ = writeValue_.size()*2;//实际需要写的寄存器个数
    readCount_ = readCount_*2;//实际需要读的寄存器个数

    for(int i = 0; i <= num; i++)
    {
        int readCount = readCount_ >= 120 ? 120 : (readCount_ % 120);
        readCount = readCount <= 0 ? 0 : readCount;
        readCount_ = readCount_ - 120;

        int writeCount = writeCount_ >= 120 ? 120 : (writeCount_ % 120);
        writeCount = writeCount <= 0 ? 0 : writeCount;
        writeCount_ = writeCount_ - 120;

        QModbusDataUnit writeUnit(registerType4, writeAddress_+120*i, writeCount);
        for(int j = 0; j < writeCount; j = j+2)
        {
            uint pTemp= *(uint *)&writeValue_[k++];
            uint chTemp[4];//a,b,c,d
            chTemp[0]= pTemp&0xff;
            chTemp[1]= (pTemp&0xff00)>>8;
            chTemp[2]= (pTemp&0xff0000)>>16;
            chTemp[3]= (pTemp&0xff000000)>>24;

            unsigned short valueTemp1, valueTemp2;
            if(analyticalModel_ == "ABCD")
            {
                valueTemp1 = ((chTemp[3]<<8)&0xff00)|(chTemp[2]&0xff);
                valueTemp2 = ((chTemp[1]<<8)&0xff00)|(chTemp[0]&0xff);

            }
            else if(analyticalModel_ == "CDAB")
            {
                valueTemp1 = ((chTemp[1]<<8)&0xff00)|(chTemp[0]&0xff);
                valueTemp2 = ((chTemp[3]<<8)&0xff00)|(chTemp[2]&0xff);
            }
            else if(analyticalModel_ == "BADC")
            {
                valueTemp1 = ((chTemp[2]<<8)&0xff00)|(chTemp[3]&0xff);
                valueTemp2 = ((chTemp[0]<<8)&0xff00)|(chTemp[1]&0xff);
            }
            else//DCBA
            {
                valueTemp1 = ((chTemp[0]<<8)&0xff00)|(chTemp[1]&0xff);
                valueTemp2 = ((chTemp[2]<<8)&0xff00)|(chTemp[3]&0xff);
            }
            writeUnit.setValue(j, valueTemp1);
            writeUnit.setValue(j+1, valueTemp2);
        }
        QModbusDataUnit readUnit(registerType4,readAddress_+120*i, readCount);
        this->readWriteRequest(readUnit, writeUnit, [&](QModbusDataUnit readUnit)
        {
            for(uint j = 0; j < readUnit.valueCount(); j = j+2)
            {
                float fTemp;
                uint *pTemp=(uint *)&fTemp;
                unsigned int chTemp[4];//a,b,c,d
                chTemp[0]=readUnit.value(j)&0xff;
                chTemp[1]=(readUnit.value(j)>>8)&0xff;
                chTemp[2]=readUnit.value(j+1)&0xff;
                chTemp[3]=(readUnit.value(j+1)>>8)&0xff;
                //这是ABCD
                if(analyticalModel_ == "ABCD")
                {
                    *pTemp=((chTemp[1]<<24)&0xff000000)|((chTemp[0]<<16)&0xff0000)|((chTemp[3]<<8)&0xff00)|(chTemp[2]&0xff);
                }
                else if(analyticalModel_ == "CDAB")
                {
                    *pTemp=((chTemp[3]<<24)&0xff000000)|((chTemp[2]<<16)&0xff0000)|((chTemp[1]<<8)&0xff00)|(chTemp[0]&0xff);
                }
                else if(analyticalModel_ == "BADC")
                {
                   *pTemp=((chTemp[0]<<24)&0xff000000)|((chTemp[1]<<16)&0xff0000)|((chTemp[2]<<8)&0xff00)|(chTemp[3]&0xff);
                }
                else//DCBA
                {
                    *pTemp=((chTemp[2]<<24)&0xff000000)|((chTemp[3]<<16)&0xff0000)|((chTemp[0]<<8)&0xff00)|(chTemp[1]&0xff);
                }
                readValue_.push_back(fTemp);
            }

            emit readWriteFloat32Signal_D();
        });
        if (ModbusDevice)
        {
            QEventLoop loop;
            connect(this, &DM_Modbus::deviceError,&loop, &QEventLoop::quit);
            connect(this, &DM_Modbus::readWriteFloat32Signal_D,&loop, &QEventLoop::quit);
            loop.exec();
        }
    }
}

void DM_Modbus::readWriteFloat32(int readAddress_, float &readValue_,
                                 int writeAddress_, float writeValue_,
                                 QString analyticalModel_, std::function<void ()> func)
{
        QModbusDataUnit writeUnit(registerType4, writeAddress_, 2);
        uint pTemp= *(uint *)&writeValue_;
        uint chTemp[4];//a,b,c,d
        chTemp[0]= pTemp&0xff;
        chTemp[1]= (pTemp&0xff00)>>8;
        chTemp[2]= (pTemp&0xff0000)>>16;
        chTemp[3]= (pTemp&0xff000000)>>24;

        unsigned short valueTemp1, valueTemp2;
        if(analyticalModel_ == "ABCD")
        {
            valueTemp1 = ((chTemp[3]<<8)&0xff00)|(chTemp[2]&0xff);
            valueTemp2 = ((chTemp[1]<<8)&0xff00)|(chTemp[0]&0xff);
        }
        else if(analyticalModel_ == "CDAB")
        {
            valueTemp1 = ((chTemp[1]<<8)&0xff00)|(chTemp[0]&0xff);
            valueTemp2 = ((chTemp[3]<<8)&0xff00)|(chTemp[2]&0xff);
        }
        else if(analyticalModel_ == "BADC")
        {
            valueTemp1 = ((chTemp[2]<<8)&0xff00)|(chTemp[3]&0xff);
            valueTemp2 = ((chTemp[0]<<8)&0xff00)|(chTemp[1]&0xff);
        }
        else//DCBA
        {
            valueTemp1 = ((chTemp[0]<<8)&0xff00)|(chTemp[1]&0xff);
            valueTemp2 = ((chTemp[2]<<8)&0xff00)|(chTemp[3]&0xff);
        }
        writeUnit.setValue(0, valueTemp1);
        writeUnit.setValue(1, valueTemp2);

        QModbusDataUnit readUnit(registerType4,readAddress_, 2);
        this->readWriteRequest(readUnit, writeUnit, [&](QModbusDataUnit readUnit)
        {
            float fTemp;
            uint *pTemp=(uint *)&fTemp;
            unsigned int chTemp[4];//a,b,c,d
            chTemp[0]=readUnit.value(0)&0xff;
            chTemp[1]=(readUnit.value(0)>>8)&0xff;
            chTemp[2]=readUnit.value(1)&0xff;
            chTemp[3]=(readUnit.value(1)>>8)&0xff;
            if(analyticalModel_ == "ABCD")
            {
                *pTemp=((chTemp[1]<<24)&0xff000000)|((chTemp[0]<<16)&0xff0000)|((chTemp[3]<<8)&0xff00)|(chTemp[2]&0xff);
            }
            else if(analyticalModel_ == "CDAB")
            {
                *pTemp=((chTemp[3]<<24)&0xff000000)|((chTemp[2]<<16)&0xff0000)|((chTemp[1]<<8)&0xff00)|(chTemp[0]&0xff);
            }
            else if(analyticalModel_ == "BADC")
            {
               *pTemp=((chTemp[0]<<24)&0xff000000)|((chTemp[1]<<16)&0xff0000)|((chTemp[2]<<8)&0xff00)|(chTemp[3]&0xff);
            }
            else//DCBA
            {
                *pTemp=((chTemp[2]<<24)&0xff000000)|((chTemp[3]<<16)&0xff0000)|((chTemp[0]<<8)&0xff00)|(chTemp[1]&0xff);
            }
            readValue_ = fTemp;

            emit readWriteFloat32Signal_S();
        });
        if (ModbusDevice)
        {
            QEventLoop loop;
            connect(this, &DM_Modbus::deviceError,&loop, &QEventLoop::quit);
            connect(this, &DM_Modbus::readWriteFloat32Signal_S,&loop, &QEventLoop::quit);
            loop.exec();
        }
}


void DM_Modbus::readWriteUint16(int readAddress_, int readCount_, std::vector<unsigned short> &readValue_,
                                int writeAddress_, std::vector<unsigned short> writeValue_, std::function<void ()> func)
{
    readValue_.clear();
    int Rnum = readCount_ / 121;
    int writeCount_ = writeValue_.size();
    int Wnum =  writeCount_/ 121;
    int num = Wnum > Rnum ? Wnum : Rnum;
    int k = 0;
    for(int i = 0; i <= num; i++)
    {
        int readCount = readCount_ >= 120 ? 120 : (readCount_ % 120);
        readCount = readCount <= 0 ? 0 : readCount;
        readCount_ = readCount_ - 120;

        int writeCount = writeCount_ >= 120 ? 120 : (writeCount_ % 120);
        writeCount = writeCount <= 0 ? 0 : writeCount;
        writeCount_ = writeCount_ - 120;


        QModbusDataUnit writeUnit(registerType4, writeAddress_+120*i, writeCount);
        for(int j = 0; j < writeCount; j++)
        {
            writeUnit.setValue(j, writeValue_[k++]);
        }

        QModbusDataUnit readUnit(registerType4,readAddress_+120*i, readCount);


        this->readWriteRequest(readUnit, writeUnit, [&](QModbusDataUnit readUnit)
        {
            for(uint j = 0; j < readUnit.valueCount(); j++)
            {
                readValue_.push_back(readUnit.value(j));
            }
            emit readWriteUint16Signal_D();
        });
        if (ModbusDevice)
        {
            QEventLoop loop;
            connect(this, &DM_Modbus::deviceError,&loop, &QEventLoop::quit);
            connect(this, &DM_Modbus::readWriteUint16Signal_D,&loop, &QEventLoop::quit);
            loop.exec();
        }
    }
}

void DM_Modbus::readWriteUint16(int readAddress_, unsigned short &readValue_,
                                int writeAddress_, unsigned short writeValue_, std::function<void ()> func)
{
    QModbusDataUnit writeUnit(registerType4, writeAddress_, 1);
    writeUnit.setValue(0, writeValue_);

    QModbusDataUnit readUnit(registerType4,readAddress_,1);

    this->readWriteRequest(readUnit, writeUnit, [&](QModbusDataUnit readUnit)
    {
        readValue_ = readUnit.value(0);
        emit readWriteUint16Signal_S();
    });

    if (ModbusDevice)
    {
        QEventLoop loop;
        connect(this, &DM_Modbus::deviceError,&loop, &QEventLoop::quit);
        connect(this, &DM_Modbus::readWriteUint16Signal_S,&loop, &QEventLoop::quit);
        loop.exec();
    }
}
bool DM_Modbus::readRequest(QModbusDataUnit unit, std::function<void(QModbusDataUnit)> func)
{
    if (!ModbusDevice) return false;

    if (QModbusReply *reply = ModbusDevice->sendReadRequest(unit, ServerID))
    {
        if (!reply->isFinished())
        {
            connect(reply, &QModbusReply::finished, this, [this, reply, func]()
            {
                if (reply->error() == QModbusDevice::NoError)
                    if (func) func(reply->result());
                    else
                        emit deviceError(tr("Device response error: %1 (code: 0x%2)").
                                         arg(reply->errorString()).
                                         arg(reply->error(), -1, 16));

                reply->deleteLater();
            });
            connect(reply,&QModbusReply::errorOccurred,this,[reply,this]()
            {
                emit deviceError(reply->errorString());
            });
        }
        else{
            delete reply; // broadcast replies return immediately
        }
    }
    else
    {
        emit deviceError(tr("Device response error: ") + ModbusDevice->errorString());
    }
    return true;
}

bool DM_Modbus::writeRequest(QModbusDataUnit unit, std::function<void ()> func)
{
    if (!ModbusDevice) return false;
    QModbusReply *reply = ModbusDevice->sendWriteRequest(unit, ServerID);
    if (!reply)
    {
        emit deviceError(tr("Device response error:") + ModbusDevice->errorString());
        return false;
    }

    if (reply->isFinished())
    {
        // broadcast replies return immediately
        reply->deleteLater();
    }
    else
    {
        connect(reply, &QModbusReply::finished, this, [this, reply, func]()
        {
            if (reply->error() != QModbusDevice::NoError)
                emit deviceError(tr("Device response error: %1 (code: 0x%2)").
                                 arg(reply->errorString()).arg(reply->error(), -1, 16));
            else
                if (func) func();
            reply->deleteLater();
        });
        connect(reply,&QModbusReply::errorOccurred,this,[reply,this](){
            emit deviceError(reply->errorString());
        });
    }
    return true;
}

bool DM_Modbus::readWriteRequest(QModbusDataUnit readUnit, QModbusDataUnit writeUnit, std::function<void(QModbusDataUnit)> func)
{
    if (!ModbusDevice) return false;
    QModbusReply *reply = ModbusDevice->sendReadWriteRequest(readUnit, writeUnit, ServerID);
    if (!reply)
    {
        emit deviceError(tr("Device response error:") + ModbusDevice->errorString());
        return false;
    }
    if (reply->isFinished())
    {
        // broadcast replies return immediately
        reply->deleteLater();
    }
    else
    {
        connect(reply, &QModbusReply::finished, this, [this, reply, func]()
        {
            if (reply->error() != QModbusDevice::NoError)
                emit deviceError(tr("Device response error: %1 (code: 0x%2)").
                                 arg(reply->errorString()).arg(reply->error(), -1, 16));
            else
                if (func) func(reply->result());
            reply->deleteLater();
        });
        connect(reply,&QModbusReply::errorOccurred,this,[reply,this](){
            emit deviceError(reply->errorString());
        });
    }
    return true;
}

void DM_Modbus::writePulse(int address, std::function<void ()> func)
{
    QModbusDataUnit writeUnit1(registerType4, address, 1);
    writeUnit1.setValue(0, 0x100);
    writeRequest(writeUnit1, nullptr);

    QModbusDataUnit writeUnit2(registerType4, address, 1);
    writeUnit2.setValue(0, 0);
    writeRequest(writeUnit2, func);
}

void DM_Modbus::writeBool(int address_, int offset_, bool b_, std::function<void ()> func)
{
    unsigned short buffer;
    readUint16(address_,buffer);
    if(b_)
        buffer|=(1<<offset_);
    else
        buffer&=~(1<<offset_);
    writeUint16(address_,buffer);
}

void DM_Modbus::readUint16(int address_, unsigned short &buffer_)
{
    QModbusDataUnit readUnit(registerType4,address_,1);
    this->readRequest(readUnit,[&](QModbusDataUnit unit)
    {
        buffer_ =unit.value(0);
        emit readUint16Signal_S();
    });
    if (ModbusDevice)
    {
        QEventLoop loop;
        connect(this, &DM_Modbus::deviceError,&loop, &QEventLoop::quit);
        connect(this, &DM_Modbus::readUint16Signal_S,&loop, &QEventLoop::quit);
        loop.exec();
    }
}

void DM_Modbus::readUint16(int address_, int count_, std::vector<unsigned short> &buffer_)
{
    buffer_.clear();
    int num = count_ / 121;
    for(int i = 0; i <= num; i++)
    {
        int count = count_ >= 120 ? 120 : (count_ % 120);
        count_ = count_ - 120;

        QModbusDataUnit readUnit(registerType4,address_+120*i,count);
        this->readRequest(readUnit,[&](QModbusDataUnit unit)
        {
            for(uint j = 0; j < readUnit.valueCount(); j++)
            {
                buffer_.push_back(unit.value(j));
            }
            emit readUint16Signal_D();
        });
        if (ModbusDevice)
        {
            QEventLoop loop;
            connect(this, &DM_Modbus::deviceError,&loop, &QEventLoop::quit);
            connect(this, &DM_Modbus::readUint16Signal_D,&loop, &QEventLoop::quit);
            loop.exec();
        }
//        else
//        {
//            QEventLoop loop;
//            connect(this, &DM_Modbus::deviceError,&loop, &QEventLoop::quit);
//            loop.exec();
//        }
    }
}

bool DM_Modbus::readBool(int address_, int offset_)
{
    unsigned short buffer;
    readUint16(address_,buffer);
    return ((buffer>>offset_)&1)==1;
}

void DM_Modbus::readBool(int address_, int count_, std::vector<std::vector<bool> > &buffer_)
{
    std::vector<unsigned short> values;
    readUint16(address_,count_,values);

    for(int i = 0; i < values.size(); i++)
    {
        std::vector<bool> valuebool;
        for(int j = 0; j < 16; j++)
        {
            valuebool.push_back((values[i]>>j)&1);
        }
       buffer_.push_back(valuebool);
    }
}

void DM_Modbus::readFloat32(int address_, float &buffer_, QString analyticalModel_)
{
    QModbusDataUnit readUnit(registerType4,address_,2);
    this->readRequest(readUnit,[&](QModbusDataUnit unit)
    {
        uint *pTemp=(uint *)&buffer_;
        unsigned int chTemp[4];//a,b,c,d
        chTemp[0]=unit.value(0)&0xff;
        chTemp[1]=(unit.value(0)>>8)&0xff;
        chTemp[2]=unit.value(1)&0xff;
        chTemp[3]=(unit.value(1)>>8)&0xff;
        //这是ABCD
        if(analyticalModel_ =="ABCD")
        {
            *pTemp=((chTemp[1]<<24)&0xff000000)|((chTemp[0]<<16)&0xff0000)|((chTemp[3]<<8)&0xff00)|(chTemp[2]&0xff);
        }
        else if(analyticalModel_ == "CDAB")
        {
            *pTemp=((chTemp[3]<<24)&0xff000000)|((chTemp[2]<<16)&0xff0000)|((chTemp[1]<<8)&0xff00)|(chTemp[0]&0xff);
        }
        else if(analyticalModel_ == "BADC")
        {
           *pTemp=((chTemp[0]<<24)&0xff000000)|((chTemp[1]<<16)&0xff0000)|((chTemp[2]<<8)&0xff00)|(chTemp[3]&0xff);
        }
        else//DCBA
        {
            *pTemp=((chTemp[2]<<24)&0xff000000)|((chTemp[3]<<16)&0xff0000)|((chTemp[0]<<8)&0xff00)|(chTemp[1]&0xff);
        }
         emit readFloat32Signal_S();
    });
    if (ModbusDevice)
    {
        QEventLoop loop;
        connect(this, &DM_Modbus::deviceError,&loop, &QEventLoop::quit);
        connect(this, &DM_Modbus::readFloat32Signal_S,&loop, &QEventLoop::quit);
        loop.exec();
    }
}

//modbus的对象是16位的寄存器，如果要接收32位的浮点数，则每次读取2个寄存器，故实际读取的寄存数量是2*count_
void DM_Modbus::readFloat32(int address_, int count_, std::vector<float> &buffer_,QString analyticalModel_)
{
    buffer_.clear();
    count_ = count_*2;

    int num = count_ / 121;
    for(int i = 0; i <= num; i++)
    {
        int count = count_ >= 120 ? 120 : (count_ % 120);
        count_ = count_ - 120;

        QModbusDataUnit readUnit(registerType4,address_+120*i,count);
        this->readRequest(readUnit,[&](QModbusDataUnit unit)
        {
            for(uint j = 0; j < readUnit.valueCount(); j = j+2)
            {
                float fTemp;
                uint *pTemp=(uint *)&fTemp;
                unsigned int chTemp[4];//a,b,c,d
                chTemp[0]=unit.value(j)&0xff;
                chTemp[1]=(unit.value(j)>>8)&0xff;
                chTemp[2]=unit.value(j+1)&0xff;
                chTemp[3]=(unit.value(j+1)>>8)&0xff;
                //这是ABCD
                if(analyticalModel_ == "ABCD")
                {
                    *pTemp=((chTemp[1]<<24)&0xff000000)|((chTemp[0]<<16)&0xff0000)|((chTemp[3]<<8)&0xff00)|(chTemp[2]&0xff);
                }
                else if(analyticalModel_ == "CDAB")
                {
                    *pTemp=((chTemp[3]<<24)&0xff000000)|((chTemp[2]<<16)&0xff0000)|((chTemp[1]<<8)&0xff00)|(chTemp[0]&0xff);
                }
                else if(analyticalModel_ == "BADC")
                {
                   *pTemp=((chTemp[0]<<24)&0xff000000)|((chTemp[1]<<16)&0xff0000)|((chTemp[2]<<8)&0xff00)|(chTemp[3]&0xff);
                }
                else//DCBA
                {
                    *pTemp=((chTemp[2]<<24)&0xff000000)|((chTemp[3]<<16)&0xff0000)|((chTemp[0]<<8)&0xff00)|(chTemp[1]&0xff);
                }
                buffer_.push_back(fTemp);
            }
//            qDebug() << 11;
            emit readFloat32Signal_D();
        });
//        qDebug() << 22;
        if (ModbusDevice)
        {
            QEventLoop loop;
            connect(this, &DM_Modbus::deviceError,&loop, &QEventLoop::quit);
            connect(this, &DM_Modbus::readFloat32Signal_D,&loop, &QEventLoop::quit);
            loop.exec();
        }
//        qDebug() << 33;
//        else
//        {
//            QEventLoop loop;
//            connect(this, &DM_Modbus::deviceError,&loop, &QEventLoop::quit);
//            loop.exec();
//        }
    }
}

void DM_Modbus::writeUint16(int address_, unsigned short value_, std::function<void ()> func)
{
    QModbusDataUnit writeUnit(registerType4, address_, 1);
    writeUnit.setValue(0, value_);
    int i;
    for(i=0;i<10;i++)
    {
        if(writeRequest(writeUnit, func))
        {
            break;
        }
        else
        {
            Sleep(10);
            i++;
        }
    }
    if(i>= 10)
        emit this->deviceError(QString::fromLocal8Bit("未写入数据,地址：")+
                               QString::number(address_)+
                               QString::fromLocal8Bit("数值:")+
                               QString::number(value_));
}

void DM_Modbus::writeFloat32(int address_, float value_, QString analyticalModel_,std::function<void ()> func)
{
    QModbusDataUnit writeUnit(registerType4, address_, 2);

    uint pTemp= *(uint *)&value_;
    uint chTemp[4];//a,b,c,d
    chTemp[0]= pTemp&0xff;
    chTemp[1]= (pTemp&0xff00)>>8;
    chTemp[2]= (pTemp&0xff0000)>>16;
    chTemp[3]= (pTemp&0xff000000)>>24;

    unsigned short valueTemp1,valueTemp2;
    //这是ABCD
    if(analyticalModel_ == "ABCD")
    {
        valueTemp1 = ((chTemp[3]<<8)&0xff00)|(chTemp[2]&0xff);
        valueTemp2 = ((chTemp[1]<<8)&0xff00)|(chTemp[0]&0xff);
    }
    else if(analyticalModel_ == "CDAB")
    {
        valueTemp1 = ((chTemp[1]<<8)&0xff00)|(chTemp[0]&0xff);
        valueTemp2 = ((chTemp[3]<<8)&0xff00)|(chTemp[2]&0xff);
    }
    else if(analyticalModel_ == "BADC")
    {
        valueTemp1 = ((chTemp[2]<<8)&0xff00)|(chTemp[3]&0xff);
        valueTemp2 = ((chTemp[0]<<8)&0xff00)|(chTemp[1]&0xff);
    }
    else//DCBA
    {
        valueTemp1 = ((chTemp[0]<<8)&0xff00)|(chTemp[1]&0xff);
        valueTemp2 = ((chTemp[2]<<8)&0xff00)|(chTemp[3]&0xff);
    }
    writeUnit.setValue(0, valueTemp1);
    writeUnit.setValue(1, valueTemp2);
    writeRequest(writeUnit, func);
}

void DM_Modbus::writeFloat32(int address_, std::vector<float> value_, QString analyticalModel_, std::function<void ()> func)
{
    if(value_.empty())
    {
        return;
    }

    int count_ = value_.size()*2;
    int num = count_ / 121;
    int k = 0;
    for(int i = 0; i <= num; i++)
    {
        int count = count_ >= 120 ? 120 : (count_ % 120);
        count_ = count_ - 120;

        QModbusDataUnit writeUnit(registerType4, address_+120*i, count);
       for(int j = i; j < count; j = j+2)
       {
           uint pTemp= *(uint *)&value_[k++];
           uint chTemp[4];//a,b,c,d
           chTemp[0]= pTemp&0xff;
           chTemp[1]= (pTemp&0xff00)>>8;
           chTemp[2]= (pTemp&0xff0000)>>16;
           chTemp[3]= (pTemp&0xff000000)>>24;

           unsigned short valueTemp1, valueTemp2;
           //这是ABCD
           if(analyticalModel_ == "ABCD")
           {
               valueTemp1 = ((chTemp[3]<<8)&0xff00)|(chTemp[2]&0xff);
               valueTemp2 = ((chTemp[1]<<8)&0xff00)|(chTemp[0]&0xff);

           }
           else if(analyticalModel_ == "CDAB")
           {
               valueTemp1 = ((chTemp[1]<<8)&0xff00)|(chTemp[0]&0xff);
               valueTemp2 = ((chTemp[3]<<8)&0xff00)|(chTemp[2]&0xff);
           }
           else if(analyticalModel_ == "BADC")
           {
               valueTemp1 = ((chTemp[2]<<8)&0xff00)|(chTemp[3]&0xff);
               valueTemp2 = ((chTemp[0]<<8)&0xff00)|(chTemp[1]&0xff);
           }
           else//DCBA
           {
               valueTemp1 = ((chTemp[0]<<8)&0xff00)|(chTemp[1]&0xff);
               valueTemp2 = ((chTemp[2]<<8)&0xff00)|(chTemp[3]&0xff);
           }
           writeUnit.setValue(j, valueTemp1);
           writeUnit.setValue(j+1, valueTemp2);
       }
       writeRequest(writeUnit, func);
    }
}

void DM_Modbus::writeUint16(int address_, std::vector<unsigned short> value_, std::function<void ()> func)
{
    if(value_.empty()) return;

    int count_ = value_.size();
    int num = count_ / 121;
    int k = 0;
    for(int i = 0; i <= num; i++)
    {
       int count = count_ >= 120 ? 120 : (count_ % 120);
       count_ = count_ - 120;
       QModbusDataUnit writeUnit(registerType4, address_+120*i, count);
       for(int j = 0; j < count; j++)
       {
           writeUnit.setValue(j, value_[k++]);
       }
       writeRequest(writeUnit, func);
    }
}

