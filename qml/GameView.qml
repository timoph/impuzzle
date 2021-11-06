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
import impuzzle 1.0
import "."

Item {
    id: rootItem
    property bool drawPieceNumbers: true
    property bool morePieces: false

    Rectangle {
        id: view
        anchors.fill: parent
        color: Style.pageBgColor

        Grid {
            id: backgroundStuff
            anchors.fill: parent
            spacing: 0 //20
            columns: view.width / 40
            clip: true

            Repeater {
                anchors.fill: parent
                model: view.width / 40 * view.height / 40

                Rectangle {
                    color: Qt.darker("#111", 1.7)
                    width: 45
                    height: width
                    radius: 45
                }
            }
        }

        Grid {
            columns: morePieces ? 4 : 3
            anchors.fill: parent
            spacing: 2

            Repeater {
                anchors.fill: parent
                model: morePieces ? 16 : 9

                PuzzleItem {
                    id: puzzleItem
                    width: parent.width / (morePieces ? 4 : 3)
                    height: parent.height / (morePieces ? 4 : 3)
                    currentIndex: index
                    correctIndex: gameEngine.correctPositionForIndex(index)
                    drawNumber: drawPieceNumbers
                    pixmap: gameEngine.imageForIndex(index)
                    visible: true

                    Connections {
                        target: gameEngine
                        onUpdate: {
                            puzzleItem.pixmap = gameEngine.imageForIndex(index)
                            correctIndex = gameEngine.correctPositionForIndex(index)
                            puzzleItem.opacity = 1
                            update()
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: gameEngine.pieceClicked(index)
                    }
                }
            }
        }
    }
}
