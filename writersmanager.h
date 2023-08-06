
#ifndef WRITERSMANAGER_H
#define WRITERSMANAGER_H

#include "writer.h"
#include "book.h"
#include "smoothlyupdatedlabel.h"

#include <QHBoxLayout>
#include <QColor>
#include <QTimer>

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

public:
    explicit WritersManager(short count,
                            Book *book,
                            QWidget *parent = nullptr);
    const QList<Writer *> getWriters();
    ~WritersManager();

signals:
    void anyoneStarted();
    void allFinished();

public slots:
    ///
    /// \brief beginningAnalysis нужен, чтобы понять, начал ли кто-нибудь
    /// из писателей дописывать книгу
    void beginningAnalysis();
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
