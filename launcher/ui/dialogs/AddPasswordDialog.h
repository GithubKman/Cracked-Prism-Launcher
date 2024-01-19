#pragma once

#include <QtCore/QEventLoop>
#include <QtWidgets/QDialog>

#include "minecraft/auth/MinecraftAccount.h"
#include "tasks/Task.h"

namespace Ui {
class AddPasswordDialog;
}

class AddPasswordDialog : public QDialog {
    Q_OBJECT

   public:
    ~AddPasswordDialog();

    static MinecraftAccountPtr newAccount(QWidget* parent, QString message);

   private:
    explicit AddPasswordDialog(QWidget* parent = 0);

    void setUserInputsEnabled(bool enable);

   protected slots:
    void accept();

    void onTaskFailed(const QString& reason);
    void onTaskSucceeded();
    void onTaskStatus(const QString& status);
    void onTaskProgress(qint64 current, qint64 total);

    void on_userTextBox_textEdited(const QString& newText);
    void on_allowLongUsernames_stateChanged(int value);

   private:
    Ui::AddPasswordDialog* ui;
    MinecraftAccountPtr m_account;
    Task::Ptr m_loginTask;
};
