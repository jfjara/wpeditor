#include "gestorconfiguracion.h"

GestorConfiguracion* GestorConfiguracion::instancia = Q_NULLPTR;

GestorConfiguracion* GestorConfiguracion::getInstancia()
{
    if (instancia == Q_NULLPTR) {
        instancia = new GestorConfiguracion();

    }
    return instancia;
}

GestorConfiguracion::GestorConfiguracion()
{
    idioma = Enumerados::IdiomasAplicacion::Castellano;
    rutaHorizon = "/home";
    leerConfiguracion();
}

void GestorConfiguracion::leerConfiguracion()
{
    SimpleCrypt crypto(Q_UINT64_C(0x0c2ad4a4acb9f023));
    QFile file(QString("./configuracion.cfg"));
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);


        while (!stream.atEnd()) {
            QString linea = stream.readLine();
            QString textoDescifrado = crypto.decryptToString(linea);
            QStringList array = textoDescifrado.split("$");
            if (array.size() != 2) {
                return;
            }

            int idiomaValor = array[CONFIG_POSICION_IDIOMA].toInt();

            switch (idiomaValor) {
            case 1:
                idioma = Enumerados::Castellano;
                break;
            case 2:
                idioma = Enumerados::English;
                break;
            }

            rutaHorizon = array[CONFIG_POSICION_RUTA_HORIZON];
        }
        file.close();
    }
}

void GestorConfiguracion::guardar()
{
    SimpleCrypt crypto(Q_UINT64_C(0x0c2ad4a4acb9f023));
    QFile file("./configuracion.cfg");
    if ( file.open(QIODevice::WriteOnly))
    {
        try {
            QTextStream stream( &file );
//            int enumIdx = Enumerados::staticMetaObject.indexOfEnumerator("IdiomasAplicacion");
//            QString idiomaTexto = QString::fromUtf8(Enumerados::staticMetaObject.enumerator(enumIdx).key(idioma));
            int valorIdioma = 0;
            switch (idioma) {
            case Enumerados::Castellano:
                valorIdioma = 1;
                break;
            case Enumerados::English:
                valorIdioma = 2;
                break;
            }

            QString encriptado = crypto.encryptToString(QString::number(valorIdioma) + "$" + rutaHorizon);
            stream << encriptado;
        } catch (...) {
            //mostrar error
            return;
        }
    }
    file.close();
}
