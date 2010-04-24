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

#include "aboutdialog.h"
#include "defines.h"

#include <QTextEdit>
#include <QVBoxLayout>

AboutDialog::AboutDialog(QWidget *parent) :
        QDialog(parent)
{
    setModal(true);

    setWindowTitle(tr("About ImPuzzle"));

    textEdit_ = new QTextEdit;

    QString txt =   "ImPuzzle - A set your pieces straight game<br />"
                    "Copyright (C) 2009  Timo H&auml;rk&ouml;nen<br />"
                    "<br />"
                    "This program is free software: you can redistribute it and/or modify<br />"
                    "it under the terms of the GNU General Public License as published by<br />"
                    "the Free Software Foundation, either version 3 of the License, or<br />"
                    "(at your option) any later version.<br />"
                    "<br />"
                    "This program is distributed in the hope that it will be useful,<br />"
                    "but WITHOUT ANY WARRANTY; without even the implied warranty of<br />"
                    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the<br />"
                    "<a href=\"http://www.gnu.org/licenses/gpl.txt\">GNU General Public License</a> for more details.<br />";

    //textEdit_->setText(txt);
    textEdit_->setHtml(txt);
    textEdit_->setReadOnly(true);

#ifdef Q_WS_MAEMO_5
    textEdit_->setProperty("FingerScrollable", true);
#endif

    mainLayout_ = new QVBoxLayout;
    mainLayout_->addWidget(textEdit_);

    setLayout(mainLayout_);
}
