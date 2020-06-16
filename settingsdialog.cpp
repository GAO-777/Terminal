#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    w_serialParameters = new QWidget(this);
    ui->HL_Parameters->addWidget(w_serialParameters);
    w_sshParameters = new QWidget(this);
    ui->HL_Parameters->addWidget(w_sshParameters);
    w_serialParameters->setVisible(false);
    w_sshParameters->setVisible(false);


    fillSerialParameters(); // заполняем виждеты
    fillSSHParameters();    // своими копонентами
    connect(ui->Serial_RB,&QRadioButton::clicked,this,&SettingsDialog::showParametersSerilParameters);
    connect(ui->SSH_RB,&QRadioButton::clicked,this,&SettingsDialog::showParametersSSHParameters);
    connect(ui->applyButton,&QPushButton::clicked,this,&SettingsDialog::apply);







}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::fillSerialParameters()
{
    /*Создает виджет для SERIAL*/
    QGridLayout *parametersLayout = new QGridLayout(this);

    cb_name = new  QComboBox(this);
    cb_baudRate = new  QComboBox(this);
    cb_dataBits = new  QComboBox(this);
    cb_parity = new  QComboBox(this);
    cb_stopBits = new  QComboBox(this);
    cb_flowControl = new  QComboBox(this);

    QLabel *lb_name = new QLabel("Port:");
    QLabel *lb_baudRate = new QLabel("BaudRate:");
    QLabel *lb_dataBits = new QLabel("Data bits:");
    QLabel *lb_parity = new QLabel("Parity:");
    QLabel *lb_stopBits = new QLabel("Stop bits:");
    QLabel *lb_flowControl = new QLabel("Flow control:");

    // ==== Name === //
    cb_name->clear();
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos){
        QStringList list;
        list << info.portName();
        cb_name->addItem(list.first(),list);
    }
    // ==== BaudRate === //
    cb_baudRate->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    cb_baudRate->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    cb_baudRate->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    cb_baudRate->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    // ==== Data bits === //
    cb_dataBits->addItem(QStringLiteral("5"), QSerialPort::Data5);
    cb_dataBits->addItem(QStringLiteral("6"), QSerialPort::Data6);
    cb_dataBits->addItem(QStringLiteral("7"), QSerialPort::Data7);
    cb_dataBits->addItem(QStringLiteral("8"), QSerialPort::Data8);
    cb_dataBits->setCurrentIndex(3);
    // ==== Parity === //
    cb_parity->addItem(tr("None"), QSerialPort::NoParity);
    cb_parity->addItem(tr("Even"), QSerialPort::EvenParity);
    cb_parity->addItem(tr("Odd"), QSerialPort::OddParity);
    cb_parity->addItem(tr("Mark"), QSerialPort::MarkParity);
    cb_parity->addItem(tr("Space"), QSerialPort::SpaceParity);
    // ==== Stop bits === //
    cb_stopBits->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    cb_stopBits->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    cb_stopBits->addItem(QStringLiteral("2"), QSerialPort::TwoStop);
    // ==== Flow control === //
    cb_flowControl->addItem(tr("None"), QSerialPort::NoFlowControl);
    cb_flowControl->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    cb_flowControl->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);

    // ==== Верстка === //
    parametersLayout->addWidget(lb_name,0,0);
    parametersLayout->addWidget(lb_baudRate,1,0);
    parametersLayout->addWidget(lb_dataBits,2,0);
    parametersLayout->addWidget(lb_parity,3,0);
    parametersLayout->addWidget(lb_stopBits,4,0);
    parametersLayout->addWidget(lb_flowControl,5,0);

    parametersLayout->addWidget(cb_name,0,1);
    parametersLayout->addWidget(cb_baudRate,1,1);
    parametersLayout->addWidget(cb_dataBits,2,1);
    parametersLayout->addWidget(cb_parity,3,1);
    parametersLayout->addWidget(cb_stopBits,4,1);
    parametersLayout->addWidget(cb_flowControl,5,1);
    w_serialParameters->setLayout(parametersLayout);
}

void SettingsDialog::fillSSHParameters()
{
    /*Создает виджет для SSH*/
    QGridLayout *parametersLayout = new QGridLayout(this);
    QLabel *lb_name = new QLabel("Port:");
    parametersLayout->addWidget(lb_name,0,0);
    w_sshParameters->setLayout(parametersLayout);
}

void SettingsDialog::showParametersSerilParameters()
{
    ui->Serial_RB->setChecked(true);
    w_serialParameters->setVisible(true);
    w_sshParameters->setVisible(false);
    typeContact = Serial;
    if(serialParameters.name != "")
        changeSerilParametersTo();

}
void SettingsDialog::showParametersSSHParameters()
{
    typeContact = SSH;
    ui->SSH_RB->setChecked(true);
    w_serialParameters->setVisible(false);
    w_sshParameters->setVisible(true);
}
void SettingsDialog::changeSerilParametersTo()
{
    cb_name->setCurrentIndex(cb_name->findData(serialParameters.name));
    cb_baudRate->setCurrentIndex(cb_baudRate->findData(serialParameters.baudRate));
    cb_dataBits->setCurrentIndex(cb_dataBits->findData(serialParameters.dataBits));
    cb_parity->setCurrentIndex(cb_parity->findData(serialParameters.parity));
    cb_stopBits->setCurrentIndex(cb_stopBits->findData(serialParameters.stopBits));
    cb_flowControl->setCurrentIndex(cb_flowControl->findData(serialParameters.flowControl));
}
void SettingsDialog::updateSettings()
{

    if(ui->Serial_RB->isChecked())
    {
        serialParameters.name = cb_name->currentText();

        serialParameters.baudRate = static_cast<QSerialPort::BaudRate>(
                    cb_baudRate->itemData(cb_baudRate->currentIndex()).toInt());
        serialParameters.dataBits = static_cast<QSerialPort::DataBits>(
                            cb_dataBits->itemData(cb_dataBits->currentIndex()).toInt());
        serialParameters.parity = static_cast<QSerialPort::Parity>(
                            cb_parity->itemData(cb_parity->currentIndex()).toInt());
        serialParameters.stopBits = static_cast<QSerialPort::StopBits>(
                            cb_stopBits->itemData(cb_stopBits->currentIndex()).toInt());
        serialParameters.flowControl = static_cast<QSerialPort::FlowControl>(
                            cb_flowControl->itemData(cb_flowControl->currentIndex()).toInt());
    }
}


void SettingsDialog::apply()
{
    updateSettings();

    switch (typeContact) {
        case Serial:
           emit sendSerialParameters(serialParameters);
        break;

        case SSH:
           emit sendSSHParameters(sshParameters);
        break;
    }

    close();
}

void SettingsDialog::setSerialParameters(SerialParameters parameters)
{
    serialParameters = parameters;
}

void SettingsDialog::setSSHParameters(SettingsDialog::SSHParameters parameters)
{
    sshParameters = parameters;
}
