#ifndef BATTERYSTATUS_H
#define BATTERYSTATUS_H

#include <QProgressBar>

class BatteryStatus : public QProgressBar
{
    Q_OBJECT

public:
    BatteryStatus(QWidget *parent = Q_NULLPTR);
};

#endif // BATTERYSTATUS_H
