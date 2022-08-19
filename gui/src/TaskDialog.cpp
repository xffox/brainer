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
        ui{},
        doneShortcut(new QShortcut(QKeySequence("Ctrl+D"), this)),
        skipShortcut(new QShortcut(QKeySequence("Ctrl+S"), this)),
        timer(new QTimer(this)),
        task{}
    {
        ui.setupUi(this);

        timer->setSingleShot(false);

        connectToSignals();
        ui.doneButton->setDefault(true);
        ui.resultEdit->setFocus();
    }

    TaskDialog::~TaskDialog()
    {
    }

    void TaskDialog::setTaskGenerator(
        std::unique_ptr<core::ITaskGenerator> taskGenerator)
    {
        this->logic.reset(new core::TaskLogic(std::move(taskGenerator)));
        clearStatus();
        describeTask();
    }

    void TaskDialog::addText(const core::String &str)
    {
        task = QString::fromStdWString(str);
        ui.taskEdit->setText(task);
    }

    void TaskDialog::showInvalid(const core::String &str,
            const std::optional<core::String> &descr)
    {
        setStatus(
            descr
            ?QString("%1 is WRONG: %2")
                .arg(QString::fromStdWString(str))
                .arg(QString::fromStdWString(*descr))
            :QString("%1 is WRONG").arg(QString::fromStdWString(str)));
        clearResult();
    }

    void TaskDialog::showValid(const core::String &str,
            const std::optional<core::String>&)
    {
        setStatus(QString("%1 is RIGHT").arg(QString::fromStdWString(str)));
        clearResult();
    }

    void TaskDialog::showAnswer(const core::String &str)
    {
        setStatus(QString("\"%1\" is \"%2\"").arg(QString::fromStdWString(str)).arg(task));
    }

    void TaskDialog::validate()
    {
        if(!getResult().isEmpty())
        {
            clearStatus();
            if(logic.get())
            {
                const auto answer = getResult().toStdWString();
                auto res = logic->validate(answer);
                if(res.validity.valid)
                {
                    showValid(answer, res.validity.description);
                }
                else
                {
                    if(res.answer)
                    {
                        showAnswer(*res.answer);
                    }
                    else
                    {
                        showInvalid(answer, res.validity.description);
                    }
                }
                if(res.answer)
                {
                    describeTask();
                }
            }
        }
    }

    void TaskDialog::skip()
    {
        if(logic.get())
            showAnswer(logic->skip());
        describeTask();
    }

    void TaskDialog::timed()
    {
        if(logic.get())
            showElapsed(logic->elapsedUs());
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

    void TaskDialog::connectToSignals()
    {
        connect(ui.doneButton, SIGNAL(clicked()), this, SLOT(validate()));
        connect(ui.skipButton, SIGNAL(clicked()), this, SLOT(skip()));
        connect(doneShortcut, SIGNAL(activated()), this, SLOT(validate()));
        connect(skipShortcut, SIGNAL(activated()), this, SLOT(skip()));
        connect(timer, SIGNAL(timeout()), this, SLOT(timed()));
    }

    void TaskDialog::describeTask()
    {
        Q_ASSERT(timer);
        timer->start(300);
        showStats();
        clearResult();
        showElapsed(0);
        if(logic.get())
        {
            logic->describe(*this);
        }
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
        ui.statusEdit->clear();
    }

    void TaskDialog::setStatus(const QString &str)
    {
        ui.statusEdit->setText(str);
    }

    void TaskDialog::showElapsed(long long elapsedUs)
    {
        ui.timeLabel->setText(QString::number(elapsedUs/1000000) +
            's');
    }
}
