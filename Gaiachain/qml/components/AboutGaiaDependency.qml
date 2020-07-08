import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import com.gaiachain.style 1.0

import "../items" as Items

Item {
    id: top

    ListModel {
        id: dependencies

        ListElement {
            name: "Qt"
            version: "v5.14"
            license: "LGPL"
            licenseUrl: "https://doc.qt.io/qt-5/lgpl.html"
        }

        ListElement {
            name: "OpenSSL"
            version: "v1.1.1"
            license: "Apache License v2"
            licenseUrl: "https://www.openssl.org/source/license.html"
        }

        ListElement {
            name: "QZXing"
            version: "v2.4"
            license: "Apache License v2"
            licenseUrl: "https://github.com/ftylitak/qzxing/blob/master/LICENSE"
        }
    }

    implicitWidth: mainLayout.implicitWidth
    implicitHeight: mainLayout.implicitHeight

    clip: true

    ColumnLayout {
        id: mainLayout

        anchors.fill: parent
        spacing: s(GStyle.tinyMargin)

        Items.GTabBar {
            id: tabBar

            Layout.fillWidth: true

            Repeater {
                model: dependencies

                Items.GTabButton {
                    text: name
                }
            }
        }

        SwipeView {
            Layout.fillWidth: true

            currentIndex: tabBar.currentIndex

            Repeater {
                model: dependencies

                AboutGaiaDependencyInfo {
                    infoVersion: version
                    infoLicense: license
                    infoLicenseUrl: licenseUrl
                }
            }
        }
    }
}