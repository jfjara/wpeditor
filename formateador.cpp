#include "formateador.h"

QChar Formateador::separadorDecimal = ',';
QChar Formateador::separadorMillar = '.';

Formateador::Formateador()
{

}

QString Formateador::formatearEntero(int valor)
{
    QString resultado;
    QString cadenaParteEntera = QString::number(valor);
    int contador = 0;
    for (int i = cadenaParteEntera.length() - 1; i >= 0; i --) {
        if (contador == 3) {
            resultado.push_front(QString(separadorMillar));
            contador = 0;
        }
        contador++;
        resultado.push_front(cadenaParteEntera.at(i));

    }
    return resultado;
}

QString Formateador::formatearDecimal(double valor, int precision)
{
    double parteEntera, parteDecimal;
    QString resultado;
    parteDecimal = modf(valor, &parteEntera);
    QString cadenaParteDecimal = QString::number(parteDecimal, 'f', precision);
    QString cadenaParteEntera = QString::number(parteEntera);
    cadenaParteDecimal = cadenaParteDecimal.replace('.', separadorDecimal);
    int contador = 0;
    for (int i = cadenaParteEntera.length() - 1; i >= 0; i --) {
        if (contador == 3) {
            resultado.push_front(QString(separadorMillar));
            contador = 0;
        }
        contador++;
        resultado.push_front(cadenaParteEntera.at(i));

    }
    if (resultado == "0") {
        resultado = cadenaParteDecimal;
        return resultado;
    }

    resultado = resultado.append(cadenaParteDecimal.remove(0, 1));
    return resultado;
}

int Formateador::formatearEntero(QString entero)
{
    entero = entero.remove(separadorMillar);
    return entero.toInt();
}

double Formateador::formatearDecimal(QString decimal, int precision)
{
    double resultado = 0.0;
    QStringList ar = decimal.split(separadorDecimal);
    if (ar.size() == 1) {
        bool ok;
        resultado = decimal.toDouble(&ok);
        if (ok) {
            return resultado;
        } else {
            return 0.1;
        }
    }
    QString parteEntera = ar[0].remove(separadorMillar);
    QString parteDecimal = ar[1];
    if (parteDecimal.length() > precision) {
        parteDecimal = parteDecimal.left(precision);
    }

    double parteDecimalDouble = parteDecimal.toDouble();
    parteDecimalDouble = parteDecimalDouble / (pow(10, precision));
    resultado = parteEntera.toDouble() + parteDecimalDouble;
    if (resultado == 0) resultado = 0.1;
    return resultado;
}
