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

    property int clicksNeeded: 0

    Rectangle {
        id: gameOverRect
        anchors.fill: parent
        color: Style.menuBgColor
        border.width: 4
        border.color: Style.highlightColor

        FireWorks {
            anchors.fill: parent
            fireWorksEnabled: parent.visible
            moreFireWorksEnabled: showMoreFireWorks && fireWorksEnabled
        }

        Text {
            id: gameOverText
            anchors.centerIn: parent
            text: qsTr("Puzzle solved")
            color: Style.highlightColor
            font.pointSize: 38
        }

        Text {
            id: movesText
            anchors.top: gameOverText.bottom
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Completion required") + " " + clicksNeeded + " " + (clicksNeeded === 1 ? qsTr("move") : qsTr("moves"))
            color: Style.menuTextColor
            font.pointSize: 20
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: rootItem.visible = false
    }
}
