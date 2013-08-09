#include <QDialog>
#include <QTimer>

#include <memory>

#include "ui_TaskDialog.h"

class QShortcut;

namespace core
{
    class ITaskGenerator;
    class TaskLogic;
}

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
        TaskDialog(std::auto_ptr<core::ITaskGenerator> taskGenerator,
            QWidget *parent = 0);
        virtual ~TaskDialog();

    private slots:
        void validate();
        void generate();
        void showTime();

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
        Ui::TaskDialog ui;

        QShortcut *doneShortcut;
        QShortcut *skipShortcut;

        QTimer *timer;

        std::auto_ptr<task::StatsTaskLogicWatcher> statsWatcher;
        std::auto_ptr<core::TaskLogic> taskLogic;
    };
}
