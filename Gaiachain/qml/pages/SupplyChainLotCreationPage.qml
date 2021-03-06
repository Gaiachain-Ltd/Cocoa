import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.types 1.0

import "../items" as Items
import "../headers" as Headers
import "../components" as Components
import "../pages" as Pages

Pages.SupplyChainPage {
    id: top

    title: Strings.supplyChainMenuActionLotCreation

    property string scannedId

    validPageData: !(lotIdComboBox.currentText === Strings.empty ||
                     qrCodeInputHeader.inputText === Strings.empty)

    Component.onCompleted: refreshData()

    function refreshData() {
        sessionManager.getUnusedLotIds()
    }

    function proceed() {
        pageManager.enter(Enums.Page.SupplyChainSummary, { "supplyChainPage": this, "summary": summary() })
    }

    function summary() {
       return [
            Utility.createSummaryItem(Strings.gpsCoordinates, gpsCoordinates, GStyle.gpsImgUrl),
            Utility.createSummaryItem(Strings.lotIdForThePackage, lotIdComboBox.currentText),
            Utility.createSummaryItem(Strings.registerQrCodeToLotId, qrCodeInputHeader.inputText),
            Utility.createSummaryItem(Strings.description, descriptionHeader.inputText)
       ]
    }

    function addAction() {
        showOverlay()

        var lotId = lotIdComboBox.currentText
        var codeData = qrCodeInputHeader.inputText
        var notes = descriptionHeader.inputText

        var properties = {
            [PackageDataProperties.Notes]: notes
        }

        top.packageId = lotId
        dataManager.addAction(lotId,
                              Enums.SupplyChainAction.LotCreation,
                              codeData,
                              coordinate(),
                              new Date,
                              properties)
    }

    Headers.ComboBoxHeader {
        id: lotIdComboBox

        Layout.fillWidth: true

        headerText: Strings.lotIdForThePackage

        displayRole: "packageId"
        model: unusedLotIdsModel
    }

    Headers.ButtonInputHeader {
        id: qrCodeInputHeader

        Layout.fillWidth: true

        iconSource: GStyle.qrImgUrl

        inputText: top.scannedId
        headerText: Strings.registerQrCodeToLotId
        placeholderText: Strings.scanQrCodeFrom.arg(Strings.lot.toUpperCase())

        onWholeInputClicked: pageManager.enter(Enums.Page.QRScanner, {
                                         "title": title,
                                         "backSupplyChainPage": page,
                                         "popupText": Strings.attachQr.arg(Strings.lot.toUpperCase()) })
    }

    Headers.TextAreaHeader {
        id: descriptionHeader

        Layout.fillWidth: true

        placeholderText: Strings.typeHere
        headerText: Strings.description
    }
}
