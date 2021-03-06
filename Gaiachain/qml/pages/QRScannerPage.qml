import QtQuick 2.11
import QtQuick.Layouts 1.11
import QtMultimedia 5.9
import QZXing 2.3

import com.gaiachain.enums 1.0
import com.gaiachain.style 1.0
import com.gaiachain.static 1.0
import com.gaiachain.helpers 1.0
import com.gaiachain.platforms 1.0

import "../items" as Items
import "../components" as Components

GPage {
    id: top

    property string scannedId
    property string popupText

    property bool displayId: false

    property int scanStatus: Enums.QRScanStatus.Unknown
    property int backSupplyChainPage: Enums.Page.InvalidPage

    Component.onCompleted: {
        if (typeof(AndroidPermissionsHandler) !== "undefined") {
            AndroidPermissionsHandler.requestPermission(AndroidPermissionsHandler.Camera)
        }
        pageManager.openPopup(Enums.Popup.Text, { "text": top.popupText })
    }

    function parseScannedId(id) {
        if (Utility.validateId(id)) {
            scannedId = Utility.formatRawId(id)
            scanStatus = Enums.QRScanStatus.Success
        } else {
            scannedId = id
            scanStatus = Enums.QRScanStatus.Failed
            console.warn("Wrong code content", id)
        }
    }

    function backToHomeHandler() {
        pageManager.openPopup(Enums.Popup.Confirm, { "text": Strings.askForExit }, "EXIT_CONFIRM")
    }

    function backHandler() {
        if (scanStatus === Enums.QRScanStatus.Success) {
            pageManager.backTo(backSupplyChainPage, { "scannedId": top.scannedId })
        } else {
            pageManager.back()
        }

        return false    // do not close application
    }

    Connections {
        target: pageManager
        enabled: pageManager.isOnTop(page)

        onPopupAction: {
            if (popupId != "EXIT_CONFIRM") {
                return
            }

            switch (action) {
            case Enums.PopupAction.Accept:
                pageManager.backTo(pageManager.homePage())
                break
            default:
                break
            }
        }

        onPopupClosed: {
            switch (popup) {
            case Enums.Popup.Text:
                scanStatus = Enums.QRScanStatus.Scanning
                break
            default:
                break
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent

        spacing: GStyle.none

        Camera {
            id: camera

            captureMode: Camera.CaptureVideo
            focus {
                focusMode: CameraFocus.FocusContinuous
                focusPointMode: CameraFocus.FocusPointAuto
            }
        }

        QZXingFilter {
            id: zxingFilter

            property real normalizedScanSize: Static.normalizedScanSize
            property real normalizedScanPos: (1.0 - normalizedScanSize) * 0.5

            captureRect: {
                videoOutput.contentRect
                videoOutput.sourceRect

                return videoOutput.mapRectToSource(videoOutput.mapNormalizedRectToItem(
                                                       Qt.rect(normalizedScanPos, normalizedScanPos, normalizedScanSize, normalizedScanSize)))
            }

            decoder {
                enabledDecoders: QZXing.DecoderFormat_QR_CODE
                onTagFound: {
                    if (scanStatus === Enums.QRScanStatus.Scanning && !qrStatus.manual) {
                        parseScannedId(tag)
                    }
                }
            }
        }

        VideoOutput {
            id: videoOutput

            Layout.fillWidth: true
            Layout.fillHeight: true

            source: camera
            filters: [zxingFilter]
            fillMode: VideoOutput.PreserveAspectCrop
            autoOrientation: true
        }

        Items.QRStatus {
            id: qrStatus

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom

            implicitHeight: GStyle.none

            scanning: (scanStatus === Enums.QRScanStatus.Scanning)

            onTypedManually: parseScannedId(manualId)

            function text() {
                switch (scanStatus) {
                    case Enums.QRScanStatus.Unknown: return Strings.empty
                    case Enums.QRScanStatus.Success: return Strings.success
                    case Enums.QRScanStatus.Failed: return Strings.error
                    case Enums.QRScanStatus.Scanning: return Strings.scanning + "..."
                }
            }

            text: text()

            Behavior on text {
                Components.ChangeAnimation {
                    target: qrStatus.textItem
                    property: "opacity"
                    inValue: GStyle.visible
                    outValue: GStyle.hidden
                    duration: GStyle.animationDuration/2
                }
            }

            states: [
                State {
                    name: "unknown"; when: (scanStatus ===  Enums.QRScanStatus.Unknown)
                    PropertyChanges { target: qrStatus; implicitHeight: GStyle.none }
                },

                State {
                    name: "scanning"; when: (scanStatus === Enums.QRScanStatus.Scanning)
                    PropertyChanges { target: qrStatus; color: GStyle.backgroundColor; textColor: GStyle.textPrimaryColor; implicitHeight: s(GStyle.footerHeight) }
                },

                State {
                    name: "success"; when: (scanStatus === Enums.QRScanStatus.Success)
                    PropertyChanges { target: qrStatus; color: GStyle.primaryColor; textColor: GStyle.textSecondaryColor; implicitHeight: s(GStyle.footerHeight) }
                },

                State {
                    name: "failed"; when: (scanStatus === Enums.QRScanStatus.Failed)
                    PropertyChanges { target: qrStatus; color: GStyle.errorColor; textColor: GStyle.textSecondaryColor; implicitHeight: s(GStyle.footerHeight) }
                }
            ]

            transitions: Transition {
                ParallelAnimation {
                    ColorAnimation { target: qrStatus; duration: GStyle.animationDuration; easing.type: GStyle.animationEasing }
                    PropertyAnimation { target: qrStatus; property: "implicitHeight"; duration: GStyle.animationDuration; easing.type: GStyle.animationEasing }
                }
            }
        }
    }

    Items.PureImageButton {
        parent: videoOutput

        width: s(Static.frameSvgImgHeight);
        height: s(Static.frameSvgImgHeight)

        source: !qrStatus.manual ? GStyle.frameImgUrl : GStyle.qrImgUrl
        anchors.centerIn: parent

        onClicked: {
            if (qrStatus.manual) {
                qrStatus.manual = false
            }
        }

        Items.PureImageButton {
            id: statusPureImageButton

            function allow() {
                return (scanStatus === Enums.QRScanStatus.Success || scanStatus === Enums.QRScanStatus.Failed)
            }

            function source() {
                switch (scanStatus) {
                    case Enums.QRScanStatus.Success: return GStyle.checkImgUrl
                    case Enums.QRScanStatus.Failed: return GStyle.refreshImgUrl
                    case Enums.QRScanStatus.Scanning: case Enums.QRScanStatus.Unknown: return Strings.empty
                }
            }

            width: s(Static.checkSvgImageHeight)
            height: s(Static.checkSvgImageHeight)

            opacity: allow()
            enabled: allow()
            source: source()

            anchors.centerIn: parent

            onClicked: {
                switch (scanStatus) {
                    case Enums.QRScanStatus.Success: pageManager.backTo(backSupplyChainPage, { "scannedId": top.scannedId }); break
                    case Enums.QRScanStatus.Failed: scanStatus = Enums.QRScanStatus.Scanning; break
                }
            }

            Behavior on opacity {
                NumberAnimation { duration: GStyle.animationDuration; easing.type: GStyle.animationEasing }
            }
        }

        Items.GText {
            id: displayIdText

            text: top.scannedId
            color: GStyle.textSecondaryColor
            visible: (top.displayId && scanStatus === Enums.QRScanStatus.Success)
            font.bold: true

            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.bottom
                topMargin: s(GStyle.middleBigMargin)
            }
        }
    }
}
