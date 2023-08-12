
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
        *charactersLeft --; // обновляем количество оставшихся символов
    }
    return word;
}

void Writer::introduction()
{
    emit updateInfo("Писатель пришел");
    QThread::msleep(QRandomGenerator::global()->bounded(pauseDuration, pauseDuration * 2));
    emit started(latestText);
    emit updateInfo("Дождался вдохновения");
    QThread::msleep(pauseDuration);
    emit updateInfo("Начинает дописывать книгу");
    QThread::msleep(pauseDuration);
}

void Writer::writing()
{
    // создаем и инициализируем все необходимые для алгоритма переменные
    short leftBorder = 3;
    short rightBorder = 333;
    short charactersLeft = QRandomGenerator::global()->bounded(leftBorder, rightBorder);
    short maxLineWidth = 100;
    QString possibleLineBreak = "<br>";
    // писатель пишет пока не закончилось вдохновение
    while (charactersLeft >= 1) {
        QThread::msleep(pauseDuration / 4);
        // что бы и при добавлении новых слов проблем не было, используем мьютекс
        textLocker->lock();
        QString newWord = makeWord(&charactersLeft);
        emit updateInfo(newWord);
        *currentWidth += newWord.size() + 1; // обновляем текущую ширину строки
        // в зависимости от длины строки переходим на следующую или остаемся на этой
        if (*currentWidth < maxLineWidth) {
            (*latestText)[latestText->size() - 1] += (newWord + " ");
            possibleLineBreak = " ";
        }
        else {
            possibleLineBreak = "<br>";
            latestText->append(newWord + " ");
            *currentWidth = newWord.size() + 1;
        }
        // ну и в конце уже заполняем поле с текстом книги словами писателя и разблокируем мьютекс
        emit writeWord(QString("<p style=\"color:%1\">%2%3</p>").arg(textColor, newWord, possibleLineBreak));
        textLocker->unlock();
    }
}

void Writer::conclusion()
{
    emit updateInfo("Вдохновение ушло");
    QThread::msleep(pauseDuration);
    emit updateInfo("Закончил писать книгу");
    QThread::msleep(pauseDuration);
    emit finished(latestText);
    emit updateInfo("Писатель уходит");
    QThread::msleep(pauseDuration);
    emit updateInfo("");
    emit endExecution(); // завершаем поток одновременно с завершением метода
}

Writer::Writer(QMutex *textLocker,
               QList<QString> *latestText,
               QString textColor,
               short *currentWidth,
               QObject *parent)
    : textLocker{textLocker}
    , latestText{latestText}
    , possibleCharacters{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"}
    , textColor{textColor}
    , currentWidth{currentWidth}
    , pauseDuration{800}
    , QObject{parent}
{
}

const QString Writer::getTextColor()
{
    return textColor;
}

void Writer::completingWork()
{
    introduction();
    writing();
    conclusion();
}
