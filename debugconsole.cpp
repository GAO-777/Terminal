#include "debugconsole.h"

#include <QScrollBar>

DebugConsole::DebugConsole(QWidget *parent) :
    QPlainTextEdit(parent)
{
    QSizePolicy policy;
    policy.setHorizontalPolicy(QSizePolicy::Preferred);

    setMaximumHeight(50);
    setSizePolicy(policy);


    insertPlainText("<< ");
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
    switch (e->key()) {
    case Qt::Key_Return:
        QPlainTextEdit::keyPressEvent(e);
        emit enterPressed(userText);
        userText.clear();
        insertPlainText("<< ");
        break;
    case Qt::Key_Backspace:
        if(!userText.isEmpty()) {
            QPlainTextEdit::keyPressEvent(e);
            userText.chop(1);
        }
        break;
    default:
        QPlainTextEdit::keyPressEvent(e);
        userText.append(e->text());
        break;
    }
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
