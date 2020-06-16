#include "tab.h"
#include<QThread>
#include<QFile>
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
        m_debugConsole->goodMessage("СOM PORT successfully opened");
    }
    else {
        setEnableConsole(false);
        m_debugConsole->errorMessage("СOM PORT cannot be opened");
    }
}

void Tab::doCommand(QString command)
{

    QStringList words = command.split(' ', QString::SkipEmptyParts);


    if(words[0] == "do") {
         m_debugConsole->errorMessage(words[1]);
         doIt(words[1]);
         //doIt("D:/Projects/GitHub/Terminal/doFile.txt");
        // m_debugConsole->output("done");

    }
    else if(words[0] == "r") {
 m_debugConsole->errorMessage("!!!!!!!!!!");
 //m_debugConsole->output("done");
    }
    else if(words[0] == "d") {
        const char t[3] ={0x31,0x1,0x7};
           m_serial->write(&t[0]);
           QThread::sleep(2);
           m_serial->write(&t[1]);
           QThread::sleep(2);
           m_serial->write(&t[2]);


    }
    else {
        m_debugConsole->output("Сommand not found",QColor(Qt::red));
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
    m_debugConsole->goodMessage("СOM PORT is closed");
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

    QFile file(pathFile);
    if(file.exists())
    {
        if(file.open(QIODevice::ReadOnly))
        {
            while(!file.atEnd())
                doCommand(file.readLine());
        } else {
            m_debugConsole->errorMessage("The file is not open!");
        }
    } else {
        m_debugConsole->errorMessage("The file does not exist!");
    }
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

