#pragma once

#include "MainContent.h"

#include <QMainWindow>
#include <QStatusBar>
#include <QStackedWidget>
#include <QSettings>
#include <QList>
#include <QVBoxLayout>

#include "LinkManager.h"
#include "LinkInterface.h"
#include "UASInterface.h"
#include "LogCompressor.h"
#include "QGCMAVLinkInspector.h"
#include "QGCMAVLinkLogPlayer.h"
#include "MAVLinkDecoder.h"
#include "Vehicle.h"
#include "QGCDockWidget.h"

#include "ui_MainWindow.h"

#if (defined QGC_MOUSE_ENABLED_WIN) | (defined QGC_MOUSE_ENABLED_LINUX)
    #include "Mouse6dofInput.h"
#endif // QGC_MOUSE_ENABLED_WIN

class QGCStatusBar;
class Linecharts;

/**
 * @brief Main Application Window
 *
 **/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /// @brief Returns the MainWindow singleton. Will not create the MainWindow if it has not already
    ///         been created.
    static MainWindow* instance(void);

    /// @brief Deletes the MainWindow singleton
    void deleteInstance(void);

    /// @brief Creates the MainWindow singleton. Should only be called once by QGCApplication.
    static MainWindow* _create();

    ~MainWindow();


    /** @brief Get low power mode setting */
    bool lowPowerModeEnabled() const
    {
        return _lowPowerMode;
    }

    /// @brief Saves the last used connection
    void saveLastUsedConnection(const QString connection);

    // Called from MainWindow.qml when the user accepts the window close dialog
    Q_INVOKABLE void reallyClose(void);

    /// @return Root qml object of main window QML
    QObject* rootQmlObject(void);

public slots:
    /** @brief Save power by reducing update rates */
    void enableLowPowerMode(bool enabled) { _lowPowerMode = enabled; }

    void closeEvent(QCloseEvent* event);

    /** @brief Update the window name */
    void configureWindowName();

signals:
    void initStatusChanged(const QString& message, int alignment, const QColor &color);
    /** Emitted when any value changes from any source */
    void valueChanged(const int uasId, const QString& name, const QString& unit, const QVariant& value, const quint64 msec);

    // Used for unit tests to know when the main window closes
    void mainWindowClosed(void);

#ifdef QGC_MOUSE_ENABLED_LINUX
    /** @brief Forward X11Event to catch 3DMouse inputs */
    void x11EventOccured(XEvent *event);
#endif //QGC_MOUSE_ENABLED_LINUX

public:
    QGCMAVLinkLogPlayer* getLogPlayer()
    {
        return logPlayer;
    }

protected:
    void connectCommonActions();

    void loadSettings();
    void storeSettings();

    QSettings settings;

    QGCMAVLinkLogPlayer* logPlayer;
#ifdef QGC_MOUSE_ENABLED_WIN
    /** @brief 3d Mouse support (WIN only) */
    Mouse3DInput* mouseInput;               ///< 3dConnexion 3dMouse SDK
    Mouse6dofInput* mouse;                  ///< Implementation for 3dMouse input
#endif // QGC_MOUSE_ENABLED_WIN

#ifdef QGC_MOUSE_ENABLED_LINUX
    /** @brief Reimplementation of X11Event to handle 3dMouse Events (magellan) */
    bool x11Event(XEvent *event);
    Mouse6dofInput* mouse;                  ///< Implementation for 3dMouse input
#endif // QGC_MOUSE_ENABLED_LINUX

    /** User interface actions **/
    QAction* connectUASAct;
    QAction* disconnectUASAct;
    QAction* startUASAct;
    QAction* returnUASAct;
    QAction* stopUASAct;
    QAction* killUASAct;


    LogCompressor* comp;
    QTimer* videoTimer;
    QTimer windowNameUpdateTimer;

private slots:
    void _closeWindow(void) { close(); }
    void _vehicleAdded(Vehicle* vehicle);
    void _showDockWidgetAction(bool show);
    void _showAdvancedUIChanged(bool advanced);

private:
    /// Constructor is private since all creation should be through MainWindow::_create
    MainWindow();

    void _openUrl(const QString& url, const QString& errorMessage);

    QMap<QString, QGCDockWidget*>   _mapName2DockWidget;
    QMap<QString, QAction*>         _mapName2Action;

    void _storeCurrentViewState(void);
    void _loadCurrentViewState(void);
    bool _createInnerDockWidget(const QString& widgetName);
    void _buildCommonWidgets(void);
    void _hideAllDockWidgets(void);
    void _showDockWidget(const QString &name, bool show);
    void _loadVisibleWidgetsSettings(void);
    void _storeVisibleWidgetsSettings(void);
    MAVLinkDecoder* _mavLinkDecoderInstance(void);

    MAVLinkDecoder*         _mavlinkDecoder;
    bool                    _lowPowerMode;           ///< If enabled, QGC reduces the update rates of all widgets
    QVBoxLayout*            _centralLayout;
    Ui::MainWindow          _ui;

    MainContent *content;

    bool    _forceClose;

    QString _getWindowGeometryKey();
};
