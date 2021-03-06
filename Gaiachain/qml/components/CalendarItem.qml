import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items
import "../components" as Components

Item {
    id: top

    property alias additionalContent: dataContent.data
    property alias todayIndicator: todayIndicator
    property alias dayNumberColor: numberText.color

    property Item gridItem
    property int currentMonth: model.month

    property bool isCurrentMonth: (currentMonth === model.month)
    property bool isToday: model.today && isCurrentMonth

    implicitHeight: s(GStyle.calendarDayMinHeight)
    implicitWidth: s(GStyle.calendarDayMinHeight)

    Components.TodayIndicator {
        id: todayIndicator
        anchors.centerIn: parent

        visible: isToday
    }

    ColumnLayout {
        id: dataContent
        anchors.centerIn: parent

        Items.GText {
            id: numberText

            Layout.alignment: Qt.AlignHCenter

            verticalAlignment: Text.AlignTop

            text: model.day
            font.pixelSize: s(GStyle.calendarNumberPixelSize)
            color: top.isCurrentMonth ? GStyle.textPrimaryColor : GStyle.textDisabledColor
        }
    }
}
