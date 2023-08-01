
#include "writer.h"

QString Writer::makeWord(short *charactersLeft)
{
    QString word("");
    short wordLenght = 0;
    short border = 12;
    if (*charactersLeft < border) {
        wordLenght = *charactersLeft;
    } else {
        wordLenght = QRandomGenerator::global()->bounded(1, border);
    }
    // заполняем слово случайными символами
    for (int i = 0; i < wordLenght; ++i) {
        word.append(possibleCharacters.
                    at(QRandomGenerator::global()->bounded(0,
                                                           possibleCharacters.size())));
    }
    *charactersLeft -= wordLenght; // напоследок обновляем количество оставшихся символов
    return word;
}

QString Writer::makeLine(short *charactersLeft)
{
    QString line;
    short lineWidth = 0;
    short border = QRandomGenerator::global()->bounded(minLineWidth, maxLineWidth);
    if (*charactersLeft < border) {
        border = *charactersLeft;
    }
    // заполняем строку случайными словами
    while (*charactersLeft > 1 && lineWidth < border) {
        QString newWord = makeWord(charactersLeft); // написали новое "слово"
        lineWidth += newWord.size(); // записали обновленную длину строки
        line.append(newWord); // пишем слово в строку
        line.append(" "); // не забываем добавить пробел между словами
    }
    line.chop(1);
    line.append(".\n");
    *charactersLeft -= lineWidth; // напоследок обновляем количество оставшихся символов
    return line;
}

void Writer::makeText()
{
    short leftBorder = 3;
    short rightBorder = 333;
    short charactersLeft = QRandomGenerator::global()->bounded(leftBorder, rightBorder);
    while (charactersLeft > 1) {
        latestText.append(makeLine(&charactersLeft));
    }
}

Writer::Writer(Book *book,
               QObject *parent)
    : QObject{parent}
    , book{book}
    , possibleCharacters{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"}
    , maxLineWidth{130}
    , minLineWidth{80}
{
    // сразу сходу соединяем сигналы начала и конца работы с книгой
    // с соотвествующими слотами у книги
    connect(this, SIGNAL(started(QList<QString>)),
            book, SLOT(read(QList<QString>)));
    connect(this, SIGNAL(finished(QList<QString>)),
            book, SLOT(write(QList<QString>)));
}

void Writer::completingWork()
{
    emit started(latestText);
    makeText();
    emit finished(latestText);
}

