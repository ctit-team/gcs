#ifndef BATTERYSTATUS_H
#define BATTERYSTATUS_H

#include <QProgressBar>
#include <QTimer>

class BatteryStatus : public QProgressBar
{
    Q_OBJECT

public:
    explicit BatteryStatus(QWidget *parent = nullptr);

public slots:
    void updateBatteryStatus();

private:
    QTimer poller;

private:
    Q_DISABLE_COPY(BatteryStatus)
};

#endif // BATTERYSTATUS_H
