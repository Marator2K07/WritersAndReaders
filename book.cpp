
#include "book.h"

Book::Book(QObject *parent)
    : QObject{parent}
{
    book.setFileName("Book.txt");
}

void Book::write(QList<QString> text)
{
    buffer = text; // делаем копию текста
    if (book.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        QTextStream out(&book);
        foreach (QString line, buffer) {
            out << line;
        }
        book.close();
    }
}

void Book::read(QList<QString> text)
{
    if (book.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&book);
        while (!in.atEnd()) {
            buffer.append(in.readLine());
        }
        book.close();
    }
    text = buffer; // делаем копию буфера
}

