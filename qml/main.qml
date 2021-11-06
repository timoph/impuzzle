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
import QtQuick.Window 2.3

Window {
    id: appWindow
    flags: Qt.Window
    visible: true
    height: windowHeight
    width: windowWidth

    Component.onCompleted: {
        if(fullScreen) appWindow.showFullScreen()
        gameEngine.findImages()
    }

    GameView {
        id: gameView
        anchors.top: parent.top
        width: parent.width
        height: parent.height
        drawPieceNumbers: true

        MouseArea {
            anchors.fill: parent
            enabled: gameMenu.visible
            onClicked: gameMenu.visible = false
        }

        GameMenu {
            id: gameMenu
            anchors.centerIn: parent
            visible: true && !loadingScreen.visible

            Connections {
                target: gameEngine
                onGameStarted: {
                    gameMenu.visible = false
                }
            }

            onSelectPictureClicked: fileDialog.visible = true

            onDrawNumbersChanged: {
                gameView.drawPieceNumbers = draw
            }

            onMorePiecesSet: {
                gameView.morePieces = more
            }
        }

        GameOverBanner {
            id: gameOverBanner
            anchors.fill: parent
            visible: false

            Connections {
                target: gameEngine
                onPuzzleSolved: {
                    gameOverBanner.clicksNeeded = moveCount
                    gameOverBanner.visible = true
                }
            }
        }
    }

    MenuButton {
        width: 40
        height: 40
        anchors.right: parent.right
        anchors.top: parent.top
        borders: false
        square: true
        transparentBackground: true
        text: "\u2699"
        visible: !gameMenu.visible && !gameOverBanner.visible
        onClicked: gameMenu.visible = !gameMenu.visible
    }


    ImagePicker {
        id: fileDialog
        width: gameView.width
        height: gameView.height
        visible: false
    }

    LoadingScreen {
        id: loadingScreen
        anchors.fill: parent
    }
}
