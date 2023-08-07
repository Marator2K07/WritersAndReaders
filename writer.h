
#ifndef WRITER_H
#define WRITER_H

#include <QObject>
#include <QRandomGenerator>
#include <QMutex>
#include <QThread>

class Writer : public QObject
{
    Q_OBJECT
private:
    QMutex *textLocker; // мьютекс для операций записи новых слов в текст
    QList<QString> *latestText;
    const QString possibleCharacters; // в этой строке представлены символы, которые использует писатель в книге
    const QString textColor; // текстовое представление цвета (в RGB стиле) дописываемого в книгу текста
    const short pauseDuration; // в миллисекундах

private:
    QString makeWord(short *charactersLeft);

public:
    explicit Writer(QMutex *textLocker,
                    QList<QString> *latestText,
                    QString textColor,
                    QObject *parent = nullptr);
    const QString getTextColor();

signals:
    /// начал писать книгу
    void started(QList<QString>*);
    /// закончил писать книгу
    void finished(QList<QString>*);
    /// для оповещения увеличения количества работающих писателей
    void came(short);
    /// для оповещения уменьшения количества работающих писателей
    void gone(short);
    /// для оповещения происходящих событий в виджете надписи
    void updateInfo(const QString);
    /// сигнал для записи слова в основной виджет для показа текста
    void writeWord(const QString);
    /// уведомление завершения основного кода для выполнения
    void endExecution();

public slots:
    ///
    /// \brief completingWork основной слот-метод класса, в котором писатель во
    /// время пришедшего вдохновения продолжает писать книгу
    void completingWork();

};

#endif // WRITER_H
