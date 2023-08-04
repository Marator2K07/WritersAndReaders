
#include "reader.h"

Reader::Reader(QObject *parent)
    : QObject{parent}
{
}

void Reader::startProcess()
{
    /// тестовая реализация

    // подготовительная часть
    short defaultPause = 1500;
    short leftPauseBorder = 2222;
    short rightPauseBorder = 3333;
    emit updateInfo("Читатель пришел");
    emit updateBookText(&bookText);
    QThread::msleep(QRandomGenerator::global()->bounded(leftPauseBorder, rightPauseBorder));
    emit updateInfo("Начинает читать книгу");
    QThread::msleep(defaultPause);

    foreach (QString line, bookText) {
        emit updateInfo(line);
        QThread::msleep(650);
    }

    // заключительная часть
    emit updateInfo("Закончил читать");
    QThread::msleep(defaultPause);
    emit updateInfo("Читатель ушел");
    QThread::msleep(defaultPause);
    emit updateInfo("");
}
