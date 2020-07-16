import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../items" as Items
import "../headers" as Headers

Headers.InputHeader {
    id: top

    signal clicked()

    readOnly: true

    MouseArea {
        anchors {
            fill: parent
            topMargin: widgetX
        }

        onClicked: {
            forceActiveFocus()
            top.clicked()
        }
    }
}
