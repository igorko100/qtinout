#include "nfc.h"
#include <iostream>

Controller::Controller() {

    Worker *worker = new Worker;
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &Controller::operate, worker, &Worker::doWork);
    connect(worker, &Worker::resultReady, this, &Controller::handleResults);
    workerThread.start();
    emit operate(QString("Testing operate"));
}



static void
print_hex(const uint8_t *pbtData, const size_t szBytes)
{
  size_t  szPos;

  for (szPos = 0; szPos < szBytes; szPos++) {
    std::cout << std::printf("%02x ", pbtData[szPos]);
  }
  std::cout << std::endl;
}

void Controller::handleResults(const QString &) {
    std::cout << "... Here we will handle reading of a card ..." << std::endl;
}


void Worker::doWork(const QString &parameter) {

    qDebug() << "In doWork!";

    QString result;

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
    std::cout << "Using libnfc" << acLibnfcVersion << std::endl;

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

//    std::cout << "NFC reader:" << nfc_device_get_name(pnd) << " opened" << std::endl;
    std::cerr << "NFC reader:" << nfc_device_get_name(pnd) << " opened" << std::endl;


    // Poll for a ISO14443A (MIFARE) tag
    const nfc_modulation nmMifare = {
      .nmt = NMT_ISO14443A,
      .nbr = NBR_106,
    };
    if (nfc_initiator_select_passive_target(pnd, nmMifare, NULL, 0, &nt) > 0) {
      std::cout << "The following (NFC) ISO14443A tag was found:" << std::endl;
      std::cout << std::printf("       UID (NFCID%c): ", (nt.nti.nai.abtUid[0] == 0x08 ? '3' : '1'));
      print_hex(nt.nti.nai.abtUid, nt.nti.nai.szUidLen);
    }
    // Close NFC device
    nfc_close(pnd);
    // Release the context
    nfc_exit(context);

    emit resultReady(result);
}

