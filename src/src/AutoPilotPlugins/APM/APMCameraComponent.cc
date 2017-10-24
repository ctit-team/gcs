#include "APMCameraComponent.h"
#include "APMAutoPilotPlugin.h"
#include "APMAirframeComponent.h"

APMCameraComponent::APMCameraComponent(Vehicle* vehicle, AutoPilotPlugin* autopilot, QObject* parent)
    : VehicleComponent(vehicle, autopilot, parent)
    , _name(tr("Camera"))
{
}

QString APMCameraComponent::name(void) const
{
    return _name;
}

QString APMCameraComponent::description(void) const
{
    return tr("Camera setup is used to adjust camera and gimbal settings.");
}

QString APMCameraComponent::iconResource(void) const
{
    return QStringLiteral("/qmlimages/CameraComponentIcon.png");
}

bool APMCameraComponent::requiresSetup(void) const
{
    return false;
}

bool APMCameraComponent::setupComplete(void) const
{
    return true;
}

QStringList APMCameraComponent::setupCompleteChangedTriggerList(void) const
{
    return QStringList();
}

QUrl APMCameraComponent::setupSource(void) const
{
    return QUrl::fromUserInput(QStringLiteral("qrc:/qml/APMCameraComponent.qml"));
}

QUrl APMCameraComponent::summaryQmlSource(void) const
{
    return QUrl::fromUserInput(QStringLiteral("qrc:/qml/APMCameraComponentSummary.qml"));
}
