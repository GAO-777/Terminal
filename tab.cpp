#include "tab.h"
#include<QThread>
#include<QFile>
#include<QDir>
#include<QFileInfo>
#include<QDebug>
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
    else if(words[0] == "r"){
        char sData[4] = {0x02,0x01,0x02,0x00};
        m_serial->write(sData,sizeof(sData));
        m_debugConsole->output("","e");

    }
    else if(words[0] == "w") {
       // int address = words[0].toInt();
        char sData[6] = {0x04,0x02,0x02,0x00,0x33,0x34};
        m_serial->write(sData,sizeof(sData));
        m_debugConsole->output("","e");


    }
    else if(words[0] == "s") {
        sendPackage(command.remove(0,2));
    m_debugConsole->lock(false);
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

bool Tab::sendInt(QString data, int size)
{
    if(size>4) return false;
    char sendData[size];
    bool ok = true;
    int  hexData = data.toInt(&ok,0);
    for (int i=0;i<size;i++){
        sendData[i] = (hexData >> 8*i) & 0xFF;
    }
    m_serial->write(sendData,sizeof(sendData));
}

bool Tab::sendPackage(QString data)
{
    bool ok = true;
    QStringList wordsList = data.split(' ', QString::SkipEmptyParts);
    if(wordsList.size()>255) return 0;
    int packgSize = wordsList.size() + 1;
    char sendData[packgSize];
    sendData[0] = (wordsList.size()-1)*2 +1;
    int p = 2;
    for (int i=0;i<wordsList.size();i++){
        int hex = wordsList[i].toInt(&ok,0);
        if(i==0) sendData[1] = hex;
        sendData[p]    = hex & 0xFF;
        sendData[p+1]  = (hex>>8) & 0xFF;
        p = p+2;
    }
    int writeBytes = m_serial->write(sendData,packgSize);
    if(writeBytes != packgSize) return 0;
    else return 1;

}

int Tab::doIt(QString pathFile)
{
    QFile file(fileCheck(pathFile,"do"),this);

    if(file.open(QIODevice::ReadOnly))
    {
        while(!file.atEnd())
            doCommand(file.readLine());
        m_debugConsole->print("Done","m");
        m_debugConsole->lock(false);
        file.close();
    } else {
        m_debugConsole->print("The file is not open!","e");
        m_debugConsole->lock(false);
        file.close();
        return -1;
    }
    return 0;
}

QString Tab::replaceToHEX(QString str)
{
    pathCL = "CommandList.cl";
    bool ok = true;
    QStringList wordsList = str.split(' ', QString::SkipEmptyParts);
    QStringList commandList;
    QString outStr;
    QFile file(fileCheck(pathCL,"cl"),this);

    if(file.open(QIODevice::ReadOnly))
    {
        while(!file.atEnd()){
            QString line = file.readLine();
            line.remove("\r\n");
            commandList.append(line);
        }
        file.close();
    } else {
        m_debugConsole->print("The file is not open!","e");
        m_debugConsole->lock(false);
        file.close();
        //return -1;
    }
    for (int i =0;i<wordsList.size();i++){
        for (int p =0;p<commandList.size();p++){
            QStringList c = commandList[p].split(':', QString::SkipEmptyParts);
            if(wordsList[i] == c[0]) wordsList[i] = c[1];
        }
        outStr =outStr +  wordsList[i] + " ";
    }
    return outStr;
}



QString Tab::fileCheck(QString pathFile, QString extension)
{
    QFile file(pathFile,this);
    QFileInfo fileInfo(file);
    // Проверка расширения файла
    if(fileInfo.suffix() != extension){
        m_debugConsole->print("Invalid file extension!","e");
        return "";
    }

    // Проверка на существование файла рядом с программой или по полному пути
    if(file.exists(QDir::currentPath()+ "/" + pathFile)) return QDir::currentPath()+ "/" + pathFile ;
    else if(file.exists(pathFile)) return pathFile;
    else{
        m_debugConsole->print("The file does not exist!","e");
        return "";
    }
}




