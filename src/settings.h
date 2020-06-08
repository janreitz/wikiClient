#ifndef SETTINGS_H
#define SETTINGS_H


#include <QObject>
#include <QColor>

class Settings : public QObject
{

    Q_OBJECT
    Q_PROPERTY(QString rootDirectory READ rootDirectory WRITE setRootDirectory NOTIFY rootDirectoryChanged);

    // Theme
    Q_PROPERTY(QColor colorTextDark READ colorTextDark WRITE setColorTextDark NOTIFY colorTextDarkChanged)
    Q_PROPERTY(QColor colorTextDarkHighlight READ colorTextDarkHighlight WRITE setColorTextDarkHighlight NOTIFY colorTextDarkHighlightChanged)
    Q_PROPERTY(QColor colorTextLight READ colorTextLight WRITE setColorTextLight NOTIFY colorTextLightChanged)
    Q_PROPERTY(QColor colorTextLightHighlight READ colorTextLightHighlight WRITE setColorTextLightHighlight NOTIFY colorTextLightHighlightChanged)
    Q_PROPERTY(QColor colorAreaBackground READ colorAreaBackground WRITE setColorAreaBackground NOTIFY colorAreaBackgroundChanged)
    Q_PROPERTY(QColor colorAreaHighlight READ colorAreaHighlight WRITE setColorAreaHighlight NOTIFY colorAreaHighlightChanged)
    Q_PROPERTY(QColor colorAreaLightBackground READ colorAreaLightBackground WRITE setColorAreaLightBackground NOTIFY colorAreaLightBackgroundChanged)
    Q_PROPERTY(QColor colorAreaLightHighlight READ colorAreaLightHighlight WRITE setColorAreaLightHighlight NOTIFY colorAreaLightHighlightChanged)

public:
    // Overwrite existing settings
    bool readSettingsFile(const QString& filePath);

    QString rootDirectory() const;
    void setRootDirectory(const QString& rootDirectory);

    // Theme
    QColor colorTextDark() const;
    void setColorTextDark(const QColor& color);

    QColor colorTextDarkHighlight() const;
    void setColorTextDarkHighlight(const QColor& color);

    QColor colorTextLight() const;
    void setColorTextLight(const QColor& color);

    QColor colorTextLightHighlight() const;
    void setColorTextLightHighlight(const QColor& color);

    QColor colorAreaBackground() const;
    void setColorAreaBackground(const QColor& color);

    QColor colorAreaHighlight() const;
    void setColorAreaHighlight(const QColor& color);

    QColor colorAreaLightBackground() const;
    void setColorAreaLightBackground(const QColor& color);

    QColor colorAreaLightHighlight() const;
    void setColorAreaLightHighlight(const QColor& color);

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
