#include "APMLightsComponent.h"
#include "APMAutoPilotPlugin.h"
#include "APMAirframeComponent.h"

APMLightsComponent::APMLightsComponent(Vehicle* vehicle, AutoPilotPlugin* autopilot, QObject* parent)
    : VehicleComponent(vehicle, autopilot, parent)
    , _name(tr("Lights"))
{
}

QString APMLightsComponent::name(void) const
{
    return _name;
}

QString APMLightsComponent::description(void) const
{
    return tr("Lights setup is used to adjust light output channels.");
}

QString APMLightsComponent::iconResource(void) const
{
    return QStringLiteral("/qmlimages/LightsComponentIcon.png");
}

bool APMLightsComponent::requiresSetup(void) const
{
    return false;
}

bool APMLightsComponent::setupComplete(void) const
{
    return true;
}

QStringList APMLightsComponent::setupCompleteChangedTriggerList(void) const
{
    return QStringList();
}

QUrl APMLightsComponent::setupSource(void) const
{
    return QUrl::fromUserInput(QStringLiteral("qrc:/qml/APMLightsComponent.qml"));
}

QUrl APMLightsComponent::summaryQmlSource(void) const
{
    return QUrl::fromUserInput(QStringLiteral("qrc:/qml/APMLightsComponentSummary.qml"));
}
