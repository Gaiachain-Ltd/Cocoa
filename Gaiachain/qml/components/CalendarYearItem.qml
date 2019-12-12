import QtQuick 2.12
import QtQuick.Layouts 1.12

import Qt.labs.calendar 1.0

import com.gaiachain.style 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items

Item {
    id: top

    implicitWidth: mainLayout.implicitWidth
    implicitHeight: mainLayout.implicitHeight

    readonly property date currentDate: new Date
    readonly property int monthColumns: 4

    property int year

    signal dateClicked(int year, int month)

    ListModel {
        id: monthModel

        function initData() {
            for (var month = Calendar.January; month <= Calendar.December; ++month) {
                append({"shortName": Helper.formatShortName(Qt.locale().monthName(month, Locale.ShortFormat)), "month": month})
            }
        }

        Component.onCompleted: {
            initData()
        }
    }

    function isCurrentDate(month) {
        return (currentDate.getFullYear() === year &&  currentDate.getMonth() === month)
    }

    ColumnLayout {
        id: mainLayout

        anchors.fill: parent

        spacing: s(Style.hugeMargin)

        Items.BasicText {
            text: year
            font.bold: true
        }

        GridLayout {
            columns: monthColumns
            columnSpacing: s(Style.middleSmallMargin)

            rowSpacing: s(Style.middleSmallMargin)

            Repeater {
                model: monthModel

                Items.GenericButton {
                    Layout.fillWidth: true

                    palette {
                        button: Style.blank
                        buttonText: Style.textPrimaryColor
                    }

                    font {
                        pixelSize: s(Style.pixelSize)
                        capitalization: Font.MixedCase
                        letterSpacing: Style.none
                        weight: Font.Normal
                    }

                    borderRadius: s(Style.tinyMargin)
                    borderColor: isCurrentDate(month) ? Style.headerBackgroundColor : Style.inputBorderColor
                    borderWidth: isCurrentDate(month) ? sr(Style.controlBoldBorderWidth) : sr(Style.controlDefaultBorderWidth)

                    text: shortName

                    onClicked: dateClicked(year, month)
                }
            }
        }
    }
}
