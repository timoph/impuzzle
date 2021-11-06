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
import QtQuick.Controls 2.4
import "."

Item {
    id: base
    height: 60
    width: 60

    property string text: ""
    property bool borders: true
    property bool transparentBackground: false
    property bool square: false

    signal clicked()

    Button {
        id: button
        height: base.height
        text: base.text
        anchors.fill: parent

        contentItem: Text {
            text: button.text
            font: button.font
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            opacity: enabled ? 1.0 : 0.3
            color: button.down ? Style.highlightColor : Style.buttonTextColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        background: Rectangle {
            implicitWidth: 60
            implicitHeight: 60
            border.color: Style.buttonBorderColor
            border.width: borders ? 2 : 0
            radius: square ? 0 : 15
            opacity: transparentBackground ? 0.5 : 1
            color: Style.buttonColor
        }

        onClicked: base.clicked()
    }
}
