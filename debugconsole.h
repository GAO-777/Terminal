#ifndef DEBUGCONSOLE_H
#define DEBUGCONSOLE_H

#include <QPlainTextEdit>
#include <QTextCodec>

class DebugConsole : public QPlainTextEdit
{
    Q_OBJECT

signals:
    void sendCMD(QString command);
    void onChange(QString);

public:
    explicit DebugConsole(QWidget *parent = nullptr);
    void output(QString, QColor c);
    void scrollDown();
    void insertPrompt(bool insertNewBlock = true);
    void onEnter();
    void errorMessage(QString str);
    void warningMessage(QString str);
    void goodMessage(QString str);
    QString prompt = "<< ";
    QString userText;
    QStringList CommandHistory;

private:
    bool isLocked;
    QStringList *history;
    int historyPos;
    void historyAdd(QString);
    void historyBack();
    void historyForward();
protected:
    void keyPressEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void contextMenuEvent(QContextMenuEvent *e) override;

private:

};
#endif // DEBUGCONSOLE_H
