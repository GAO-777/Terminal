#include "debugconsole.h"

#include <QScrollBar>
#include <QTextBlock>

DebugConsole::DebugConsole(QWidget *parent) :
    QPlainTextEdit(parent)
{
    QSizePolicy policy;
    policy.setHorizontalPolicy(QSizePolicy::Preferred);

    setMaximumHeight(50);
    setSizePolicy(policy);


    QPalette p = palette();
    setPalette(p);

    history = new QStringList;
    historyPos = 0;
    insertPrompt(false);
    isLocked = false;
}


void DebugConsole::onEnter()
{
    if(textCursor().positionInBlock() == prompt.length())
    {
    insertPrompt();
    return;
    }
    QString cmd = textCursor().block().text().mid(prompt.length());
    isLocked = true;
    historyAdd(cmd);
    emit sendCMD(cmd);
}

void DebugConsole::errorMessage(QString str)
{
    QTextCharFormat format = currentCharFormat();
    QTextCharFormat oldformat = currentCharFormat();
    format.setForeground(QBrush(Qt::red));
    QFont font;
    font.setBold(true);
    format.setFont(font);
    setCurrentCharFormat(format);
    insertPlainText(">> ERROR : " + str + "\n");
    setCurrentCharFormat(oldformat);
}

void DebugConsole::warningMessage(QString str)
{
    QTextCharFormat format = currentCharFormat();
    QTextCharFormat oldformat = currentCharFormat();
    format.setForeground(QBrush(QColor(236,124,38)));
    QFont font;
    font.setBold(true);
    format.setFont(font);
    setCurrentCharFormat(format);
    insertPlainText(">> WARNING : " + str + "\n");
    setCurrentCharFormat(oldformat);
}

void DebugConsole::goodMessage(QString str)
{
    QTextCharFormat format = currentCharFormat();
    QTextCharFormat oldformat = currentCharFormat();
    format.setForeground(QBrush(QColor(0,156,39)));
    QFont font;
    font.setBold(true);
    format.setFont(font);
    setCurrentCharFormat(format);
    appendPlainText(">> " + str + "\n");
    setCurrentCharFormat(oldformat);
    insertPlainText("<< ");
}


void DebugConsole::keyPressEvent(QKeyEvent *e)
{
    if(isLocked)
        return;
    switch (e->key()) {
    case Qt::Key_Return:
            onEnter();
        break;
    case Qt::Key_Backspace:
        if( e->modifiers() == Qt::NoModifier
               && textCursor().positionInBlock() > prompt.length())
                QPlainTextEdit::keyPressEvent(e);
        break;
   case Qt::Key_Up:
            historyBack();
        break;

    case Qt::Key_Down :
             historyForward();
         break;

    default:
                QPlainTextEdit::keyPressEvent(e);
        break;
    }
    QString cmd = textCursor().block().text().mid(prompt.length());
    emit onChange(cmd);
}

void DebugConsole::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    setFocus();
}

void DebugConsole::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    setFocus();
}

void DebugConsole::contextMenuEvent(QContextMenuEvent *e)
{
    Q_UNUSED(e)
}
void DebugConsole::output(QString s, QColor c)
{
    textCursor().insertBlock();
    QTextCharFormat format;
    format.setForeground(c);
    textCursor().setBlockCharFormat(format);
    textCursor().insertText(s);
    insertPrompt();
    isLocked = false;
}

void DebugConsole::insertPrompt(bool insertNewBlock)
{
    if(insertNewBlock)
    textCursor().insertBlock();
    QTextCharFormat format;
    format.setForeground(Qt::black);
    textCursor().setBlockCharFormat(format);
    textCursor().insertText(prompt);
    scrollDown();
}

void DebugConsole::scrollDown()
{
    QScrollBar *vbar = verticalScrollBar();
    vbar->setValue(vbar->maximum());
}

void DebugConsole::historyAdd(QString cmd)
{
    history->append(cmd);
    historyPos = history->length();
}

void DebugConsole::historyBack()
{
    if(!historyPos)
    return;
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.insertText(prompt + history->at(historyPos-1));
    setTextCursor(cursor);
    historyPos--;
}

void DebugConsole::historyForward()
{
    if(historyPos == history->length())
    return;
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    if(historyPos == history->length() - 1)
    cursor.insertText(prompt);
    else
    cursor.insertText(prompt + history->at(historyPos + 1));
    setTextCursor(cursor);
    historyPos++;
}
