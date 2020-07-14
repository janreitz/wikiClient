#include "settings.h"
#include "utilities.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaProperty>

Settings* Settings::instance = nullptr;

Settings::Settings()
{
}

bool Settings::readSettingsFile(const QString &filePath)
{
    auto contentOpt = Utilities::stringFromFile(filePath);
    if (!contentOpt)
        return false;

    auto doc = QJsonDocument::fromJson((*contentOpt).toUtf8());
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "Settings::readSettingsFile -> Settings file is not a valid JSON Object";
        return false;
    }

    auto settingsObj = doc.object();

    auto myMetaObj = metaObject();
    auto propCount = metaObject()->propertyCount();

    for (int i=0; i<propCount; ++i) {
        QMetaProperty metaproperty = myMetaObj->property(i);
        auto metaPropName = metaproperty.name();
        if (settingsObj.contains(metaPropName)) {
            auto settingsValue = settingsObj[metaPropName].toVariant();
            if (settingsValue.canConvert(metaproperty.type())) {
                settingsValue.convert(metaproperty.type());
                if (!setProperty(metaPropName, settingsValue)) {
                    qDebug() << "Settings::readSettingsFile -> could not set property "
                             << metaPropName
                             << " to "
                             << settingsValue;
                }
            } else {
                qDebug() << "Settings::readSettingsFile -> Can't convert  "
                         << settingsValue
                         << " to "
                         << metaproperty.type();
            }
        }
    }

    return true;
}


QString Settings::rootDirectory() const{
    return m_rootDirectory;
}
void Settings::setRootDirectory(const QString& rootDirectory) {
    m_rootDirectory = rootDirectory;
    emit rootDirectoryChanged();
}

int Settings::spacesPerTab() const
{
    return m_spacesPerTab;
}

void Settings::setSpacesPerTab(int spacesPerTab)
{
    m_spacesPerTab = spacesPerTab;
}
QColor Settings::colorTextDark() const{
    return m_colorTextDark;
}
void Settings::setColorTextDark(const QColor& color) {
    m_colorTextDark = color;
    emit colorTextDarkChanged();
}
QColor Settings::colorTextDarkHighlight() const{
    return m_colorTextDarkHighlight;
}
void Settings::setColorTextDarkHighlight(const QColor& color) {
    m_colorTextDarkHighlight = color;
    emit colorTextDarkHighlightChanged();
}
QColor Settings::colorTextLight() const{
    return m_colorTextLight;
}
void Settings::setColorTextLight(const QColor& color) {
    m_colorTextLight = color;
    emit colorTextLightChanged();
}
QColor Settings::colorTextLightHighlight() const{
    return m_colorTextLightHighlight;
}
void Settings::setColorTextLightHighlight(const QColor& color) {
    m_colorTextLightHighlight = color;
    emit colorTextLightHighlightChanged();
}
QColor Settings::colorAreaBackground() const{
    return m_colorAreaBackground;
}
void Settings::setColorAreaBackground(const QColor& color) {
    m_colorAreaBackground = color;
    emit colorAreaBackgroundChanged();
}
QColor Settings::colorAreaHighlight() const{
    return m_colorAreaHighlight;
}
void Settings::setColorAreaHighlight(const QColor& color) {
    m_colorAreaHighlight = color;
    emit colorAreaHighlightChanged();
}
QColor Settings::colorAreaLightBackground() const{
    return m_colorAreaLightBackground;
}
void Settings::setColorAreaLightBackground(const QColor& color) {
    m_colorAreaLightBackground = color;
    emit colorAreaLightBackgroundChanged();
}
QColor Settings::colorAreaLightHighlight() const{
    return m_colorAreaLightHighlight;
}
void Settings::setColorAreaLightHighlight(const QColor& color) {
    m_colorAreaLightHighlight = color;
    emit colorAreaLightHighlightChanged();
}


Settings *Settings::getInstance()
{
    if (!instance) {
        instance = new Settings();
    }

    return instance;
}






