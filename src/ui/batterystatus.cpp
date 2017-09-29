#include "batterystatus.h"

BatteryStatus::BatteryStatus(QWidget *parent) :
    QProgressBar(parent)
{
    setFixedWidth(100);
}
