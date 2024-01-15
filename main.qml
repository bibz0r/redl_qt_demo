import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.15


ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: "Eingabemaske"
    color: "#3c3c3c"


    Item {
        anchors.fill: parent
        Column {
            spacing: 10
            width: parent.width

            /* IP ADDRESS */
            Label {
                text: "IP-Adresse"
                width: parent.width
                color: "#ffffff"
            }
            TextField {
                id: ipInput
                placeholderText: "IP-Adresse"
                width: parent.width
                validator: RegExpValidator {
                    regExp: /^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/
                }
            }

            /* UHRZEIT */
            Label {
                text: "Uhrzeit"
                width: parent.width
                color: "#ffffff"
            }
            TextField {
                id: uhrzeitInput
                width: parent.width
                placeholderText: "Uhrzeit (HH:mm)"
                inputMask: "99:99"
            }

            /* TEXT; IRGENDWAS */
            Label {
                text: "Text-Eingabe"
                width: parent.width
                color: "#ffffff"
            }
            TextField {
                id: textInput
                width: parent.width
                placeholderText: "Text"
            }

            /* ZAHL (NUR ZAHLEN) */
            Label {
                text: "Zahlen eingabe"
                width: parent.width
                color: "#ffffff"
            }
            TextField {
                id: numberInput
                width: parent.width
                placeholderText: "Zahl"
                inputMethodHints: Qt.ImhDigitsOnly
            }


            Label {
                text: "Port-Auswahl"
                width: parent.width
                color: "#ffffff"
            }
            ComboBox {
                id: dropdownInput
                model: [8080, 443, 80]
            }


            Row {

                RadioButton {
                    id: radio1
                    text: "Dark Mode"

                    contentItem: Row {
                        spacing: 5 // Adjust the spacing as needed

                        // Custom indicator
                        Rectangle {
                            width: 20
                            height: 20
                            radius: 10
                            color: radio1.checked ? "white" : "transparent" // Adjust colors as needed
                            border.color: "white"
                            border.width: 2
                        }

                        // Text
                        Text {
                            text: radio1.text
                            color: "#ffffff" // Your desired text color
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    // Override the default indicator to prevent duplication
                    indicator: Item {}
                }



                RadioButton {
                    id: radio2
                    text: "Light Mode"

                    contentItem: Row {
                        spacing: 5 // Adjust the spacing as needed

                        // Custom indicator
                        Rectangle {
                            width: 20
                            height: 20
                            radius: 10
                            color: radio2.checked ? "white" : "transparent" // Adjust colors as needed
                            border.color: "white"
                            border.width: 2
                        }

                        // Text
                        Text {
                            text: radio2.text
                            color: "#ffffff" // Your desired text color
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    // Override the default indicator to prevent duplication
                    indicator: Item {}
                }

                // Weitere Radiobuttons hier
            }

            Row {

                spacing: 20
                anchors.horizontalCenter: parent.horizontalCenter


                Button {
                    text: "Speichern"
                    onClicked: {
                        messageDialog.visible = true; // Show the dialog
                    }

                    Layout.preferredWidth: parent.width / 2

                    // Change background color on hover using 'hovered' property
                    background: Rectangle {
                        radius: 8
                        color: parent.hovered ? "#4CAF50" : "#8BC34A"
                        border.color: "#388E3C"
                        border.width: 1
                    }

                    contentItem: Text {
                        text: parent.text
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    height: 40
                    font.pixelSize: 16

                }



                Dialog {
                    id: messageDialog
                    visible: false
                    title: "Bestätigung"

                    contentItem: Column {
                        spacing: 10
                        padding: 10

                        Text {
                            text: "Sind Sie sicher, dass Sie speichern möchten?"
                        }

                        Row {
                            spacing: 10

                            // Custom 'Yes' button
                            Button {
                                text: "Ja"
                                onClicked: {
                                    messageDialog.accept();
                                }
                            }

                            // Custom 'No' button
                            Button {
                                text: "Nein"
                                onClicked: {
                                    messageDialog.reject();
                                }
                            }
                        }
                    }

                    onAccepted: {
                        var radioWert;

                        if (radio1.checked) {
                            radioWert = "dark";
                        } else if (radio2.checked) {
                            radioWert = "light";
                        }

                        dbManager.speichereDaten(ipInput.text, uhrzeitInput.text, textInput.text, parseInt(numberInput.text), dropdownInput.currentText, radioWert);
                    }
                }

                Button {
                    text: "Zurücksetzen"
                    Layout.preferredWidth: parent.width / 2

                    // Change background color on hover using 'hovered' property
                    background: Rectangle {
                        radius: 8
                        color: parent.hovered ? "#4CAF50" : "#8BC34A"
                        border.color: "#388E3C"
                        border.width: 1
                    }

                    contentItem: Text {
                        text: parent.text
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    height: 40
                    font.pixelSize: 16
                    function inputsChanged() {
                        console.log("Current savedIp:", dbManager.savedIp);
                        return ipInput.text !== dbManager.savedIp ||
                               uhrzeitInput.text !== dbManager.savedUhrzeit ||
                               textInput.text !== dbManager.savedText ||
                               numberInput.text !== dbManager.savedZahl.toString() ||
                               dropdownInput.currentText !== dbManager.savedDropdown ||
                               (radio1.checked && dbManager.savedRadio !== "dark") ||
                               (radio2.checked && dbManager.savedRadio !== "light");
                    }

                    onClicked: {
                        console.log("Checking for changes...");
                        if (inputsChanged()) {
                            console.log("Changes detected, requesting saved data...");
                            dbManager.requestSavedData();
                        } else {
                            console.log("No changes detected.");
                        }
                    }
                }

            }





            Connections {
                target: dbManager

                onDataFetched: function(ip, uhrzeit, text, zahl, dropdown, radio) {
                    fuelleFormular(ip, uhrzeit, text, zahl, dropdown, radio);
                }
            }

        }





        Component.onCompleted: {
            console.log("dbManager ist", dbManager ? "verfügbar" : "nicht verfügbar")
            dbManager.datenGeladen.connect(fuelleFormular)
            dbManager.loadInitialData()
        }
    }

    function fuelleFormular(ip, uhrzeit, text, zahl, dropdown, radio) {
        console.log("Daten in QML erhalten:", ip, uhrzeit, text, zahl, dropdown, radio);
        ipInput.text = ip;
        uhrzeitInput.text = uhrzeit;
        textInput.text = text;
        numberInput.text = zahl.toString();

        // Set the ComboBox selection directly using the integer value
        if (dropdownInput.model.includes(dropdown)) {
                dropdownInput.currentIndex = dropdownInput.model.indexOf(dropdown);
            } else {
                dropdownInput.currentIndex = -1;
            }


        if (radio === "dark") {
            radio1.checked = true;
        } else if (radio === "light") {
            radio2.checked = true;
        }
    }
}
