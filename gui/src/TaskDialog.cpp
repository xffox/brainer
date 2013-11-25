#include "TaskDialog.h"

#include <numeric>
#include <QShortcut>

#include "core/IRender.h"
#include "core/ITaskGenerator.h"
#include "core/TaskLogic.h"
#include "core/ITaskLogicWatcher.h"

namespace gui
{
    TaskDialog::TaskDialog(QWidget *parent)
        :QDialog(parent),
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

    void TaskDialog::setStats(const core::Stats &stats)
    {
        this->stats = stats;
        showStats();
    }

    void TaskDialog::setTask(const QString &task)
    {
        Q_ASSERT(timer);
        ui.taskLabel->setText(task);
        timer->start(300);
        showStats();
        clearResult();
        clearStatus();
        setElapsed(0);
    }

    void TaskDialog::setValidity(bool valid)
    {
        if(valid)
        {
            timer->stop();
        }
        else
        {
            showInvalid(getResult());
        }
    }

    void TaskDialog::setElapsed(long long elapsedUs)
    {
        ui.timeLabel->setText(QString::number(elapsedUs/1000000) +
            's');
    }

    void TaskDialog::validate()
    {
        const QString &result = getResult();
        if(!result.isEmpty())
            emit entered(result);
    }

    void TaskDialog::showStats()
    {
        ui.tasksLabel->setText(QString::number(stats.getTaskCount()));
        ui.validLabel->setText(QString::number(stats.getValidCount()));
        ui.triesLabel->setText(QString::number(stats.getTriesCount()));
        ui.avTimeLabel->setText(QString::number(
                stats.getAverageTime()/1000000.0, 'f', 2) + 's');
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
        connect(ui.skipButton, SIGNAL(clicked()), this, SIGNAL(skipped()));
        connect(doneShortcut, SIGNAL(activated()), this, SLOT(validate()));
        connect(skipShortcut, SIGNAL(activated()), this, SIGNAL(skipped()));
        connect(timer, SIGNAL(timeout()), this, SIGNAL(timed()));
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
