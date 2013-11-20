#ifndef GUI_MENUDIALOG_H
#define GUI_MENUDIALOG_H

#include <QDialog>

#include "ui_MenuDialog.h"

namespace gui
{
    class MenuDialog: public QDialog
    {
        Q_OBJECT
    public:
        MenuDialog(QWidget *parent = 0);

    private slots:
        void onHexByte();
        void onMultiplication();
        void onDict();

    private:
        void connectToSignals();
        
    private:
        Ui::MenuDialog ui;
    };
}

#endif
