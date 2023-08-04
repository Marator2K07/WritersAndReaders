
#include "book.h"

Book::Book(QObject *parent)
    : QObject{parent}
{
    book.setFileName("Book.txt");
    currentWritersNumber = new short(0);
}

const QList<QString> Book::getText()
{
    access.lock();
    QList<QString> result;
    if (book.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&book);
        while (!in.atEnd()) {
            result.append(in.readLine());
        }
        book.close();
    }
    access.unlock();
    return result;
}

void Book::write(QList<QString> *text)
{
    // только в случае если писатель один, он имеет право сохранить
    // написанный текст, писал ли он его ранее один или нет..., это решает
    // проблему с копиями слов в сохраненном тексте
    if (*currentWritersNumber == 1) {
        access.lock();
        buffer = Formatter::splitIntoBookLines(*text); // перед сохранением, приведем коллекцию к нормальному виду
        if (book.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&book);
            foreach (QString line, buffer) {
                out << line;
            }
            book.close();
        }
        access.unlock();
    }
}

void Book::read(QList<QString> *text)
{
    // у первого пришедшего писателя копия книги на руках и он думает, что дописать.
    // Если приходит второй - то думают уже вместе. НО к тому времени, как пришел второй,
    // первый писатель уже мог что-то придумать!!! И он условно пересказывает о своих начинаниях.
    // Это решает проблему затирания уже новых данных.
    if (*currentWritersNumber == 1) {
        access.lock();
        if (book.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&book);
            while (!in.atEnd()) {
                buffer.append(in.readLine());
            }
            book.close();
        }
        access.unlock();
        // делаем копию буфера
        for (int i = buffer.size()-1; i >= 0; --i) {
            text->push_front(buffer.value(i));
        }
    }
}

void Book::simpleRead(QList<QString> *text)
{
    access.lock();
    if (book.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&book);
        while (!in.atEnd()) {
            text->append(in.readLine());
        }
        book.close();
    }
    access.unlock();
}

void Book::updateWritersNumber(short num)
{
    access.lock();
    *currentWritersNumber += num;
    access.unlock();
}

