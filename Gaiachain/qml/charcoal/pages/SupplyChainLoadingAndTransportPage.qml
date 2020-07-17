import QtQuick 2.14
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../../items" as Items
import "../../common" as Common
import "../../headers" as Headers
import "../headers" as CharcoalHeaders
import "../../pages" as Pages

Pages.SupplyChainPageBase {
    id: top

    property string transportId

    property var scannedQrs: []

    proceedButtonEnabled: (scannedQrs.length > 0
                           && harvestIdComboBox.currentText.length > 0
                           && plateNumberHeader.inputText.length > 0
                           && deliveryDestinationComboBox.currentText.length > 0)

    function prepareScannedIds() {
        let result = []
        for (let qr of scannedQrs) {
            result.push([harvestIdComboBox.currentText + "/B"
                         + Utility.constDigitsNumber(
                             scannedQrs.indexOf(qr), 3), qr])
        }
        return result;
    }

    title: Strings.loadingAndTransport

    Component.onCompleted: refreshData()

    function refreshData() {
        dataManager.unusedHarvestIdsModel.refresh()
        dataManager.destinationsModel.refresh()
        dataManager.minimumDateModel.plotId = ""
    }

    function proceed() {
        pageManager.enter(Enums.Page.SupplyChainSummary, {
                              "supplyChainPage": this,
                              "summary": summary(),
                              "proceedButtonText": Strings.proceed
                          });
    }

    function summary() {
        transportId = dataManager.actionController.generateTransportId(
                        harvestIdComboBox.currentText,
                        plateNumberHeader.inputText,
                        dataManager.actionController.nextTransportNumber(
                            harvestIdComboBox.currentText),
                        loadingDateHeader.selectedDate
                    )
        var summary = [
                    Utility.createSummaryItem(
                        Strings.transportId,
                        transportId,
                        "", "",
                        GStyle.delegateHighlightColor4,
                        GStyle.fontHighlightColor4,
                        GStyle.fontHighlightColor4,
                        Enums.DelegateType.Standard,
                        false),
                    Utility.createSummaryItem(
                        "",
                        [
                        [ scannedQrs.length ],
                        [ Strings.numberOfBags ]
                        ],
                        "", "",
                        GStyle.delegateHighlightColor4,
                        GStyle.fontHighlightColor4,
                        GStyle.textPrimaryColor,
                        Enums.DelegateType.ColumnStack,
                        true
                        ),
                    Utility.createSummaryItem(
                        Strings.harvestId,
                        harvestIdComboBox.currentText,
                        "", "",
                        GStyle.delegateHighlightColor2,
                        GStyle.fontHighlightColor2,
                        GStyle.fontHighlightColor2,
                        Enums.DelegateType.Standard,
                        true),
                    Utility.createSummaryItem(Strings.plateNumber,
                                              plateNumberHeader.inputText),
                    Utility.createSummaryItem(
                        Strings.loadingDate,
                        loadingDateHeader.selectedDate.toLocaleDateString(
                            Qt.locale(), Strings.dateFormat)),
                    Utility.createSummaryItem(Strings.deliveryDestination,
                                      deliveryDestinationComboBox.currentText),
                    Utility.createSummaryItem(Strings.gpsCoordinates,
                                              gpsSource.coordinateString)
                ]
        return summary
    }

    function addAction() {
        dataManager.actionController.registerLoadingAndTransport(
                    (gpsSource.coordinate? gpsSource.coordinate
                                         : QtPositioning.coordinate()),
                    new Date,
                    loadingDateHeader.selectedDate,
                    userManager.userData.code,
                    transportId,
                    harvestIdComboBox.currentText,
                    plateNumberHeader.inputText,
                    deliveryDestinationComboBox.currentText,
                    scannedQrs
                    )

        pageManager.enter(Enums.Page.MainMenu)
    }

    CharcoalHeaders.CharcoalComboBoxHeader {
        id: harvestIdComboBox
        Layout.fillWidth: true
        headerText: Strings.harvestId
        helpButtonVisible: true
        helpText: Strings.loadingAndTransportHarvestIdHelp
        popupTitle: Strings.selectHarvestId

        checkIcon: GStyle.checkGreenUrl
        delegateTextColor: GStyle.fontHighlightColor2

        model: dataManager.unusedHarvestIdsModel

        onCurrentTextChanged: dataManager.minimumDateModel.plotId = currentText
    }

    Headers.InputHeader {
        id: plateNumberHeader
        Layout.fillWidth: true
        headerText: Strings.plateNumber
        helpButtonVisible: true
        helpText: Strings.loadingAndTransportPlateNumberHelp
        validator: RegularExpressionValidator {
            regularExpression: /[0-9A-F]{1,8}+/
        }
    }

    CharcoalHeaders.CharcoalButtonHeader {
        id: loadAndScanBagsHeader

        Layout.fillWidth: true
        forceBoldValue: true
        valueFontSize: s(GStyle.titlePixelSize)
        text: Strings.loadAndScanBags
        onClicked: pageManager.enter(
                       Enums.Page.QRScanner,
                       {
                           "title": top.title,
                           "infoText": Strings.scanAllBagsInfoText,
                           "backToPage":
                           Enums.Page.SupplyChainLoadingAndTransport,
                           "infoImages": [ GStyle.bagsLoadingUrl ],
                           "showProceedPage": true,
                           "truckId": plateNumberHeader.inputText,
                           "idBase": harvestIdComboBox.currentText,
                           "scannedQrs": scannedQrs
                       })
    }

    CharcoalHeaders.CharcoalInputDateHeader {
        id: loadingDateHeader
        Layout.fillWidth: true
        headerText: Strings.loadingDate
        helpButtonVisible: true
        helpText: Strings.loadingAndTransportLoadingDateHelp
        minimumDate: dataManager.minimumDateModel.date
    }

    CharcoalHeaders.CharcoalComboBoxHeader {
        id: deliveryDestinationComboBox
        Layout.fillWidth: true
        headerText: Strings.deliveryDestination
        helpButtonVisible: true
        helpText: Strings.loadingAndTransportDeliveryDestinationHelp
        popupTitle: Strings.selectDeliveryDestination

        model: dataManager.destinationsModel
    }

    Common.PositionSourceHandler {
        id: gpsSource

        function errorMessage() {
            if (noAccess) {
                return Strings.enableGpsLocation
            } else if (!valid) {
                return Strings.gpsNotAvailable
            } else if (!positioningSupported) {
                return Strings.gpsTurnedOff
            } else {
                return Strings.gpsInvalid
            }
        }
    }

    Headers.ButtonInputHeader {
        id: gpsCoordinatesButtonInputHeader
        helpButtonVisible: true
        helpText: Strings.loadingAndTransportGpsHelp

        Layout.fillWidth: true

        headerText: Strings.gpsCoordinates
        inputText: (gpsSource.validCoordinate ? Helper.formatCoordinate(gpsSource.coordinateString) : gpsSource.errorMessage())
        iconSource: (gpsSource.validCoordinate ? GStyle.gpsOkImgUrl : GStyle.gpsFailedImgUrl)

        onClicked: gpsSource.update()
    }
}



