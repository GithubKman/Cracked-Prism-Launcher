#include "AddPasswordDialog.h"
#include "ui_AddPasswordDialog.h"

#include "minecraft/auth/AccountTask.h"

#include <QtWidgets/QPushButton>
#include <QString>
#include <QFile>
#include <QTextStream>

AddPasswordDialog::AddPasswordDialog(QWidget* parent) : QDialog(parent), ui(new Ui::AddPasswordDialog)
{
    ui->setupUi(this);
    ui->progressBar->setVisible(false);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

AddPasswordDialog::~AddPasswordDialog()
{
    delete ui;
}

// Stage 1: User interaction
void AddPasswordDialog::accept()
{
    setUserInputsEnabled(false);
    ui->progressBar->setVisible(true);

    // Get the user's input for the password and save it to a text file
    QString password = ui->userTextBox->text();
    QString filename = m_profileName + "-password.txt";
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "profileName: " << m_profileName << "\n";
        out << "password: " << password;
    }
    ui->progressBar->setVisible(false);
    QDialog::accept();
}

void AddPasswordDialog::setUserInputsEnabled(bool enable)
{
    ui->userTextBox->setEnabled(enable);
    ui->buttonBox->setEnabled(enable);
}

void AddPasswordDialog::on_allowLongUsernames_stateChanged(int value)
{
    if (value == Qt::Checked) {
        ui->userTextBox->setMaxLength(INT_MAX);
    } else {
        ui->userTextBox->setMaxLength(116);
    }
}

// Enable the OK button only when the textbox contains something.
void AddPasswordDialog::on_userTextBox_textEdited(const QString& newText)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!newText.isEmpty());
}

// Public interface
void AddPasswordDialog::newPassword(QWidget* parent, QString msg, QString profileName)
{
    AddPasswordDialog dlg(parent);
    dlg.ui->label->setText(msg);
    dlg.m_profileName = profileName; 

    if (dlg.exec() == QDialog::Accepted) {
        // Do nothing for now
        return;
    }
    return;
}
