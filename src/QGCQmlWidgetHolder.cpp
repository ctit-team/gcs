#include "QGCQmlWidgetHolder.h"
#include "ui_QGCQmlWidgetHolder.h"

#include "QGCApplication.h"
#include "QmlControls/AppMessages.h"
#include "QmlControls/QGCImageProvider.h"

#include <QQuickImageProvider>

QGCQmlWidgetHolder::QGCQmlWidgetHolder(QWidget *parent) :
    QGCDockWidget(QString(), nullptr, parent),
    ui(new Ui::QGCQmlWidgetHolder)
{
    ui->setupUi(this);

    layout()->setContentsMargins(0,0,0,0);

    ui->qml->setAttribute(Qt::WA_AcceptTouchEvents);
    ui->qml->rootContext()->engine()->addImportPath("qrc:/qml");
    ui->qml->rootContext()->setContextProperty("joystickManager", qgcApp()->toolbox()->joystickManager());
    ui->qml->engine()->addImageProvider(QLatin1String("QGCImages"), dynamic_cast<QQuickImageProvider *>(qgcApp()->toolbox()->imageProvider()));
    ui->qml->rootContext()->setContextProperty("controller", parent);
    ui->qml->rootContext()->setContextProperty("debugMessageModel", AppMessages::getModel());
    ui->qml->setResizeMode(QQuickWidget::SizeRootObjectToView);
    ui->qml->setSource(QUrl::fromUserInput("qrc:qml/MainWindowHybrid.qml"));
}

QGCQmlWidgetHolder::~QGCQmlWidgetHolder()
{
    delete ui;
}

QQuickItem* QGCQmlWidgetHolder::getRootObject(void)
{
    return ui->qml->rootObject();
}
