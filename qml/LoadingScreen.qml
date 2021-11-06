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

Item {
    id: introContainer
    anchors.fill: parent
    visible: true
    state: "Visible"

    states: [
        State {
            name: "Visible"
            PropertyChanges { target: introContainer ; opacity: 1.0 }
            PropertyChanges { target: introContainer ; visible: true }
        },
        State {
            name: "InVisible"
            PropertyChanges { target: introContainer ; opacity: 0.0 }
            PropertyChanges { target: introContainer ; visible: false }
        }
    ]

    transitions: Transition {
        from: "Visible"
        to: "InVisible"
        SequentialAnimation {
            NumberAnimation { property: "opacity"; duration: 1000 }
            NumberAnimation { property: "visible"; duration: 0 }
        }
    }

    Text {
        id: introTitleText
        anchors.centerIn: parent
        color: Style.introTextColor
        font.pointSize: 48
        text: "ImPuzzle 2"
    }

    Text {
        anchors.top: introTitleText.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: introTitleText.horizontalCenter
        color: Style.menuTextColor
        font.pointSize: 20
        text: qsTr("Finding images...")
    }

    Connections {
        target: gameEngine
        onImageListChanged: closeTimer.start()
    }

    Timer {
        id: closeTimer
        interval: 1000
        repeat: false
        running: false
        onTriggered: introContainer.state = "InVisible"
    }
}
