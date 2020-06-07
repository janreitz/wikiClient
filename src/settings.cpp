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


void Settings::setRootDirectory(const QString& rootDirectory) {
    m_rootDirectory = rootDirectory;
}

QString Settings::rootDirectory() const{
    return m_rootDirectory;
}

QColor Settings::colorTextDark() const{
    return m_colorTextDark;
}
QColor Settings::colorTextDarkHighlight() const{
    return m_colorTextDarkHighlight;
}
QColor Settings::colorTextLight() const{
    return m_colorTextLight;
}
QColor Settings::colorTextLightHighlight() const{
    return m_colorTextLightHighlight;
}
QColor Settings::colorAreaBackground() const{
    return m_colorAreaBackground;
}
QColor Settings::colorAreaHighlight() const{
    return m_colorAreaHighlight;
}
QColor Settings::colorAreaLightBackground() const{
    return m_colorAreaLightBackground;
}
QColor Settings::colorAreaLightHighlight() const{
    return m_colorAreaLightHighlight;
}

Settings *Settings::getInstance()
{
    if (!instance) {
        instance = new Settings();
    }

    return instance;
}






