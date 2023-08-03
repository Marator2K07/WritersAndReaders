
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

