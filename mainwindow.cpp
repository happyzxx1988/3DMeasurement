#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("非接触式三维测量平台");
    this->init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    appcore.initSettings();
    appcore.initDevice();
    connect(&timer,&QTimer::timeout,this,&MainWindow::startReadData);

    connect(&dataOper, &DataOper::sendDataMessage, this, [this](const QString& info){
        QMessageBox::information(this, tr("提示"), info,tr("确定"));
    });
    connect(&appcore, &AppCore::deviceConnected, this, [this](){
        qDebug() << "emit deviceConnected()";

        ui->connectPLCBtn->setText("已连接");
        ui->connectPLCBtn->setStyleSheet("background-color: rgb(85, 255, 0);");
        ui->connectPLCBtn->setEnabled(false);

        unlockUiOperation();

        PLCIsConnected = true;

        //设备启动，设置手动模式
        appcore.setRunMode(ManualMode);
        ui->runningMode->setText("手动");
        ui->manualOper->setChecked(true);
//        getInitEquipmentStatus();//读取设备初始化状态

        if(!compressorTimer.isActive()){
            compressorTimer.start(READ_TIME);
        }

    });
    connect(&appcore, &AppCore::deviceDisconnected, this, [this](){
        qDebug() << "emit deviceDisconnected()";
        ui->connectPLCBtn->setText("已断开");
        ui->connectPLCBtn->setStyleSheet("background-color: rgb(255, 0, 0);");
        ui->connectPLCBtn->setEnabled(true);
        lockUiOperation();
        PLCIsConnected = false;
        if(compressorTimer.isActive()){
            compressorTimer.stop();
        }
    });
    connect(&appcore, &AppCore::deviceError, this, [this](const QString& error){
        qDebug() << "emit deviceError(error):" << error;
        ui->statusBar->showMessage(error);
    });
    connect(&appcore, &AppCore::infoMessage, this, [this](const QString& info){
        qDebug() << "emit infoMessage(info):" << info;
    });

}
void MainWindow::startReadData()
{

}
void MainWindow::unlockUiOperation()
{

}
void MainWindow::lockUiOperation()
{

}
