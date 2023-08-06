
#include "writersmanager.h"

WritersManager::WritersManager(short count,
                               Book *book,
                               QWidget *parent)
    : count{count}
    , book{book}
    , QWidget{parent}
{
    // создание и инициализация переменных для каждого из писателей
    QList<QString> *latestText = new QList<QString>;
    QMutex *textLocker = new QMutex;
    // создание инициализированного списка писателей с случайным цветом текста
    short rgbLeftBorder = 100;
    short rgbRightBorder = 200;
    for (int i = 0; i < count; ++i) {
        writers.append(new Writer(textLocker, latestText,
                                  QColor::fromRgb(QRandomGenerator::global()->bounded(rgbLeftBorder, rgbRightBorder),
                                                  QRandomGenerator::global()->bounded(rgbLeftBorder, rgbRightBorder),
                                                  QRandomGenerator::global()->bounded(rgbLeftBorder, rgbRightBorder)).name()));
    }
    // заранее создаем и инциализируем компоновщика, чтобы использовать его в цикле далее
    QHBoxLayout *layout = new QHBoxLayout(this);
    // сигнально - слотовые и другие операции над каждым из писателей
    foreach (Writer *writer, writers) {
        // соединяем сигналы начала и конца работы писателей с соотвествующими слотами у книги
        connect(writer, SIGNAL(started(QList<QString>*)), book, SLOT(read(QList<QString>*)));
        connect(writer, SIGNAL(finished(QList<QString>*)), book, SLOT(write(QList<QString>*)));
        // сейчас сигналы прихода и ухода писателя с слотом книги
        connect(writer, SIGNAL(came(short)), book, SLOT(updateWritersNumber(short)));
        connect(writer, SIGNAL(gone(short)), book, SLOT(updateWritersNumber(short)));
        // теперь идет соединение сигнала писателя со слотом изменения текста в свежесозданном виджете плавной надписи
        SmoothlyUpdatedLabel *writerInfo = new SmoothlyUpdatedLabel(Qt::AlignCenter, 14, writer->getTextColor(), this);
        connect(writer, SIGNAL(updateInfo(QString)), writerInfo, SLOT(changeText(QString)));
        // layout settings
        layout->addWidget(writerInfo);
        // теперь работа с потоком
        QThread *thread = new QThread(this);
        // и его сигналами/слотами
        connect(thread, SIGNAL(started()), writer, SLOT(completingWork()));
        connect(thread, SIGNAL(finished()), this, SLOT(completionAnalysis()));
        connect(writer, SIGNAL(beginExecution()), this, SLOT(beginningAnalysis()));
        connect(writer, SIGNAL(endExecution()), thread, SLOT(quit()));
        writer->moveToThread(thread);
        threads.append(thread);
        thread->start();
    }
}

const QList<Writer *> WritersManager::getWriters()
{
    return writers;
}

WritersManager::~WritersManager()
{
    foreach (QThread *thread, threads) {
        thread->quit();
        thread->wait();
    }
}

void WritersManager::beginningAnalysis()
{
    short numberOfBeginners = 0;
    foreach (QThread *thread, threads) {
        if (thread->isRunning()) {
            numberOfBeginners++;
        }
    }
    // достаточно один раз отправить сигнал о приходе какого-либо писателя
    if (numberOfBeginners >= 1) {
        emit anyoneStarted();
    }
}

void WritersManager::completionAnalysis()
{
    bool isComplete = true;
    foreach (QThread *thread, threads) {
        if (thread->isRunning()) {
            isComplete = false;
        }
    }
    if (isComplete) {
        emit allFinished();
    }
}
