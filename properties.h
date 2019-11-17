#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QObject>
#include <QMap>
#include <QFile>
#include <QTextStream>

class Properties
{
public:
    Properties(QString fichero);
    QString getValor(QString clave);

private:
    QString fichero;
    QMap<QString, QString> valores;

    void leerFichero();    
};

#endif // PROPERTIES_H
