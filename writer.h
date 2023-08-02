
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
    QMutex *openWriteLocker; // мьютекс для операций загрузки/сохранения
    QMutex *textLocker; // мьютекс для операций записи новых слов в текст
    QList<QString> *latestText;
    const QString possibleCharacters; // в этой строке представлены символы, которые использует писатель в книге
    const short maxLineWidth;
    const short minLineWidth;

private:
    QString makeWord(short *charactersLeft);

public:
    explicit Writer(QMutex *openWriteLocker,
                    QMutex *textLocker,
                    QList<QString> *latestText,
                    QObject *parent = nullptr);

signals:
    /// начал писать книгу
    void started(QList<QString>*);
    /// закончил писать книгу
    void finished(QList<QString>*);
    /// для оповещения увеличения количества работающих писателей
    void came(short);
    /// для оповещения уменьшения количества работающих писателей
    void gone(short);

public slots:
    ///
    /// \brief working основной слот-метод класса, в котором писатель во
    /// время пришедшего вдохновения продолжает писать книгу
    void working();
};

#endif // WRITER_H
