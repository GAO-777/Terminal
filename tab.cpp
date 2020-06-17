#include "tab.h"
#include<QThread>
#include<QFile>
#include<QDir>
#include<QFileInfo>

Tab::Tab(QWidget *parent) : QWidget(parent)
{
   // m_desktop = new QWidget(this);
   // m_desktop->setMinimumSize(QSize(500,500));






    QSizePolicy desktopPolicy;
    desktopPolicy.setVerticalPolicy(QSizePolicy::Preferred);
    desktopPolicy.setHorizontalPolicy(QSizePolicy::Preferred);
   // m_desktop->setSizePolicy(desktopPolicy);

    // Индикатор соединения
    QLabel *m_label1 = new QLabel("Console");
    m_led = new QLabel(this);


    m_debugConsole = new DebugConsole(this);
    connect(m_debugConsole, &DebugConsole::sendCMD, this, &Tab::doCommand);


    // Консоль
    m_console = new Console(parent);
    //m_console->setMaximumWidth(500);
    connect(m_console, &Console::getData, this, &Tab::writeData);



    m_connect = new QPushButton(parent);

    m_connectSettings = new QPushButton("Settings",parent);
    connect(m_connectSettings,&QPushButton::clicked,this,&Tab::settings_action);

    QHBoxLayout *m_pbLayout = new QHBoxLayout(parent);
    m_pbLayout->addWidget(m_led);
    m_pbLayout->addWidget(m_connect);

    m_VLayout = new QVBoxLayout(parent);
    m_HLayout = new QHBoxLayout(parent);
    QVBoxLayout *m_VLayoutConsole = new QVBoxLayout(parent);
    m_VLayoutConsole->addWidget(m_label1);
    m_VLayoutConsole->addWidget(m_console,Qt::AlignRight);
    m_VLayoutConsole->addLayout(m_pbLayout,Qt::AlignRight);
    m_VLayoutConsole->addWidget(m_connectSettings);
   // m_HLayout->addWidget(m_desktop);
    m_HLayout->addLayout(m_VLayoutConsole);
    m_VLayout->addLayout(m_HLayout);
    m_VLayout->addWidget(m_debugConsole);
    setLayout(m_VLayout);


    settings_action();
    setEnableConsole(false);
}

void Tab::connect_action()
{
    switch (typeContact) {
        case SettingsDialog::Serial:
                serialInit();
        break;

        case SettingsDialog::SSH:
                sshInit();
        break;
    }
}

void Tab::doCommand(QString command)
{

    QStringList words = command.split(' ', QString::SkipEmptyParts);


    if(words[0] == "do"){
        if(words.size() == 1)
            m_debugConsole->output("Specify the file to execute!","e");
        else doIt(words[1]);
    }
    else if(words[0] == "r") {


    }
    else if(words[0] == "d") {



    }
    else {
        m_debugConsole->output("Сommand not found","e");
    }



}

void Tab::setEnableConsole(bool t)
{
    /* Отрабатывает подлючение и отлючение контакта
       true - подлючение (открыть консоль), поменять цвет индикатора и тд*/
    if(t) {
        m_led->setStyleSheet("background-color: green;");
        m_connect->setText("Disconnect");
        disconnect(m_connect,&QPushButton::clicked,this,&Tab::connect_action);
        connect(m_connect,&QPushButton::clicked,this,&Tab::disconnect_action);
        m_console->setEnabled(true);
        m_connectSettings->setEnabled(false);
    }else {
        m_led->setStyleSheet("background-color: red;");
        m_connect->setText("Connect");
        connect(m_connect,&QPushButton::clicked,this,&Tab::connect_action);
        disconnect(m_connect,&QPushButton::clicked,this,&Tab::disconnect_action);
        m_console->setEnabled(false);
        m_connectSettings->setEnabled(true);
    }
}

void Tab::disconnect_action()
{
    m_serial->close();
    setEnableConsole(false);
    m_debugConsole->output("СOM PORT is closed","m");
}

void Tab::settings_action()
{
    SettingsDialog *m_ContactSettings = new SettingsDialog(this);
    connect(m_ContactSettings,&SettingsDialog::sendSerialParameters,this,&Tab::receiveSerialParameters);
    connect(m_ContactSettings,&SettingsDialog::sendSSHParameters,this,&Tab::receiveSSHParameters);
    switch (typeContact) {
        case SettingsDialog::Serial:
            m_ContactSettings->setSerialParameters(serialParameters);
            m_ContactSettings->showParametersSerilParameters();
        break;

        case SettingsDialog::SSH:
            m_ContactSettings->setSSHParameters(sshParameters);
            m_ContactSettings->showParametersSSHParameters();
        break;
    }

    m_ContactSettings->show();
}

void Tab::serialInit()
{
    // === SERIAL === //
    m_serial = new QSerialPort();
    connect(m_serial, &QSerialPort::readyRead, this, &Tab::readData);
    m_serial->setPortName(serialParameters.name);
    m_serial->setBaudRate(serialParameters.baudRate);
    m_serial->setDataBits(serialParameters.dataBits);
    m_serial->setParity(serialParameters.parity);
    m_serial->setStopBits(serialParameters.stopBits);
    m_serial->setFlowControl(serialParameters.flowControl);

    if (m_serial->open(QIODevice::ReadWrite)){
        setEnableConsole(true);
        m_debugConsole->output("СOM PORT successfully opened","m");
    }
    else {
        setEnableConsole(false);
        m_debugConsole->output("СOM PORT cannot be opened","e");
    }
}

void Tab::sshInit()
{
    if(sshParameters.isTCP){
        m_socket = new QAbstractSocket(QAbstractSocket::TcpSocket,this);
    } else {
        m_socket = new QAbstractSocket(QAbstractSocket::UdpSocket,this);
    }

}

void Tab::receiveSerialParameters(SettingsDialog::SerialParameters paremeters)
{
    typeContact = SettingsDialog::Serial;
    serialParameters = paremeters;
}

void Tab::receiveSSHParameters(SettingsDialog::SSHParameters paremeters)
{
    typeContact = SettingsDialog::SSH;
    sshParameters = paremeters;
}

void Tab::readData()
{
    const QByteArray data = m_serial->readAll();
    m_console->putData(data);
}
void Tab::writeData(QString data)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QByteArray encodedData = codec->fromUnicode(data);
    m_serial->write(encodedData);
}

void Tab::doIt(QString pathFile)
{
    QStringList strList;
    QFile file(pathFile,this);
    QFileInfo fileInfo(file);

    if(fileInfo.suffix() == "do"){

        if(file.exists(pathFile))
        {
            if(file.open(QIODevice::ReadOnly))
            {
                while(!file.atEnd())
                    doCommand(file.readLine());
                m_debugConsole->output("Done","m");
            } else {
                m_debugConsole->output("The file is not open!","e");
            }
        } else if(file.exists(QDir::currentPath()+ "/" + pathFile)){

            if(file.open(QIODevice::ReadOnly))
            {
                while(!file.atEnd())
                    doCommand(file.readLine());
                m_debugConsole->output("Done","m");
            } else {
                m_debugConsole->output("The file is not open!","e");
            }
        }
        else{
            m_debugConsole->output("The file does not exist!","e");
        }
    }else m_debugConsole->output("Invalid file extension!","e");
    file.close();
}

void Tab::r(QString addressesWord)
{
    QStringList addresses = addressesWord.split(',', QString::SkipEmptyParts);
    int sizeAdresses = addresses.size();
    int8_t C = 0x1;
    int16_t A[sizeAdresses];
    for (int i = 0; i < sizeAdresses; i++) {
        int a = addresses[i].toInt();
        A[i] = a;
    }
    //writeData(address);
    //writeData("\n");
}

void Tab::w(QString address, QString data)
{

}

