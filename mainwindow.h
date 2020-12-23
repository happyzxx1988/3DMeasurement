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
    void startReadData();
    void lockUiOperation();
    void unlockUiOperation();

private:
    Ui::MainWindow *ui;
    AppCore appcore;
    DataOper dataOper;
    QTimer timer;
};

#endif // MAINWINDOW_H
