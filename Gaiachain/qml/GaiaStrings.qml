pragma Singleton

import QtQuick 2.11

QtObject {
    readonly property string gaiachain: qsTr("Gaiachain")

    readonly property string timber: qsTr("Timber")
    readonly property string charcoal: qsTr("Charcoal")
    readonly property string cocoa: qsTr("Cocoa")

    readonly property string calendar: qsTr("Calendar")
    readonly property string map: qsTr("Map")
    readonly property string list: qsTr("List")

    readonly property string registerRequest: qsTr("Please register")
    readonly property string login: qsTr("Login")
    readonly property string skipLogin: qsTr("Skip login")
    readonly property string emailAddress: qsTr("Email address")
    readonly property string password: qsTr("Password")

    readonly property string january: qsTr("January")
    readonly property string february: qsTr("February")
    readonly property string march: qsTr("March")
    readonly property string april: qsTr("April")
    readonly property string may: qsTr("May")
    readonly property string june: qsTr("June")
    readonly property string july: qsTr("July")
    readonly property string august: qsTr("August")
    readonly property string september: qsTr("September")
    readonly property string october: qsTr("October")
    readonly property string november: qsTr("November")
    readonly property string december: qsTr("December")

    // QR
    readonly property string scanning: qsTr("Scanning")
    readonly property string scanFailed: qsTr("Scan failed")
    readonly property string typeId: qsTr("Type ID")

    readonly property string id: qsTr("ID")
    readonly property string batch: qsTr("Batch")

    // Event Details Page
    readonly property string companyName: qsTr("Company name")
    readonly property string gpsHarvestLocation: qsTr("Location of harvest (GPS point)")
    readonly property string harvestDate: qsTr("Date of harvest")
    readonly property string logID: qsTr("Log ID")
    readonly property string dateTimeDeparture: qsTr("Date and time of departure")

    // popup
    readonly property string close: qsTr("Close")
    readonly property string logout: qsTr("Logout")
    readonly property string cancel: qsTr("Cancel")
    readonly property string exit: qsTr("Exit")
    readonly property string save: qsTr("Save")

    readonly property string loginErrorInfo: qsTr("Login failed. Try again.")

    readonly property string saveQuestion: qsTr("Do you want to save your changes?")
    readonly property string exitWithoutSaveQuestion: qsTr("Are you sure do you want to exit without saving?")
    readonly property string logoutQuestion: qsTr("Do you want to logout?")

}
