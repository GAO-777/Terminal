#include "console.h"
#include <QMenu>
#include <QScrollBar>

Console::Console(QWidget *parent) :
    QPlainTextEdit(parent)
{
    setMinimumHeight(300);
    document()->setMaximumBlockCount(100);
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::green);
    setPalette(p);
    dontPrint_Flag = true;

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,&Console::customContextMenuRequested,this,&Console::tableContextMenu);
}

void Console::putData(const QByteArray data)
{
    if(dontPrint_Flag){
        if(HEX_mode) insertPlainText(data.toHex());
        else insertPlainText(data);
    }
    else dontPrint_Flag = true;
    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}

void Console::changePrintMode()
{   HEX_mode = !HEX_mode;   }

void Console::tableContextMenu()
{
    QMenu * menu = new QMenu(this);

    //=== Выбор режима печати консоли ===//
    QAction* printMode = new QAction(this);
    printMode->setCheckable(true);
    printMode->setChecked(HEX_mode);
    if(HEX_mode) printMode->setText("Режим печати: HEX -> UTF-8");
    else printMode->setText("Режим печати: UTF-8 -> HEX");
    connect(printMode,&QAction::triggered,this,&Console::changePrintMode);



    //=== Добавляю в меню ===//
    menu->addSeparator();
    menu->addAction(printMode);
    //=== Показываю меню ===//
    menu->popup(QCursor::pos());
}

void Console::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
        case Qt::Key_Return:
            QPlainTextEdit::keyPressEvent(e);
            emit getData(e->text());
            userText.clear();
            break;
        case Qt::Key_Tab:
            emit getData("\x09");
            break;
        case Qt::Key_Backspace:
            if(!userText.isEmpty()) {
                QPlainTextEdit::keyPressEvent(e);
                emit getData(e->text());
                userText.chop(1);
                dontPrint_Flag = false;
            }
            break;
        case Qt::Key_Left:
            moveCursor(QTextCursor::Left);
            emit getData("\x1b\x5b\x44");
            dontPrint_Flag = false;
            break;
        case Qt::Key_Right:
            moveCursor(QTextCursor::Right);
            emit getData("\x1b\x5b\x43");
            dontPrint_Flag = false;
            break;

        default:
            dontPrint_Flag = false;
            emit getData(e->text());
            QPlainTextEdit::keyPressEvent(e);
            userText.append(e->text());
            break;
    }
}

void Console::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    setFocus();
}

void Console::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    setFocus();
}

/*void Console::contextMenuEvent(QContextMenuEvent *e)
{
    Q_UNUSED(e)
}*/
