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

import QtQuick 2.6
import QtQuick.Controls 2.4
import "."

CheckBox
{
    id: checkBox

    indicator: Rectangle {
        implicitHeight: 26
        implicitWidth: 26
        x: checkBox.leftPadding
        y: parent.height / 2 - height / 2
        radius: 3
        border.width: 0
        border.color: Style.highlightColor

        Text {
            anchors.fill: parent
            text: "\u2717"
            font.pointSize: 16
            color: Style.highlightColor
            visible: checkBox.checked
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
    }

    contentItem: Text {
        text: checkBox.text
        font: checkBox.font
        opacity: enabled ? 1.0 : 0.3
        color: Style.menuTextColor
        verticalAlignment: Text.AlignVCenter
        leftPadding: checkBox.indicator.width + checkBox.spacing
    }
}
