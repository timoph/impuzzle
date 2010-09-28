/*
  Image Puzzle - A set your pieces straight game
  Copyright (C) 2009  Timo Härkönen

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "trackerfiles.h"

#include <QDebug>

TrackerFiles::TrackerFiles(QObject *parent) :
    QObject(parent)
{
    tracker_ = new QProcess(this);

    connect(tracker_, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(processFinished(int,QProcess::ExitStatus)));
}

void TrackerFiles::readFiles()
{
    QString program("tracker-files");
    QStringList args;
    args << "-m" << "image/png" << "-m" << "image/jpeg";
    tracker_->start(program, args);
    // FIXME: this blocks execution
    tracker_->waitForFinished();
}

void TrackerFiles::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QStringList files;

    if(exitCode == 0 && exitStatus == QProcess::NormalExit) {
        QString stdOut = tracker_->readAllStandardOutput();
        files = stdOut.split("\n");
        // first row is not a path
        if(!files.isEmpty()) {
            files.removeAt(0);
        }

        files.removeAll("");
        qDebug() << QString("Found %1 images").arg(files.count());
    }
    else {
        qDebug() << tracker_->errorString();
    }

    emit filesRead(files);
}
