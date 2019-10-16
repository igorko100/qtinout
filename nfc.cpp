#include "nfc.h"
#include <iostream>
#include <QByteArray>
#include <QString>

Controller::Controller() {

    Worker *worker = new Worker;
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &Controller::operate, worker, &Worker::doWork);
    connect(worker, &Worker::resultReady, this, &Controller::handleResults);
    workerThread.start();
    emit operate(QByteArray("Testing operate"));
}


void Controller::handleResults(const QByteArray &id) {
    std::cerr << "... Writing to DB ..." << std::endl;

//    QThread::sleep(1); // Just wait for a second

    emit operate(QByteArray("Testing operate"));
}


void Worker::doWork(const QByteArray &parameter) {

    QByteArray result;

    /* ... here is the expensive or blocking operation ... */

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
      std::cerr << "UID (NFCID):";
      std::cerr << QString(result.toHex()).toLocal8Bit().constData() << std::endl;
    }
    std::cerr << "Wait for target remove..." << std::endl;
    elt.start();
    while (0 == nfc_initiator_target_is_present(pnd, NULL)) {
    }
    // Here we try to exclude too close events
    if (elt.elapsed() > 250) {
        std::cerr << "elapsed time:" << elt.elapsed()<< std::endl;
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

