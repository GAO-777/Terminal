#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include <QSerialPort>
#include <QSerialPortInfo>
namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    enum TypeContact{SSH = 1, Serial = 2 };
    struct SerialParameters
    {
        QString name;
        qint32 baudRate;
        QSerialPort::DataBits dataBits;
        QSerialPort::Parity parity;
        QSerialPort::StopBits stopBits;
        QSerialPort::FlowControl flowControl;
    };
    struct SSHParameters
    {};

    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();
    void fillSerialParameters();
    QWidget *w_serialParameters;
    QWidget *w_sshParameters;
    void fillSSHParameters();
    void showParametersSerilParameters();
    void showParametersSSHParameters();
    void changeSerilParametersTo();
    QComboBox *cb_name;
    QComboBox *cb_baudRate;
    QComboBox *cb_dataBits;
    QComboBox *cb_parity;
    QComboBox *cb_stopBits;
    QComboBox *cb_flowControl;


    void updateSettings();
    void apply();
    void setSerialParameters(SerialParameters parameters );
    void setSSHParameters(SSHParameters parameters);
    SerialParameters serialParameters;
    SSHParameters sshParameters;
    TypeContact typeContact;

signals:
    void sendSerialParameters(SerialParameters parameters);
    void sendSSHParameters(SSHParameters sshParameters);
private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
