#include "AddPasswordDialog.h"
#include "ui_AddPasswordDialog.h"

#include "minecraft/auth/AccountTask.h"

#include <QtWidgets/QPushButton>

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

    // Setup the login task and start it
    m_account = MinecraftAccount::createOffline(ui->userTextBox->text());
    m_loginTask = m_account->loginOffline();
    connect(m_loginTask.get(), &Task::failed, this, &AddPasswordDialog::onTaskFailed);
    connect(m_loginTask.get(), &Task::succeeded, this, &AddPasswordDialog::onTaskSucceeded);
    connect(m_loginTask.get(), &Task::status, this, &AddPasswordDialog::onTaskStatus);
    connect(m_loginTask.get(), &Task::progress, this, &AddPasswordDialog::onTaskProgress);
    m_loginTask->start();
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
        ui->userTextBox->setMaxLength(16);
    }
}

// Enable the OK button only when the textbox contains something.
void AddPasswordDialog::on_userTextBox_textEdited(const QString& newText)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!newText.isEmpty());
}

void AddPasswordDialog::onTaskFailed(const QString& reason)
{
    // Set message
    auto lines = reason.split('\n');
    QString processed;
    for (auto line : lines) {
        if (line.size()) {
            processed += "<font color='red'>" + line + "</font><br />";
        } else {
            processed += "<br />";
        }
    }
    ui->label->setText(processed);

    // Re-enable user-interaction
    setUserInputsEnabled(true);
    ui->progressBar->setVisible(false);
}

void AddPasswordDialog::onTaskSucceeded()
{
    QDialog::accept();
}

void AddPasswordDialog::onTaskStatus(const QString& status)
{
    ui->label->setText(status);
}

void AddPasswordDialog::onTaskProgress(qint64 current, qint64 total)
{
    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(current);
}

// Public interface
MinecraftAccountPtr AddPasswordDialog::newPassword(QWidget* parent, QString msg)
{
    AddPasswordDialog dlg(parent);
    dlg.ui->label->setText(msg);
    if (dlg.exec() == QDialog::Accepted) {
        return dlg.m_account;
    }
    return nullptr;
}
