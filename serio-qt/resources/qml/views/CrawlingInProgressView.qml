import QtQuick 2.12
import QtQuick.Layouts 1.12
import '../widgets'

CenteredViewLayout {
    layoutSpacing: globalPadding * 3
    NonDeterministicCircularProgressBar {
        Layout.fillWidth: true
    }
    AccentTitle {
        Layout.fillWidth: true
        text: "Crawling in progress. Please stand by."
    }
}
