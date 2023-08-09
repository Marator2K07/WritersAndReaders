
#ifndef READERSMANAGER_H
#define READERSMANAGER_H

#include "book.h"
#include "reader.h"
#include "smoothlyupdatedlabel.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QTimer>

class ReadersManager : public QWidget
{
    Q_OBJECT
private:
    Book *book;
    const short count;
    const short minWaitingTime; // в миллисекундах
    const short maxWaitingTime; // в миллисекундах
    QList<Reader *> readers;
    QList<QThread *> threads; // потоки для работы читателей
    QWaitCondition *writersInactivity;
    QMutex *waitConditionLocker;

public:
    explicit ReadersManager(short count,
                            Book *book,
                            QWaitCondition *writersInactivity,
                            QMutex *waitConditionLocker,
                            QWidget *parent);
    const QList<Reader *> getReaders();
    ~ReadersManager();    

public slots:        
    ///
    /// \brief startReading начать чтение для всех читателей
    void startReading();
    ///
    /// \brief completionAnalysis нужен чтобы понять, все ли читатели
    /// закончили читать книгу
    void completionAnalysis();

};

#endif // READERSMANAGER_H
