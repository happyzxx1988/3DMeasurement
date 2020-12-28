#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    setWindowTitle("非接触式三维测量平台");
    this->init();
    this->parameterInit();
    this->initToolBtn();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::parameterInit()
{
    isDetectioniing = true;
    plc502IsConnect = false;
    plc503IsConnect = false;
    plc504IsConnect = false;
    plc505IsConnect = false;
    plc506IsConnect = false;
    isDisposeOver506 = true;
}
void MainWindow::init()
{
    appcore.initSettings();
    appcore.initDevice();

    lockUiOperation();

    connect(&timer,&QTimer::timeout,this,&MainWindow::startReadData506);

    connect(&dataOper, &DataOper::sendDataMessage, this, [this](const QString& info){
        QMessageBox::information(this, tr("提示"), info,tr("确定"));
    });
    connect(&appcore, &AppCore::infoMessage, this, [this](const QString& info){
        qDebug() << "emit infoMessage(info):" << info;
    });



    connect(&appcore, &AppCore::deviceConnected502, this, [this](){
        qDebug() << "emit deviceConnected502";
        unlockUiOperation();
    });
    connect(&appcore, &AppCore::deviceDisconnected502, this, [this](){
        qDebug() << "emit deviceDisconnected502";
        lockUiOperation();
    });
    connect(&appcore, &AppCore::deviceError502, this, [this](const QString& error){
        qDebug() << "emit deviceError502:" << error;
        ui->statusBar->showMessage(error);
    });

    connect(&appcore, &AppCore::deviceConnected503, this, [this](){
        qDebug() << "emit deviceConnected503";
        unlockUiOperation();
    });
    connect(&appcore, &AppCore::deviceDisconnected503, this, [this](){
        qDebug() << "emit deviceDisconnected503";
        lockUiOperation();
    });
    connect(&appcore, &AppCore::deviceError503, this, [this](const QString& error){
        qDebug() << "emit deviceError503:" << error;
        ui->statusBar->showMessage(error);
    });

    connect(&appcore, &AppCore::deviceConnected504, this, [this](){
        qDebug() << "emit deviceConnected504";
        unlockUiOperation();
    });
    connect(&appcore, &AppCore::deviceDisconnected504, this, [this](){
        qDebug() << "emit deviceDisconnected504";
        lockUiOperation();
    });
    connect(&appcore, &AppCore::deviceError504, this, [this](const QString& error){
        qDebug() << "emit deviceError504:" << error;
        ui->statusBar->showMessage(error);
    });

    connect(&appcore, &AppCore::deviceConnected505, this, [this](){
        qDebug() << "emit deviceConnected505";
        unlockUiOperation();
    });
    connect(&appcore, &AppCore::deviceDisconnected505, this, [this](){
        qDebug() << "emit deviceDisconnected505";
        lockUiOperation();
    });
    connect(&appcore, &AppCore::deviceError505, this, [this](const QString& error){
        qDebug() << "emit deviceError505:" << error;
        ui->statusBar->showMessage(error);
    });

    connect(&appcore, &AppCore::deviceConnected506, this, [this](){
        qDebug() << "emit deviceConnected506";
        unlockUiOperation();
        std::vector<unsigned short> buffer;
        appcore.readUint16(19,16,buffer);
        startReadInitData(buffer);
    });
    connect(&appcore, &AppCore::deviceDisconnected506, this, [this](){
        qDebug() << "emit deviceDisconnected506";
        lockUiOperation();
    });
    connect(&appcore, &AppCore::deviceError506, this, [this](const QString& error){
        qDebug() << "emit deviceError506:" << error;
        ui->statusBar->showMessage(error);
    });

}
void MainWindow::initToolBtn()
{
    QList<QToolButton *> btns = this->findChildren<QToolButton *>();
    foreach (QToolButton * btn, btns) {
        connect(btn, &QToolButton::clicked, this, &MainWindow::toolButtonClick);
    }
}

void MainWindow::toolButtonClick()
{
    QToolButton *btn = (QToolButton *)sender();
    QString objName = btn->objectName();
    qDebug() << "toolBtn objName:" << objName;
    if (objName == "XLeftBtn") {

    } else if (objName == "XRightBtn") {

    } else if (objName == "YBeforeBtn") {

    } else if (objName == "XAfterBtn") {

    } else if (objName == "ZUpBtn") {

    } else if (objName == "ZDownBtn") {

    }
}
//开始检测按钮
void MainWindow::on_startDetectionBtn_clicked()
{
    if(isDetectioniing){
        isDetectioniing = false;
        ui->startDetectionBtn->setText("停止检测");
        if(!timer.isActive()){
            timer.start(100);
        }
        ui->statusBar->showMessage("正在检测中");
        ui->statusBar->setStyleSheet("background-color: green;");
    }else{
        isDetectioniing = true;
        ui->startDetectionBtn->setText("开始检测");
        if(timer.isActive()){
            timer.stop();
        }
        ui->statusBar->showMessage("已停止检测");
        ui->statusBar->setStyleSheet("background-color:red;");
    }
}

//程序启动506读取一次初始参数，用于显示
void MainWindow::startReadInitData(std::vector<unsigned short> buffer)
{
//    工件长度设置
//    工件宽度设置
//    工件厚度设置
//    扫描间距（X）
//    采样间距(Y)
//    X轴自动速度
//    Y轴自动速度
//    Z轴自动速度
    float length;
    float width;
    float thickness;
    float XInterval;
    float YInterval;
    float XSpeed;
    float YSpeed;
    float ZSpeed;
    int size = buffer.size();
//    qDebug() << size;
    if(size != 16){
        return;
    }
    for(int i = 0; i < size; i = i+2 ){
        switch (i) {
        case 0:
            int_to_float(buffer.at(i), buffer.at(i+1), length);
            break;
        case 2:
            int_to_float(buffer.at(i), buffer.at(i+1), width);
            break;
        case 4:
            int_to_float(buffer.at(i), buffer.at(i+1), thickness);
            break;
        case 6:
            int_to_float(buffer.at(i), buffer.at(i+1), XInterval);
            break;
        case 8:
            int_to_float(buffer.at(i), buffer.at(i+1), YInterval);
            break;
        case 10:
            int_to_float(buffer.at(i), buffer.at(i+1), XSpeed);
            break;
        case 12:
            int_to_float(buffer.at(i), buffer.at(i+1), YSpeed);
            break;
        case 14:
            int_to_float(buffer.at(i), buffer.at(i+1), ZSpeed);
            break;
        default:
            break;
        }
    }
    ui->lengthSetVal->setText(QString::number(length,'f',2));
    ui->widthSetVal->setText(QString::number(width,'f',2));
    ui->thicknessSetVal->setText(QString::number(thickness,'f',2));
    ui->XIntervalSetVal->setText(QString::number(XInterval,'f',2));
    ui->YIntervalSetVal->setText(QString::number(YInterval,'f',2));
    ui->XSpeedSetVal->setText(QString::number(XSpeed,'f',2));
    ui->YSpeedSetVal->setText(QString::number(YSpeed,'f',2));
    ui->ZSpeedSetVal->setText(QString::number(ZSpeed,'f',2));
}
void MainWindow::startReadData506()
{
//    当前测量值
//    当前X坐标
//    当前Y坐标
//    当前Z坐标
    if(isDisposeOver506){

        isDisposeOver506 = false;
    std::vector<unsigned short> buffer;
    std::vector<float> buffer502;
    std::vector<float> buffer503;
    std::vector<float> buffer504;
    std::vector<float> buffer505;
    float measureCurrentVal;
    float XCurrentVal;
    float YCurrentVal;
    float ZCurrentVal;

    appcore.readUint16(0,9,buffer);
    int bufferSize = buffer.size();
    qDebug() << bufferSize;
    if(bufferSize != 9){
        return;
    }
    qDebug() << "buffer:" << buffer;
    for(int i = 0; i < bufferSize; i = i+2){
        switch (i) {
        case 0:
            int_to_float(buffer.at(i), buffer.at(i+1), measureCurrentVal);
            break;
        case 2:
            int_to_float(buffer.at(i), buffer.at(i+1), XCurrentVal);
            break;
        case 4:
            int_to_float(buffer.at(i), buffer.at(i+1), YCurrentVal);
            break;
        case 6:
            int_to_float(buffer.at(i), buffer.at(i+1), ZCurrentVal);
            break;
        default:
            break;
        }
    }
    ui->XCurrentVal->setText(QString::number(XCurrentVal,'f',2));
    ui->YCurrentVal->setText(QString::number(YCurrentVal,'f',2));
    ui->ZCurrentVal->setText(QString::number(ZCurrentVal,'f',2));
    ui->MeasureCurrentVal->setText(QString::number(measureCurrentVal,'f',2));
    /*解析4008号寄存器*/
    QVector<bool> intParse = dec2BinTrans(buffer.at(8));
    for(int i = 0; i < 16; i++){
        switch (i) {
        case 0:
            if(intParse.at(i)){
                ui->ZRNOver->setPixmap(QPixmap(":/img/images/正常.png"));
            }else{
                ui->ZRNOver->setPixmap(QPixmap(":/img/images/警告.png"));
            }
            break;
        case 1:
            if(intParse.at(i)){
                ui->XFault->setPixmap(QPixmap(":/img/images/正常.png"));
            }else{
                ui->XFault->setPixmap(QPixmap(":/img/images/警告.png"));
            }
            break;
        case 2:
            if(intParse.at(i)){
                ui->YFault->setPixmap(QPixmap(":/img/images/正常.png"));
            }else{
                ui->YFault->setPixmap(QPixmap(":/img/images/警告.png"));
            }
            break;
        case 3:
            if(intParse.at(i)){
                ui->ZFault->setPixmap(QPixmap(":/img/images/正常.png"));
            }else{
                ui->ZFault->setPixmap(QPixmap(":/img/images/警告.png"));
            }
            break;
        case 4:
            if(intParse.at(i)){
                ui->probeFault->setPixmap(QPixmap(":/img/images/正常.png"));
            }else{
                ui->probeFault->setPixmap(QPixmap(":/img/images/警告.png"));
            }
            break;
        case 5:

            break;
        case 6:

            break;
        case 7:
            if(intParse.at(i)){
                ui->parameterFault->setPixmap(QPixmap(":/img/images/正常.png"));
            }else{
                ui->parameterFault->setPixmap(QPixmap(":/img/images/警告.png"));
            }
            break;
        case 8:
//            appcore.readFloat32(502,0,1000,buffer502);
//            appcore.readFloat32(503,0,1000,buffer503);
//            appcore.readFloat32(504,0,1000,buffer504);
//            appcore.readFloat32(505,0,1000,buffer505);
            qDebug() << buffer502.size() << ":" << buffer503.size() << ":" << buffer504.size() << ":" << buffer505.size();
            break;
        case 9:

            break;
        default:
            break;
        }
    }

    isDisposeOver506 = true;
     }

}

void MainWindow::unlockUiOperation()
{
    ui->alarmResetBtn->setEnabled(true);
    ui->ZLocationBtn->setEnabled(true);
    ui->servoMoveBtn->setEnabled(true);
    ui->startDetectionBtn->setEnabled(true);
    /*运动控制*/
    ui->XLeftBtn->setEnabled(true);
    ui->XRightBtn->setEnabled(true);
    ui->YBeforeBtn->setEnabled(true);
    ui->YAfterBtn->setEnabled(true);
    ui->ZUpBtn->setEnabled(true);
    ui->ZDownBtn->setEnabled(true);
    /*参数设置*/
    ui->lengthSetBtn->setEnabled(true);
    ui->widthSetBtn->setEnabled(true);
    ui->thicknessSetBtn->setEnabled(true);
    ui->XIntervalSetBtn->setEnabled(true);
    ui->YIntervalSetBtn->setEnabled(true);
    ui->XSpeedSetBtn->setEnabled(true);
    ui->YSpeedSetBtn->setEnabled(true);
    ui->ZSpeedSetBtn->setEnabled(true);

}
void MainWindow::lockUiOperation()
{
    ui->alarmResetBtn->setEnabled(false);
    ui->ZLocationBtn->setEnabled(false);
    ui->servoMoveBtn->setEnabled(false);
    ui->startDetectionBtn->setEnabled(false);
    /*运动控制*/
    ui->XLeftBtn->setEnabled(false);
    ui->XRightBtn->setEnabled(false);
    ui->YBeforeBtn->setEnabled(false);
    ui->YAfterBtn->setEnabled(false);
    ui->ZUpBtn->setEnabled(false);
    ui->ZDownBtn->setEnabled(false);
    /*参数设置*/
    ui->lengthSetBtn->setEnabled(false);
    ui->widthSetBtn->setEnabled(false);
    ui->thicknessSetBtn->setEnabled(false);
    ui->XIntervalSetBtn->setEnabled(false);
    ui->YIntervalSetBtn->setEnabled(false);
    ui->XSpeedSetBtn->setEnabled(false);
    ui->YSpeedSetBtn->setEnabled(false);
    ui->ZSpeedSetBtn->setEnabled(false);
}


//解析无符号的整型数据
QVector<bool> MainWindow::dec2BinTrans(unsigned int data)
{
    QVector<bool> bin(16,0);
    for(int i = 0; i < 16 && data != 0; i++)
    {
        bin[i] = data%2;
        data/=2;
    }
    return bin;
}

void MainWindow::int_to_float(quint16 a,quint16 b, float &buffer_,QString analyticalModel_)
{
    uint *pTemp=(uint *)&buffer_;
    unsigned int chTemp[4];//a,b,c,d
    chTemp[0]=a&0xff;
    chTemp[1]=(a>>8)&0xff;
    chTemp[2]=b&0xff;
    chTemp[3]=(b>>8)&0xff;
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
}
