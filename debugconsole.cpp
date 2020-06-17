#include "debugconsole.h"

#include <QScrollBar>
#include <QTextBlock>

DebugConsole::DebugConsole(QWidget *parent) :
    QPlainTextEdit(parent)
{
    QSizePolicy policy;
    policy.setHorizontalPolicy(QSizePolicy::Preferred);

    setMinimumHeight(100);
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
    moveCursor(QTextCursor::End);
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

    case Qt::Key_Left :
        if(textCursor().positionInBlock() != prompt.length())
             moveCursor(QTextCursor::PreviousCharacter);
         break;

    case Qt::Key_End :
             moveCursor(QTextCursor::EndOfBlock);
         break;

    case Qt::Key_Home :
             moveCursor(QTextCursor::StartOfBlock);
             moveCursor(QTextCursor::NextWord);
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
void DebugConsole::output(QString s, QString type)
{
    textCursor().insertBlock();
    QTextCharFormat format;

    if(type == "e"){
        format.setForeground(Qt::red);
        QFont font;
        font.setBold(true);
        format.setFont(font);
        s = "ERROR : " + s;
    } else if(type == "w"){
        format.setForeground(QBrush(QColor(236,124,38)));
        QFont font;
        font.setBold(true);
        format.setFont(font);
        s = "WARNING : " + s;
    } else if(type == "m"){
        format.setForeground(Qt::blue);
        QFont font;
        font.setBold(false);
        format.setFont(font);
    }
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
