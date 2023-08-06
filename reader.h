
#ifndef READER_H
#define READER_H

#include <QObject>
#include <QRandomGenerator>
#include <QList>
#include <QThread>

class Reader : public QObject
{
    Q_OBJECT
private:
    QList<QString> bookText;

public:
    explicit Reader(QObject *parent = nullptr);

signals:
    /// сигнал о необходимости подгрузки всего текста книги
    void updateBookText(QList<QString> *text);
    /// для оповещения происходящих событий в виджете надписи
    void updateInfo(const QString);
    /// уведомление завершения основного кода для выполнения
    void endExecution();

public slots:
    ///
    /// \brief completingWork основной слот-метод класса, в котором читатель,
    /// пока нет писателей - спокойно читает книгу
    void completingWork();

};

#endif // READER_H
