#ifndef DATENBANKMANAGER_H
#define DATENBANKMANAGER_H

#include <QObject>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTime>

class DatenbankManager : public QObject {
    Q_OBJECT

    // Declare the properties
    Q_PROPERTY(QString savedIp READ getSavedIp NOTIFY dataFetched)
    Q_PROPERTY(QString savedUhrzeit READ getSavedUhrzeit NOTIFY dataFetched)
    Q_PROPERTY(QString savedText READ getSavedText NOTIFY dataFetched)
    Q_PROPERTY(int savedZahl READ getSavedZahl NOTIFY dataFetched)
    Q_PROPERTY(int savedDropdown READ getSavedDropdown NOTIFY dataFetched)
    Q_PROPERTY(QString savedRadio READ getSavedRadio NOTIFY dataFetched)

public:
    explicit DatenbankManager(const QString& path, QObject *parent = nullptr);
    ~DatenbankManager();

    Q_INVOKABLE bool speichereDaten(const QString& ip, const QTime& uhrzeit, const QString& text, int zahl, int dropdown, const QString& radio);
    Q_INVOKABLE bool ladeDaten(QString& ip, QTime& uhrzeit, QString& text, int& zahl, int& dropdown, QString& radio);
    Q_INVOKABLE bool resetEinstellungen();
    Q_INVOKABLE void loadInitialData();

    // New function to request saved data
    Q_INVOKABLE void requestSavedData();

    // Getters for the properties
    QString getSavedIp() const { return savedIp; }
    QString getSavedUhrzeit() const { return savedUhrzeit; }
    QString getSavedText() const { return savedText; }
    int getSavedZahl() const { return savedZahl; }
    int getSavedDropdown() const { return savedDropdown; }
    QString getSavedRadio() const { return savedRadio; }

signals:
    void datenGeladen(const QString& ip, const QString& uhrzeit, const QString& text, int zahl, int dropdown, const QString& radio);
    void dataFetched(const QString& ip, const QString& uhrzeit, const QString& text, int zahl, int dropdown, const QString& radio);

private:
    QSqlDatabase m_database;

    // Variables to store the fetched data
    QString savedIp;
    QString savedUhrzeit;
    QString savedText;
    int savedZahl;
    int savedDropdown;
    QString savedRadio;
};

#endif // DATENBANKMANAGER_H
