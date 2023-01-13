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
import "."

Item {
    id: rootItem
    anchors.fill: parent

    Rectangle {
        anchors.fill: parent
        color: Style.menuBgColor

        Text {
            anchors.centerIn: parent
            visible: repeater.count < 1
            color: Style.menuTextColor
            text: qsTr("No images found")
        }

        MouseArea {
            anchors.fill: parent
            onClicked: rootItem.visible = false
        }

        Text {
            id: titleText
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            color: Style.menuTextColor
            text: qsTr("Choose image")
        }

        Rectangle {
            id: imageRect
            width: parent.width - parent.width / 5
            height: parent.height - parent.height / 5
            anchors.top: titleText.bottom
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            border.width: 4
            border.color: Style.highlightColor
            radius: 15
            color: Style.menuBgColor

            Flickable {
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.topMargin: 10
                anchors.rightMargin: 10
                anchors.bottomMargin: 10
                contentHeight: imageGrid.height
                contentWidth: imageGrid.width
                clip: true

                Grid {
                    id: imageGrid
                    anchors.horizontalCenter: parent.horizontalCenter
                    flow: Grid.TopToBottom
                    columns: 4
                    spacing: 2
                    clip: true

                    Repeater {
                        id: repeater
                        model: gameEngine.imageList
                        Image {
                            id: image
                            width: imageRect.width / 4
                            height: width
                            fillMode: Image.PreserveAspectCrop
                            source: modelData
                            cache: false

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    coverRect.visible = true
                                    gameEngine.setImage(image.source)
                                }
                            }
                        }
                    }
                }
            }
            Rectangle {
                id: coverRect
                anchors.fill: parent
                color: "#000"
                opacity: 0.8
                radius: 15
                visible: false

                Connections {
                    target: gameEngine
                    onImageLoaded: {
                        rootItem.visible = false
                        coverRect.visible = false
                    }
                }

                Text {
                    id: loadingText
                    anchors.centerIn: parent
                    font.pointSize: 30
                    color: Style.buttonTextColor
                    text: qsTr("preparing image...")
                }

                MouseArea {
                    anchors.fill: parent
                }
            }
        }
    }
}
