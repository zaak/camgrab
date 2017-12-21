#ifndef CONSOLE_H
#define CONSOLE_H

#include <QTextEdit>

class Console
{
private:
    static QTextEdit *outputControl;

public:
    static void setOutputControl(QTextEdit *textEdit)
    {
        outputControl = textEdit;
    }

    static void log(QString message)
    {
        outputControl->append(message + "\n");
    }
};

#endif // CONSOLE_H
