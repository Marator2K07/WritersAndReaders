
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
    *charactersLeft -= wordLenght; // обновляем количество оставшихся символов
    // один к 6 шанс поставить точку, чтобы больше имитировать текст
    short chance = QRandomGenerator::global()->bounded(1,6);
    if (chance == 1) {
        word.append(".");
        *charactersLeft -= wordLenght; // обновляем количество оставшихся символов
    }
    return word;
}

Writer::Writer(QMutex *textLocker,
               QList<QString> *latestText,
               QString textColor,
               QObject *parent)
    : textLocker{textLocker}
    , latestText{latestText}
    , possibleCharacters{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"}
    , textColor{textColor}
    , QObject{parent}
{
}

const QString Writer::getTextColor()
{
    return textColor;
}

void Writer::completingWork()
{
    /// тестовая реализация,    

    // подготовительная часть
    emit beginExecution();
    short defaultPause = 1500;
    short leftPauseBorder = 2222;
    short rightPauseBorder = 3333;
    emit came(1);
    emit updateInfo("Писатель пришел");    
    emit started(latestText);
    QThread::msleep(QRandomGenerator::global()->bounded(leftPauseBorder, rightPauseBorder));
    emit updateInfo("Дождался вдохновения");
    QThread::msleep(defaultPause);
    emit updateInfo("Начинает дописывать книгу");
    QThread::msleep(defaultPause);

    // основная часть
    short leftBorder = 3;
    short rightBorder = 333;
    short charactersLeft = QRandomGenerator::global()->bounded(leftBorder, rightBorder);
    while (charactersLeft >= 1) {
        QThread::msleep(222);
        // что бы и при добавлении новых слов проблем не было, используем другой мьютекс
        textLocker->lock();
        QString newWord = makeWord(&charactersLeft);
        emit updateInfo(newWord);
        emit writeWord(QString("<p style=\"color:%1\">%2 </p>").arg(textColor, newWord));
        latestText->append(newWord);
        textLocker->unlock();
    }

    // заключительная часть
    emit updateInfo("Вдохновение ушло");
    QThread::msleep(defaultPause);
    emit updateInfo("Закончил писать книгу");
    QThread::msleep(defaultPause);
    emit updateInfo("Писатель уходит");
    emit finished(latestText);
    QThread::msleep(defaultPause);
    emit updateInfo("");
    emit gone(-1);
    emit endExecution(); // завершаем поток одновременно с завершением метода
}

