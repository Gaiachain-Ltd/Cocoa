import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.types 1.0

import "../../common" as Common
import "../../headers" as Headers
import "../headers" as CharcoalHeaders
import "../../pages" as Pages

Pages.SupplyChainPageBase {
    id: top

    title: Strings.loggingEnding

    proceedButtonEnabled: (plotIdComboBox.currentText.length > 0
                           && numberOfTreesHeader.inputText.length > 0)

    Component.onCompleted: refreshData()

    function refreshData() {
        dataManager.unusedPlotIdsModel.refresh()
        dataManager.minimumDateModel.plotId = ""
    }

    function proceed() {
        pageManager.enter(Enums.Page.SupplyChainSummary, {
                              "supplyChainPage": this,
                              "summary": summary(),
                              "proceedButtonText": Strings.updatePlotId
                          });
    }

    function summary() {
        var summary = [
                    Utility.createSummaryItem(Strings.plotId, plotIdComboBox.currentText,
                                      "", "",
                                      GStyle.delegateHighlightColor,
                                      GStyle.fontHighlightColor,
                                      "",
                                      Enums.DelegateType.Standard),
                    Utility.createSummaryItem(Strings.malebiRepsId, repsIdInputHeader.inputText),
                    Utility.createSummaryItem(Strings.endingDate,
                                      endingDateHeader.selectedDate.toLocaleDateString(
                                          Qt.locale(), Strings.dateFormat)),
                    Utility.createSummaryItem(Strings.numberOfTrees, numberOfTreesHeader.inputText),
                    Utility.createSummaryItem(Strings.gpsCoordinates, gpsSource.coordinate.toString())
                ]

        return summary
    }

    function addAction() {
        dataManager.actionController.registerLoggingEnding(
                    (gpsSource.coordinate? gpsSource.coordinate
                                         : QtPositioning.coordinate()),
                    new Date,
                    endingDateHeader.selectedDate,
                    repsIdInputHeader.inputText,
                    plotIdComboBox.currentText,
                    numberOfTreesHeader.inputText)

        pageManager.enter(Enums.Page.MainMenu)
    }

    CharcoalHeaders.CharcoalComboBoxHeader {
        id: plotIdComboBox
        Layout.fillWidth: true
        headerText: Strings.plotId
        helpButtonVisible: true
        helpText: Strings.loggingEndingPlotIdHelp
        popupTitle: Strings.selectPlotId

        checkIcon: GStyle.checkBlueUrl
        delegateTextColor: GStyle.fontHighlightColor

        model: dataManager.unusedPlotIdsModel

        onCurrentTextChanged: dataManager.minimumDateModel.plotId = currentText
    }

    CharcoalHeaders.UserInfoHeader {
        id: repsIdInputHeader
        Layout.fillWidth: true
        helpText: Strings.loggingEndingRepsIdHelp
    }

    CharcoalHeaders.CharcoalInputDateHeader {
        id: endingDateHeader
        Layout.fillWidth: true
        headerText: Strings.endingDate
        helpButtonVisible: true
        helpText: Strings.loggingEndingEndingDateHelp
        minimumDate: dataManager.minimumDateModel.date
    }

    Headers.InputHeader {
        id: numberOfTreesHeader
        Layout.fillWidth: true
        headerText: Strings.numberOfTrees
        helpButtonVisible: true
        helpText: Strings.loggingEndingNumberOfTreesHelp
        validator: IntValidator {
            bottom: 0
            top: 32767
        }
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
        helpText: Strings.loggingEndingGpsHelp

        Layout.fillWidth: true

        headerText: Strings.gpsCoordinates
        inputText: (gpsSource.validCoordinate ? Helper.formatCoordinate(gpsSource.coordinate.toString()) : gpsSource.errorMessage())
        iconSource: (gpsSource.validCoordinate ? GStyle.gpsOkImgUrl : GStyle.gpsFailedImgUrl)

        onClicked: gpsSource.update()
    }
}