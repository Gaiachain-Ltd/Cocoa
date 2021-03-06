import QtQuick 2.12
import QtQuick.Layouts 1.12

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items
import "../headers" as Headers

ColumnLayout {
    property bool isSummaryMode
    property var summary

    spacing: 0

    Repeater {
        model: summary

        delegate: Loader {
            Layout.fillWidth: true

            readonly property var value: summary[index].value
            readonly property int delegateType: summary[index].delegateType
            readonly property string headerValue: summary[index].headerValue
            readonly property string inputIconSource: summary[index].inputIconSource
            readonly property string suffixValue: summary[index].suffixValue
            readonly property color highlightColor: summary[index].highlightColor
            readonly property color decorationColor: summary[index].decorationColor
            readonly property color secondaryFontColor: summary[index].secondaryTextColor
            readonly property bool isHighlighted: summary[index].isHighlighted
            readonly property bool hasSecondaryTextColor: summary[index].hasSecondaryTextColor
            readonly property bool hasUnderline: summary[index].hasUnderline

            sourceComponent: {
                if (delegateType === Enums.DelegateType.Column) {
                    return columnComponent
                } else if (delegateType === Enums.DelegateType.ColumnStack) {
                    return columnStackComponent
                } else if (delegateType === Enums.DelegateType.Row) {
                    return rowComponent
                } else if (delegateType === Enums.DelegateType.Collapsible) {
                    return collapsibleComponent
                } else if (delegateType === Enums.DelegateType.BeginEndRow) {
                    return beginEndRowComponent
                } else if (delegateType === Enums.DelegateType.Standard) {
                    return standardComponent
                }

                console.warn("No handling specified for delegate type:",
                             delegateType)
            }
        }
    }

    Component {
        id: columnComponent
        Headers.ColumnHeader {
            readonly property var composite: value
            Layout.fillWidth: true

            secondaryColor: isHighlighted? decorationColor
                                         : GStyle.separatorColor
            secondaryTextColor: hasSecondaryTextColor? secondaryFontColor
                                                     : GStyle.textReadonlyColor
            backgroundColor: isHighlighted? highlightColor : GStyle.backgroundColor
            headerText: headerValue
            titles: composite[0]
            values: composite[1]
            icons: composite[2]
            readOnly: true
            summaryMode: isSummaryMode
            highlighted: isHighlighted
            underlineVisible: hasUnderline
            extraLeftMargin: !isHighlighted
        }
    }

    Component {
        id: columnStackComponent
        Headers.ColumnStackHeader {
            readonly property var composite: value
            Layout.fillWidth: true

            secondaryColor: isHighlighted? decorationColor
                                         : GStyle.separatorColor
            secondaryTextColor: hasSecondaryTextColor? secondaryFontColor
                                                     : GStyle.textReadonlyColor
            backgroundColor: isHighlighted? highlightColor : GStyle.backgroundColor
            headerText: headerValue
            titles: composite[0]
            values: composite[1]
            icons: composite[2]
            links: composite[3]
            linkDatas: composite[4]
            readOnly: true
            summaryMode: isSummaryMode
            highlighted: isHighlighted
            underlineVisible: hasUnderline
            extraLeftMargin: !isHighlighted
        }
    }

    Component {
        id: rowComponent
        Headers.RowHeader {
            readonly property var composite: value
            Layout.fillWidth: true

            secondaryColor: isHighlighted? decorationColor
                                         : GStyle.separatorColor
            secondaryTextColor: hasSecondaryTextColor? secondaryFontColor
                                                     : GStyle.textReadonlyColor
            backgroundColor: isHighlighted? highlightColor : GStyle.backgroundColor
            headerText: headerValue
            titles: composite[0]
            values: composite[1]
            icons: composite[2]
            readOnly: true
            summaryMode: isSummaryMode
            highlighted: isHighlighted
            underlineVisible: hasUnderline
            extraLeftMargin: !isHighlighted
        }
    }

    Component {
        id: collapsibleComponent

        Headers.CollapsibleHeader {
            Layout.fillWidth: true

            headerText: headerValue
            summary: value
        }
    }

    Component {
        id: beginEndRowComponent

        Headers.BeginEndRowHeader {
            readonly property var composite: value
            Layout.fillWidth: true

            secondaryColor: isHighlighted? decorationColor
                                         : GStyle.separatorColor
            secondaryTextColor: hasSecondaryTextColor? secondaryFontColor
                                                     : GStyle.textReadonlyColor
            backgroundColor: isHighlighted? highlightColor : GStyle.backgroundColor
            headerTexts: composite[0]
            inputTexts: composite[1]
            summaryMode: isSummaryMode
            highlighted: isHighlighted
            extraLeftMargin: !isHighlighted
        }
    }

    Component {
        id: standardComponent

        Headers.TextHeader {
            Layout.fillWidth: true

            secondaryColor: isHighlighted? decorationColor
                                         : GStyle.separatorColor
            secondaryTextColor: hasSecondaryTextColor? secondaryFontColor
                                                       : GStyle.textReadonlyColor
            backgroundColor: isHighlighted? highlightColor : GStyle.backgroundColor
            headerText: headerValue
            text: value
            summaryMode: isSummaryMode
            highlighted: isHighlighted
            underlineVisible: hasUnderline
            extraLeftMargin: !isHighlighted
        }
    }
}
