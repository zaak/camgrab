#ifndef CONSOLE_H
#define CONSOLE_H

#include <QTextEdit>

class Console
{
private:
    static QTextEdit *output;

public:
    static void setOutputControl(QTextEdit *textEdit)
    {
        output = textEdit;
    }

    static void log(QString message, QColor color = Qt::black)
    {
        output->setTextColor(color);
        output->append(message);
    }

    static void error(QString message)
    {
        log(message, Qt::red);
    }
};

#endif // CONSOLE_H
