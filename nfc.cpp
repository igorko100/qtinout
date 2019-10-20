#include "nfc.h"
#include <iostream>
#include <QByteArray>
#include <QString>
#include <QProcess>
#include <QApplication>
#include <QDateTime>
#include <QMessageBox>
#include <QPushButton>


#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlError>


Controller::Controller() {

    Worker *worker = new Worker;
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &Controller::operate, worker, &Worker::doWork);
    connect(worker, &Worker::resultReady, this, &Controller::handleResults);
    workerThread.start();

    db = QSqlDatabase ();

    openDB();

    emit operate();
}

void Controller::openDB() {

    db = QSqlDatabase::addDatabase("QPSQL", "work");
    db.setHostName("10.10.11.201");
    db.setDatabaseName("work");
    db.setUserName("postgres");
    db.setPassword("postgres123");
    bool ok = db.open();

//ToDo: handle opening error(s)

    qDebug() << "Opening DB:" << ok << "  " <<  db.lastError().text();
}

void Controller::handleResults(const QByteArray &id) {

    QDateTime timestamp;
    timestamp = QDateTime::currentDateTime();
    QString ts = timestamp.toString("yyyy-MM-dd hh:mm:ss");
    std::cerr << "Inserted card with UID:" << QString(id.toHex()).toLocal8Bit().constData() << std::endl;
    std::cerr << "... Writing to DB at " << ts.toLocal8Bit().data() << " ..." << std::endl;

    QSqlQuery query(db);
    query.prepare("INSERT INTO inout(scid) VALUES(?::bytea) "
                  "RETURNING status, "
                            "(SELECT to_char(timestamp, 'DD-MM-YYYY HH24:MI:SS')), "
                            "(SELECT scnr "
                             "FROM people WHERE people.scid=inout.scid)");

    query.bindValue(0, id);

    query.exec();
    qDebug() << "Query execution:" <<  query.lastError().text();

//    while (query.next()) {
    query.next();
    QString status = query.value(0).toString();
    QString timestampFromDB = query.value(1).toString();
    int scnr = query.value(2).toString();
    qDebug() << "status:" << status << "  timestampFromDB:" << timestampFromDB << "  scnr:" << scnr;
//    }

    if(scnr == 0) {

        QMessageBox msgBox;
        msgBox.setText("New tag is inserted. Select action:");
        QPushButton *createNewUser = msgBox.addButton(tr("Create new user..."), QMessageBox::ActionRole);
        createNewUser->setMinimumSize(96,24);
        QPushButton *cancelButton = msgBox.addButton(QMessageBox::Cancel);
        cancelButton->setMinimumSize(48,24);

        int ret = msgBox.exec();

        if(ret == QMessageBox::Cancel) {
            emit operate();
            return;
        }
    }

    QProcess process;
    if (status == "in") {
        emit updateText(States::WELCOME);
        QApplication::processEvents();
        process.start("aplay doorbell-shortened.wav");
    }
    else {
        emit updateText(States::BYE);
        QApplication::processEvents();
        process.start("aplay skibka-music-logo-pn22.wav");
    }
    process.waitForFinished(-1); // will wait forever until finished
    emit updateText(States::WAIT);
    emit operate();
}


void Worker::doWork() {

    QByteArray result;

    /* ... here we are reading NFC device in a separate thread ... */

    nfc_device *pnd;
    nfc_target nt;

    // Allocate only a pointer to nfc_context
    nfc_context *context;

    // Initialize libnfc and set the nfc_context
    nfc_init(&context);
    if (context == NULL) {
      std::cerr << "Unable to init libnfc (malloc)\n";
      return; // EXIT ?
    }

    // Display libnfc version
    const char *acLibnfcVersion = nfc_version();
//    std::cerr << "Using libnfc" << acLibnfcVersion << std::endl;

    // Open, using the first available NFC device which can be in order of selection:
    //   - default device specified using environment variable or
    //   - first specified device in libnfc.conf (/etc/nfc) or
    //   - first specified device in device-configuration directory (/etc/nfc/devices.d) or
    //   - first auto-detected (if feature is not disabled in libnfc.conf) device
    pnd = nfc_open(context, NULL);

    if (pnd == NULL) {
      std::cerr << "ERROR: Unable to open NFC device.";
      return; // EXIT ?
    }
    // Set opened NFC device to initiator mode
    if (nfc_initiator_init(pnd) < 0) {
      nfc_perror(pnd, "nfc_initiator_init");
      return; // EXIT ?
    }

//    std::cerr << "NFC reader:" << nfc_device_get_name(pnd) << " opened" << std::endl;
//    std::cerr << "NFC reader:" << nfc_device_get_name(pnd) << " opened" << std::endl;


    // Poll for a ISO14443A (MIFARE) tag
    const nfc_modulation nmMifare = {
      .nmt = NMT_ISO14443A,
      .nbr = NBR_106,
    };

label:
    std::cerr << std::endl << "Wait for target..." << std::endl;
    if (nfc_initiator_select_passive_target(pnd, nmMifare, NULL, 0, &nt) > 0) {
      result = QByteArray((char*)nt.nti.nai.abtUid, nt.nti.nai.szUidLen);
//      std::cerr << "UID (NFCID):";
//      std::cerr << QString(result.toHex()).toLocal8Bit().constData() << std::endl;
    }
    std::cerr << "Wait for target remove..." << std::endl;
    elt.start();
    while (0 == nfc_initiator_target_is_present(pnd, NULL)) {
    }
    // Here we try to exclude too close events
    if (elt.elapsed() > 250) {
//        std::cerr << "elapsed time:" << elt.elapsed()<< std::endl;
        emit resultReady(result);
    } else {
        std::cerr << ">>>>> Duplicate events occured <<<<< waiting a second..." << std::endl;
        QThread::sleep(1); // Just wait for a second
        goto label; // ??? no better idea yet
    }

    // Close NFC device
    nfc_close(pnd);
    // Release the context
    nfc_exit(context);
}

