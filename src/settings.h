#ifndef SETTINGS_H
#define SETTINGS_H


#include <QObject>

class Settings : public QObject
{

    Q_OBJECT

public:
    // Overwrite existing settings
    bool readSettingsFile(const QString& filePath);

    QString rootDirectory;

private:

// Singleton
public:
    static Settings* getInstance();

private:
    Settings();

    static Settings* instance;

};

#endif // SETTINGS_H
