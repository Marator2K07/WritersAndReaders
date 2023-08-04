
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
    short rgbRightBorder = 256;
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
        // соединяем сигналы начала и конца работы писателей
        // с соотвествующими слотами у книги
        connect(writer, SIGNAL(started(QList<QString>*)), book, SLOT(read(QList<QString>*)));
        connect(writer, SIGNAL(finished(QList<QString>*)), book, SLOT(write(QList<QString>*)));
        connect(writer, SIGNAL(came(short)), book, SLOT(updateWritersNumber(short)));
        connect(writer, SIGNAL(gone(short)), book, SLOT(updateWritersNumber(short)));
        // теперь идет соединение сигнала писателя со слотом изменения текста в свежесозданном виджете плавной надписи
        SmoothlyUpdatedLabel *writerInfo = new SmoothlyUpdatedLabel(writer->getTextColor(), this);
        connect(writer, SIGNAL(updateInfo(QString)), writerInfo, SLOT(changeText(QString)));
        // layout settings
        layout->addWidget(writerInfo);
        // теперь работа с потоком
        QThread *thread = new QThread(this);
        connect(thread, SIGNAL(started()), writer, SLOT(working()));
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

