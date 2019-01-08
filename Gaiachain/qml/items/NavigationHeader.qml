import QtQuick 2.11
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0

// TO_DO consider using Toolbar
Item {
    id: top

    signal headerClicked()

    property var currentResource: Enums.CommodityType.Timber //TO_DO set proper text after user set

    function getResourceName(res) {
        switch(res) {
        case Enums.CommodityType.Timber: return Strings.timber
        case Enums.CommodityType.Charcoal: return Strings.charcoal
        case Enums.CommodityType.Cocoa: return Strings.cocoa
        default:
            console.warn("Invalid resource type!")
        }

        return Strings.timber + "!" // Add "!" if invalid
    }

    function getResourceUrl(res, green) {
        switch (res) {
        case Enums.CommodityType.Timber: return green ? Style.miniTimberGreenImgUrl : Style.miniTimberImgUrl
            //case Enums.CommodityType.Charcoal: return Style.charcoalImgUrl
            //case Enums.CommodityType.Cocoa: return Style.cocoaImgUrl
        default:
            console.warn("Invalid resource type. Return empty url!")
        }

        return ""
    }

    function sectionToUrl(section, green, editSection) {
        if (editSection) {
            return green ? Style.editGreenImgUrl : Style.editImgUrl
        }

        switch(section) {
        case Enums.PageSections.ViewTypeSection: return getResourceUrl(currentResource, green)
        case Enums.PageSections.CalendarSection: return green ? Style.miniCalendarGreenImgUrl : Style.miniCalendarImgUrl
        case Enums.PageSections.EventsListSection: return green ? Style.miniListGreenImgUrl : Style.miniListImgUrl
        case Enums.PageSections.EventsDetailsSection: return green ? Style.detailsGreenImgUrl : Style.detailsImgUrl
        case Enums.PageSections.ShipmentDetailsSection: return green ? Style.timelineGreenImgUrl : Style.timelineImgUrl
        case Enums.PageSections.QRSection: return green ? Style.qrCodeGreenImgUrl : Style.qrCodeImgUrl

        case Enums.PageSections.DefaultSection: return ""

        default:
            console.warn("Invalid section!")
        }

        return ""
    }

    ColumnLayout {
        anchors {
            fill: parent
            leftMargin: s(40)
            rightMargin: s(40)
            topMargin: s(10)
            bottomMargin: s(10)
        }

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true

            spacing: 0

            ImageButton {
                Layout.fillHeight: true
                Layout.preferredWidth: height

                padding: s(Style.headerButtonsPadding)
                fillMode: Image.PreserveAspectFit

                // do not change `!=` to `!==`. It is here on purpose!
                property bool isLoggedUser: userManager.userType != Enums.UserType.NotLoggedUser
                property bool isOnHomePage: pageManager.isOnHomePage()
                source: {
                    if (!isOnHomePage) return Style.backImgUrl

                    return isLoggedUser ? Style.logoutImgUrl : Style.exitToLoginImgUrl
                }

                onClicked: {
                    if (isLoggedUser && isOnHomePage) {
                        pageManager.enterPopup(Enums.Page.InformationPopup, {
                                                   "text" : "Do you want to logout?",
                                                   "acceptButtonText": "Logout",
                                                   "rejectButtonText": "Cancel"})
                    } else {
                        pageManager.back()
                    }
                }
            }

            LayoutSpacer {}

            ImageButton {
                Layout.fillHeight: true
                Layout.preferredWidth: height

                padding: s(Style.headerButtonsPadding)
                fillMode: Image.PreserveAspectFit
                source: Style.homeImgUrl

                onClicked: pageManager.backTo(pageManager.homePage())
            }

            ListView {
                Layout.fillHeight: true
                Layout.preferredWidth: sectionsModel.maxSectionsDepth() * (height + s(Style.headerArrowWidth)) + s(Style.headerButtonsPadding)

                orientation: ListView.Horizontal
                interactive: false
                spacing: 0

                model: sectionsModel

                delegate: Item {
                    id: delegateId
                    height: ListView.view.height
                    width: s(Style.headerArrowWidth) + height + (delegateId.isLast ? s(Style.headerButtonsPadding) : 0)

                    property bool isLast: (ListView.view.count - 1) === index

                    RowLayout {
                        anchors.fill: parent

                        spacing: delegateId.isLast ?  s(Style.headerButtonsPadding) : 0

                        SvgImage {
                            Layout.fillHeight: true
                            Layout.preferredWidth: s(Style.headerArrowWidth)

                            fillMode: Image.PreserveAspectFit
                            source: Style.rightArrowImgUrl
                        }

                        ImageButton {
                            // From delegate size and arrow size, image button will have squere shape.
                            // Do not set width: height, to force square shape as it leads to "holes" between arrow and button.
                            Layout.fillHeight: true
                            Layout.fillWidth: true

                            padding: s(Style.headerButtonsPadding)

                            fillMode: Image.PreserveAspectFit
                            source: sectionToUrl(id, !delegateId.isLast, editSection)
                            backgroundColor: delegateId.isLast ? Style.buttonBackColor : "transparent"

                            onClicked: pageManager.backToSection(id)
                        }
                    }
                }
            }
        }

        BasicText {
            Layout.fillWidth: true

            color: Style.textGreenColor
            verticalAlignment: Text.AlignTop
            font.capitalization: Font.AllUppercase
            text: getResourceName(currentResource)
        }
    }
}
