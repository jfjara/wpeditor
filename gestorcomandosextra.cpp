#include "gestorcomandosextra.h"

GestorComandosExtra* GestorComandosExtra::instancia = Q_NULLPTR;

GestorComandosExtra::GestorComandosExtra()
{
    leerComandos();
}

GestorComandosExtra* GestorComandosExtra::getInstancia()
{
    if (instancia == Q_NULLPTR) {
        instancia = new GestorComandosExtra();
    }
    return instancia;
}

void GestorComandosExtra::procesarComando(QTextStream* stream)
{
    QString lineaNombre = stream->readLine();
    QString lineaComando = stream->readLine();
    QString lineaModo = stream->readLine();
    QString lineaParam1 = stream->readLine();
    QString lineaParam2 = stream->readLine();

    QStringList arrayLineaNombre = lineaNombre.split("=");
    QStringList arrayLineaComando = lineaComando.split("=");
    QStringList arrayLineaModo = lineaModo.split("=");
    QStringList arrayLineaParam1 = lineaParam1.split("=");
    QStringList arrayLineaParam2 = lineaParam2.split("=");

    if (arrayLineaNombre.size() < 2) {
        return;
    }

    if (arrayLineaComando.size() < 2) {
        return;
    }

    ComandoExtra comando;
    comando.nombre = arrayLineaNombre[1];
    comando.comando = arrayLineaComando[1];
    comando.modo = arrayLineaModo[1].toInt();
    if (comando.modo == 0) {
        return;
    }
    if (arrayLineaParam1.size() == 2) {
        comando.valor1 = arrayLineaParam1[1].toInt();
    }
    if (arrayLineaParam2.size() == 2) {
        comando.valor2 = arrayLineaParam2[1].toInt();
    }
    comandos.push_back(comando);
}

ComandoExtra* GestorComandosExtra::getComandoEnFly(QString linea)
{
    for (int i = 0; i < comandos.size(); i++) {
        if (linea.startsWith(comandos[i].comando)) {
            return &comandos[i];
        }
    }
    return Q_NULLPTR;
}

void GestorComandosExtra::leerComandos()
{
    QFile file(GestorConfiguracion::getInstancia()->rutaHorizon + "\\Simulate.ini");
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        bool bloqueComandosEncontrado = false;
        int bloquesLeidos = 0;
        while (!stream.atEnd()) {

            if (bloquesLeidos < 8 && bloqueComandosEncontrado) {
                procesarComando(&stream);
                bloquesLeidos++;
                continue;
            } else if (bloquesLeidos >= 8){
                break;
            }
            QString linea = stream.readLine();
            if (linea.trimmed().startsWith("[WPTCMD]")) {
                bloqueComandosEncontrado = true;
            }
        }
    }
    file.close();
}
