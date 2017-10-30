#include "FactSystemTestBase.h"
#include "LinkManager.h"
#ifdef QT_DEBUG
#include "MockLink.h"
#endif
#include "MultiVehicleManager.h"
#include <Application.h>
#include "ParameterManager.h"

#include <Application.h>

#include <QQuickItem>

FactSystemTestBase::FactSystemTestBase(void)
{
}

void FactSystemTestBase::_init(MAV_AUTOPILOT autopilot)
{
    UnitTest::init();

    _connectMockLink(autopilot);

    _plugin = qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->autopilotPlugin();
    Q_ASSERT(_plugin);
}

void FactSystemTestBase::_cleanup(void)
{
    UnitTest::cleanup();
}

/// Basic test of parameter values in Fact System
void FactSystemTestBase::_parameter_default_component_id_test(void)
{
    QVERIFY(_vehicle->parameterManager()->parameterExists(FactSystem::defaultComponentId, "RC_MAP_THROTTLE"));
    Fact* fact = _vehicle->parameterManager()->getParameter(FactSystem::defaultComponentId, "RC_MAP_THROTTLE");
    QVERIFY(fact != NULL);
    QVariant factValue = fact->rawValue();
    QCOMPARE(factValue.isValid(), true);

    QCOMPARE(factValue.toInt(), 3);
}

void FactSystemTestBase::_parameter_specific_component_id_test(void)
{
    QVERIFY(_vehicle->parameterManager()->parameterExists(MAV_COMP_ID_AUTOPILOT1, "RC_MAP_THROTTLE"));
    Fact* fact = _vehicle->parameterManager()->getParameter(MAV_COMP_ID_AUTOPILOT1, "RC_MAP_THROTTLE");
    QVERIFY(fact != NULL);
    QVariant factValue = fact->rawValue();
    QCOMPARE(factValue.isValid(), true);
    QCOMPARE(factValue.toInt(), 3);
}

/// Test that QML can reference a Fact
void FactSystemTestBase::_qml_test(void)
{
    auto widget = createQuickWidget();

    QQuickItem* rootObject = widget->rootObject();
    QObject* control = rootObject->findChild<QObject*>("testControl");
    QVERIFY(control != NULL);
    QVariant qmlValue = control->property("text").toInt();

    QCOMPARE(qmlValue.toInt(), 3);

    delete widget;
}

/// Test QML getting an updated Fact value
void FactSystemTestBase::_qmlUpdate_test(void)
{
    auto widget = createQuickWidget();

    // Change the value

    QVariant paramValue = 12;
    qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->parameterManager()->getParameter(FactSystem::defaultComponentId, "RC_MAP_THROTTLE")->setRawValue(paramValue);

    QTest::qWait(500); // Let the signals flow through

    // Make sure the qml has the right value

    QQuickItem* rootObject = widget->rootObject();
    QObject* control = rootObject->findChild<QObject*>("testControl");
    QVERIFY(control != NULL);
    QCOMPARE(control->property("text").toInt(), 12);

    delete widget;
}

QQuickWidget *FactSystemTestBase::createQuickWidget()
{
    auto widget = new QQuickWidget;

    widget->setAttribute(Qt::WA_AcceptTouchEvents);

    widget->rootContext()->engine()->addImportPath("qrc:/qml");
    widget->rootContext()->setContextProperty("joystickManager", qgcApp()->toolbox()->joystickManager());
    widget->rootContext()->setContextProperty("autopilot", _plugin);

    widget->setSource(QUrl::fromUserInput("qrc:unittest/FactSystemTest.qml"));

    return widget;
}
