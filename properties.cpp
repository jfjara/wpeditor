#include "properties.h"

Properties::Properties(QString fichero)
{
    this->fichero = fichero;
    leerFichero();
}

void Properties::leerFichero()
{
    QFile file(fichero);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);

        while (!stream.atEnd()) {
            QString linea = stream.readLine();
            QStringList array = linea.split("=");
            if (array.size() != 2) {
                continue;
            }
            valores.insert(array[0], array[1]);
        }
        file.close();
    }
}

QString Properties::getValor(QString clave)
{
    if (valores.contains(clave)) {
        return valores[clave];
    }
    return clave;
}
