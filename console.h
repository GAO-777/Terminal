#ifndef CONSOLE_H
#define CONSOLE_H

#include <QPlainTextEdit>
#include <QTextCodec>
class Console : public QPlainTextEdit
{
    Q_OBJECT

signals:
    void getData(QString data);


public:
    explicit Console(QWidget *parent = nullptr);

    void putData(const QByteArray data);
    bool dontPrint_Flag;
    QString userText;
protected:
    void keyPressEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void contextMenuEvent(QContextMenuEvent *e) override;

private:

};

#endif // CONSOLE_H
