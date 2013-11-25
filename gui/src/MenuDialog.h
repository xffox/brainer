#ifndef GUI_MENUDIALOG_H
#define GUI_MENUDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QString>

#include "ui_MenuDialog.h"

namespace gui
{
    class MenuDialog: public QDialog
    {
        Q_OBJECT
    public:
        MenuDialog(QWidget *parent = 0);

    public slots:
        void showTasks(const QStringList &tasks);

    signals:
        void selected(const QString &task);

    private slots:
        void onSelected();

    private:
        Ui::MenuDialog ui;
    };
}

#endif
