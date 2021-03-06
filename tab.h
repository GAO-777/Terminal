#ifndef TAB_H
#define TAB_H

#include <QWidget>
#include <QObject>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSerialPort>
#include <QLineEdit>
#include <QAbstractSocket>
#include "console.h"
#include "settingsdialog.h"
#include "debugconsole.h"


class Tab : public QWidget
{
    Q_OBJECT
public:
    explicit Tab(QWidget *parent = nullptr);
    // ==== Управление (Кнопки) === //
    QPushButton *m_connect;         // открываем порт
    QPushButton *m_connectSettings; // настройки подлючения
    // ==== Управление === //
    Console *m_console;             // консоль порта
    DebugConsole *m_debugConsole;   // отладочная консоль
    QLabel *m_led;                  // индикатор успешного подлючения
    // ==== Верстка === //
    QVBoxLayout *m_VLayout; // главный вертикальный Layout
    QHBoxLayout *m_HLayout; // главный горзонтальный Layout
    QWidget *m_desktop;     // рабочий стол пользователя

    // ==== Параметры подлючения === //
    SettingsDialog::TypeContact typeContact;            // тип подлючения
    SettingsDialog::SerialParameters serialParameters;  // параметры подлючения по Serial
    SettingsDialog::SSHParameters sshParameters;        // параметры подлючения по SSH

    // === Указатели на подлючения === //
    QSerialPort *m_serial;
    QAbstractSocket *m_socket;

    // ==== Подлючение файлов === //
    QString pathCL;

   // === Метод ... === //
public slots:
    // ==== Управление (Кнопки) === //
    void connect_action();          // отрабатывает нажатие на кнопку Connect
    void disconnect_action();       // разорвать соединение
    void doCommand(QString command);             // принимает команду от пользователя из консоли команд
    void settings_action();         // вызов настроек подлючения

    // ==== Общие === //
    void serialInit();  // инициализация последовательного порта
    void sshInit();  // инициализация ssh соединения
    void setEnableConsole(bool t);  // закрыть(false)/открыть(true) кнопки и консоль
    void receiveSerialParameters(SettingsDialog::SerialParameters paremeters);  // принять праметры подлючения по Serial
    void receiveSSHParameters(SettingsDialog::SSHParameters paremeters);        // принять праметры подлючения по SSH

    // ==== Взаимодействие с портом ==== //
    void readData();                // прочитать данные из порта
    void writeData(QString data);   // запись в порт
    bool sendInt(QString data, int size);
    bool sendPackage(QString data);

    // === Список команд для сценария === //
    int doIt(QString pathFile);
    QString replaceToHEX(QString str);


    // ==== Работа с файлами === //
    QString fileCheck(QString pathFile, QString extension); // проверка файла на пригодность

};

#endif // TAB_H
