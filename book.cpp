
#include "book.h"

void Book::getLastNLines(QList<QString> &text, short count)
{
    while(text.size() > count) {
        text.removeFirst();
    }
}

Book::Book(QObject *parent)
    : QObject{parent}
{
    book.setFileName("Book.txt");
    currentWritersNumber = 0;
}

void Book::finish(QList<QString> *text)
{
    // только в случае если писатель один, он имеет право сохранить
    // написанный текст, писал ли он его ранее один или нет..., это решает
    // проблему с копиями слов в сохраненном тексте
    if (currentWritersNumber == 1) {
        access.lock();
        buffer = *text;
        if (book.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&book);
            foreach (QString line, buffer) {
                out << line << "\n";
            }
            book.close();
            buffer.clear(); // не забываем чистить буффер
            emit clearText(); // уборка лишнего текста из поля с текстом книги
        }
        emit resetCurrentLineWidth(); // обнуляем текущую длину
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
            // писатели начинают писать с абзаца
            if (buffer.size() > 0) {
                buffer.append("");
            }
            book.close();
        }
        access.unlock();
        // делаем копию буфера
        *text = buffer;
        // теперь для поля вывода текста книги оставляем N последних строк и выводим их
        getLastNLines(buffer, 22);
        emit updateText(buffer);
        // и не забываем почистить буффер
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
