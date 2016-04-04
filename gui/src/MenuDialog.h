#ifndef GUI_MENUDIALOG_H
#define GUI_MENUDIALOG_H

#include <memory>

#include <QDialog>
#include <QStringList>
#include <QString>

#include "TaskDialog.h"
#include "ui_MenuDialog.h"

namespace core
{
    class ITaskProvider;
}

namespace gui
{
    class MenuDialog: public QDialog
    {
        Q_OBJECT
    public:
        MenuDialog(std::unique_ptr<core::ITaskProvider> taskProvider,
            QWidget *parent = 0);
        virtual ~MenuDialog();

    public slots:
        void showTasks(const QStringList &tasks);

    private slots:
        void onSelected();

    private:
        std::unique_ptr<core::ITaskProvider> taskProvider;

        Ui::MenuDialog ui;

        gui::TaskDialog taskDialog;
    };
}

#endif
