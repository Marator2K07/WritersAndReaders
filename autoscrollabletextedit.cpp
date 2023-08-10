
#include "autoscrollabletextedit.h"

AutoScrollableTextEdit::AutoScrollableTextEdit(QWidget *parent)
    : QTextEdit(parent)
{
}

void AutoScrollableTextEdit::insertHtmlWithAutoScroll(const QString &text)
{
    insertHtml(text);
    moveCursor(QTextCursor::End);
    ensureCursorVisible();
}

void AutoScrollableTextEdit::updateText(const QList<QString> text)
{
    setTextColor(QColor(Qt::white));
    foreach (QString line, text) {
        append(line);
    }
    append("");
}

