#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#include <QQuickWindow>
#include <QNetworkProxy>
#include <QSslConfiguration>
#include <QProcess>
#include <FramelessHelper/Quick/framelessquickmodule.h>
#include <FramelessHelper/Core/private/framelessconfig_p.h>
#include "src/component/CircularReveal.h"
#include "src/component/FileWatcher.h"
#include "AppInfo.h"

FRAMELESSHELPER_USE_NAMESPACE
#ifdef FLUENTUI_BUILD_STATIC_LIB
Q_IMPORT_PLUGIN(FluentUIPlugin)
#endif
int main(int argc, char *argv[])
{
    //将样式设置为Basic，不然会导致组件显示异常
    qputenv("QT_QUICK_CONTROLS_STYLE","Basic");
    FramelessHelper::Quick::initialize();
    QGuiApplication::setOrganizationName("ZhuZiChu");
    QGuiApplication::setOrganizationDomain("https://zhuzichu520.github.io");
    QGuiApplication::setApplicationName("FluentUI");
    QGuiApplication app(argc, argv);
#ifdef Q_OS_WIN // 此设置仅在Windows下生效
    FramelessConfig::instance()->set(Global::Option::ForceHideWindowFrameBorder);
#endif
    FramelessConfig::instance()->set(Global::Option::DisableLazyInitializationForMicaMaterial);
    FramelessConfig::instance()->set(Global::Option::CenterWindowBeforeShow);
    FramelessConfig::instance()->set(Global::Option::ForceNonNativeBackgroundBlur);
    FramelessConfig::instance()->set(Global::Option::EnableBlurBehindWindow,false);
#ifdef Q_OS_MACOS
    FramelessConfig::instance()->set(Global::Option::ForceNonNativeBackgroundBlur,false);
    FramelessConfig::instance()->set(Global::Option::EnableBlurBehindWindow,true);
#endif
    AppInfo* appInfo = new AppInfo();
    QQmlApplicationEngine engine;
    FramelessHelper::Quick::registerTypes(&engine);
#ifdef FLUENTUI_BUILD_STATIC_LIB
    qobject_cast<QQmlExtensionPlugin*>(qt_static_plugin_FluentUIPlugin().instance())->initializeEngine(&engine, "FluentUI"); // 让静态资源可以被QML引擎搜索到
    //qobject_cast<QQmlExtensionPlugin*>(qt_static_plugin_FluentUIPlugin().instance())->registerTypes("FluentUI");
#endif
    qmlRegisterType<CircularReveal>("example", 1, 0, "CircularReveal");
    qmlRegisterType<FileWatcher>("example", 1, 0, "FileWatcher");
    appInfo->init(&engine);
    const QUrl url(QStringLiteral("qrc:/example/qml/App.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);
    return app.exec();
}
