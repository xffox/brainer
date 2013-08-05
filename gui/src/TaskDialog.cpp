#include "TaskDialog.h"

#include <QShortcut>

#include "core/IRender.h"
#include "core/ITask.h"
#include "core/ITaskGenerator.h"

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

    TaskDialog::TaskDialog(std::auto_ptr<core::ITaskGenerator> taskGenerator,
        QWidget *parent)
        :QDialog(parent),
        doneShortcut(new QShortcut(QKeySequence("Ctrl+D"), this)),
        skipShortcut(new QShortcut(QKeySequence("Ctrl+S"), this)),
        taskGenerator(taskGenerator)
    {
        ui.setupUi(this);

        connectToSignals();

        generate();
    }

    TaskDialog::~TaskDialog()
    {
    }

    void TaskDialog::validate()
    {
        if(task.get())
        {
            const QString &result = getResult();
            if(!result.isEmpty())
            {
                if(task->validate(qPrintable(result)))
                    generate();
                else
                    showInvalid(result);
            }
        }
    }

    void TaskDialog::generate()
    {
        if(taskGenerator.get())
            task.reset(taskGenerator->generateTask().release());
        showTask();
        clearStatus();
        clearResult();
    }

    void TaskDialog::showTask()
    {
        if(task.get())
        {
            Render render(*ui.taskLabel);
            task->describe(render);
        }
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
