
#include "reader.h"

void Reader::introduction()
{
    emit updateBookText(&bookText);
    emit updateInfo("Читатель пришел");
    QThread::msleep(QRandomGenerator::global()->bounded(pauseDuration, pauseDuration * 2));
    emit updateInfo("Начинает читать книгу");
    QThread::msleep(pauseDuration);
}

void Reader::reading()
{
    bool writersIsActive = false;
    foreach (QString line, bookText) {
        emit checkWritersActivity(&writersIsActive);
        // на каждой итерации смотрим, не появился ли писатель,
        // если появился, то переходим в режим ожидания его ухода
        if (writersIsActive) {
            waitConditionLocker->lock();
            emit updateInfo("Ждет, пока писатель/писатели допишет/допишут книгу");
            QThread::msleep(pauseDuration / 2);
            writersInactivity->wait(waitConditionLocker);
            emit updateInfo("Продолжает читать после ухода писателя");
            QThread::msleep(pauseDuration / 2);
            waitConditionLocker->unlock();
        }
        emit updateInfo(line);
        QThread::msleep(pauseDuration / 2);
    }
}

void Reader::conclusion()
{
    emit updateInfo("Закончил читать");
    QThread::msleep(pauseDuration);
    emit updateInfo("Читатель ушел");
    QThread::msleep(pauseDuration);
    emit updateInfo("");
    emit endExecution(); // завершаем поток чтения одновременно с завершением метода
}

Reader::Reader(QWaitCondition *writersInactivity,
               QMutex *waitConditionLocker,
               QObject *parent)
    : writersInactivity{writersInactivity}
    , waitConditionLocker{waitConditionLocker}
    , pauseDuration{700}
    , QObject{parent}
{
}

void Reader::completingWork()
{
    introduction();
    reading();
    conclusion();
}
