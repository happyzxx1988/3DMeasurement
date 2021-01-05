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
    void startMeasureData();
    void startReadInitData(std::vector<unsigned short> buffer);
    void startReadData();
    void lockUiOperation();
    void unlockUiOperation();
    QVector<bool> dec2BinTrans(unsigned int data);
    void int_to_float(quint16 a,quint16 b, float &buffer_,QString analyticalModel_ = "ABCD");
    void toolButtonClick();
    void pushButtonClick();
    void startDetectionBtn_clicked();
    void startDealWithMeasureData();

private:
    Ui::MainWindow *ui;
    AppCore appcore;
    DataOper dataOper;
    QTimer timer;

    bool isDetectioniing;

    std::vector<float> buffer502;
    std::vector<float> buffer503;
    std::vector<float> buffer504;
    std::vector<float> buffer505;
    std::vector<unsigned short> buffer506;

    int XCol;


};

#endif // MAINWINDOW_H
