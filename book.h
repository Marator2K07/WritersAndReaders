
#ifndef BOOK_H
#define BOOK_H

#include <QObject>
#include <QFile>
#include <QList>
#include <QTextStream>

class Book : public QObject
{
    Q_OBJECT
private:
    QFile book;
    QList<QString> buffer;

public:
    explicit Book(QObject *parent = nullptr);

signals:

};

#endif // BOOK_H
