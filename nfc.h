#ifndef NFC_H
#define NFC_H

#include <QDebug>

#include <QThread>
#include <QElapsedTimer>

#include <stdlib.h>
#include <nfc/nfc.h>

// Threads for NFC reading (taken from QT docs)
class Worker : public QObject
{
    Q_OBJECT

    QElapsedTimer elt;

public slots:
    void doWork(const QByteArray &parameter);

signals:
    void resultReady(const QByteArray &result);
};

class Controller : public QObject
{
    Q_OBJECT
    QThread workerThread;
    unsigned prevState;
public:
    Controller();
    ~Controller() {
        workerThread.quit();
        workerThread.wait();
    }
public slots:
    void handleResults(const QByteArray &);
signals:
    void operate(const QByteArray &);
    void updateText(const QString &);
};



#endif // NFC_H
