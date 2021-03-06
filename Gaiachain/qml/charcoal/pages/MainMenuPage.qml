import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

import "../../pages"
import "../../items" as Items
import "../components" as Components

GPage {
    id: top

    title: Strings.gaiachain
    logoVisible: true

    readonly property int userType: userManager.userData.type

    function closeEventHandler() {
        return true // android back button will close app
    }

    function supplyChainPageForAction(action) {
        switch (Number(action)) {
        case Enums.SupplyChainAction.LoggingBeginning:
            return Enums.Page.SupplyChainLoggingBeginning;
        case Enums.SupplyChainAction.LoggingEnding:
            return Enums.Page.SupplyChainLoggingEnding;
        case Enums.SupplyChainAction.CarbonizationBeginning:
            return Enums.Page.SupplyChainCarbonizationBeginning;
        case Enums.SupplyChainAction.CarbonizationEnding:
            return Enums.Page.SupplyChainCarbonizationEnding;
        case Enums.SupplyChainAction.LoadingAndTransport:
            return Enums.Page.SupplyChainLoadingAndTransport;
        case Enums.SupplyChainAction.Reception:
            return Enums.Page.SupplyChainReception;
        case Enums.SupplyChainAction.Tracking:
            return Enums.Page.Tracking;
        case Enums.SupplyChainAction.Replantation:
            return Enums.Page.Replantation;
        default:
            console.warn("navigateSupplyChainPages: Invalid action provided!")
        }
        return Enums.Page.InvalidPage;
    }

    function navigateSupplyChainPages(action) {
        var supplyChainPage = supplyChainPageForAction(action)
        if (supplyChainPage !== Number(Enums.Page.InvalidPage)) {
            pageManager.enter(supplyChainPage, { "action": action })
            return true
        }
        return false
    }

    Component.onCompleted: {
        if (mainController.flavor === "cocoa") {
            if (!userManager.offlineMode && localEventsModel.size) {
                dataManager.sendOfflineActions()
            }

            sessionManager.getAdditionalData()
        } else {
            dataManager.sendOfflineActions()
        }
    }

    Flickable {
        anchors.fill: parent

        contentHeight: menuColumn.height
        boundsBehavior: Flickable.StopAtBounds

        Column {
            id: menuColumn

            width: parent.width

            spacing: 0

            Components.ExpandableMenuButton {
                width: parent.width

                text: Strings.supplyChain
                icon: GStyle.menuSupplyChainUrl
                color: GStyle.menuSupplyChainColor

                visible: userType !== Enums.UserType.Anonymous

                onClicked: supplyChainSubmenu.visible = !supplyChainSubmenu.visible
            }

            Column {
                id: supplyChainSubmenu
                spacing: 0
                visible: false

                width: parent.width

                Components.DoubleMenuButton {
                    width: parent.width

                    visible: userType === Enums.UserType.SuperUser
                             || userType === Enums.UserType.Logger
                             || userType === Enums.UserType.Carbonizer
                             || userType === Enums.UserType.Anonymous

                    text: Strings.logging
                    color: GStyle.submenuLoggingColor
                    icon: GStyle.submenuLoggingUrl
                    fontColor: GStyle.textPrimaryColor
                    fontBold: false

                    leftText: Strings.loggingBeginning
                    leftColor: GStyle.submenuLoggingBeginningColor
                    leftIcon:  GStyle.submenuLoggingBeginningUrl
                    onLeftClicked: pageManager.enter(
                                       Enums.Page.SupplyChainLoggingBeginning)

                    rightText: Strings.loggingEnding
                    rightColor: GStyle.submenuLoggingEndingColor
                    rightIcon:  GStyle.submenuLoggingEndingUrl
                    onRightClicked: pageManager.enter(
                                        Enums.Page.SupplyChainLoggingEnding)
                }

                Components.DoubleMenuButton {
                    width: parent.width

                    visible: userType === Enums.UserType.SuperUser
                             || userType === Enums.UserType.Carbonizer
                             || userType === Enums.UserType.Anonymous

                    text: Strings.carbonization
                    color: GStyle.submenuCarbonizationColor
                    icon: GStyle.submenuCarbonizationUrl
                    fontColor: GStyle.textPrimaryColor
                    fontBold: false

                    leftText: Strings.carbonizationBeginning
                    leftColor: GStyle.submenuCarbonizationBeginningColor
                    leftIcon:  GStyle.submenuCarbonizationBeginningUrl
                    onLeftClicked: pageManager.enter(
                                       Enums.Page.SupplyChainCarbonizationBeginning)

                    rightText: Strings.carbonizationEnding
                    rightColor: GStyle.submenuCarbonizationEndingColor
                    rightIcon:  GStyle.submenuCarbonizationEndingUrl
                    onRightClicked: pageManager.enter(
                                        Enums.Page.SupplyChainCarbonizationEnding)
                }

                Components.MenuButton {
                    width: parent.width

                    visible: userType === Enums.UserType.SuperUser
                             || userType === Enums.UserType.Carbonizer
                             || userType === Enums.UserType.Anonymous

                    text: Strings.loadingAndTransport
                    icon: GStyle.submenuLoadingAndTransportUrl
                    color: GStyle.submenuLoadingAndTransportColor
                    fontColor: GStyle.textPrimaryColor
                    fontBold: false

                    onClicked: pageManager.enter(
                                   Enums.Page.SupplyChainLoadingAndTransport)
                }

                Components.DoubleMenuButton {
                    width: parent.width

                    visible: userType === Enums.UserType.SuperUser
                             || userType === Enums.UserType.Anonymous

                    text: Strings.reception
                    color: GStyle.submenuReceptionColor
                    icon: GStyle.submenuReceptionUrl
                    fontColor: GStyle.textPrimaryColor
                    fontBold: false

                    leftText: Strings.sellAtTheLocalMarket
                    leftColor: GStyle.submenuReceptionColor
                    leftIcon:  GStyle.submenuSellAtLocalMarketUrl
                    onLeftClicked: pageManager.enter(
                                       Enums.Page.SupplyChainSellAtTheLocalMarket)

                    rightText: Strings.finalReception
                    rightColor: GStyle.submenuReceptionColor
                    rightIcon:  GStyle.submenuFinalReceptionUrl
                    onRightClicked: pageManager.enter(
                                        Enums.Page.SupplyChainReception)
                }

            }

            Components.MenuButton {
                width: parent.width

                text: Strings.tracking
                icon: GStyle.menuTrackingUrl
                color: GStyle.menuTrackingColor

                onClicked: pageManager.enter(Enums.Page.PageTracking)
            }

            Components.MenuButton {
                width: parent.width

                text: Strings.replantation
                icon: GStyle.menuReplantationUrl
                color: GStyle.menuReplantationColor

                visible: userType === Enums.UserType.SuperUser

                onClicked: pageManager.enter(Enums.Page.PageReplantation)
            }
        }
    }
}

