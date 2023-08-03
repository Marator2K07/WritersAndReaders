
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
               QString textColor,
               QObject *parent)
    : openWriteLocker{openWriteLocker}
    , textLocker{textLocker}
    , latestText{latestText}
    , possibleCharacters{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"}
    , textColor{textColor}
    , maxLineWidth{130}
    , minLineWidth{80}
    , QObject{parent}
{
}

const QString Writer::getTextColor()
{
    return textColor;
}

void Writer::working()
{
    /// тестовая реализация, уже более продвинутая
    ///
    short defaultPause = 1500;
    short leftPauseBorder = 2222;
    short rightPauseBorder = 3333;

    emit updateInfo("Писатель пришел");
    QThread::msleep(QRandomGenerator::global()->bounded(leftPauseBorder, rightPauseBorder));
    emit updateInfo("Дождался вдохновения");
    QThread::msleep(defaultPause);
    emit updateInfo("Начинает дописывать книгу");
    QThread::msleep(defaultPause);

    // при открытии файла и записи данных из него используем мьютекс
    openWriteLocker->lock();
    emit came(1); // также в синхронайзд области находится и изменение количества писателей
    emit started(latestText);
    openWriteLocker->unlock();
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
    // при открытии файла и записи данных в него используем мьютекс
    openWriteLocker->lock();
    emit finished(latestText);
    emit gone(-1); // также в синхронайзд области находится и изменение количества писателей
    openWriteLocker->unlock();

    emit updateInfo("Вдохновение ушло");
    QThread::msleep(defaultPause);
    emit updateInfo("Закончил писать книгу");
    QThread::msleep(defaultPause);
    emit updateInfo("Писатель уходит");
    QThread::msleep(defaultPause);
    emit updateInfo("");
}

