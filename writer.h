
#ifndef WRITER_H
#define WRITER_H

#include "book.h"

#include <QObject>

class Writer : public QObject
{
    Q_OBJECT
private:
    Book *book;
    QList<QString> latestText;

public:
    explicit Writer(Book *book,
                    QObject *parent);
    ///
    /// \brief completingWork основной метод класса, в котором писатель во
    /// время пришедшего вдохновения продолжает писать книгу
    void completingWork();

signals:
    /// начал писать книгу
    void started(QList<QString>);
    /// закончил писать книгу
    void finished(QList<QString>);
};

#endif // WRITER_H
