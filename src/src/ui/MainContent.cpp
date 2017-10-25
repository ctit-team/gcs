#include "MainContent.h"
#include "ui_MainContent.h"

#include "QGCApplication.h"
#include "QmlControls/AppMessages.h"
#include "QmlControls/QGCImageProvider.h"

#include <QQuickImageProvider>

MainContent::MainContent(QWidget *parent) :
    QGCDockWidget(QString(), nullptr, parent),
    ui(new Ui::MainContent)
{
    // Setup UI.
    ui->setupUi(this);
    layout()->setContentsMargins(0,0,0,0);

    ui->qml->setAttribute(Qt::WA_AcceptTouchEvents);
    ui->qml->rootContext()->engine()->addImportPath("qrc:/qml");
    ui->qml->rootContext()->setContextProperty("joystickManager", qgcApp()->toolbox()->joystickManager());
    ui->qml->engine()->addImageProvider(QLatin1String("QGCImages"), dynamic_cast<QQuickImageProvider *>(qgcApp()->toolbox()->imageProvider()));
    ui->qml->rootContext()->setContextProperty("controller", parent);
    ui->qml->rootContext()->setContextProperty("debugMessageModel", AppMessages::getModel());
    ui->qml->setResizeMode(QQuickWidget::SizeRootObjectToView);

    // Load QML.
    ui->qml->setSource(QUrl::fromUserInput("qrc:/qml/MainWindow.qml"));
}

MainContent::~MainContent()
{
    delete ui;
}

QQuickItem *MainContent::getRootObject()
{
    return ui->qml->rootObject();
}
