#ifndef SYSSETWIDGET_H
#define SYSSETWIDGET_H

#include <QWidget>
#include "appcore.h"

namespace Ui {
class SysSetWidget;
}

class SysSetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SysSetWidget(float XStartPoint, float YStartPoint, float XOffset, float YOffset, AppCore *core = 0, QWidget *parent = 0);
    ~SysSetWidget();

private slots:
    void on_sysAllSetBtn_clicked();

private:
    Ui::SysSetWidget *ui;
    AppCore *appcore;

    float XStartPoint_;
    float YStartPoint_;
    float XOffset_;
    float YOffset_;

};

#endif // SYSSETWIDGET_H
