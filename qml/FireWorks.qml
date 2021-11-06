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
import QtQuick.Particles 2.0

Item {
    id: rootItem
    width: 800
    height: 480

    property bool fireWorksEnabled: false
    property bool moreFireWorksEnabled: false

    ParticleSystem {
        id: parSystem
        anchors.fill: parent

        running: fireWorksEnabled

        ItemParticle {
            anchors.fill: parent
            delegate: Rectangle {
                    height: Math.random() * (5 - 2) + 2
                    width: height
                    color: Qt.rgba(Math.random(), Math.random(),Math.random(), 0.8)
                    radius: 45
                }
        }

        Timer {
            running: fireWorksEnabled
            interval: showMoreFireWorks ? 500 : 1000
            repeat: true
            onTriggered: fireWorksEmitter.burst(26, Math.random() * (parent.width - 20) + 20, Math.random() * (parent.height - 20) + 20)
        }

        Emitter {
            id: fireWorksEmitter
            group: "A"
            x: parent.width / 2
            y: parent.height / 3
            emitRate: 0
            lifeSpan: 1800
            lifeSpanVariation: 300
            maximumEmitted: moreFireWorksEnabled ? 100 : 50
            acceleration: PointDirection { x: 0; y: 10 }
            size: 20
            endSize: 1
            velocity: AngleDirection { magnitude: 100; angleVariation: 360 }
        }

        TrailEmitter {
            emitHeight: 2
            emitWidth: 2
            lifeSpan: 1100
            emitRatePerParticle: 20
            follow: "A"
            velocity: PointDirection {}
            acceleration: PointDirection {}
        }

        Gravity {
            magnitude: 18
            angle: 90
        }

        Turbulence {
            anchors.fill: parent
            strength: 10
        }
    }
}
