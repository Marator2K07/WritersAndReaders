
#ifndef BOOK_H
#define BOOK_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QMutex>

class Book : public QObject
{
    Q_OBJECT
private:
    QFile book;
    QList<QString> buffer;
    short currentWritersNumber;
    QMutex access;

public:
    explicit Book(QObject *parent = nullptr);

signals:
    void clearText();
    /// обновляем текст в виджете представления текста книги
    void updateText(const QList<QString> text);

public slots:
    ///
    /// \brief finish условный писатель дописал, что хотел и сохраняет изменения
    /// \param text обновленный текст книги
    void finish(QList<QString> *text);
    ///
    /// \brief remember условный писатель вспомнил, что было в книге
    /// \param text скопированный текст книги
    void remember(QList<QString> *text);
    ///
    /// \brief simpleRead спец слот для читателей, чтобы получать текст книги
    /// \param text текст - куда нужно загрузить строки
    void simpleRead(QList<QString> *text);

};

#endif // BOOK_H
