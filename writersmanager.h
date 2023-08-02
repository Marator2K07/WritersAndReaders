
#ifndef WRITERSMANAGER_H
#define WRITERSMANAGER_H

#include "writer.h"
#include "book.h"

#include <QObject>
#include <QThread>

class WritersManager : public QObject
{
    Q_OBJECT
private:
    Book *book;
    const short count;
    QList<Writer *> writers;
    QList<QThread *> threads; // потоки для работы писателей

public:
    explicit WritersManager(short count,
                            Book *book,
                            QObject *parent = nullptr);
    ~WritersManager();

};

#endif // WRITERSMANAGER_H
