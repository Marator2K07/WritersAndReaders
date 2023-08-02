
#include "writersmanager.h"

WritersManager::WritersManager(short count,
                               Book *book,
                               QObject *parent)
    : count{count}
    , book{book}
    , QObject{parent}
{
    // создание и инициализация переменных для каждого из писателей
    QList<QString> *latestText = new QList<QString>;
    QMutex *openWriteLocker = new QMutex;
    QMutex *textLocker = new QMutex;
    // создание инициализированного списка писателей
    for (int i = 0; i < count; ++i) {
        writers.append(new Writer(openWriteLocker, textLocker, latestText));
    }
    // создание и инициализация списка потоков для писателей
    // и инициализация соединений у списка писателей
    foreach (Writer *writer, writers) {
        // соединяем сигналы начала и конца работы писателей
        // с соотвествующими слотами у книги
        connect(writer, SIGNAL(started(QList<QString>*)), book, SLOT(read(QList<QString>*)));
        connect(writer, SIGNAL(finished(QList<QString>*)), book, SLOT(write(QList<QString>*)));
        connect(writer, SIGNAL(came(short)), book, SLOT(updateWritersNumber(short)));
        connect(writer, SIGNAL(gone(short)), book, SLOT(updateWritersNumber(short)));
        // теперь работа с потоком
        QThread *thread = new QThread(this);
        connect(thread, SIGNAL(started()), writer, SLOT(working()));
        writer->moveToThread(thread);
        threads.append(thread);
        thread->start();
    }
}

WritersManager::~WritersManager()
{
    foreach (QThread *thread, threads) {
        thread->quit();
        thread->wait();
    }
}

