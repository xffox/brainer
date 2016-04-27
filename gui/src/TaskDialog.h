#ifndef GUI_TASKDIALOG_H
#define GUI_TASKDIALOG_H

#include <QDialog>
#include <QTimer>

#include <memory>

#include "core/Stats.h"
#include "core/IRender.h"

#include "ui_TaskDialog.h"

class QShortcut;

namespace task
{
    class StatsTaskLogicWatcher;
}

namespace core
{
    class ITaskGenerator;
    class TaskLogic;
}

namespace gui
{
    class TaskDialog: public QDialog, core::IRender
    {
        Q_OBJECT
    public:
        TaskDialog(QWidget *parent = 0);
        virtual ~TaskDialog();

        void setTaskGenerator(
            std::unique_ptr<core::ITaskGenerator> taskGenerator);

    signals:
        void entered(const QString &value);

    protected:
        virtual void addText(const core::String &str);

    private slots:
        void validate();
        void skip();
        void timed();

    private:
        void connectToSignals();

        void describeTask();

        void showStats();
        void showElapsed(long long elapsedUs);

        void showValid(const core::String &str);
        void showInvalid(const core::String &str);
        void showAnswer(const core::String &str);

        void clearResult();
        QString getResult() const;
        
        void clearStatus();
        void setStatus(const QString &str);

    private:
        std::unique_ptr<core::TaskLogic> logic;

        Ui::TaskDialog ui;

        QShortcut *doneShortcut;
        QShortcut *skipShortcut;

        QTimer *timer;

        QString task;
    };
}

#endif
