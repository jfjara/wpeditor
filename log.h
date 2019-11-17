#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QDateTime>
#include <QTextStream>

class Log
{
public:
    static Log* getInstancia();

    void info(QString mensaje);

private:
    Log();

    static Log* instancia;
};

#endif // LOG_H
