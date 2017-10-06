#ifndef QGCQMLWIDGETHOLDER_H
#define QGCQMLWIDGETHOLDER_H

#include "QGCDockWidget.h"
#include "AutoPilotPlugin.h"

namespace Ui {
class QGCQmlWidgetHolder;
}

class QGCQmlWidgetHolder : public QGCDockWidget
{
    Q_OBJECT

public:
    QGCQmlWidgetHolder(QWidget *parent = nullptr);
    ~QGCQmlWidgetHolder();

    /// Get Root Object
    QQuickItem* getRootObject(void);

private:
    Ui::QGCQmlWidgetHolder *ui;
};

#endif
