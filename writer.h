
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
    short *currentWidth; // ширина текущей заполняемой строки

private:
    QString makeWord(short *charactersLeft);
    ///
    /// \brief introduction вынесенная вступительная часть алгоритма
    /// основного метода-слота completingWork
    void introduction();
    ///
    /// \brief writing вынесенная основная часть алгоритма
    /// писателя главного метода-слота completingWork
    void writing();
    ///
    /// \brief conclusion вынесенная заключительная часть алгоритма
    /// основного метода-слота completingWork
    void conclusion();

public:
    explicit Writer(QMutex *textLocker,
                    QList<QString> *latestText,
                    QString textColor,
                    short *currentWidth,
                    QObject *parent = nullptr);
    const QString getTextColor();

signals:
    /// начал писать книгу
    void started(QList<QString>*);
    /// закончил писать книгу
    void finished(QList<QString>*);
    /// для оповещения происходящих событий в виджете надписи
    void updateInfo(const QString);
    /// сигнал для записи слова в основной виджет для показа текста
    void writeWord(const QString);
    /// уведомление завершения основного кода для выполнения
    void endExecution();

public slots:
    ///
    /// \brief completingWork основной слот-метод класса, в котором писатель
    /// после прихода на рабочее место ждет вдохновения...
    /// во время пришедшего вдохновения продолжает писать книгу,
    /// после того как закончит - уходит
    void completingWork();

};

#endif // WRITER_H
