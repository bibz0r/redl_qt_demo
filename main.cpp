#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "datenbankmanager.h" // Stellen Sie sicher, dass Sie den Pfad anpassen, falls nötig

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Erstellen Sie eine Instanz Ihrer DatenbankManager-Klasse
    DatenbankManager dbManager("settings.db"); // Passen Sie den Pfad entsprechend an

    // Setzen Sie die Instanz als Kontexteigenschaft für QML
    engine.rootContext()->setContextProperty("dbManager", &dbManager);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
