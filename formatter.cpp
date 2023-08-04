
#include "formatter.h"

Formatter::Formatter(QObject *parent)
    : QObject{parent}
{
}

QList<QString> Formatter::splitIntoBookLines(const QList<QString> text)
{
    short thisIndex = 0;
    QList<QString> result;

    foreach (QString listElement, text) {
        thisIndex += listElement.size(); // сразу измеряем ширину элемента коллекции
        // а теперь смотрим выходит ли данная строка за границы книги или нет
        // и в зависимости от результата ставим либо перенос строки, либо пробел между словами
        if (thisIndex >= maxLineWidth && listElement.indexOf("\n") == -1) {
            listElement.append("\n");
            thisIndex = 0;
        }
        else if (!listElement.endsWith(" ")) {
            listElement.append(" ");
            thisIndex++;
        }

        result.append(listElement);
    }
    return result;
}

