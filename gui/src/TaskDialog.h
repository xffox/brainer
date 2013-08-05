#include <QDialog>

#include <memory>

#include "ui_TaskDialog.h"

class QShortcut;

namespace core
{
    class ITask;
    class ITaskGenerator;
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

    private:
        void connectToSignals();

        void showTask();
        void showInvalid(const QString &str);

        void clearResult();
        QString getResult() const;
        
        void clearStatus();
        void setStatus(const QString &str);

    private:
        Ui::TaskDialog ui;

        QShortcut *doneShortcut;
        QShortcut *skipShortcut;

        std::auto_ptr<core::ITaskGenerator> taskGenerator;
        std::auto_ptr<core::ITask> task;
    };
}
