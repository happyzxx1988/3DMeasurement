﻿#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QProcess>

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
    XCol = 1;
    isMeasureOver = true;
    isExportSuccess = false;
    isServoMoveBtn = true;

    ui->tableWidget->horizontalHeader()->setSectionResizeMode( QHeaderView::Fixed );
    ui->tableWidget->verticalHeader()->setSectionResizeMode( QHeaderView::Fixed );

    ui->lengthSetBtn->hide();
    ui->widthSetBtn->hide();
    ui->thicknessSetBtn->hide();
    ui->XIntervalSetBtn->hide();
    ui->YIntervalSetBtn->hide();
    ui->XSpeedSetBtn->hide();
    ui->YSpeedSetBtn->hide();
    ui->ZSpeedSetBtn->hide();
}
void MainWindow::init()
{
    appcore.initSettings();
    appcore.initDevice();

    lockUiOperation();

    connect(&timer,&QTimer::timeout,this,&MainWindow::startReadData);

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
        timer.start(1000);
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
//        connect(btn, &QToolButton::clicked, this, &MainWindow::toolButtonClick);
        connect(btn, &QToolButton::pressed, this, &MainWindow::toolButtonPressed);
        connect(btn, &QToolButton::released, this, &MainWindow::toolButtonReleased);
    }

    QList<QPushButton *> btns1 = this->findChildren<QPushButton *>();
    foreach (QPushButton * btn, btns1) {
        connect(btn, &QPushButton::clicked, this, &MainWindow::pushButtonClick);
        connect(btn, &QPushButton::pressed, this, &MainWindow::pushButtonPressed);
        connect(btn, &QPushButton::released, this, &MainWindow::pushButtonReleased);
    }

    QList<QLineEdit *> edits = this->findChildren<QLineEdit *>();
    foreach (QLineEdit * edit, edits) {
        connect(edit, &QLineEdit::returnPressed, this, &MainWindow::lineEditReturnPressed);
    }

}

void MainWindow::toolButtonClick()
{
    QToolButton *btn = (QToolButton *)sender();
    QString objName = btn->objectName();
    qDebug() << "toolButtonClick objName:" << objName;
    if (objName == "XLeftBtn") {
        appcore.writeBool(41, 4, true);
    } else if (objName == "XRightBtn") {
        appcore.writeBool(41, 5, true);
    } else if (objName == "YBeforeBtn") {
        appcore.writeBool(41, 6, true);
    } else if (objName == "YAfterBtn") {
        appcore.writeBool(41, 7, true);
    } else if (objName == "ZUpBtn") {
        appcore.writeBool(41, 8, true);
    } else if (objName == "ZDownBtn") {
        appcore.writeBool(41, 9, true);
    }
}
void MainWindow::toolButtonPressed()
{
    QToolButton *btn = (QToolButton *)sender();
    QString objName = btn->objectName();
    qDebug() << "toolButtonPressed objName:" << objName;
    if (objName == "XLeftBtn") {
        appcore.writeBool(41, 5, true);
    } else if (objName == "XRightBtn") {
        appcore.writeBool(41, 4, true);
    } else if (objName == "YBeforeBtn") {
        appcore.writeBool(41, 6, true);
    } else if (objName == "YAfterBtn") {
        appcore.writeBool(41, 7, true);
    } else if (objName == "ZUpBtn") {
        appcore.writeBool(41, 8, true);
    } else if (objName == "ZDownBtn") {
        appcore.writeBool(41, 9, true);
    }
}
void MainWindow::toolButtonReleased()
{
    QToolButton *btn = (QToolButton *)sender();
    QString objName = btn->objectName();
    qDebug() << "toolButtonReleased objName:" << objName;
    if (objName == "XLeftBtn") {
        appcore.writeBool(41, 5, false);
    } else if (objName == "XRightBtn") {
        appcore.writeBool(41, 4, false);
    } else if (objName == "YBeforeBtn") {
        appcore.writeBool(41, 6, false);
    } else if (objName == "YAfterBtn") {
        appcore.writeBool(41, 7, false);
    } else if (objName == "ZUpBtn") {
        appcore.writeBool(41, 8, false);
    } else if (objName == "ZDownBtn") {
        appcore.writeBool(41, 9, false);
    }
}

void MainWindow::pushButtonClick()
{
    QPushButton *btn = (QPushButton *)sender();
    QString objName = btn->objectName();
    qDebug() << "pushButtonClick objName:" << objName;

    if (objName == "alarmResetBtn") {//报警复位
    } else if (objName == "ZLocationBtn") {//Z轴定位
        appcore.writeBool(41, 10, true);
    } else if (objName == "backZeroBtn") {//回零
        appcore.writeBool(41, 1, true);
    } else if (objName == "servoMoveBtn") {//伺服使能
        if(isServoMoveBtn){
            isServoMoveBtn = false;
            ui->servoMoveBtn->setText("禁用轴");
            appcore.writeBool(41, 2, true);
        }else{
            isServoMoveBtn = true;
            ui->servoMoveBtn->setText("启用轴");
            appcore.writeBool(41, 2, false);
        }
    } else if (objName == "startDetectionBtn") {//开始检测
        startDetectionBtn_clicked();
    } else if (objName == "lengthSetBtn") {//长度设置
        if(ui->lengthSetVal->text().isEmpty()){
            QMessageBox::information(this, "提示", "输入工件长度值","确定");
        }else{
            appcore.writeFloat32(19,ui->lengthSetVal->text().toFloat());
        }
    } else if (objName == "widthSetBtn") {//宽度设置
        if(ui->widthSetVal->text().isEmpty()){
            QMessageBox::information(this, "提示", "输入工件宽度值","确定");
        }else{
            appcore.writeFloat32(21,ui->widthSetVal->text().toFloat());
        }
    } else if (objName == "thicknessSetBtn") {//厚度设置
        if(ui->thicknessSetVal->text().isEmpty()){
            QMessageBox::information(this, "提示", "输入工件厚度值","确定");
        }else{
            appcore.writeFloat32(23,ui->thicknessSetVal->text().toFloat());
        }
    } else if (objName == "XIntervalSetBtn") {//扫描间距X
        if(ui->XIntervalSetVal->text().isEmpty()){
            QMessageBox::information(this, "提示", "输入扫描间距（X）","确定");
        }else{
            appcore.writeFloat32(25,ui->XIntervalSetVal->text().toFloat());
        }
    } else if (objName == "YIntervalSetBtn") {//采样间距Y
        if(ui->YIntervalSetVal->text().isEmpty()){
            QMessageBox::information(this, "提示", "输入采样间距（Y）","确定");
        }else{
            appcore.writeFloat32(27,ui->YIntervalSetVal->text().toFloat());
        }
    } else if (objName == "XSpeedSetBtn") {//X轴速度
        if(ui->XSpeedSetVal->text().isEmpty()){
            QMessageBox::information(this, "提示", "输入X轴速度","确定");
        }else{
            appcore.writeFloat32(29,ui->XSpeedSetVal->text().toFloat());
        }
    } else if (objName == "YSpeedSetBtn") {//Y轴速度
        if(ui->YSpeedSetVal->text().isEmpty()){
            QMessageBox::information(this, "提示", "输入Y轴速度","确定");
        }else{
            appcore.writeFloat32(31,ui->YSpeedSetVal->text().toFloat());
        }
    } else if (objName == "ZSpeedSetBtn") {//Z轴速度
        if(ui->ZSpeedSetVal->text().isEmpty()){
            QMessageBox::information(this, "提示", "输入Z轴速度","确定");
        }else{
            appcore.writeFloat32(33,ui->ZSpeedSetVal->text().toFloat());
        }
    } else if (objName == "exportBtn") {//导出Excel
        if(!isDetectioniing){
            QMessageBox::information(this,"提示","正在测量，不能导出，请先停止测量","确定");
            return;
        }
        QString location = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);//桌面路径
        QString fileName;
        QFile file;

        fileName = QFileDialog::getSaveFileName(this, "导出Excel文件",location,"Files (*.xlsx)");
        if(fileName.isEmpty()){
            return;
        }
        file.setFileName(fileName);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QMessageBox::information(this,"提示","导出文件失败","确定");
            return;
        }
        //写入Excel
        xlsx.mergeCells("B1:D1", format);//合并单元格
        xlsx.write(1, 1, "测量时间:");
        xlsx.write(2, 1, "Z轴坐标:");
        xlsx.write(3, 1, "长宽厚:");
        xlsx.write(4, 1, "扫描间距(X):");
        xlsx.write(5, 1, "XYV值:");
        xlsx.write(4, 3, "采样间距(Y):");

        if(ui->tableWidget->item(0,1)==NULL|| (ui->tableWidget->item(0,1)&& ui->tableWidget->item(0,1)->text()=="")){
            xlsx.write(1,2, "");
        }else{
            xlsx.write(1,2, ui->tableWidget->item(0,1)->text());
        }
        if(ui->tableWidget->item(1,1)==NULL|| (ui->tableWidget->item(1,1)&& ui->tableWidget->item(1,1)->text()=="")){
            xlsx.write(2,2, 0);
        }else{
            xlsx.write(2,2, ui->tableWidget->item(1,1)->text().toFloat());
        }
        if(ui->tableWidget->item(2,1)==NULL|| (ui->tableWidget->item(2,1)&& ui->tableWidget->item(2,1)->text()=="")){
            xlsx.write(3,2, 0);
        }else{
            xlsx.write(3,2, ui->tableWidget->item(2,1)->text().toFloat());
        }

        if(ui->tableWidget->item(2,2)==NULL|| (ui->tableWidget->item(2,2)&& ui->tableWidget->item(2,2)->text()=="")){
            xlsx.write(3,3, 0);
        }else{
            xlsx.write(3,3, ui->tableWidget->item(2,2)->text().toFloat());
        }
        if(ui->tableWidget->item(2,3)==NULL|| (ui->tableWidget->item(2,3)&& ui->tableWidget->item(2,3)->text()=="")){
            xlsx.write(3,4, 0);
        }else{
            xlsx.write(3,4, ui->tableWidget->item(2,3)->text().toFloat());
        }
        if(ui->tableWidget->item(3,1)==NULL|| (ui->tableWidget->item(3,1)&& ui->tableWidget->item(3,1)->text()=="")){
            xlsx.write(4,2, 0);
        }else{
            xlsx.write(4,2, ui->tableWidget->item(3,1)->text().toFloat());
        }
        if(ui->tableWidget->item(3,3)==NULL|| (ui->tableWidget->item(3,3)&& ui->tableWidget->item(3,3)->text()=="")){
            xlsx.write(4,4, 0);
        }else{
            xlsx.write(4,4, ui->tableWidget->item(3,3)->text().toFloat());
        }




        isExportSuccess = xlsx.saveAs(fileName);
        if(isExportSuccess){
            QMessageBox::information(this, "导出数据成功", QString("信息已保存在：%1！").arg(fileName),"确定");
        }else{
            QMessageBox::information(this, "提示", "导出文件失败","确定");
        }
    }
}
void MainWindow::pushButtonPressed()
{
    QPushButton *btn = (QPushButton *)sender();
    QString objName = btn->objectName();
    qDebug() << "pushButtonPressed objName:" << objName;

    if (objName == "alarmResetBtn") {//报警复位
        appcore.writeBool(41, 3, true);
    } else if (objName == "ZLocationBtn") {//Z轴定位
    } else if (objName == "servoMoveBtn") {//伺服使能
    } else if (objName == "startDetectionBtn") {//开始检测
    } else if (objName == "lengthSetBtn") {//长度设置
    } else if (objName == "widthSetBtn") {//厚度设置
    } else if (objName == "thicknessSetBtn") {//宽度设置
    } else if (objName == "XIntervalSetBtn") {//扫描间距X
    } else if (objName == "YIntervalSetBtn") {//采样间距Y
    } else if (objName == "XSpeedSetBtn") {//X轴速度
    } else if (objName == "YSpeedSetBtn") {//Y轴速度
    } else if (objName == "ZSpeedSetBtn") {//Z轴速度
    } else if (objName == "exportBtn") {//导出Excel
    }
}
void MainWindow::pushButtonReleased()
{
    QPushButton *btn = (QPushButton *)sender();
    QString objName = btn->objectName();
    qDebug() << "pushButtonReleased objName:" << objName;

    if (objName == "alarmResetBtn") {//报警复位
        appcore.writeBool(41, 3, false);
    } else if (objName == "ZLocationBtn") {//Z轴定位
    } else if (objName == "servoMoveBtn") {//伺服使能
    } else if (objName == "startDetectionBtn") {//开始检测
    } else if (objName == "lengthSetBtn") {//长度设置
    } else if (objName == "widthSetBtn") {//厚度设置
    } else if (objName == "thicknessSetBtn") {//宽度设置
    } else if (objName == "XIntervalSetBtn") {//扫描间距X
    } else if (objName == "YIntervalSetBtn") {//采样间距Y
    } else if (objName == "XSpeedSetBtn") {//X轴速度
    } else if (objName == "YSpeedSetBtn") {//Y轴速度
    } else if (objName == "ZSpeedSetBtn") {//Z轴速度
    } else if (objName == "exportBtn") {//导出Excel
    }
}

void MainWindow::lineEditReturnPressed()
{
    QLineEdit *eidt = (QLineEdit *)sender();
    QString objName = eidt->objectName();
    qDebug() << "lineEditReturnPressed objName:" << objName;
    if (objName == "lengthSetVal") {
        if(ui->lengthSetVal->text().isEmpty()){
            QMessageBox::information(this, "提示", "输入工件长度值","确定");
        }else{
            appcore.writeFloat32(19,ui->lengthSetVal->text().toFloat());
        }
    } else if (objName == "widthSetVal") {
        if(ui->widthSetVal->text().isEmpty()){
            QMessageBox::information(this, "提示", "输入工件宽度值","确定");
        }else{
            appcore.writeFloat32(21,ui->widthSetVal->text().toFloat());
        }
    } else if (objName == "thicknessSetVal") {
        if(ui->thicknessSetVal->text().isEmpty()){
            QMessageBox::information(this, "提示", "输入工件厚度值","确定");
        }else{
            appcore.writeFloat32(23,ui->thicknessSetVal->text().toFloat());
        }
    } else if (objName == "XIntervalSetVal") {
        if(ui->XIntervalSetVal->text().isEmpty()){
            QMessageBox::information(this, "提示", "输入扫描间距（X）","确定");
        }else{
            appcore.writeFloat32(25,ui->XIntervalSetVal->text().toFloat());
        }
    } else if (objName == "YIntervalSetVal") {
        if(ui->YIntervalSetVal->text().isEmpty()){
            QMessageBox::information(this, "提示", "输入采样间距（Y）","确定");
        }else{
            appcore.writeFloat32(27,ui->YIntervalSetVal->text().toFloat());
        }
    } else if (objName == "XSpeedSetVal") {
        if(ui->XSpeedSetVal->text().isEmpty()){
            QMessageBox::information(this, "提示", "输入X轴速度","确定");
        }else{
            appcore.writeFloat32(29,ui->XSpeedSetVal->text().toFloat());
        }
    } else if (objName == "YSpeedSetVal") {
        if(ui->YSpeedSetVal->text().isEmpty()){
            QMessageBox::information(this, "提示", "输入Y轴速度","确定");
        }else{
            appcore.writeFloat32(31,ui->YSpeedSetVal->text().toFloat());
        }
    } else if (objName == "ZSpeedSetVal") {
        if(ui->ZSpeedSetVal->text().isEmpty()){
            QMessageBox::information(this, "提示", "输入Z轴速度","确定");
        }else{
            appcore.writeFloat32(33,ui->ZSpeedSetVal->text().toFloat());
        }
    }
}
//开始检测按钮
void MainWindow::startDetectionBtn_clicked()
{
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    if(isDetectioniing){
        isDetectioniing = false;
        ui->startDetectionBtn->setText("停止检测");
        ui->statusBar->showMessage("正在检测中");
        ui->statusBar->setStyleSheet("background-color: green;");
        appcore.writeBool(41, 0, true);
        ui->tableWidget->setSpan(0, 1, 1, 3);
        ui->tableWidget->setItem(0,1,new QTableWidgetItem(currentDateTime));//测量时间
    }else{
        isDetectioniing = true;
        ui->startDetectionBtn->setText("开始检测");
        ui->statusBar->showMessage("已停止检测");
        ui->statusBar->setStyleSheet("background-color:red;");
        appcore.writeBool(41, 0, false);
    }
}

//程序启动定时读取所有需要读取的数据
void MainWindow::startReadData()
{
    float measureCurrentVal;
    float XCurrentVal;
    float YCurrentVal;
    float ZCurrentVal;

    if(appcore.deviceIsConnected502() && appcore.deviceIsConnected503() &&
            appcore.deviceIsConnected504() && appcore.deviceIsConnected505() && appcore.deviceIsConnected506()){
        appcore.readFloat32(502,0,400,buffer502);//测量值
        appcore.readFloat32(503,0,1,buffer503);//X坐标值
        appcore.readFloat32(504,0,400,buffer504);//Y轴坐标值
        appcore.readFloat32(505,0,1,buffer505);//Z轴坐标值
        appcore.readUint16(0,9,buffer506);
        //启动显示报警信息，显示当前测量值、X、Y、Z值
        int buffer506Size = buffer506.size();
        if(buffer506Size == 9){
            for(int i = 0; i < buffer506Size; i = i+2){
                switch (i) {
                case 0:
                    int_to_float(buffer506.at(i), buffer506.at(i+1), measureCurrentVal);
                    break;
                case 2:
                    int_to_float(buffer506.at(i), buffer506.at(i+1), XCurrentVal);
                    break;
                case 4:
                    int_to_float(buffer506.at(i), buffer506.at(i+1), YCurrentVal);
                    break;
                case 6:
                    int_to_float(buffer506.at(i), buffer506.at(i+1), ZCurrentVal);
                    break;
                default:
                    break;
                }
            }
            ui->XCurrentVal->setText(QString::number(XCurrentVal,'f',2));
            ui->YCurrentVal->setText(QString::number(YCurrentVal,'f',2));
            ui->ZCurrentVal->setText(QString::number(ZCurrentVal,'f',2));
            ui->MeasureCurrentVal->setText(QString::number(measureCurrentVal,'f',2));
            /*报警信息*/
            /*解析4008号寄存器*/
            QVector<bool> intParse = dec2BinTrans(buffer506.at(8));
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
                case 7:
                    if(intParse.at(i)){
                        ui->emergencyStop->setPixmap(QPixmap(":/img/images/正常.png"));
                    }else{
                        ui->emergencyStop->setPixmap(QPixmap(":/img/images/警告.png"));
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }
    if(!isDetectioniing){//开始检测
        startMeasureData();
    }
}
//程序启动506读取一次初始参数，用于显示
void MainWindow::startReadInitData(std::vector<unsigned short> buffer)
{
    float length;
    float width;
    float thickness;
    float XInterval;
    float YInterval;
    float XSpeed;
    float YSpeed;
    float ZSpeed;
    int size = buffer.size();
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
//定时测量函数
void MainWindow::startMeasureData()
{
    int buffer506Size = buffer506.size();
    qDebug() << "buffer506Size:" << buffer506Size;
    if(buffer506Size != 9){
        return;
    }
    /*解析4008号寄存器*/
    QVector<bool> intParse = dec2BinTrans(buffer506.at(8));

    qDebug() << intParse;

    for(int i = 0; i < 16; i++){
        switch (i) {
        case 8:
            if(intParse.at(i)){
                if(isMeasureOver){
                    isMeasureOver = false;
                    startDealWithMeasureData();
                }
            }
            break;
        case 9://测量完毕，模拟点击停止测量按钮
            if(intParse.at(i)){
                startDetectionBtn_clicked();
            }
            break;
        default:
            break;
        }
    }
}
//开始处理拿到的数据
void MainWindow::startDealWithMeasureData()
{
    //给实时数据表格显示固定数据
    int size502 = buffer502.size();
    int size503 = buffer503.size();
    int size504 = buffer504.size();
    int size505 = buffer505.size();
    qDebug() << size502 << ":" << size503 << ":" << size504 << ":" << size505;

    if(size502 == 400 && size503 == 1 && size504 == 400 && size505 == 1){
        ui->tableWidget->setItem(1,1,new QTableWidgetItem(QString::number(buffer505.at(0))));//Z轴坐标
        ui->tableWidget->setItem(2,1,new QTableWidgetItem(ui->lengthSetVal->text()));//长
        ui->tableWidget->setItem(2,2,new QTableWidgetItem(ui->widthSetVal->text()));//宽
        ui->tableWidget->setItem(2,3,new QTableWidgetItem(ui->thicknessSetVal->text()));//厚
        ui->tableWidget->setItem(3,1,new QTableWidgetItem(ui->XIntervalSetVal->text()));//扫描间距(X)
        ui->tableWidget->setItem(3,3,new QTableWidgetItem(ui->YIntervalSetVal->text()));//采样间距(Y)
        ui->tableWidget->setItem(4,XCol,new QTableWidgetItem(QString::number(buffer503.at(0))));//X轴值   4行 动态列

        xlsx.write(5,XCol+1, buffer503.at(0));
        for(int i = 5; i<size504+5; i++){
            ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::number(buffer504.at(i-5))));//Y轴值   4行
            ui->tableWidget->setItem(i,XCol,new QTableWidgetItem(QString::number(buffer502.at(i-5))));//测量值   4行
            xlsx.write(i+1,1, buffer504.at(i-5));
            xlsx.write(i+1,XCol+1, buffer502.at(i-5));
        }
        //处理完一次，即采集完毕发送采集接受信号，同时移动X轴下一列
        appcore.writeBool(41, 12, true);
        XCol = XCol + 1;
        isMeasureOver = true;
    }
}


void MainWindow::unlockUiOperation()
{
    ui->backZeroBtn->setEnabled(true);
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
    /*参数设置 - 输入框*/
    ui->lengthSetVal->setEnabled(true);
    ui->widthSetVal->setEnabled(true);
    ui->thicknessSetVal->setEnabled(true);
    ui->XIntervalSetVal->setEnabled(true);
    ui->YIntervalSetVal->setEnabled(true);
    ui->XSpeedSetVal->setEnabled(true);
    ui->YSpeedSetVal->setEnabled(true);
    ui->ZSpeedSetVal->setEnabled(true);

    /**/
    ui->exportBtn->setEnabled(true);

}
void MainWindow::lockUiOperation()
{
    ui->backZeroBtn->setEnabled(false);
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
    /*参数设置 - 输入框*/
    ui->lengthSetVal->setEnabled(false);
    ui->widthSetVal->setEnabled(false);
    ui->thicknessSetVal->setEnabled(false);
    ui->XIntervalSetVal->setEnabled(false);
    ui->YIntervalSetVal->setEnabled(false);
    ui->XSpeedSetVal->setEnabled(false);
    ui->YSpeedSetVal->setEnabled(false);
    ui->ZSpeedSetVal->setEnabled(false);
    /**/
    ui->exportBtn->setEnabled(false);
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

//窗口关闭事件
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(timer.isActive()){
        timer.stop();
    }
    if(appcore.deviceIsConnected502())
        appcore.disconnectDevice502();
    if(appcore.deviceIsConnected503())
        appcore.disconnectDevice503();
    if(appcore.deviceIsConnected504())
        appcore.disconnectDevice504();
    if(appcore.deviceIsConnected505())
        appcore.disconnectDevice505();
    if(appcore.deviceIsConnected506())
        appcore.disconnectDevice506();
}

