import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

import com.gaiachain.style 1.0
import com.gaiachain.enums 1.0
import com.gaiachain.helpers 1.0

import "../items" as Items
import "../components" as Components

Components.PackageDataDetails {
    id: top

    onPackageDataChanged: {
        const data = [
           { "headerValue": Strings.nameOfProducer,      "inputValue": emptyIfNotDefnied(packageData.properties[packageData.ProducerName]) },
           { "headerValue": Strings.producerIdNumber,    "inputValue": emptyIfNotDefnied(packageData.properties[packageData.ProducerId]) },
           { "headerValue": Strings.village,             "inputValue": emptyIfNotDefnied(packageData.properties[packageData.Village]) },
           { "headerValue": Strings.fieldCode,           "inputValue": emptyIfNotDefnied(packageData.properties[packageData.ParcelCode]) },
           { "headerValue": Strings.harvestDate,         "inputValue": emptyIfNotIncludesAction(Enums.SupplyChainAction.Harvest) },
           { "headerValue": Strings.dateOfBreaking,      "inputValue": emptyIfNotDate(packageData.properties[packageData.BreakingDate]) }
       ]

        for (var i = 0; i < data.length; ++i) {
            detailsModel.append(data[i])
        }
    }

    ListModel {
        id: detailsModel
    }

    Repeater {
        model: detailsModel

        Items.InputHeader {
            Layout.fillWidth: true

            color: Style.headerBackgroundColor
            headerText: headerValue
            inputText: inputValue
        }
    }
}
