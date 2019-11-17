#ifndef FORMATEADOR_H
#define FORMATEADOR_H

#include <QString>
#include <QChar>
#include <QStringList>

class Formateador
{
public:
    Formateador();

    static QChar separadorDecimal;
    static QChar separadorMillar;
    static QString formatearEntero(int entero);
    static int formatearEntero(QString entero);
    static QString formatearDecimal(double decimal, int precision);
    static double formatearDecimal(QString decimal, int precision);

};

#endif // FORMATEADOR_H
