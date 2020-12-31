#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <appcore.h>
#include <dataoper.h>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void init();
    void parameterInit();
    void initToolBtn();
    void startReadData506();
    void startReadInitData(std::vector<unsigned short> buffer);
    void lockUiOperation();
    void unlockUiOperation();
    QVector<bool> dec2BinTrans(unsigned int data);
    void int_to_float(quint16 a,quint16 b, float &buffer_,QString analyticalModel_ = "ABCD");
    void toolButtonClick();
    void pushButtonClick();
    void startDetectionBtn_clicked();

private:
    Ui::MainWindow *ui;
    AppCore appcore;
    DataOper dataOper;
    QTimer timer;
    bool isDetectioniing;
    bool plc502IsConnect;
    bool plc503IsConnect;
    bool plc504IsConnect;
    bool plc505IsConnect;
    bool plc506IsConnect;

    bool isDisposeOver506;
};

#endif // MAINWINDOW_H
