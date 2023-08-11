
#ifndef WRITERSMANAGER_H
#define WRITERSMANAGER_H

#include "writer.h"
#include "book.h"
#include "smoothlyupdatedlabel.h"

#include <QHBoxLayout>
#include <QColor>
#include <QTimer>
#include <QWaitCondition>

class WritersManager : public QWidget
{
    Q_OBJECT
private:
    Book *book;
    const short count;
    const short minWaitingTime; // в миллисекундах
    const short maxWaitingTime; // в миллисекундах
    QList<Writer *> writers;
    QList<QThread *> threads; // потоки для работы писателей
    QWaitCondition *writersInactivity;

public:
    explicit WritersManager(short count,
                            Book *book,
                            QWaitCondition *writersInactivity,
                            QWidget *parent = nullptr);
    const QList<Writer *> getWriters();
    ~WritersManager();    

public slots:
    ///
    /// \brief writersActivity проверка, активен ли какойнибудь писатель в данный момент
    /// \param activity результат проверки
    void writersActivityAnalysis(bool *activity);
    ///
    /// \brief completionAnalysis нужен чтобы понять, все ли писатели
    /// закончили дописывать книгу
    void completionAnalysis();
    ///
    /// \brief startWriting запуск всех потоков-писателей с их
    /// главными методами задания
    void startWriting();    
};

#endif // WRITERSMANAGER_H
