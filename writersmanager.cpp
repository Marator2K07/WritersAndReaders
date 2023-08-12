
#include "writersmanager.h"

WritersManager::WritersManager(short count,
                               Book *book,
                               QWaitCondition *writersInactivity,
                               QWidget *parent)
    : count{count} 
    , book{book}
    , writersInactivity{writersInactivity}
    , minWaitingTime{3000}
    , maxWaitingTime{5500}
    , QWidget{parent}
{
    // создание и инициализация переменных для каждого из писателей
    QList<QString> *latestText = new QList<QString>;
    QMutex *textLocker = new QMutex;
    currentLineWidth = new short(0);
    // создание инициализированного списка писателей с случайным цветом текста
    short rgbLeftBorder = 100;
    short rgbRightBorder = 200;
    for (int i = 0; i < count; ++i) {
        writers.append(new Writer(textLocker, latestText,
                                  QColor::fromRgb(QRandomGenerator::global()->bounded(rgbLeftBorder, rgbRightBorder),
                                                  QRandomGenerator::global()->bounded(rgbLeftBorder, rgbRightBorder),
                                                  QRandomGenerator::global()->bounded(rgbLeftBorder, rgbRightBorder)).name(),
                                  currentLineWidth));
    }
    // заранее создаем и инциализируем компоновщика, чтобы использовать его в цикле далее
    QHBoxLayout *layout = new QHBoxLayout(this);
    // сигнально - слотовые и другие операции над каждым из писателей
    foreach (Writer *writer, writers) {
        // соединяем сигналы писателей с соотвествующими слотами у книги
        connect(writer, SIGNAL(started(QList<QString>*)), book, SLOT(remember(QList<QString>*)));
        connect(writer, SIGNAL(finished(QList<QString>*)), book, SLOT(finish(QList<QString>*)));
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
        connect(writer, SIGNAL(endExecution()), thread, SLOT(quit()));
        writer->moveToThread(thread);
        threads.append(thread);
    }
    // через время от 3 до 5.5 секунд запускаем потоки писателей
    QTimer::singleShot(QRandomGenerator::global()->bounded(minWaitingTime, maxWaitingTime),
                       this, SLOT(startWriting()));
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

void WritersManager::writersActivityAnalysis(bool *activity)
{
    foreach (QThread *thread, threads) {
        if (thread->isRunning()) {
            *activity = true;
            return;
        }
    }
    *activity = false;
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
        writersInactivity->wakeAll(); // БУДИМ всех читателей
        *currentLineWidth = 0; // обнуляем текущую ширину строки
        // после ухода последнего писателя начинаем все действия заново..
        QTimer::singleShot(QRandomGenerator::global()->bounded(minWaitingTime, maxWaitingTime),
                           this, SLOT(startWriting()));
    }
}

void WritersManager::startWriting()
{
    foreach (QThread *thread, threads) {
        thread->start();
    }
}
