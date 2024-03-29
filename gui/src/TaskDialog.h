#ifndef GUI_TASKDIALOG_H
#define GUI_TASKDIALOG_H

#include <QDialog>
#include <QTimer>

#include <chrono>
#include <memory>
#include <optional>

#include "core/Stats.h"
#include "core/String.h"
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
        TaskDialog(QWidget *parent = nullptr);

        ~TaskDialog() override;

        TaskDialog(const TaskDialog&) = delete;
        TaskDialog &operator=(const TaskDialog&) = delete;

        void setTaskGenerator(
            std::unique_ptr<core::ITaskGenerator> taskGenerator);

    signals:
        void entered(const QString &value);

    protected:
        void addText(const core::String &str) override;

    private slots:
        void validate();
        void skip();
        void timed();

    private:
        void connectToSignals();

        void describeTask();

        void showStats();
        void showElapsed(std::chrono::microseconds elapsed);

        void showValid(const core::String &str,
            const std::optional<core::String> &descr);
        void showInvalid(const core::String &str,
            const std::optional<core::String> &descr);
        void showAnswer(const core::String &str);

        void clearResult();
        [[nodiscard]]
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
