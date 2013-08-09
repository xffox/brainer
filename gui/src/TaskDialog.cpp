#include "TaskDialog.h"

#include <numeric>
#include <QShortcut>

#include "core/IRender.h"
#include "core/ITaskGenerator.h"
#include "core/TaskLogic.h"
#include "core/ITaskLogicWatcher.h"
#include "task/StatsTaskLogicWatcher.h"

namespace gui
{
    namespace
    {
        class Render: public core::IRender
        {
        public:
            Render(QLabel &edit)
                :edit(edit)
            {}

            virtual void addText(const std::string &str)
            {
                edit.setText(QString(str.c_str()));
            }

        private:
            QLabel &edit;
        };
    }

    namespace
    {
        double average(
            const task::StatsTaskLogicWatcher::TimeCollection &times)
        {
            if(!times.empty())
            {
                return static_cast<double>(std::accumulate(times.begin(),
                        times.end(), 0))/times.size();
            }
            return 0.0;
        }
    }

    TaskDialog::TaskDialog(std::auto_ptr<core::ITaskGenerator> taskGenerator,
        QWidget *parent)
        :QDialog(parent),
        doneShortcut(new QShortcut(QKeySequence("Ctrl+D"), this)),
        skipShortcut(new QShortcut(QKeySequence("Ctrl+S"), this)),
        timer(new QTimer(this)),
        statsWatcher(new task::StatsTaskLogicWatcher()),
        taskLogic(new core::TaskLogic(taskGenerator, statsWatcher.get()))
    {
        ui.setupUi(this);

        timer->setSingleShot(false);
        timer->start(100);

        connectToSignals();

        generate();
    }

    TaskDialog::~TaskDialog()
    {
    }

    void TaskDialog::validate()
    {
        const QString &result = getResult();
        if(!result.isEmpty())
        {
            Q_ASSERT(taskLogic.get());
            if(taskLogic->validate(qPrintable(result)))
                generate();
            else
                showInvalid(result);
            showStats();
        }
    }

    void TaskDialog::generate()
    {
        Q_ASSERT(taskLogic.get());
        taskLogic->generate();
        showTask();
        showStats();
        clearStatus();
        clearResult();
    }

    void TaskDialog::showTime()
    {
        Q_ASSERT(taskLogic.get());
        ui.timeLabel->setText(QString::number(taskLogic->elapsed()/1000000) +
            's');
    }

    void TaskDialog::showTask()
    {
        Render render(*ui.taskLabel);
        Q_ASSERT(taskLogic.get());
        taskLogic->describe(render);
    }

    void TaskDialog::showStats()
    {
        Q_ASSERT(statsWatcher.get());
        ui.tasksLabel->setText(QString::number(statsWatcher->getTasksCount()));
        ui.validLabel->setText(QString::number(statsWatcher->getValidCount()));
        ui.invalidLabel->setText(QString::number(
                statsWatcher->getInvalidCount()));
        ui.avTimeLabel->setText(QString::number(
                average(statsWatcher->getValidTimes())/1000000.0, 'f', 2) +
            's');
    }

    void TaskDialog::showInvalid(const QString &str)
    {
        setStatus("!" + str);
        clearResult();
    }

    void TaskDialog::connectToSignals()
    {
        connect(ui.resultEdit, SIGNAL(returnPressed()),
            this, SLOT(validate()));
        connect(ui.doneButton, SIGNAL(clicked()), this, SLOT(validate()));
        connect(ui.skipButton, SIGNAL(clicked()), this, SLOT(generate()));
        connect(doneShortcut, SIGNAL(activated()), this, SLOT(validate()));
        connect(skipShortcut, SIGNAL(activated()), this, SLOT(generate()));
        connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
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
}
