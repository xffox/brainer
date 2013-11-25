#include <QDialog>
#include <QTimer>

#include <memory>

#include "core/Stats.h"

#include "ui_TaskDialog.h"

class QShortcut;

namespace task
{
    class StatsTaskLogicWatcher;
}

namespace gui
{
    class TaskDialog: public QDialog
    {
        Q_OBJECT
    public:
        TaskDialog(QWidget *parent = 0);
        virtual ~TaskDialog();

        void setStats(const core::Stats &stats);

    public slots:
        void setTask(const QString &task);
        void setValidity(bool valid);
        void setElapsed(long long elapsedUs);

    signals:
        void skipped();
        void entered(const QString &value);
        void timed();

    private slots:
        void validate();

    private:
        void connectToSignals();

        void showTask();
        void showStats();
        void showInvalid(const QString &str);

        void clearResult();
        QString getResult() const;
        
        void clearStatus();
        void setStatus(const QString &str);

    private:
        core::Stats stats;

        Ui::TaskDialog ui;

        QShortcut *doneShortcut;
        QShortcut *skipShortcut;

        QTimer *timer;
    };
}
