#include "batterystatus.h"

#include <QFile>
#include <QTextStream>

BatteryStatus::BatteryStatus(QWidget *parent) :
    QProgressBar(parent)
{
    // Setup UI.
    setFixedWidth(100);
    setMaximum(100);
    setMinimum(0);
    setValue(0);

    // Get current status.
    updateBatteryStatus();

    // Polling.
    connect(&poller, SIGNAL(timeout()), SLOT(updateBatteryStatus()));
    poller.start(5000);
}

void BatteryStatus::updateBatteryStatus()
{
    QFile file("/sys/class/power_supply/BAT0/capacity");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream stream(&file);
    setValue(stream.readLine().toInt());
}
