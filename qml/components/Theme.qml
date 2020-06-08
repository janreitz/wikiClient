import QtQuick 2.0

Item {
    FontLoader { id: robotoRegular; source: "qrc:/resources/fonts/Roboto-Regular.ttf"}

    property font fontTextBody: Qt.font({
        family: robotoRegular.name,
        weight: 75,
        italic: false,
        pointSize: 12
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
//    property string colorTextDark: "#1E1E1E"
//    property string colorTextDarkHighlight: "#68217A"
//    property string colorTextLight: "#cccccc"
//    property string colorTextLightHighlight: "#ffcc66"
//    property string colorAreaBackground: "#2d2d2d"
//    property string colorAreaHighlight: "#323233"
//    property string colorAreaLightBackground: "#F2F2F2"
//    property string colorAreaLightHighlight: "#F5F5F5"
}
