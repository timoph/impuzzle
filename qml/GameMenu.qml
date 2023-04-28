/*
 ImPuzzle 2 - a 15-puzzle inspired game
 Copyright (C) 2021 Timo Härkönen

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

import QtQuick 2.0
import QtQuick.Controls 2.5
import "."

Item {
    id: rootItem
    implicitHeight: 370
    implicitWidth: 420

    signal selectPictureClicked()
    signal drawNumbersChanged(bool draw)
    signal morePiecesSet(bool more)

    property bool morePieces: false

    Rectangle {
        anchors.fill: parent
        color: Style.menuBgColor
        radius: 15
        border.width: 4
        border.color: Style.menuBorderColor


        MenuButton {
            id: newGameButton
            anchors.top: parent.top
            anchors.topMargin: 15
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 20
            text: qsTr("New game")

            onClicked: {
                rootItem.drawNumbersChanged(drawNumbersCheck.checked)
                rootItem.morePiecesSet(morePiecesCheck.checked)
                gameEngine.start(morePiecesCheck.checked)
            }
        }

        MenuButton {
            id: selectPictureButton
            anchors.top: newGameButton.bottom
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 20
            text: qsTr("Select image")

            onClicked: selectPictureClicked()
        }

        Text {
            id: currentImageText
            anchors.top: selectPictureButton.bottom
            anchors.topMargin: 5
            text: qsTr("image:")
            color: Style.menuTextColor
            font.pointSize: 12
            anchors.left: currentImage.left
        }

        Image {
            id: currentImage
            anchors.top: currentImageText.bottom
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 15
            width: parent.width / 3
            height: parent.height / 3
            source: gameEngine.currentImage()

            Connections {
                target: gameEngine
                onImageUrlChanged: {
                    currentImage.source = gameEngine.currentImage()
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: selectPictureClicked()
            }
        }

        SettingsCheckBox {
            id: drawNumbersCheck
            anchors.bottom: currentImage.bottom
            anchors.left: currentImage.right
            anchors.leftMargin: 10
            width: parent.width - currentImage.width - 10
            text: qsTr("Show numbers")
            checked: true

            Connections {
                target: drawNumbersCheck
                onCheckedChanged: {
                    rootItem.drawNumbersChanged(drawNumbersCheck.checked)
                }
            }
        }

        SettingsCheckBox {
            id: morePiecesCheck
            anchors.bottom: drawNumbersCheck.top
            anchors.left: currentImage.right
            anchors.leftMargin: 10
            width: parent.width - currentImage.width - 10
            text: qsTr("Make it harder!")
            checked: false

            Connections {
                target: morePiecesCheck
                onCheckedChanged: {
                    rootItem.drawNumbersChanged(morePiecesCheck.checked)
                }
            }
        }

        MenuButton {
            id: quitButton
            anchors.top: currentImage.bottom
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 20
            text: qsTr("Quit")

            onClicked: Qt.quit()
        }
    }
}
