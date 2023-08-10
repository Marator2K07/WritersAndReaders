
#ifndef AUTOSCROLLABLETEXTEDIT_H
#define AUTOSCROLLABLETEXTEDIT_H

#include <QTextEdit>
#include <QWidget>
#include <QList>

class AutoScrollableTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    AutoScrollableTextEdit(QWidget *parent = nullptr);    

public slots:
    void insertHtmlWithAutoScroll(const QString &text);
    void updateText(const QList<QString> text);
};

#endif // AUTOSCROLLABLETEXTEDIT_H
