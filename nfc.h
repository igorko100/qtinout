#ifndef NFC_H
#define NFC_H

#include <QDebug>

#include <QThread>
#include <QElapsedTimer>
#include <QSqlDatabase>

#include <stdlib.h>
#include <nfc/nfc.h>

enum States
{
   WELCOME,
   BYE,
   WAIT
};

// Threads for NFC reading (taken from QT docs)
class Worker : public QObject
{
    Q_OBJECT

    QElapsedTimer elt;

public slots:
    void doWork();

signals:
    void resultReady(const QByteArray &result);
};

class Controller : public QObject
{
    Q_OBJECT
    QThread workerThread;
    unsigned prevState;

    QSqlDatabase db;

public:
    Controller();
    ~Controller() {
        workerThread.quit();
        workerThread.wait();
    }

    QString lastPIB;
    QString lastStatus;
    QString lastTimestamp;

private:
    void openDB();

public slots:
    void handleResults(const QByteArray &);
signals:
    void operate();
    void updateText(States state);

};



#endif // NFC_H
