
#include "book.h"

Book::Book(QObject *parent)
    : QObject{parent}
{
    book.setFileName("Book.txt");
    // если такой файл существует, то открываем его и запоминаем данные в буфере
    if (book.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&book);
        while (!in.atEnd()) {
            buffer.append(in.readLine());
        }
    }
}

