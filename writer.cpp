
#include "writer.h"

Writer::Writer(Book *book,
               QObject *parent)
    : QObject{parent}
    , book{book}
{
    // сразу сходу соединяем сигналы начала и конца работы с книгой
    // с соотвествующими слотами у книги
    connect(this, SIGNAL(started(QList<QString>)),
            book, SLOT(read(QList<QString>)));
    connect(this, SIGNAL(finished(QList<QString>)),
            book, SLOT(write(QList<QString>)));
}

void Writer::completingWork()
{
    emit started(latestText);
    latestText << "BREDD SIVOY COBILI.\n";
    emit finished(latestText);
}

