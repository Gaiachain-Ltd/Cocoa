import QtQuick 2.12
import QtQuick.Controls 2.12

import com.gaiachain.style 1.0

import "../items" as Items
import "../popups" as Popups

Popups.GPopup {
    id: top

    property int openedInterval: GStyle.notificationPopupOpenedDefaultInterval

    function forceClose() {
        popupTimer.stop()
        close()
    }

    onOpened: popupTimer.start()
    onClosed: pageManager.closePopup()

    Item {
        Timer {
            id: popupTimer

            interval: top.openedInterval
            onTriggered: top.close()
        }
    }
}
