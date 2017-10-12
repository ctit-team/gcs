import QtQuick          2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts  1.2

import QGroundControl                       1.0
import QGroundControl.AutoPilotPlugin       1.0
import QGroundControl.Palette               1.0
import QGroundControl.Controls              1.0
import QGroundControl.ScreenTools           1.0
import QGroundControl.MultiVehicleManager   1.0

Rectangle {
    id:     setupView
    color:  qgcPal.window
    z:      QGroundControl.zOrderTopMost

    QGCPalette { id: qgcPal; colorGroupEnabled: true }

    ExclusiveGroup { id: setupButtonGroup }

    readonly property real      _defaultTextHeight: ScreenTools.defaultFontPixelHeight
    readonly property real      _defaultTextWidth:  ScreenTools.defaultFontPixelWidth
    readonly property real      _horizontalMargin:  _defaultTextWidth / 2
    readonly property real      _verticalMargin:    _defaultTextHeight / 2
    readonly property real      _buttonWidth:       _defaultTextWidth * 18
    readonly property string    _armedVehicleText:  qsTr("This operation cannot be performed while the vehicle is armed.")

    property string _messagePanelText:              qsTr("missing message panel text")
    property bool   _fullParameterVehicleAvailable: QGroundControl.multiVehicleManager.parameterReadyVehicleAvailable && !QGroundControl.multiVehicleManager.activeVehicle.parameterManager.missingParameters
    property var    _corePlugin:                    QGroundControl.corePlugin

    function showSummaryPanel()
    {
        if (_fullParameterVehicleAvailable) {
            if (QGroundControl.multiVehicleManager.activeVehicle.autopilot.vehicleComponents.length == 0) {
                panelLoader.setSourceComponent(noComponentsVehicleSummaryComponent)
            } else {
                panelLoader.setSource("VehicleSummary.qml")
            }
        } else if (QGroundControl.multiVehicleManager.parameterReadyVehicleAvailable) {
            panelLoader.setSourceComponent(missingParametersVehicleSummaryComponent)
        } else {
            panelLoader.setSourceComponent(disconnectedVehicleSummaryComponent)
        }
    }

    function showFirmwarePanel()
    {
        if (!ScreenTools.isMobile) {
            panelLoader.setSource("FirmwareUpgrade.qml")
        }
    }

    function showJoystickPanel()
    {
        panelLoader.setSource("JoystickConfig.qml")
    }

    function showParametersPanel()
    {
        panelLoader.setSource("SetupParameterEditor.qml")
    }

    function showPX4FlowPanel()
    {
        panelLoader.setSource("PX4FlowSensor.qml")
    }

    function showVehicleComponentPanel(vehicleComponent)
    {
        var autopilotPlugin = QGroundControl.multiVehicleManager.activeVehicle.autopilot
        var prereq = autopilotPlugin.prerequisiteSetup(vehicleComponent)
        if (prereq !== "") {
            //-- TODO: This cannot be translated when built this way.
            _messagePanelText = prereq + " setup must be completed prior to " + vehicleComponent.name + " setup."
            panelLoader.setSourceComponent(messagePanelComponent)
        } else {
            panelLoader.setSource(vehicleComponent.setupSource, vehicleComponent)
            for(var i = 0; i < componentRepeater.count; i++) {
                var obj = componentRepeater.itemAt(i);
                if (obj.text === vehicleComponent.name) {
                    obj.checked = true;
                    break;
                }
            }
        }
    }

    Component.onCompleted: showSummaryPanel()

    Connections {
        target: QGroundControl.multiVehicleManager

        onParameterReadyVehicleAvailableChanged: {
            if(!QGroundControl.skipSetupPage) {
                if (parameterReadyVehicleAvailable || summaryButton.checked || setupButtonGroup.current != firmwareButton) {
                    // Show/Reload the Summary panel when:
                    //      A new vehicle shows up
                    //      The summary panel is already showing and the active vehicle goes away
                    //      The active vehicle goes away and we are not on the Firmware panel.
                    summaryButton.checked = true
                    showSummaryPanel()
                }
            }
        }
    }

    Component {
        id: noComponentsVehicleSummaryComponent

        Rectangle {
            color: qgcPal.windowShade

            QGCLabel {
                anchors.margins:        _defaultTextWidth * 2
                anchors.fill:           parent
                verticalAlignment:      Text.AlignVCenter
                horizontalAlignment:    Text.AlignHCenter
                wrapMode:               Text.WordWrap
                font.pointSize:         ScreenTools.mediumFontPointSize
                text:                   qsTr("%1 does not currently support setup of your vehicle type. ").arg(QGroundControl.appName) +
                                        "If your vehicle is already configured you can still Fly."

                onLinkActivated: Qt.openUrlExternally(link)
            }
        }
    }

    Component {
        id: disconnectedVehicleSummaryComponent

        Rectangle {
            color: qgcPal.windowShade

            QGCLabel {
                anchors.margins:        _defaultTextWidth * 2
                anchors.fill:           parent
                verticalAlignment:      Text.AlignVCenter
                horizontalAlignment:    Text.AlignHCenter
                wrapMode:               Text.WordWrap
                font.pointSize:         ScreenTools.largeFontPointSize
                text:                   qsTr("Vehicle settings and info will display after connecting your vehicle.") +
                                        (ScreenTools.isMobile ? "" : " Click Firmware on the left to upgrade your vehicle.")

                onLinkActivated: Qt.openUrlExternally(link)
            }
        }
    }

    Component {
        id: missingParametersVehicleSummaryComponent

        Rectangle {
            color: qgcPal.windowShade

            QGCLabel {
                anchors.margins:        _defaultTextWidth * 2
                anchors.fill:           parent
                verticalAlignment:      Text.AlignVCenter
                horizontalAlignment:    Text.AlignHCenter
                wrapMode:               Text.WordWrap
                font.pointSize:         ScreenTools.mediumFontPointSize
                text:                   qsTr("You are currently connected to a vehicle but it did not return the full parameter list. ") +
                                        qsTr("As a result, the full set of vehicle setup options are not available.")

                onLinkActivated: Qt.openUrlExternally(link)
            }
        }
    }

    Component {
        id: messagePanelComponent

        Item {
            QGCLabel {
                anchors.margins:        _defaultTextWidth * 2
                anchors.fill:           parent
                verticalAlignment:      Text.AlignVCenter
                horizontalAlignment:    Text.AlignHCenter
                wrapMode:               Text.WordWrap
                font.pointSize:         ScreenTools.mediumFontPointSize
                text:                   _messagePanelText
            }
        }
    }

    ColumnLayout {
        id: buttons
        spacing: _defaultTextHeight / 2

        anchors.topMargin: _defaultTextHeight / 2
        anchors.top: parent.top
        anchors.bottomMargin: _defaultTextHeight / 2
        anchors.bottom: parent.bottom
        anchors.leftMargin: _horizontalMargin
        anchors.left: parent.left

        QGCLabel {
            text: qsTr("Vehicle Setup")
            horizontalAlignment: Text.AlignHCenter

            anchors.left: parent.left
            anchors.right: parent.right
        }

        Repeater {
            model: _corePlugin ? _corePlugin.settingsPages : []
            visible: _corePlugin && _corePlugin.options.combineSettingsAndSetup

            SubMenuButton {
                text: modelData.title
                exclusiveGroup: setupButtonGroup
                setupIndicator: false
                visible: _corePlugin && _corePlugin.options.combineSettingsAndSetup
                imageResource: modelData.icon
                onClicked: panelLoader.setSource(modelData.url)

                Layout.fillWidth: true
            }
        }

        SubMenuButton {
            id: summaryButton
            text: qsTr("Summary")
            exclusiveGroup: setupButtonGroup
            setupIndicator: false
            imageResource: "/qmlimages/VehicleSummaryIcon.png"
            checked: true
            onClicked: showSummaryPanel()

            Layout.fillWidth: true
        }

        SubMenuButton {
            id: firmwareButton
            text: sTr("Firmware")
            exclusiveGroup: setupButtonGroup
            setupIndicator: false
            visible: _corePlugin.options.showFirmwareUpgrade
            imageResource: "/qmlimages/FirmwareUpgradeIcon.png"
            onClicked: showFirmwarePanel()

            Layout.fillWidth: true
        }

        SubMenuButton {
            text: qsTr("PX4Flow")
            exclusiveGroup: setupButtonGroup
            setupIndicator: false
            visible: QGroundControl.multiVehicleManager.activeVehicle ? QGroundControl.multiVehicleManager.activeVehicle.genericFirmware : false
            onClicked: showPX4FlowPanel()

            Layout.fillWidth: true
        }

        Repeater {
            id: componentRepeater
            model: _fullParameterVehicleAvailable ? QGroundControl.multiVehicleManager.activeVehicle.autopilot.vehicleComponents : 0

            SubMenuButton {
                text: modelData.name
                exclusiveGroup: setupButtonGroup
                setupIndicator: modelData.requiresSetup
                visible: modelData.setupSource.toString() !== ""
                imageResource: modelData.iconResource
                setupComplete: modelData.setupComplete
                onClicked: showVehicleComponentPanel(modelData)

                Layout.fillWidth: true
            }
        }

        SubMenuButton {
            text: qsTr("Parameters")
            exclusiveGroup: setupButtonGroup
            setupIndicator: false
            visible: QGroundControl.multiVehicleManager && QGroundControl.multiVehicleManager.parameterReadyVehicleAvailable && _corePlugin.showAdvancedUI
            onClicked: showParametersPanel()

            Layout.fillWidth: true
        }

        Rectangle {
            color: qgcPal.window

            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        SubMenuButton {
            text: qsTr("Factory Reset")
            exclusiveGroup: setupButtonGroup
            setupIndicator: false
            visible: QGroundControl.multiVehicleManager && QGroundControl.multiVehicleManager.parameterReadyVehicleAvailable

            Layout.fillWidth: true
        }
    }

    Rectangle {
        id:                     divider
        anchors.topMargin:      _verticalMargin
        anchors.bottomMargin:   _verticalMargin
        anchors.leftMargin:     _horizontalMargin
        anchors.left:           buttons.right
        anchors.top:            parent.top
        anchors.bottom:         parent.bottom
        width:                  1
        color:                  qgcPal.windowShade
    }

    Loader {
        id:                     panelLoader
        anchors.topMargin:      _verticalMargin
        anchors.bottomMargin:   _verticalMargin
        anchors.leftMargin:     _horizontalMargin
        anchors.rightMargin:    _horizontalMargin
        anchors.left:           divider.right
        anchors.right:          parent.right
        anchors.top:            parent.top
        anchors.bottom:         parent.bottom

        function setSource(source, vehicleComponent) {
            panelLoader.vehicleComponent = vehicleComponent
            panelLoader.source = source
        }

        function setSourceComponent(sourceComponent, vehicleComponent) {
            panelLoader.vehicleComponent = vehicleComponent
            panelLoader.sourceComponent = sourceComponent
        }

        property var vehicleComponent
    }
}
