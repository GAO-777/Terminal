#ifndef DEBUGCONSOLE_H
#define DEBUGCONSOLE_H

#include <QPlainTextEdit>
#include <QTextCodec>

class DebugConsole : public QPlainTextEdit
{
    Q_OBJECT

signals:
    void enterPressed(QString command);

public:
    explicit DebugConsole(QWidget *parent = nullptr);
    void errorMessage(QString str);
    void warningMessage(QString str);
    void goodMessage(QString str);
    QString userText;

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void contextMenuEvent(QContextMenuEvent *e) override;

private:

};
#endif // DEBUGCONSOLE_H
