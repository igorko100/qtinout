#ifndef NFC_H
#define NFC_H

#include <QDebug>

#include <QThread>

#include <stdlib.h>
#include <nfc/nfc.h>

// Threads for NFC reading (taken from QT docs)
class Worker : public QObject
{
    Q_OBJECT

public slots:
    void doWork(const QString &parameter);

signals:
    void resultReady(const QString &result);
};

class Controller : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    Controller();
    ~Controller() {
        workerThread.quit();
        workerThread.wait();
    }
public slots:
    void handleResults(const QString &);
signals:
    void operate(const QString &);
};



#endif // NFC_H
