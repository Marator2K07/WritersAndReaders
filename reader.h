
#ifndef READER_H
#define READER_H

#include <QObject>
#include <QRandomGenerator>
#include <QList>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class Reader : public QObject
{
    Q_OBJECT
private:
    QList<QString> bookText;
    const short pauseDuration; // в миллисекундах
    QWaitCondition *writersInactivity;
    QMutex *waitConditionLocker;

private:
    ///
    /// \brief introduction вынесенная вступительная часть
    /// главного метода-слота completingWork
    void introduction();
    ///
    /// \brief reading вынесенная основная часть алгоритма
    /// читателя главного метода-слота completingWork
    void reading();
    ///
    /// \brief conclusion вынесенная заключительная часть
    /// главного метода-слота completingWork
    void conclusion();

public:
    explicit Reader(QWaitCondition *writersInactivity,
                    QMutex *waitConditionLocker,
                    QObject *parent = nullptr);

signals:
    /// таким образом узнаем, активен ли в данный момент какой либо из писателей
    void checkWritersActivity(bool *activity);
    /// сигнал о необходимости подгрузки всего текста книги
    void updateBookText(QList<QString> *text);
    /// для оповещения происходящих событий в виджете надписи
    void updateInfo(const QString);
    /// уведомление завершения основного кода для выполнения
    void endExecution();    

public slots:
    ///
    /// \brief completingWork основной слот-метод класса, в котором читатель,
    /// приходит, пока нет писателей - спокойно читает книгу,
    /// и когда дочитывает - уходит
    void completingWork();

};

#endif // READER_H
