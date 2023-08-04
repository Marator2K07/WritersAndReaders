
#include "reader.h"

Reader::Reader(QObject *parent)
    : QObject{parent}
{
}

void Reader::startProcess()
{
    /// тестовая реализация, уже более продвинутая
    ///
    // подготовительная часть
    short defaultPause = 1500;
    short leftPauseBorder = 2222;
    short rightPauseBorder = 3333;
    emit updateInfo("Читатель пришел");
    QThread::msleep(QRandomGenerator::global()->bounded(leftPauseBorder, rightPauseBorder));
    emit updateInfo("После того, как дождался ухода писателей");
    QThread::msleep(defaultPause);
    emit updateInfo("Начинает читать книгу");
    QThread::msleep(defaultPause);
}

