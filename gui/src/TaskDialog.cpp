#include "TaskDialog.h"

#include <numeric>
#include <QShortcut>
#include <stdexcept>

#include "core/IRender.h"
#include "core/ITaskGenerator.h"
#include "core/TaskLogic.h"

namespace gui
{
    TaskDialog::TaskDialog(QWidget *parent)
        :QDialog(parent),
        logic(),
        doneShortcut(new QShortcut(QKeySequence("Ctrl+D"), this)),
        skipShortcut(new QShortcut(QKeySequence("Ctrl+S"), this)),
        timer(new QTimer(this))
    {
        ui.setupUi(this);

        timer->setSingleShot(false);

        connectToSignals();
    }

    TaskDialog::~TaskDialog()
    {
    }

    void TaskDialog::setTaskGenerator(
        std::unique_ptr<core::ITaskGenerator> taskGenerator)
    {
        this->logic.reset(new core::TaskLogic(std::move(taskGenerator),
                *this));
        clearResult();
        clearStatus();
        showStats();
    }

    void TaskDialog::displayTask(const QString &task)
    {
        Q_ASSERT(timer);
        ui.taskLabel->setText(task);
        timer->start(300);
        showStats();
        clearResult();
        showElapsed(0);
    }

    void TaskDialog::showTask(const core::String &str)
    {
        task = QString::fromStdWString(str);
        displayTask(task);
    }

    void TaskDialog::showInvalid(const core::String &str)
    {
        showInvalid(QString::fromStdWString(str));
    }

    void TaskDialog::showAnswer(const core::String &str)
    {
        setStatus(task + "=" + QString::fromStdWString(str));
    }

    void TaskDialog::validate()
    {
        if(!getResult().isEmpty())
        {
            clearStatus();
            if(logic.get())
                logic->validate(getResult().toStdWString());
        }
    }

    void TaskDialog::skip()
    {
        if(logic.get())
            logic->skip();
    }

    void TaskDialog::timed()
    {
        if(logic.get())
            showElapsed(logic->elapsed());
    }

    void TaskDialog::showStats()
    {
        if(logic.get())
        {
            int validCount = 0;
            int triesCount = 0;
            long long totalTimeUs = 0;
            const auto &stats = logic->getStats();
            int taskCount = stats.size();
            for(const auto &s : stats)
            {
                if(s.answered)
                    ++validCount;
                triesCount += s.tries;
                totalTimeUs += s.timeUs;
            }
            ui.tasksLabel->setText(QString::number(taskCount));
            ui.validLabel->setText(QString::number(validCount));
            ui.triesLabel->setText(QString::number(triesCount));
            if(taskCount > 0)
                ui.avTimeLabel->setText(QString::number(
                        totalTimeUs/1000000.0/taskCount, 'f', 2) + 's');
            else
                ui.avTimeLabel->setText("");
        }
    }

    void TaskDialog::showInvalid(const QString &str)
    {
        setStatus("!" + str);
        clearResult();
    }

    void TaskDialog::connectToSignals()
    {
        // TODO: need this?
//      connect(ui.resultEdit, SIGNAL(returnPressed()),
//          this, SLOT(validate()));
        connect(ui.doneButton, SIGNAL(clicked()), this, SLOT(validate()));
        connect(ui.skipButton, SIGNAL(clicked()), this, SLOT(skip()));
        connect(doneShortcut, SIGNAL(activated()), this, SLOT(validate()));
        connect(skipShortcut, SIGNAL(activated()), this, SLOT(skip()));
        connect(timer, SIGNAL(timeout()), this, SLOT(timed()));
    }

    void TaskDialog::clearResult()
    {
        ui.resultEdit->clear();
    }

    QString TaskDialog::getResult() const
    {
        return ui.resultEdit->text();
    }

    void TaskDialog::clearStatus()
    {
        ui.statusLabel->clear();
    }

    void TaskDialog::setStatus(const QString &str)
    {
        ui.statusLabel->setText(str);
    }

    void TaskDialog::showElapsed(long long elapsedUs)
    {
        ui.timeLabel->setText(QString::number(elapsedUs/1000000) +
            's');
    }
}
