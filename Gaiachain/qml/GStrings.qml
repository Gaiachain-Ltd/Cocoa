pragma Singleton

import QtQuick 2.12

QtObject {
    // general
    property string gaiachain: tr.empty + qsTr("Gaiachain")

    property string ok: tr.empty + qsTr("Ok")
    property string cancel: tr.empty + qsTr("Cancel")
    property string yes: tr.empty + qsTr("Yes")
    property string no: tr.empty + qsTr("No")
    property string save: tr.empty + qsTr("Save")
    property string clear: tr.empty + qsTr("Clear")

    property string empty: ""

    property string success: tr.empty + qsTr("Success")
    property string failed: tr.empty + qsTr("Failed")

    property string harvestId: tr.empty + qsTr("Harvest ID")
    property string sacId: tr.empty + qsTr("Sac ID")
    property string lotId: tr.empty + qsTr("Lot ID")

    property string sac: tr.empty + qsTr("sac")
    property string lot: tr.empty + qsTr("lot")

    property string details: tr.empty + qsTr("%1 details")

    property string kg: tr.empty + qsTr("%1 kg")

    property string transport: tr.empty + qsTr("Transport")
    property string reception: tr.empty + qsTr("Reception")

    property string january: tr.empty + qsTr("January")
    property string february: tr.empty + qsTr("February")
    property string march: tr.empty + qsTr("March")
    property string april: tr.empty + qsTr("April")
    property string may: tr.empty + qsTr("May")
    property string june: tr.empty + qsTr("June")
    property string july: tr.empty + qsTr("July")
    property string august: tr.empty + qsTr("August")
    property string september: tr.empty + qsTr("September")
    property string october: tr.empty + qsTr("October")
    property string november: tr.empty + qsTr("November")
    property string december: tr.empty + qsTr("December")

    property string aboutApp: "<h2>%1</h2><br/>v. %2 (%3)<br/><a href=\"http://%4\">http://%4</a>"
    property string userInfo: tr.empty + qsTr("Current user") + ":<br/><b>%1</b>"

    // login
    property string emailAddress: tr.empty + qsTr("Email address")
    property string password: tr.empty + qsTr("Password")
    property string login: tr.empty + qsTr("Login")
    property string skipLogin: tr.empty + qsTr("Skip login")

    property string loading: tr.empty + qsTr("Loading")
    property string anonymousUser: tr.empty + qsTr("Anonymous")
    property string searching: tr.empty + qsTr("<b>Searching...</b>")
    property string online: tr.empty + qsTr("<b>Online</b>")
    property string offline: tr.empty + qsTr("<b>Offline</b>")
    property string onlineUserInfo: "<b>%1</b><br/>%2"

    property string serverConnectionError: tr.empty + qsTr("Server connection error.\nCheck your internet connection.")
    property string loginError: tr.empty + qsTr("Login failed. Try again.")
    property string offlineModeQuestion: tr.empty + qsTr("There is no internet connection available at the moment. Would you like to continue with an offline mode?")
    property string onlineLogoutQuestion: tr.empty + qsTr("The internet connection is now available. Do you want to log in online mode?")

    property string dbUpdateProgress: tr.empty + qsTr("Database updating. Please wait...")
    property string dbUpdateError: tr.empty + qsTr("Database update failed. If the error repeats, try to clean the application data in the system.")

    // main menu
    property string supplyChain: tr.empty + qsTr("Supply chain")
    property string transactions: tr.empty + qsTr("Transactions")
    property string calendar: tr.empty + qsTr("Calendar")
    property string waitingTransactions: tr.empty + qsTr("Waiting transactions")

    property string supplyChainMenuActionHarvest: tr.empty + qsTr("Harvest")
    property string supplyChainMenuActionGrainProcessing: tr.empty + qsTr("Breaking, Fermentation, Drying")
    property string supplyChainMenuActionBagging: tr.empty + qsTr("Bagging")
    property string supplyChainMenuActionLotCreation: tr.empty + qsTr("Lot creation")

    property string supplyChainMenuActionTransport: tr.empty + qsTr("Transport")
    property string supplyChainMenuActionWarehouseTransport: tr.empty + qsTr("to Central Warehouse")

    property string supplyChainMenuActionReception: tr.empty + qsTr("Reception")
    property string supplyChainMenuActionSectionReception: tr.empty + qsTr("at the Section")
    property string supplyChainMenuActionExportReception: tr.empty + qsTr("in Cooperative HQ")

    // events action description
    property string actionHarvestDescriptionStatus: tr.empty + qsTr("Harvest done | Harvest ID created")
    property string actionGrainProcessingDescriptionStatus: tr.empty + qsTr("Breaking, fermentation and drying finished")
    property string actionSectionReceptionDescriptionStatus: tr.empty + qsTr("Received at the section")
    property string actionBaggingDescriptionStatus: tr.empty + qsTr("Bagging finished | Sac ID created")
    property string actionLotCreationDescriptionStatus: tr.empty + qsTr("Lot ID created")
    property string actionWarehouseTransportDescriptionStatus: tr.empty + qsTr("Transport sent to Central Warehouse")
    property string actionExportReceptionDescriptionStatus: tr.empty + qsTr("Received in the Cooperative HQ")

    // transactions page
    property string searchForTransaction: tr.empty + qsTr("Search for a transaction")
    property string onlyMyTransactions: tr.empty + qsTr("Only my transactions")
    property string recentTransactions: tr.empty + qsTr("Recent transactions")

    // supply chain pages
    property string search: tr.empty + qsTr("Search")
    property string proceed: tr.empty + qsTr("Proceed")

    property string addActionErrorOffline: tr.empty + qsTr("Cannot reach the server. Please try again later.")
    property string addActionErrorUnknown: tr.empty + qsTr("Cannot add the action to the server.")
    property string addActionErrorMissing: tr.empty + qsTr("Data is missing. Please check your input and try again.")
    property string addActionErrorDuplicated: tr.empty + qsTr("The action already exists. Please check your input and try again.")
    property string addActionErrorLocalData: tr.empty + qsTr("Cannot add the action to the server. Local action data was corrupted! Possible attempt to change the action data!")
    property string offlineActionAdded: tr.empty + qsTr("Created in offline mode. It will be added automatically to the database when the application is online.")

    property string toSelect: tr.empty + qsTr("To select")
    property string askForExit: tr.empty + qsTr("Are you sure you want to exit?")
    property string askForActionDelete: tr.empty + qsTr("Are you sure you want to delete the action?")

    property string gpsCoordinates: tr.empty + qsTr("GPS coordinates")
    property string gpsInvalid: tr.empty + qsTr("Please check your GPS signal")
    property string gpsTurnedOff: tr.empty + qsTr("GPS turned off")
    property string gpsNotAvailable: tr.empty + qsTr("GPS not available")
    property string enableGpsLocation: tr.empty + qsTr("Enable GPS location")

    // package data details pages
    property string nameOfProducer: tr.empty + qsTr("Name of the producer")
    property string producerIdNumber: tr.empty + qsTr("Producer ID number")
    property string village: tr.empty + qsTr("Village")
    property string parcelCode: tr.empty + qsTr("Parcel code")
    property string harvestDate: tr.empty + qsTr("Harvest date")
    property string breakingDate: tr.empty + qsTr("Breaking date")
    property string estimatedBeansVolume: tr.empty + qsTr("Estimated volume on fresh beans kg")

    property string organicCocoaTransporter: tr.empty + qsTr("Organic cocoa transporter")
    property string destination: tr.empty + qsTr("Destination")
    property string transportDate: tr.empty + qsTr("Transport date")

    property string selectedAutomatically: tr.empty + qsTr("[selected automatically]")
    property string selectProducer: tr.empty + qsTr("[select producer]")

    property string noEntries: tr.empty + qsTr("No valid entries")
    property string notRequired: tr.empty + qsTr("not required")

    property string endFermentationDate: tr.empty + qsTr("End of fermentation date")
    property string typeHere: tr.empty + qsTr("Type here")

    property string receptionAtSection: tr.empty + qsTr("Reception at the section")
    property string receptionDate: tr.empty + qsTr("Reception date")
    property string buyer: tr.empty + qsTr("Buyer")

    property string lotIdTheSackBelongsTo: tr.empty + qsTr("LOT ID the sack belongs to")
    property string nameCooperative: tr.empty + qsTr("Cooperative name")
    property string addHarvestId: tr.empty + qsTr("Add Harvest ID")
    property string addLot: tr.empty + qsTr("Add lot")
    property string alreadyAdded: tr.empty + qsTr("%1 already added!")
    property string bagWeight: tr.empty + qsTr("Bag weight")
    property string cocoaFromSeveralPlotsComment: tr.empty + qsTr("If there is cocoa from several plots in the bag, please add the other Harvest IDs")

    property string lotIdForThePackage: tr.empty + qsTr("LOT ID for the package")
    property string registerQrCodeToLotId: tr.empty + qsTr("Register QR code to LOT ID")
    property string description: tr.empty + qsTr("Description")

    property string transportToCentralWarehouse: tr.empty + qsTr("Transport to Central Warehouse")

    property string receptionInCooperativeHQ: tr.empty + qsTr("Reception in Cooperative HQ")
    property string lotWeight: tr.empty + qsTr("Weight of LOT")

    property string summary: tr.empty + qsTr("Summary")
    property string confirm: tr.empty + qsTr("Confirm")

    // qr code page
    property string qrCode: tr.empty + qsTr("QR Code")
    property string scanning: tr.empty + qsTr("Scanning")
    property string error: tr.empty + qsTr("Error")
    property string scanQrCodeFrom: tr.empty + qsTr("Scan QR code from %1")
    property string attachQr: tr.empty + qsTr("Attach the QR code to the %1 and scan the QR code")

    // calendar
    property string activities: tr.empty + qsTr("activities")

    // special
    property string dateFormat: tr.empty + ((mainController.flavor === "cocoa")?
                                                qsTr("MM/dd/yyyy") : qsTr("dd/MM/yyyy"))
    property string idDateFormat: tr.empty + qsTr("dd-MM-yyyy")
    property string checkMark: "\u2713"
    property string minus: "-"

    // miscellaneous
    property string height: tr.empty + qsTr("height")
    property string length: tr.empty + qsTr("length")
    property string width: tr.empty + qsTr("width")
    property string heightA: tr.empty + qsTr("height A")
    property string heightB: tr.empty + qsTr("height B")

    // Help
    property string helpTitle: tr.empty + qsTr("What is %1?")

    // Charcoal
    property string tracking: tr.empty + qsTr("Tracking")
    property string replantation: tr.empty + qsTr("Replantation")
    property string logging: tr.empty + qsTr("Logging")
    property string loggingBeginning: tr.empty + qsTr("Logging beginning")
    property string loggingEnding: tr.empty + qsTr("Logging ending")
    property string carbonization: tr.empty + qsTr("Carbonization")
    property string carbonizationBeginning: tr.empty + qsTr("Carbonization beginning")
    property string carbonizationEnding: tr.empty + qsTr("Carbonization ending")
    property string loadingAndTransport: tr.empty + qsTr("Loading and \ntransport")
    property string finalReception: tr.empty + qsTr("Final reception")
    property string sellAtTheLocalMarket: tr.empty + qsTr("Sell at the local market")

    // Charcoal - confirmations
    property string plotCreated: tr.empty + qsTr("Plot ID created")
    property string plotUpdated: tr.empty + qsTr("Plot ID updated")
    property string harvestCreated: tr.empty + qsTr("Harvest ID created")
    property string harvestUpdated: tr.empty + qsTr("Harvest ID updated")
    property string transportCreated: tr.empty + qsTr("Transport ID created")
    property string transportUpdated: tr.empty + qsTr("Transport ID updated")


    // Charcoal - user info
    property string userInfoPageTitle: tr.empty + qsTr("Malebi rep's ID details")
    property string userInfoName: tr.empty + qsTr("Name and surname")
    property string userInfoJob: tr.empty + qsTr("Function")
    property string userInfoContact: tr.empty + qsTr("Contact")
    property string userInfoCode: tr.empty + qsTr("Code")

    // Charcoal - logging beginning page
    property string parcel: tr.empty + qsTr("Parcel")
    property string malebiRepsId: tr.empty + qsTr("Malebi Rep's ID")
    //property string village: tr.empty + qsTr("Village")
    property string treeSpecies: tr.empty + qsTr("Tree species")
    property string beginningDate: tr.empty + qsTr("Beginning date")
    property string createPlotId: tr.empty + qsTr("Create plot ID")
    property string plotId: tr.empty + qsTr("Plot ID")
    property string selectParcel: tr.empty + qsTr("Select parcel")
    property string selectVillage: tr.empty + qsTr("Select village")
    property string selectTreeSpecies: tr.empty + qsTr("Select tree species")
    property string noActiveParcels: tr.empty + qsTr("All parcels in use or no parcels added in web panel")

    property string loggingBeginningParcelHelp: tr.empty + qsTr("Dummy text - parcel help")
    property string loggingBeginningRepsIdHelp: tr.empty + qsTr("Dummy text - Malebi representative ID help")
    property string loggingBeginningVillageHelp: tr.empty + qsTr("Dummy text - village help")
    property string loggingBeginningTreeSpeciesHelp: tr.empty + qsTr("Dummy text - tree species help")
    property string loggingBeginningDateHelp: tr.empty + qsTr("Dummy text - beginning date help")
    property string loggingBeginningGpsHelp: tr.empty + qsTr("Dummy text - GPS position help")

    // Charcoal - logging ending page
    property string endingDate: tr.empty + qsTr("Ending date")
    property string numberOfTrees: tr.empty + qsTr("Number of trees")
    property string numberOfTreesCut: tr.empty + qsTr("Number of trees cut")
    property string tapToAddATreeBeingCut: tr.empty + qsTr("Tap to add a tree being cut")
    property string totalCountOfTreesCut: tr.empty + qsTr("Total count of trees cut")
    property string updatePlotId: tr.empty + qsTr("Update Plot ID")

    property string loggingEndingPlotIdHelp: tr.empty + qsTr("Dummy text - plot id help")
    property string loggingEndingRepsIdHelp: tr.empty + qsTr("Dummy text - Malebi representative ID help")
    property string loggingEndingEndingDateHelp: tr.empty + qsTr("Dummy text - ending date help")
    property string loggingEndingNumberOfTreesHelp: tr.empty + qsTr("Dummy text - number of trees help")
    property string loggingEndingGpsHelp: tr.empty + qsTr("Dummy text - GPS position help")

    // Charcoal - carbonization beginning page
    property string ovenId: tr.empty + qsTr("Oven ID")
    property string carbonizerId: tr.empty + qsTr("Carbonizer ID")
    property string timberVolume: tr.empty + qsTr("Timber volume")
    property string ovenType: tr.empty + qsTr("Oven type")
    property string ovenDimensions: tr.empty + qsTr("Oven measurement (meters)")
    property string selectPlotId: tr.empty + qsTr("Select plot ID")
    property string selectOvenType: tr.empty + qsTr("Select oven type")

    property string set: tr.empty + qsTr("Set")
    property string setOvenMeasurements: tr.empty + qsTr("Set oven measurements")
    property string widthPlaceholder: tr.empty + qsTr("Width (A)")
    property string lengthPlaceholder: tr.empty + qsTr("Length (B)")
    property string height1Placeholder: tr.empty + qsTr("Height (C)")
    property string height2Placeholder: tr.empty + qsTr("Height (D)")

    property string carbonizationBeginningPlotIdHelp: tr.empty + qsTr("Dummy text - plot id help")
    property string carbonizationBeginningOvenIdHelp: tr.empty + qsTr("Dummy text - oven id help")
    property string carbonizationBeginningCarbonizerIdHelp: tr.empty + qsTr("Dummy text - carbonizer id help")
    property string carbonizationBeginningBeginningDateHelp: tr.empty + qsTr("Dummy text - beginning date help")
    property string carbonizationBeginningOvenTypeHelp: tr.empty + qsTr("Dummy text - oven type help")
    property string carbonizationBeginningOvenDimensionsHelp: tr.empty + qsTr("Dummy text - oven measurements help")
    property string carbonizationBeginningGpsHelp: tr.empty + qsTr("Dummy text - GPS position help")

    // Charcoal - carbonization ending page
    property string carbonizationEndingDate: tr.empty + qsTr("Carbonization end date")
    property string selectOvenId: tr.empty + qsTr("Select oven ID")
    property string selectHarvestId: tr.empty + qsTr("Select harvest ID")

    property string carbonizationEndingHarvestIdHelp: tr.empty + qsTr("Dummy text - harvest ID help")
    property string carbonizationEndingOvenIdHelp: tr.empty + qsTr("Dummy text - oven ID help")
    property string carbonizationEndingCarbonizerIdHelp: tr.empty + qsTr("Dummy text -  carbonizer ID help")
    property string carbonizationEndingEndingDateHelp: tr.empty + qsTr("Dummy text - carbonization ending date help")
    property string carbonizationEndingGpsHelp: tr.empty + qsTr("Dummy text - GPS help")

    // Charcoal - loading and transport page
    property string transportId: tr.empty + qsTr("Transport Id")
    property string numberOfBags: tr.empty + qsTr("Number of bags")
    property string plateNumber: tr.empty + qsTr("Plate number")
    property string loadAndScanBags: tr.empty + qsTr("Load & scan bags")
    property string loadingDate: tr.empty + qsTr("Loading date")
    property string deliveryDestination: tr.empty + qsTr("Delivery destination")
    property string selectDeliveryDestination: tr.empty + qsTr("Select delivery destination")
    property string greenBagCount: "<font color=\"#65cb00\">%1</font>"

    property string loadingAndTransportHarvestIdHelp: tr.empty + qsTr("Dummy text - harvest ID help")
    property string loadingAndTransportPlateNumberHelp: tr.empty + qsTr("Dummy text - plate number help")
    property string loadingAndTransportLoadingDateHelp: tr.empty + qsTr("Dummy text - loading date help")
    property string loadingAndTransportDeliveryDestinationHelp: tr.empty + qsTr("Dummy text - delivery destination help")
    property string loadingAndTransportGpsHelp: tr.empty + qsTr("Dummy text - GPS help")
    property string pausedLoading: tr.empty + qsTr("You have a truck which has not been fully loaded. Please finish it")

    // Charcoal - reception at a local market page
    property string scanBagsYouWantToSell: tr.empty + qsTr("Scan bags you want to sell at the local market")
    property string scanBagsInfoText: tr.empty + qsTr("Scan QR code from bags sold on the local market")
    property string numberOfBagsSold: tr.empty + qsTr("Numer of bags sold")
    property string numberOfBagsLeft: tr.empty + qsTr("Number of bags left on truck")
    property string duplicatedBagError: tr.empty + qsTr("Duplicated bag - at least one of the QR codes has already been scanned")

    // Charcoal - final reception page
    property string numberOfBagsReceived: tr.empty + qsTr("Number of bags received")
    property string totalNumberOfBags: tr.empty + qsTr("Total number of bags")
    property string receiveDocumentsAndReceipt: tr.empty + qsTr("Receive documents and receipt")
    property string scanAllBagsFromTruck: tr.empty + qsTr("Scan all bags from truck")
    property string receptionDateCharcoal: tr.empty + qsTr("Time & Date stamp")

    property string documents: tr.empty + qsTr("Documents")
    property string receipt: tr.empty + qsTr("Receipt")
    property string approved: tr.empty + qsTr("Approved")
    property string noPhoto: tr.empty + qsTr("No photo")
    property string numberOfBagsDetail: tr.empty + qsTr("%1 of %2")
    property string greenDocsCount: tr.empty + qsTr("<font color=\"#65cb00\">Documents: %1<br/>Receipts: %2</font>")

    property string receptionUnloadingDateHelp: tr.empty + qsTr("Dummy text - unloading timestamp help")
    property string receptionGpsHelp: tr.empty + qsTr("Dummy text - GPS help")

    // Charcoal - QrMismatchSummaryPage
    property string missingBags: tr.empty + qsTr("Missing bags")
    property string extraBags: tr.empty + qsTr("Extra bags not found in transport")

    // Charcoal - supply chain finalization page
    property string doYouWantToCompleteSupplyChain: tr.empty + qsTr("Do you want to complete this supply chain?")
    property string totalNumberOfScannedBags: tr.empty + qsTr("total number of scanned bags")
    property string totalNumberOfRegisteredTrucks: tr.empty + qsTr("total number of registered trucks")

    // Charcoal - tracking page
    property string allTab: tr.empty + qsTr("All")
    property string plotTab: tr.empty + qsTr("Plot")
    property string harvestTab: tr.empty + qsTr("Harvest")
    property string transportTab: tr.empty + qsTr("Transport")

    // Charcoal - replantation page
    property string numberOfTreesPlanted: tr.empty + qsTr("Number of trees planted")
    property string tapToAddATreeBeingPlanted: tr.empty + qsTr("Tap to add a tree being planted")
    property string totalCountOfTreesPlanted: tr.empty + qsTr("Total count of trees planted")
    property string userId: tr.empty + qsTr("User Id")

    property string replantationPlotIdHelp: tr.empty + qsTr("Dummy text - plot ID help")
    property string replantationNumberOfTreesHelp: tr.empty + qsTr("Dummy text - number of trees planted help")
    property string replantationTreeSpeciesHelp: tr.empty + qsTr("Dummy text - tree species help")
    property string replantationUserIdHelp: tr.empty + qsTr("Dummy text - user id help")
    property string replantationBeginningDateHelp: tr.empty + qsTr("Dummy text - beginning date help")
    property string replantationEndingDateHelp: tr.empty + qsTr("Dummy text - ending date help")
    property string replantationGpsHelp: tr.empty + qsTr("Dummy text - GPS help")

    // QR
    property string qrCodeAlreadyAssigned: tr.empty + qsTr("QR code already assigned!")
    property string qrCodeScannedSuccessfully: tr.empty + qsTr("QR code scanned successfully")
    property string doYouGoToTheNextOven: tr.empty + qsTr("Do you go to the next oven?")

    property string fixQrCodeToEachBagInfoText: tr.empty + qsTr("Fix QR code to each bag, then scan the qr code in order to assign it to the bag")
    property string scanAllBagsInfoText: tr.empty + qsTr("Scan all bags' QR codes that go to selected truck")
    property string scanAllBagsToCheckInfoText: tr.empty + qsTr("Scan QR code from all bags on truck to check transport")

    property string deleteAllScannedCodes: tr.empty + qsTr("Delete all scanned QR codes")
    property string scannedBagsPopupText: tr.empty + qsTr("You have <font color=\"#65cb00\">%1</font> scanned bags in your local working copy")
    property string scannedBagsSavePopupText: tr.empty + qsTr("You have <font color=\"#65cb00\">%1</font> scanned bags. Do you want to save them?")

    property string clearAllScannedQrCodes: tr.empty + qsTr("Do you want to <font color=\"#E40000\">clear</font> all scanned QR codes?")

    property string deleteText: tr.empty + qsTr("Delete")
    property string restore: tr.empty + qsTr("Restore")

    // Taking photos
    property string takePhotoDocumentsInfoText: tr.empty + qsTr("Take a photo of the signature and stamp on the permit and the Carnet de Voyage by the authorities")
    property string takePhotoReceiptInfoText: tr.empty + qsTr("Take a photo of the receipt")
    property string skip: tr.empty + qsTr("Skip")
    property string takeAPhoto: tr.empty + qsTr("Take a photo")
    property string takeAPhotoDocument: tr.empty + qsTr("Scan a document")
    property string takeAPhotoReceipt: tr.empty + qsTr("Scan a receipt")

    property string photoTaken: tr.empty + qsTr("Photo taken")
    property string uploadThis: tr.empty + qsTr("Upload this?")
    property string keepThePhoto: tr.empty + qsTr("Keep the photo?")
    property string keepTheReceipt: tr.empty + qsTr("Keep the receipt?")

    property string photoAdded: tr.empty + qsTr("Photo added")
    property string photoAddedDocument: tr.empty + qsTr("Document added")
    property string photoAddedReceipt: tr.empty + qsTr("Receipt added")

    property string photoDiscarded: tr.empty + qsTr("Photo discarded")
    property string addAnother: tr.empty + qsTr("Add another?")
    property string addAnotherDocument: tr.empty + qsTr("Add another document?")
    property string addAnotherReceipt: tr.empty + qsTr("Add another receipt?")
    property string cameraErrorString: tr.empty + qsTr("Camera error: %1")

    // Charcoal misc
    property string selectDate: tr.empty + qsTr("Select date")
    property string cantSelectThisDay: tr.empty + qsTr("You can't select this day")

    property string plotIdDetails: tr.empty + qsTr("Plot ID details")
    property string harvestIdDetails: tr.empty + qsTr("Harvest ID details")
    property string transportIdDetails: tr.empty + qsTr("Transport ID details")
    property string transportReceptionDate: tr.empty + qsTr("Reception at the storage facility")
    property string uploaded: tr.empty + qsTr("Uploaded")

    property string select: tr.empty + qsTr("Select")

    // Notifications
    property string redirect: tr.empty + qsTr("Go to: %1")
}
