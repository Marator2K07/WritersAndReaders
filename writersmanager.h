
#ifndef WRITERSMANAGER_H
#define WRITERSMANAGER_H

#include "writer.h"
#include "book.h"
#include "smoothlyupdatedlabel.h"

#include <QHBoxLayout>
#include <QColor>

class WritersManager : public QWidget
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
                            QWidget *parent = nullptr);
    const QList<Writer *> getWriters();
    ~WritersManager();

};

#endif // WRITERSMANAGER_H
