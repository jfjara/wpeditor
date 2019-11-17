#include "log.h"

Log* Log::instancia = Q_NULLPTR;

Log* Log::getInstancia()
{
    if (instancia == Q_NULLPTR) {
        instancia = new Log();
    }
    return instancia;
}

Log::Log()
{

}

void Log::info(QString mensaje)
{

    QDateTime dt = QDateTime::currentDateTime();
    QString fecha = dt.toString("yyyyMMdd");
    QString datetime = dt.toString("dddd, d MMMM yy hh:mm:ss");
    QFile file(fecha + ".log");
    if (file.open(QIODevice::ReadWrite | QIODevice::Append))
    {
        QTextStream stream( &file );
        stream << datetime << " - INFO - " << mensaje << endl;
    }
    file.close();
}
