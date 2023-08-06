
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

public slots:
    ///
    /// \brief startReading начать чтение для всех читателей
    void startReading();
    ///
    /// \brief stopReading команда остановить выполнение всех потоков чтения
    /// при первой же возможности
    void stopReading();

};

#endif // READERSMANAGER_H
