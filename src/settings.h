#ifndef SETTINGS_H
#define SETTINGS_H


#include <QObject>
#include <QColor>

class Settings : public QObject
{

    Q_OBJECT
    Q_PROPERTY(QString rootDirectory READ rootDirectory WRITE setRootDirectory NOTIFY rootDirectoryChanged);

    // Theme
    Q_PROPERTY(QColor colorTextDark READ colorTextDark NOTIFY colorTextDarkChanged)
    Q_PROPERTY(QColor colorTextDarkHighlight READ colorTextDarkHighlight NOTIFY colorTextDarkHighlightChanged)
    Q_PROPERTY(QColor colorTextLight READ colorTextLight NOTIFY colorTextLightChanged)
    Q_PROPERTY(QColor colorTextLightHighlight READ colorTextLightHighlight NOTIFY colorTextLightHighlightChanged)
    Q_PROPERTY(QColor colorAreaBackground READ colorAreaBackground NOTIFY colorAreaBackgroundChanged)
    Q_PROPERTY(QColor colorAreaHighlight READ colorAreaHighlight NOTIFY colorAreaHighlightChanged)
    Q_PROPERTY(QColor colorAreaLightBackground READ colorAreaLightBackground NOTIFY colorAreaLightBackgroundChanged)
    Q_PROPERTY(QColor colorAreaLightHighlight READ colorAreaLightHighlight NOTIFY colorAreaLightHighlightChanged)

public:
    // Overwrite existing settings
    bool readSettingsFile(const QString& filePath);

    QString rootDirectory() const;
    void setRootDirectory(const QString& rootDirectory);

    // Theme
    QColor colorTextDark() const;
    QColor colorTextDarkHighlight() const;
    QColor colorTextLight() const;
    QColor colorTextLightHighlight() const;
    QColor colorAreaBackground() const;
    QColor colorAreaHighlight() const;
    QColor colorAreaLightBackground() const;
    QColor colorAreaLightHighlight() const;


signals:
    void rootDirectoryChanged();
    // Theme
    void colorTextDarkChanged();
    void colorTextDarkHighlightChanged();
    void colorTextLightChanged();
    void colorTextLightHighlightChanged();
    void colorAreaBackgroundChanged();
    void colorAreaHighlightChanged();
    void colorAreaLightBackgroundChanged();
    void colorAreaLightHighlightChanged();

private:

    // Theme
    void setColorTextDark(const QColor& color);
    void setColorTextDarkHighlight(const QColor& color);
    void setColorTextLight(const QColor& color);
    void setColorTextLightHighlight(const QColor& color);
    void setColorAreaBackground(const QColor& color);
    void setColorAreaHighlight(const QColor& color);
    void setColorAreaLightBackground(const QColor& color);
    void setColorAreaLightHighlight(const QColor& color);

    QString m_rootDirectory;

    QColor m_colorTextDark;
    QColor m_colorTextDarkHighlight;
    QColor m_colorTextLight;
    QColor m_colorTextLightHighlight;
    QColor m_colorAreaBackground;
    QColor m_colorAreaHighlight;
    QColor m_colorAreaLightBackground;
    QColor m_colorAreaLightHighlight;

// Singleton
public:
    static Settings* getInstance();

private:
    Settings();

    static Settings* instance;

};

#endif // SETTINGS_H
