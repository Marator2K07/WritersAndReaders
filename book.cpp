
#include "book.h"

Book::Book(QObject *parent)
    : QObject{parent}
{
    book.setFileName("Book.txt");
    currentWritersNumber = 0;
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

void Book::finish(QList<QString> *text)
{
    // только в случае если писатель один, он имеет право сохранить
    // написанный текст, писал ли он его ранее один или нет..., это решает
    // проблему с копиями слов в сохраненном тексте
    if (currentWritersNumber == 1) {
        access.lock();
        buffer = Formatter::splitIntoBookLines(*text); // перед сохранением, приведем коллекцию к нормальному виду
        if (book.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&book);
            foreach (QString line, buffer) {
                out << line;
            }
            book.close();
            buffer.clear(); // не забываем чистить буффер
        }
        access.unlock();
    }
    // после сохранения изменений, изменяем количество писетелей
    access.lock();
    currentWritersNumber--;
    access.unlock();
}

void Book::remember(QList<QString> *text)
{
    // сначала обновляем количество писателей
    access.lock();
    currentWritersNumber++;
    access.unlock();
    // у первого пришедшего писателя копия книги на руках и он думает, что дописать.
    // Если приходит второй - то думают уже вместе. НО к тому времени, как пришел второй,
    // первый писатель уже мог что-то придумать!!! И он условно пересказывает о своих начинаниях.
    // Это решает проблему затирания уже новых данных.
    if (currentWritersNumber == 1) {
        access.lock();
        if (book.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&book);
            while (!in.atEnd()) {
                buffer.append(in.readLine());
            }
            book.close();
        }
        access.unlock();
        // делаем копию буфера и очищаем его
        *text = buffer;
        buffer.clear();
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
