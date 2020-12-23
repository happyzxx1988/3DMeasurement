#ifndef OBJECTINFO_H
#define OBJECTINFO_H
#include <QString>
#include <QDateTime>


struct User
{
    QString name;
    QString password;
    QString date;
    int isUse;
};

struct Warning
{
    QString address;
    QString deviceName;
    QString info;
    QString date;
};

struct Log
{
    QString address;
    QString device;
    QString operType;
    QString userName;
    QString date;
};

struct CompressorSet
{
    float uninstallPressure;//卸载压力
    float pressureDiff;//加载压差
    QString date;//设置时间
    int flag;//空压机NO
};
struct Compressor
{
    double runTimeL;//累计运行时间 L
    double runTimeH;//累计运行时间 H
    double loadTimeL;//累计加载时间 L
    double loadTimeH;//累计加载时间 H
    double electricityType;//机型（电流类型）
    double airDemand;//供气量
    double jointControlMode;//联控模式
    double voltageDeviation;//电压偏差
    double hostCurrent;//主电机电流 Ic
    double dewPointTemperature;//露点温度 Td
    double EnvironmentalTemperature;//环境温度
    double T1;//T1
    double T2;//T2
    double P1;//P1
    double P2;//P2
    double T3;//T3
    double T4;//T4
    double P3;//P3
    double P4;//P4
    double T5;//T5
    double T6;//T6
    double runMode1;//运行状态 1 （注 1）
    double runMode2;//运行状态 2 （注 2）
    double runMode3;//运行状态 3 （注 3）
    double dp1;//dp1（06－3－13 >v2.49）
    double pressureDiff;//加载压差
    double uninstallPressure;//卸载压力
    double MaxManifoldPressure;//最高总管压力
    double MinManifoldPressure;//最低总管压力
    double MinimalPressure;//最低压力
    double StartLoadDelayTime;//启动加载延时时间
    double StopTime;//卸载停机时间
    double OrderTime;//顺序时间
    double RotateTime;//轮换时间
    double TransitionTime;//Y-△转换时间
    QDateTime date;//存储时间
};

struct Dryer
{
    double runHint;//运行提示
    double faultHint;//故障提示 1
    double compressor;//压缩机
    double drainer;//排水器
    double phaseOrderFault;//相序故障 2
    double overloadSave;//故障保护 3
    double sysHighVoltage;//系统高压
    double sysLowVoltage;//系统低压
    double dewPointProbeFault;//露点探头故障 4
    double dewPointH;//露点偏高
    double dewPointL;//露点偏低
    double faultWarn;//故障报警 5
    double faultStop;//故障停机 6
    double countDown;//倒计时
    double dewPointT;//露点温度
    double runTimeH;//运行计时（时）
    double runTimeM;//运行计时（分）
    QDateTime date;//存储时间
};

#endif // OBJECTINFO_H
