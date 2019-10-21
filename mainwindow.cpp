/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>

#include "mainwindow.h"
#include "nfc.h"
#include "digitalclock.h"
#include "addnewuserdialog.h"
#include "showlasteventdialog.h"

MainWindow::MainWindow()
{
    QWidget *widget = new QWidget;

    setCentralWidget(widget);

    QWidget *topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    infoLabel = new QLabel();
    infoLabel->setFrameStyle(QFrame::NoFrame);

    infoLabel->setAlignment(Qt::AlignCenter);

    iconLabel = new QLabel();
    iconLabel->setFrameStyle(QFrame::NoFrame);
    iconLabel->setAlignment(Qt::AlignCenter);

    logoLabel = new QLabel();
//    logoLabel->setAlignment(Qt::AlignRight);
    logoLabel->setFrameStyle(QFrame::NoFrame);

    updateLabel(States::WAIT);

    QWidget *bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    DigitalClock *clock = new DigitalClock();
    clock->setFixedSize(450, 180);
    clock->setFrameStyle(QFrame::NoFrame);


/*    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(5, 5, 5, 5);
    layout->addWidget(clock, 0, Qt::AlignCenter);
    layout->addWidget(topFiller);
    layout->addWidget(iconLabel);
    layout->addWidget(infoLabel);
    layout->addWidget(bottomFiller);
    layout->addWidget(logoLabel);
    widget->setLayout(layout);
*/
    QPushButton *showLastActivityButton = new QPushButton;
    showLastActivityButton->setText("Show last event");
    showLastActivityButton->setMinimumHeight(50);

    QGridLayout *layout = new QGridLayout;
    layout->setContentsMargins(5, 5, 5, 5);
    layout->addWidget(clock, 0, 0, 1, 7, Qt::AlignCenter);
    layout->addWidget(topFiller,1,0,1,7);
    layout->addWidget(iconLabel,2,0,1,7);
    layout->addWidget(infoLabel,3,0,1,7,Qt::AlignCenter);
    layout->addWidget(bottomFiller,4,0,1,7);
    layout->addWidget(logoLabel,5,0);
    layout->addWidget(showLastActivityButton, 5, 6);
    widget->setLayout(layout);

    logoLabel->setPixmap(QPixmap("CNIua.png"));

    //    widget->setStyleSheet("*{background-image: url(bg.jpg;}");
//        widget->setStyleSheet("QMainWindow::centralwidget { background-color: rgb(0, 0, 0); }");
    QPalette Pal(palette());

    // устанавливаем цвет фона
    Pal.setColor(QPalette::Background, Qt::gray);
    widget->setAutoFillBackground(true);
    widget->setPalette(Pal);
    widget->show();

/*
#centralwidget { background-color: rgb(0, 0, 0); }
*/
    createActions();
    createMenus();

    setWindowTitle(tr("CNI inout"));
    setMinimumSize(160, 160);
    resize(480, 320);
    c = new Controller;
    connect (c, &Controller::updateText, this, &MainWindow::updateLabel);
    connect (showLastActivityButton, SIGNAL(clicked()), this, SLOT(showLastEventDialog()));
}

void MainWindow::showLastEventDialog() {
    ShowLastEventDialog dlg(c->lastPIB,c->lastStatus,c->lastTimestamp);
    dlg.showFullScreen();
    dlg.exec();
}



void MainWindow::updateLabel(States state) {

    QString style, text;
    QPixmap icon;

    switch (state) {
        case States::WELCOME:
            style = "QLabel { font: bold 72pt; color: green; }";
            text = "Welcome!";
            icon = QPixmap("work.png");
            break;
        case States::BYE:
            style = "QLabel { font: bold 72pt; color: blue; }";
            text = "Bye-bye!";
            icon = QPixmap("home.png");
            break;
        default:
            style = "QLabel { font: normal 20pt; color: grey; }";
            text = "Waiting for card...";
            icon = QPixmap("wait.png");

    }
    infoLabel->setStyleSheet(style);
    iconLabel->setPixmap(icon);
    infoLabel->setText(text);

}

void MainWindow::newFile()
{
//    infoLabel->setText(tr("Invoked <b>File|New</b>"));
}

void MainWindow::open()
{
//    infoLabel->setText(tr("Invoked <b>File|Open</b>"));
}


void MainWindow::about()
{
//    infoLabel->setText(tr("Invoked <b>Help|About</b>"));
    QMessageBox::about(this, tr("About Menu"),
            tr("In/out data collection application."
               "Works with PostgreSQL database."));
}

void MainWindow::aboutQt()
{
//    infoLabel->setText(tr("Invoked <b>Help|About Qt</b>"));
}

void MainWindow::createActions()
{
    showLastActivity = new QAction(tr("Show last &activity"), this);
    showLastActivity->setShortcuts(QKeySequence::New);
    showLastActivity->setStatusTip(tr("Show last activity"));
    connect(showLastActivity, &QAction::triggered, this, &MainWindow::newFile);

    showLatestStatuses = new QAction(tr("&Show latest statuses"), this);
    showLatestStatuses->setShortcuts(QKeySequence::Open);
    showLatestStatuses->setStatusTip(tr("Show latest inout statuses"));
    connect(showLatestStatuses, &QAction::triggered, this, &MainWindow::open);

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(aboutQtAct, &QAction::triggered, this, &MainWindow::aboutQt);

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&Administration"));
    fileMenu->addAction(showLastActivity);
    fileMenu->addAction(showLatestStatuses);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}
