import QtQuick 2.0

Item {
    FontLoader { id: robotoRegular; source: "qrc:/resources/fonts/Roboto-Regular.ttf"}

    property font fontTextBody: Qt.font({
        family: robotoRegular.name,
        weight: 75,
        italic: false,
        pointSize: 14
        })
    property font fontSideBarNormal: Qt.font({
        family: robotoRegular.name,
        weight: 75,
        italic: false,
        pointSize: 14,
        })
    property font fontSideBarHeader: Qt.font({
        family: robotoRegular.name,
        weight: 75,
        italic: false,
        pointSize: 24,
        })
    property string colorSideBarText: "#cccccc"
    property string colorSideBarTextHighlight: "#ffcc66"

}
