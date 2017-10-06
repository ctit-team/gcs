#pragma once

#include "QGCDockWidget.h"

#include <QQuickItem>

namespace Ui {
class MainContent;
}

class MainContent : public QGCDockWidget
{
    Q_OBJECT

public:
    MainContent(QWidget *parent = nullptr);
    ~MainContent();

    /// Get Root Object
    QQuickItem *getRootObject();

private:
    Ui::MainContent *ui;
};
