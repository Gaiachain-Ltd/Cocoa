import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../../items" as Items
import "../../headers" as Headers

Column {
    id: root

    Layout.fillWidth: true

    property color secondaryColor
    property color secondaryTextColor
    property color backgroundColor
    property bool summaryMode: true
    property bool highlighted: false
    property bool extraLeftMargin: false
    property bool forceBoldValue: false
    property int valueFontSize: s(GStyle.inputPixelSize)
    property int margins: s(GStyle.bigMargin)

    Headers.GHeader {
        id: button

        property alias text: buttonTextWidget.text

        headerText: "Number of trees planted"
        helpButtonVisible: true
        helpText: "Trees number help"
        text: "Tap to add a tree being planted"

        function clear() {
            text = Strings.empty
        }

        widget: Items.GText {
            id: buttonTextWidget

            Layout.fillWidth: true
            Layout.margins: root.margins
            Layout.leftMargin: extraLeftMargin? root.margins * 2 : root.margins

            focus: false
            color: secondaryTextColor
            font.bold: root.highlighted || forceBoldValue
            font.pixelSize: valueFontSize

            wrapMode: Text.WrapAnywhere
            horizontalAlignment: Text.AlignLeft
        }
    }

    Headers.GHeader {
        id: counter

        property alias text: counterTextWidget.text

        property bool forceBoldValue: false
        property int valueFontSize: s(GStyle.inputPixelSize)

        function clear() {
            text = Strings.empty
        }

        headerText: "Total count of trees planted"
        helpButtonVisible: true
        helpText: "Total trees number help"
        //inputText: "Tap to add a tree being planted"

        widget: Rectangle {
            id: inputRectangle

            Layout.fillWidth: true
            Layout.margins: root.margins
            Layout.leftMargin: extraLeftMargin? root.margins * 2 : root.margins

            color: GStyle.blank
            border {
                width: sr(GStyle.controlDefaultBorderWidth)
                color: GStyle.buttonBorderColor
            }

            height: 100
            radius: s(GStyle.buttonRadius)

            RowLayout {
                anchors.fill: parent

                TextField {
                    id: counterTextWidget
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    focus: false
                    color: secondaryTextColor
                    font.bold: root.highlighted || forceBoldValue
                    font.pixelSize: valueFontSize

                    wrapMode: Text.WrapAnywhere
                    horizontalAlignment: Text.AlignLeft

                    validator: IntValidator {
                        bottom: 0
                        top: 32767
                    }

                    background: Item {
                         }
                }

                Rectangle {
                    Layout.fillHeight: true
                    Layout.maximumWidth: inputRectangle.height
                    width: inputRectangle.height

                    color: "#FBE0E0"
                    radius: s(GStyle.buttonRadius)

                    border {
                        width: sr(GStyle.controlDefaultBorderWidth)
                        color: GStyle.buttonBorderColor
                    }
                }
            }
        }
    }
}
