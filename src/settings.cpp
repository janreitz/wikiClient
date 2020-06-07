#include "settings.h"

Settings* Settings::instance = nullptr;

Settings::Settings()
{
}

Settings *Settings::getInstance()
{
    if (!instance) {
        instance = new Settings();
    }

    return instance;
}
