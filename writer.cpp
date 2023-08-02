
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

Writer::Writer(QMutex *openWriteLocker,
               QMutex *textLocker,
               QList<QString> *latestText,
               QObject *parent)
    : openWriteLocker{openWriteLocker}
    , textLocker{textLocker}
    , latestText{latestText}
    , QObject{parent}
    , possibleCharacters{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"}
    , maxLineWidth{130}
    , minLineWidth{80}
{
}

void Writer::working()
{
    /// тестовая реализация, но недалеко от правды находящаяся
    ///
    QThread::msleep(3333);
    // при открытии файла и записи данных из него используем мьютекс
    openWriteLocker->lock();
    emit came(1); // также в синхронайзд области находится и изменение количества писателей
    emit started(latestText);
    openWriteLocker->unlock();
    short leftBorder = 3;
    short rightBorder = 333;
    short charactersLeft = QRandomGenerator::global()->bounded(leftBorder, rightBorder);
    while (charactersLeft >= 1) {
        QThread::msleep(123);
        // что бы и при добавлении новых слов проблем не было, используем другой мьютекс
        textLocker->lock();
        latestText->append(makeWord(&charactersLeft));
        textLocker->unlock();
    }
    // при открытии файла и записи данных в него используем мьютекс
    openWriteLocker->lock();
    emit finished(latestText);
    emit gone(-1); // также в синхронайзд области находится и изменение количества писателей
    openWriteLocker->unlock();
}

