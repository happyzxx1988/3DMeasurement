#pragma execution_character_set("utf-8")
#include "syssetwidget.h"
#include "ui_syssetwidget.h"
#include <QMessageBox>

SysSetWidget::SysSetWidget(float XStartPoint, float YStartPoint, float XOffset, float YOffset, AppCore *core, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysSetWidget),
    appcore(core),
    XStartPoint_(XStartPoint),
    YStartPoint_(YStartPoint),
    XOffset_(XOffset),
    YOffset_(YOffset)
{
    ui->setupUi(this);
    ui->XStartPoint->setText(QString::number(XStartPoint_,'f',3));
    ui->YStartPoint->setText(QString::number(YStartPoint_,'f',3));
    ui->XOffset->setText(QString::number(XOffset_,'f',3));
    ui->YOffset->setText(QString::number(YOffset_,'f',3));
}

SysSetWidget::~SysSetWidget()
{
    delete ui;
}

void SysSetWidget::on_sysAllSetBtn_clicked()
{
    std::vector<float> values;
    if(ui->XStartPoint->text().isEmpty()){
        QMessageBox::information(this, "提示", "输入X起点坐标","确定");
        return;
    }
    if(ui->YStartPoint->text().isEmpty()){
        QMessageBox::information(this, "提示", "输入Y起点坐标","确定");
        return;
    }
    if(ui->XOffset->text().isEmpty()){
        QMessageBox::information(this, "提示", "输入X第一条曲线偏移","确定");
        return;
    }
    if(ui->YOffset->text().isEmpty()){
        QMessageBox::information(this, "提示", "输入Y第一条曲线偏移","确定");
        return;
    }
    values.push_back(ui->XStartPoint->text().toFloat());
    values.push_back(ui->YStartPoint->text().toFloat());
    values.push_back(ui->XOffset->text().toFloat());
    values.push_back(ui->YOffset->text().toFloat());
    appcore->writeFloat32(35,values);
}
