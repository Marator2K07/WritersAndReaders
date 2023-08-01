
#ifndef WRITER_H
#define WRITER_H

#include "book.h"

#include <QObject>
#include <QRandomGenerator>

class Writer : public QObject
{
    Q_OBJECT
private:
    Book *book;
    QList<QString> latestText;
    const QString possibleCharacters; // в этой строке представлены символы, которые использует писатель в книге
    const short maxLineWidth;
    const short minLineWidth;

private:
    QString makeWord(short *charactersLeft);
    QString makeLine(short *charactersLeft);
    void makeText();

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
