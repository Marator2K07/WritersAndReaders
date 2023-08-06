
#include "readersmanager.h"

ReadersManager::ReadersManager(short count,
                               Book *book,
                               QWidget *parent)
    : book{book}
    , count{count}
    , QWidget{parent}
{
    // заполнение списка читаталей
    for (int i = 0; i < count; ++i) {
        readers.append(new Reader());
    }
    // заранее создаем и инциализируем переменные, чтобы использовать их в цикле далее
    QVBoxLayout *layout = new QVBoxLayout(this);
    short rgbLeftBorder = 200;
    short rgbRightBorder = 256;
    // сигнально - слотовые и другие операции над каждым из писателей
    foreach (Reader *reader, readers) {
        // соединение сигнала читателя со слотом изменения текста в свежесозданном виджете плавной надписи
        // и со слотом добавления текста из книги
        QString randomColor = QColor::fromRgb(QRandomGenerator::global()->bounded(rgbLeftBorder, rgbRightBorder),
                                              QRandomGenerator::global()->bounded(rgbLeftBorder, rgbRightBorder),
                                              QRandomGenerator::global()->bounded(rgbLeftBorder, rgbRightBorder)).name();
        SmoothlyUpdatedLabel *readerInfo = new SmoothlyUpdatedLabel(Qt::AlignLeft | Qt::AlignVCenter, 2, randomColor, this);
        connect(reader, SIGNAL(updateInfo(QString)), readerInfo, SLOT(changeText(QString)));
        connect(reader, SIGNAL(updateBookText(QList<QString>*)), book, SLOT(simpleRead(QList<QString>*)));
        // layout settings
        layout->addWidget(readerInfo);
        // теперь работа с потоком
        QThread *thread = new QThread(this);
        connect(thread, SIGNAL(started()), reader, SLOT(completingWork()));
        connect(reader, SIGNAL(endExecution()), thread, SLOT(quit()));
        reader->moveToThread(thread);
        threads.append(thread);
        thread->start();
    }
}

ReadersManager::~ReadersManager()
{
    foreach (QThread *thread, threads) {
        thread->quit();
        thread->wait();
    }
}

void ReadersManager::startReading()
{
    foreach (QThread *thread, threads) {
        thread->start();
    }
}

void ReadersManager::stopReading()
{
    // даем знать всем подручным потокам, что при первой же возможности, стоить завершить работу
    foreach (QThread *thread, threads) {
        thread->requestInterruption();
    }
}

