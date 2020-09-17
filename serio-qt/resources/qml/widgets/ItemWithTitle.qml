import QtQuick 2.12
import QtQuick.Layouts 1.12
import '../widgets'

ColumnLayout {
    property string title
    GreySubtitle {
        text: title
    }
}
