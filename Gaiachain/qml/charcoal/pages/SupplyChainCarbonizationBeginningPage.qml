import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.items 1.0

import "../../common" as Common
import "../../headers" as Headers
import "../headers" as CharcoalHeaders
import "../items" as CharcoalItems
import "../../pages" as Pages

Pages.SupplyChainPageBase {
    id: top

    property var ovenDimensions: dataManager.actionController.emptyOvenDimensions

    readonly property string fixedDimensionsOven: "2"
    readonly property string variableDimensionsOven: "1"

    title: Strings.carbonizationBeginning

    // 1 - Traditional oven
    // 2 - Metallic oven
    // These ids come from Web :shrug:
    proceedButtonEnabled: (ovenIdHeader.inputText.length > 0
                           && plotIdComboBox.currentText.length > 0
                           && (ovenTypeComboBox.ovenType === fixedDimensionsOven
                               || (ovenTypeComboBox.ovenType === variableDimensionsOven
                                   && ovenDimensions.count() === 4)))

    Component.onCompleted: refreshData()

    function refreshData() {
        dataManager.unusedPlotIdsForCarbonizationModel.refresh()
        dataManager.ovenTypesModel.refresh()
        dataManager.minimumDateModel.plotId = -1
    }

    function proceed() {
        pageManager.enter(Enums.Page.SupplyChainSummary, {
                              "supplyChainPage": this,
                              "summary": summary(),
                              "proceedButtonText": Strings.proceed
                          });
    }

    function summary() {
        let dims = []
        let unit = " m";

        dims.push(ovenDimensions.width + unit)
        dims.push(ovenDimensions.length + unit)
        dims.push(ovenDimensions.height1 + unit)
        if (ovenDimensions.count() === 4) {
            dims.push(ovenDimensions.height2 + unit)
        }

        var summary = [
                    Utility.createSummaryItem(
                        Strings.ovenId,
                        [
                            [ ovenIdHeader.inputText ],
                            [ ovenTypeComboBox.currentText
                             + " - "
                             + ovenDimensions.volume() + " m³"
                            ]
                        ],
                        "", "",
                        GStyle.delegateHighlightColor3,
                        GStyle.fontHighlightColor3,
                        GStyle.fontHighlightColor3,
                        Enums.DelegateType.Column,
                        false)
                ]

        if (ovenDimensions.hasFixedDimensions) {
            summary.push(Utility.createSummaryItem(
                             "",
                             [ ovenDimensionsHeader.titles, dims ],
                             "", "",
                             GStyle.delegateHighlightColor3,
                             GStyle.fontHighlightColor3,
                             GStyle.fontHighlightColor3,
                             Enums.DelegateType.Row,
                             true)
                         )
        } else {
            let row1 = [ ovenDimensionsHeader.titles[0], ovenDimensionsHeader.titles[1] ]
            let row2 = [ ovenDimensionsHeader.titles[2], ovenDimensionsHeader.titles[3] ]
            let val1 = [ dims[0], dims[1] ]
            let val2 = [ dims[2], dims[3] ]
            summary.push(Utility.createSummaryItem(
                             "",
                             [ row1, val1 ],
                             "", "",
                             GStyle.delegateHighlightColor3,
                             GStyle.fontHighlightColor3,
                             GStyle.fontHighlightColor3,
                             Enums.DelegateType.Row,
                             false),
                         Utility.createSummaryItem(
                             "",
                             [ row2, val2 ],
                             "", "",
                             GStyle.delegateHighlightColor3,
                             GStyle.fontHighlightColor3,
                             GStyle.fontHighlightColor3,
                             Enums.DelegateType.Row,
                             true),
                         )
        }

        summary.push(
                    Utility.createSummaryItem(
                        Strings.harvestId,
                        dataManager.actionController.generateHarvestId(
                            plotIdComboBox.currentText,
                            carbonizerIdInputHeader.inputText
                            ),
                        "", "",
                        GStyle.delegateHighlightColor2,
                        GStyle.fontHighlightColor2,
                        GStyle.fontHighlightColor2,
                        Enums.DelegateType.Standard,
                        true),
                    Utility.createSummaryItem(
                        Strings.plotId,
                        plotIdComboBox.currentText,
                        "", "",
                        GStyle.delegateHighlightColor,
                        GStyle.fontHighlightColor,
                        GStyle.fontHighlightColor,
                        Enums.DelegateType.Standard,
                        true),
                    Utility.createSummaryItem(Strings.carbonizerId,
                                              carbonizerIdInputHeader.inputText),
                    Utility.createSummaryItem(
                        Strings.beginningDate,
                        beginningDateHeader.selectedDate.toLocaleDateString(
                            Qt.locale(), Strings.dateFormat)),
                    Utility.createSummaryItem(Strings.ovenType,
                                              ovenTypeComboBox.currentText),
                    Utility.createSummaryItem(Strings.gpsCoordinates,
                                              gpsSource.coordinateString)
                    )

        return summary
    }

    function addAction() {
        if (dataManager.actionController.registerCarbonizationBeginning(
                    (gpsSource.coordinate? gpsSource.coordinate
                                         : QtPositioning.coordinate()),
                    new Date,
                    beginningDateHeader.selectedDate,
                    carbonizerIdInputHeader.inputText,
                    plotIdComboBox.currentText,
                    plotIdComboBox.currentId,
                    ovenIdHeader.inputText,
                    ovenTypeComboBox.ovenIdNumber,
                    ovenDimensions))
        {
            pageManager.enterPageAndPopup(Enums.Page.MainMenu, {}, false,
                                          Enums.Popup.Notification,
                                          {
                                              "text": Strings.harvestCreated,
                                              "backgroundColor": GStyle.okColor
                                          })
        }
        else
        {
            pageManager.enter(Enums.Page.MainMenu)
        }
    }

    CharcoalHeaders.CharcoalComboBoxHeader {
        id: plotIdComboBox
        Layout.fillWidth: true
        headerText: Strings.plotId
        helpButtonVisible: true
        helpText: Strings.carbonizationBeginningPlotIdHelp
        popupTitle: Strings.selectPlotId

        checkIcon: GStyle.checkBlueUrl
        delegateTextColor: GStyle.fontHighlightColor

        model: dataManager.unusedPlotIdsForCarbonizationModel

        onCurrentTextChanged: {
            ovenIdHeader.inputText = dataManager.actionController.nextOvenNumber(currentId)
            dataManager.minimumDateModel.plotId = currentId
        }
    }

    Headers.InputHeader {
        id: ovenIdHeader
        Layout.fillWidth: true
        headerText: Strings.ovenId
        helpButtonVisible: true
        helpText: Strings.carbonizationBeginningOvenIdHelp
        readOnly: true
    }

    CharcoalHeaders.UserInfoHeader {
        id: carbonizerIdInputHeader
        Layout.fillWidth: true
        headerText: Strings.carbonizerId
        helpText: Strings.carbonizationBeginningCarbonizerIdHelp
    }

    CharcoalHeaders.CharcoalInputDateHeader {
        id: beginningDateHeader
        Layout.fillWidth: true
        headerText: Strings.beginningDate
        helpButtonVisible: true
        helpText: Strings.carbonizationBeginningBeginningDateHelp
        minimumDate: dataManager.minimumDateModel.date
    }

    CharcoalHeaders.OvenTypeComboBox {
        id: ovenTypeComboBox
        Layout.fillWidth: true
        headerText: Strings.ovenType
        helpButtonVisible: true
        helpText: Strings.carbonizationBeginningOvenTypeHelp
        popupTitle: Strings.selectOvenType

        model: dataManager.ovenTypesModel

        onCurrentTextChanged: {
            if (ovenType === fixedDimensionsOven) {
                ovenDimensions = dataManager.actionController.defaultOvenDimensions(
                            ovenIdNumber)
            } else {
                ovenDimensions = dataManager.actionController.emptyOvenDimensions
            }
        }
    }

    CharcoalHeaders.CharcoalButtonHeader {
        property var titles: (ovenTypeComboBox.ovenType === fixedDimensionsOven)?
             [ Strings.width, Strings.length, Strings.height ]
           : [ Strings.width, Strings.length, Strings.heightA, Strings.heightB ]

        id: ovenDimensionsHeader

        Layout.fillWidth: true

        headerText: Strings.ovenDimensions
        helpButtonVisible: true
        helpText: Strings.carbonizationBeginningOvenDimensionsHelp
        enabled: ovenTypeComboBox.isTraditional

        text: ovenDimensions.volume() < 0? Strings.set : (ovenDimensions.volume() + "m³")
        extraText: ""
        iconVisible: false

        onClicked: pageManager.enter(
                       Enums.Page.SetOvenMeasurements,
                       {
                           "backToPage": Enums.Page.SupplyChainCarbonizationBeginning,
                           "ovenDimensions": top.ovenDimensions
                       })
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
        helpText: Strings.carbonizationBeginningGpsHelp

        Layout.fillWidth: true

        headerText: Strings.gpsCoordinates
        inputText: (gpsSource.validCoordinate ? Helper.formatCoordinate(gpsSource.coordinateString) : gpsSource.errorMessage())
        iconSource: (gpsSource.validCoordinate ? GStyle.gpsOkImgUrl : GStyle.gpsFailedImgUrl)

        onWholeInputClicked: gpsSource.update()
    }
}
