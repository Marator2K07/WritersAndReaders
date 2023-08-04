
#ifndef BOOK_H
#define BOOK_H

#include "formatter.h"

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QMutex>

class Book : public QObject
{
    Q_OBJECT
private:
    QFile book;
    QList<QString> buffer;
    short *currentWritersNumber;
    QMutex access;

public:
    explicit Book(QObject *parent = nullptr);
    const QList<QString> getText();

public slots:
    ///
    /// \brief write условный писатель дописал, что хотел и сохраняет изменения
    /// \param text обновленный текст книги
    void write(QList<QString> *text);
    ///
    /// \brief read условный писатель прочитал, что было в книге
    /// \param text скопированный текст книги
    void read(QList<QString> *text);
    void updateWritersNumber(short num);


};

#endif // BOOK_H
