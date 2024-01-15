#include "datenbankmanager.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

DatenbankManager::DatenbankManager(const QString& path, QObject *parent)
    : QObject(parent) {
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(path);

    if (!m_database.open()) {
        qDebug() << "Error: Verbindung zur Datenbank konnte nicht hergestellt werden.";
    } else {
        qDebug() << "Datenbankverbindung erfolgreich hergestellt.";

        // Überprüfen, ob die Tabelle existiert und sie ggf. erstellen
        QSqlQuery query(m_database);
        if (!query.exec("CREATE TABLE IF NOT EXISTS einstellungen (id INTEGER PRIMARY KEY, ip TEXT, uhrzeit TEXT, text TEXT, zahl INTEGER, dropdown TEXT, radio TEXT)")) {
            qDebug() << "Fehler beim Erstellen der Tabelle: " << query.lastError().text();
        } else {
            // Laden der Daten beim Start der Anwendung, falls vorhanden
            QString ip;
            QTime uhrzeit;
            QString text;
            int zahl;
            int dropdown;
            QString radio;
            if (ladeDaten(ip, uhrzeit, text, zahl, dropdown, radio)) {
                qDebug() << "Daten geladen, IP: " << ip;
                emit datenGeladen(ip, uhrzeit.toString("HH:mm"), text, zahl, dropdown, radio);
            }
        }

    }
}

void DatenbankManager::loadInitialData() {
    QString ip;
    QTime uhrzeit;
    QString text;
    int zahl;
    int dropdown;
    QString radio;
    if (ladeDaten(ip, uhrzeit, text, zahl, dropdown, radio)) {
        qDebug() << "Daten geladen, IP: " << ip;
        emit datenGeladen(ip, uhrzeit.toString("HH:mm"), text, zahl, dropdown, radio);
    }
}



DatenbankManager::~DatenbankManager() {
    m_database.close();
}

bool DatenbankManager::speichereDaten(const QString& ip, const QTime& uhrzeit, const QString& text, int zahl, int dropdown, const QString& radio) {
    qDebug() << "speichereDaten aufgerufen, ip: " << ip << " uhrzeit: " << uhrzeit << " text" << text << " zahl: " << zahl << " dropdown: " << dropdown << " radio-value: " << radio;

    QSqlQuery query;
    query.prepare("INSERT OR REPLACE INTO einstellungen (id, ip, uhrzeit, text, zahl, dropdown, radio) VALUES ((SELECT id FROM einstellungen), ?, ?, ?, ?, ?, ?)");
    query.addBindValue(ip);
    query.addBindValue(uhrzeit.toString("HH:mm"));
    query.addBindValue(text);
    query.addBindValue(zahl);
    query.addBindValue(dropdown);
    query.addBindValue(radio);

    if (!query.exec()) {
        qDebug() << "Speichern fehlgeschlagen: " << query.lastError();
        return false;
    }

    return true;
}

bool DatenbankManager::ladeDaten(QString& ip, QTime& uhrzeit, QString& text, int& zahl, int& dropdown, QString& radio) {
    QSqlQuery query("SELECT * FROM einstellungen ORDER BY id DESC LIMIT 1");

    if (query.next()) {
        ip = query.value("ip").toString();
        uhrzeit = QTime::fromString(query.value("uhrzeit").toString(), "HH:mm");
        text = query.value("text").toString();
        zahl = query.value("zahl").toInt();
        dropdown = query.value("dropdown").toInt();
        qDebug() << "Geladener Dropdown-Wert: " << dropdown;
        radio = query.value("radio").toString();
        return true;
    } else {
        qDebug() << "Laden fehlgeschlagen: " << query.lastError();
        return false;
    }
}

bool DatenbankManager::resetEinstellungen() {
    qDebug() << "Methode resetEinstellungen wurde aufgerufen";

    QSqlQuery query;
    query.prepare("DELETE FROM einstellungen");

    if (!query.exec()) {
        qDebug() << "Zurücksetzen fehlgeschlagen: " << query.lastError();
        return false;
    }

    return true;
}

void DatenbankManager::requestSavedData() {
    // Gespeicherte Daten abrufen
    QSqlQuery query;
    query.prepare("SELECT ip, uhrzeit, text, zahl, dropdown, radio FROM einstellungen");

    if (query.exec() && query.next()) {
        // Daten erfolgreich abgerufen
        savedIp = query.value(0).toString();
        savedUhrzeit = query.value(1).toString();
        savedText = query.value(2).toString();
        savedZahl = query.value(3).toInt();
        savedDropdown = query.value(4).toInt();
        savedRadio = query.value(5).toString();

        // Senden Sie die abgerufenen Daten zurück an QML
        emit dataFetched(savedIp, savedUhrzeit, savedText, savedZahl, savedDropdown, savedRadio);
    } else {
        qDebug() << "Fehler beim Abrufen der gespeicherten Daten: " << query.lastError();
    }
}

