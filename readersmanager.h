
#ifndef READERSMANAGER_H
#define READERSMANAGER_H

#include "book.h"
#include "reader.h"
#include "smoothlyupdatedlabel.h"

#include <QWidget>
#include <QVBoxLayout>

class ReadersManager : public QWidget
{
    Q_OBJECT
private:
    Book *book;
    const short count;
    QList<Reader *> readers;
    QList<QThread *> threads; // потоки для работы читателей

public:
    explicit ReadersManager(short count,
                            Book *book,
                            QWidget *parent);
    ~ReadersManager();

signals:

};

#endif // READERSMANAGER_H
