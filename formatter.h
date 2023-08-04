
#ifndef FORMATTER_H
#define FORMATTER_H

#include <QObject>
#include <QList>

class Formatter : public QObject
{
    Q_OBJECT
private:
    const static short maxLineWidth = 100;

public:
    explicit Formatter(QObject *parent = nullptr);
    QList<QString> static splitIntoBookLines(const QList<QString> text);

};

#endif // FORMATTER_H
