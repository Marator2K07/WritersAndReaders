
#include "book.h"

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
        // создаем и инициализируем все необходимые для расчетов переменные
        short maxNumberOfLines = 44;
        short currentLine = 0;
        short currentPosition = 0;
        QString currentSymbol = "";
        // блокируем доступ для других потоков и пытаемся открыть книгу
        access.lock();
        if (book.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&book);
            // узнаем позицию последнего символа и перемещаемся туда
            currentPosition = book.size() - 1;
            in.seek(currentPosition);
            buffer.append(""); // сначала нужно проинициализировать буффер
            // а теперь посимвольно считываем строки в буффер начиная с конца и двигаясь в начало!
            while (currentPosition >= 0 && currentLine <= maxNumberOfLines) {
                currentSymbol = in.read(1); // читаем один символ
                if (currentSymbol != "\n") {
                    buffer[0].push_front(currentSymbol);
                }
                else {
                    buffer.push_front("");
                    currentPosition--;
                    currentLine++;
                }
                currentPosition--;
                in.seek(currentPosition);
            }
            // добавляем доп абзац для наглядности
            if (buffer.size() > 0) {
                buffer.append("");
            }
            book.close();
        }
        emit updateText(buffer); // пишем загруженный текст
        *text = buffer; // делаем копию буфера
        access.unlock();
        buffer.clear(); // и не забываем почистить буффер
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
