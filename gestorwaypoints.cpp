#include "gestorwaypoints.h"
#include <QDebug>

GestorWayPoints::GestorWayPoints(DatosProyecto* proyecto)
{
    this->proyecto = proyecto;
}

void GestorWayPoints::reorganizarOrdenWP()
{
    for (int i = 0; i < wayPoints.size(); i++) {
        wayPoints[i]->orden = i + 1;
    }
}

WayPoint* GestorWayPoints::clonarWayPoint(WayPoint* wp)
{
    WayPoint* wpClonado = new WayPoint();
    wpClonado->altura = wp->altura;
    wpClonado->bloqueado = wp->bloqueado;
    wpClonado->cajaEnvolvente = wp->cajaEnvolvente;
    wpClonado->cuartosCirculo = wp->cuartosCirculo;
    wpClonado->diametro = wp->diametro;
    wpClonado->diametroMarcaWP = wp->diametroMarcaWP;
    wpClonado->direccionCirculo = wp->direccionCirculo;
    wpClonado->distanciaWPAnterior = wp->distanciaWPAnterior;
    wpClonado->heading = wp->heading;
    wpClonado->headingCircuit = wp->headingCircuit;
    wpClonado->indiceRepeticion = wp->indiceRepeticion;
    wpClonado->coordenadaY = wp->coordenadaY;
    wpClonado->coordenadaX = wp->coordenadaX;
    wpClonado->marcado = wp->marcado;
    wpClonado->modo = wp->modo;
    wpClonado->nivelado = wp->nivelado;
    wpClonado->orden = wp->orden;
    wpClonado->punto = wp->punto;
    wpClonado->radioMarcaWP = wp->radioMarcaWP;
    wpClonado->recorridoAterrizaje = wp->recorridoAterrizaje;
    wpClonado->tipo = wp->tipo;
    wpClonado->uDerecha = wp->uDerecha;
   // wpClonado->aterrizaje = new LineaAterrizaje();
    return wpClonado;
}

void GestorWayPoints::deshacerCrear(WayPoint* wp)
{
    wayPoints.removeAt(wp->orden - 1);
    delete wp;
    reorganizarOrdenWP();
    refrescarWayPoints();
}

void GestorWayPoints::deshacerModificar(WayPoint* wp)
{
    wayPoints[wp->orden - 1] = wp;
    refrescarWayPoints();
}

void GestorWayPoints::deshacerBorrar(WayPoint* wp)
{
    wayPoints.insert(wp->orden - 1, wp);
    reorganizarOrdenWP();
    refrescarWayPoints();
}

void GestorWayPoints::deshacerBorrar(QList<WayPoint*> lista)
{
    for (int i = 0; i < lista.size(); i++) {
        deshacerBorrar(lista[i]);
    }
}

void GestorWayPoints::realizarOperacionHistorica(OperacionRealizada* operacion)
{
    switch (operacion->accion) {
    case Enumerados::Crear:
        deshacerCrear(operacion->listaWaypoints[0]);
        break;
    case Enumerados::Modificar:
        deshacerModificar(operacion->listaWaypoints[0]);
        break;
    case Enumerados::Borrar:
        deshacerBorrar(operacion->listaWaypoints);
        break;
    }
}

void GestorWayPoints::desplazarWayPoint(QPointF punto)
{
    for (int i = wayPoints.size() - 1; i >= 0; i--) {
        if (wayPoints[i]->marcado && !wayPoints[i]->bloqueado) {
            QPointF puntoReal = HerramientaConversorCoordenadas::getInstancia()->convertirCoordenada(Enumerados::LatLng, punto.x() - HerramientasVisor::getInstancia()->getDesplazamientoX(), punto.y() - HerramientasVisor::getInstancia()->getDesplazamientoY());
            double lat = puntoReal.y();
            double lng = puntoReal.x();
            wayPoints[i]->coordenadaY = lat;
            wayPoints[i]->coordenadaX = lng;
            return;
        }
    }
}

void GestorWayPoints::crearProtocoloAterrizaje(LineaAterrizaje* aterrizaje)
{
    WayPoint* wp = getCircuit();
    if (wp == Q_NULLPTR) {
        return;
    }
    modoCrear = true;
    WayPoint* flyto = crearWayPoint(wp->orden - 1, wp->punto, 70);
    flyto->tipo = Enumerados::WayPoint;
    flyto->modo = Enumerados::FlyTo;
    WayPoint* circulo = crearWayPoint(wayPoints.size() - 1, wp->punto, 70);
    circulo->tipo = Enumerados::Circulo;
    circulo->diametro = aterrizaje->base * 2;
    circulo->nivelado = 1;
    circulo->cuartosCirculo = 4;
    circulo->headingCircuit = wp->headingCircuit;
    modoCrear = false;
}


void GestorWayPoints::crearWayPoint(QPointF punto)
{
    crearWayPoint(punto, alturaWaypoints);
}

void GestorWayPoints::crearWayPoint(QPointF punto, int altura)
{    
    if (modoCrear) {
        WayPoint* wp = new WayPoint(punto, altura);
        wayPoints.push_back(wp);
        reorganizarOrdenWP();

        OperacionRealizada* operacion = new OperacionRealizada();
        operacion->accion = Enumerados::Crear;
        operacion->tipoObjeto = Enumerados::ObjetoWayPoint;
        operacion->listaWaypoints.push_back(wp);
        GestorHistoricoOperaciones::getInstancia()->apilar(operacion);
    }       
}

WayPoint* GestorWayPoints::crearWayPoint(int posicion, QPointF punto, int altura)
{
    WayPoint* wp = new WayPoint(punto, altura);
//    if (wayPoints.size() != 0) {
//        wp->altura = wayPoints[wayPoints.size() - 1]->altura;
//    }
    wayPoints.insert(posicion, wp);
    reorganizarOrdenWP();
    OperacionRealizada* operacion = new OperacionRealizada();
    operacion->accion = Enumerados::Crear;
    operacion->tipoObjeto = Enumerados::ObjetoWayPoint;
    operacion->listaWaypoints.push_back(wp);
    GestorHistoricoOperaciones::getInstancia()->apilar(operacion);
    return wayPoints[posicion];
}

WayPoint* GestorWayPoints::crearWayPointAnteriorAlMarcado()
{
    WayPoint* wpMarcado = getWayPointMarcado();
    if (wpMarcado == Q_NULLPTR) {
        return Q_NULLPTR;
    }
    if (wpMarcado->orden - 1 == 0) {
        return Q_NULLPTR;
    }

    if (wpMarcado->orden <= 1) {
        return Q_NULLPTR;
    }

    WayPoint* wpAnteriorAlMarcado = wayPoints[wpMarcado->orden - 2];
    QLineF lineaWayPoints(wpAnteriorAlMarcado->punto, wpMarcado->punto);
    QPointF nuevoPunto;    
    nuevoPunto.setX(lineaWayPoints.center().x());
    nuevoPunto.setY(lineaWayPoints.center().y());
    return crearWayPoint(wpMarcado->orden - 1, nuevoPunto, alturaWaypoints);
}

bool GestorWayPoints::hayWayPointMarcado()
{
    for (int i = 0; i < wayPoints.size(); i++) {
        if (wayPoints[i]->marcado) {
            return true;
        }
    }
    return false;
}

void GestorWayPoints::seleccionarPuntosPorCaja(QRectF caja)
{
    marcarTodosWayPoint(false);
    for (int i = 0; i < wayPoints.size(); i++) {
        if (caja.contains(wayPoints[i]->punto)) {
            wayPoints[i]->marcado = true;
        }
    }
}

bool GestorWayPoints::hayWayPoint(QPointF punto)
{
    for (int i = wayPoints.size() - 1; i >= 0; i--) {
        if (wayPoints[i]->cajaEnvolvente.contains(punto)) {
            return true;
        }
    }
    return false;
}

int GestorWayPoints::marcaWayPoint(QPointF puntoPulsado)
{
    for (int i = wayPoints.size() - 1; i >= 0; i--) {
        if (wayPoints[i]->cajaEnvolvente.contains(puntoPulsado)) {
            wayPoints[i]->marcado = !wayPoints[i]->marcado;
            return i;
        }
    }
    return -1;
}

void GestorWayPoints::marcaWayPoint(int orden)
{
    if (modoCrear) return;
    marcarTodosWayPoint(false);
    for (int i = 0; i < wayPoints.size(); i++) {
        if (wayPoints[i]->orden == orden) {
            wayPoints[i]->marcado = true;
            return;
        }
    }
}

int GestorWayPoints::marcarSiguienteWayPoint()
{
    for (int i = 0; i < wayPoints.size(); i++) {
        if (wayPoints[i]->marcado) {
            if (i + 1 < wayPoints.size()) {
                wayPoints[i]->marcado = false;
                wayPoints[i + 1]->marcado = true;
                return i + 1;
            }
        }
    }
    return -1;
}

int GestorWayPoints::marcarAnteriorWayPoint()
{
    for (int i = 1; i < wayPoints.size(); i++) {
        if (wayPoints[i]->marcado) {
                wayPoints[i]->marcado = false;
                wayPoints[i - 1]->marcado = true;
                return i - 1;
        }
    }
    return -1;
}

void GestorWayPoints::marcarTodosWayPoint(bool valor)
{
    for (int i = wayPoints.size() - 1; i >= 0; i--) {
        wayPoints[i]->marcado = valor;
    }
}

WayPoint* GestorWayPoints::getWayPointMarcado()
{
    for (int i = 0; i < wayPoints.size(); i++) {
        if (wayPoints[i]->marcado) {
            return wayPoints[i];
        }
    }
    return Q_NULLPTR;
}

void GestorWayPoints::refrescarWayPoints()
{
    for (int i = 0; i < wayPoints.size(); i++) {
        wayPoints[i]->refrescarPosicion();
    }
}

QList<WayPoint*> GestorWayPoints::eliminarWayPointMarcados()
{
    QList<WayPoint*> wpEliminar;
    for (int i = 0; i < wayPoints.size(); i++) {       
        if (wayPoints[i]->marcado) {            
            wpEliminar.push_back(wayPoints[i]);
            wayPoints.removeAt(i);
            i--;
        }
    }

 //   for (int i = 0; i < wpEliminar.size(); i++) {
        OperacionRealizada* operacion = new OperacionRealizada();
        operacion->accion = Enumerados::Borrar;
        operacion->tipoObjeto = Enumerados::ObjetoWayPoint;
        operacion->listaWaypoints = wpEliminar;
        GestorHistoricoOperaciones::getInstancia()->apilar(operacion);
   // }
    reorganizarOrdenWP();
    return wpEliminar;
}

WayPoint* GestorWayPoints::getCircuit()
{
    for (int i = 0; i < wayPoints.size(); i++) {
        if (wayPoints[i]->tipo == Enumerados::Circuit) {
            return wayPoints[i];
        }
    }
    return Q_NULLPTR;
}

int GestorWayPoints::borrarPuntosDespuesLanding()
{    
    int posicionLanding = -1;
    for (int i = 0; i < wayPoints.size(); i++) {
        if (wayPoints[i]->tipo == Enumerados::Circuit) {
            posicionLanding = i;
            break;
        }
    }
    if (posicionLanding + 1 >= wayPoints.size()) {
        return -1;
    }
    for (int i = posicionLanding + 1; i < wayPoints.size(); i++) {                
        wayPoints.removeAt(i);
        i--;
    }
    return posicionLanding;
}

double GestorWayPoints::getDistanciaTotalVuelo()
{
    double total = 0.0;
    for (int i = 1; i < wayPoints.size(); i++) {
        wayPoints[i]->distanciaWPAnterior = calcularDistanciaWP(Enumerados::LatLng, wayPoints[i-1], wayPoints[i]);        
    }

    for (int i = 0; i < wayPoints.size(); i++) {
        total += wayPoints[i]->distanciaWPAnterior;
    }    
    return total;
}



void GestorWayPoints::pintar(QPainter* painter, Enumerados::Coordenadas tipo, bool verDistanciasEntrePuntos, bool verLineasDireccion,  bool distanciaTotalRecorrido)
{
    if (wayPoints.size() == 1) {
        wayPoints[0]->pintar(painter);
    }

    double recorrido = 0.0;    
    for (int i = 1; i < wayPoints.size(); i++) {
        if (verLineasDireccion) {
            if (wayPoints[i - 1]->indiceRepeticion > 0) {
                pintarLineaRepeticion(wayPoints[i - 1], wayPoints[wayPoints[i - 1]->indiceRepeticion - 1], painter, verDistanciasEntrePuntos, distanciaTotalRecorrido, recorrido);
            } else {
                pintarLinea(tipo, wayPoints[i - 1], wayPoints[i], painter, verDistanciasEntrePuntos, distanciaTotalRecorrido, recorrido);
            }
            recorrido += wayPoints[i]->distanciaWPAnterior;
        }
        wayPoints[i - 1]->pintar(painter);
        wayPoints[i]->pintar(painter);
    }
}

void GestorWayPoints::pintarLineaRepeticion(WayPoint* wp1, WayPoint* wp2, QPainter* painter, bool verDistanciasEntrePuntos, bool distanciaTotalRecorrido, double recorrido)
{
    if (!mostrarLineas) {
        return;
    }
    painter->save();
    QPen pen(QColor(0xFF, 0x22, 0x33), 2);
    pen.setStyle(Qt::DotLine);
    painter->setPen(pen);

    QPointF punto1(wp1->punto.x(), wp1->punto.y());
    QPointF punto2(wp2->punto.x(), wp2->punto.y());
    QLineF linea(punto1, punto2);
    painter->drawLine(linea);

    int cx = 0;
    int cy = 0;
    if (punto1.x() < punto2.x()) {
        cx = punto1.x() + ((punto2.x() - punto1.x()) / 2);
    } else {
        cx = punto1.x() - ((punto1.x() - punto2.x()) / 2);
    }
    if (punto1.y() < punto2.y()) {
        cy = punto1.y() + ((punto2.y() - punto1.y()) / 2);
    } else {

        cy = punto1.y() - ((punto1.y() - punto2.y()) / 2);
    }

    int size = 15;
    qreal Pi = 3.1415;

    double angle = ::acos(linea.dx() / linea.length());
    if (linea.dy() >= 0)
           angle = (Pi * 2) - angle;

    QPointF arrowP1 = QPointF(cx, cy) - QPointF(sin(angle + Pi / 3) * size,
                                   cos(angle + Pi / 3) * size);
    QPointF arrowP2 = QPointF(cx, cy) - QPointF(sin(angle + Pi - Pi / 3) * size,
                                   cos(angle + Pi - Pi / 3) * size);

    QPolygonF flecha;
    flecha << QPointF(cx, cy) << arrowP1 << arrowP2;
    painter->setBrush(QBrush(QColor(0xFF, 0x22, 0x33)));
    painter->drawPolygon(flecha);

    wp2->distanciaWPAnterior = calcularDistanciaWP(Enumerados::LatLng, wp1, wp2);

    if (verDistanciasEntrePuntos) {
        //QRectF cajaTexto = QRectF(arrowP2.x(), arrowP2.y() + DIAMETRO_MARCA_WAYPOINT, DIAMETRO_MARCA_WAYPOINT + 20, DIAMETRO_MARCA_WAYPOINT);
        QRectF cajaTexto = QRectF(linea.center().x(), linea.center().y() + DIAMETRO_MARCA_WAYPOINT, DIAMETRO_MARCA_WAYPOINT + 20, DIAMETRO_MARCA_WAYPOINT);
        painter->setPen(QPen(QColor(0xFF, 0xFF, 0xFF), 2));
        painter->setFont(QFont("Arial", 7, QFont::Bold));
        if (distanciaTotalRecorrido) {
            painter->drawText(cajaTexto, QString::number(recorrido + wp2->distanciaWPAnterior, 'f', 0) + QString("m"),  Qt::AlignLeft | Qt::AlignVCenter);
        } else {
            painter->drawText(cajaTexto, QString::number(wp2->distanciaWPAnterior, 'f', 0) + QString("m"),  Qt::AlignLeft | Qt::AlignVCenter);
        }
    }
    painter->restore();
}

void GestorWayPoints::pintarLinea( Enumerados::Coordenadas tipo, WayPoint* wp1, WayPoint* wp2, QPainter* painter, bool verDistanciasEntrePuntos, bool distanciaTotalRecorrido, double recorrido)
{
    if (!mostrarLineas) {
        return;
    }
    painter->save();
    painter->setPen(QPen(QColor(0xFF, 0xD8, 0x00), 2));

    QPointF punto1(wp1->punto.x(), wp1->punto.y());
    QPointF punto2(wp2->punto.x(), wp2->punto.y());
    QLineF linea(punto1, punto2);
    painter->drawLine(linea);

    int cx = 0;
    int cy = 0;
    if (punto1.x() < punto2.x()) {
        cx = punto1.x() + ((punto2.x() - punto1.x()) / 2);
    } else {
        cx = punto1.x() - ((punto1.x() - punto2.x()) / 2);
    }
    if (punto1.y() < punto2.y()) {
        cy = punto1.y() + ((punto2.y() - punto1.y()) / 2);
    } else {

        cy = punto1.y() - ((punto1.y() - punto2.y()) / 2);
    }

    int size = 15;
    qreal Pi = 3.1415;

    double angle = ::acos(linea.dx() / linea.length());
    if (linea.dy() >= 0)
           angle = (Pi * 2) - angle;

    QPointF arrowP1 = QPointF(cx, cy) - QPointF(sin(angle + Pi / 3) * size,
                                   cos(angle + Pi / 3) * size);
    QPointF arrowP2 = QPointF(cx, cy) - QPointF(sin(angle + Pi - Pi / 3) * size,
                                   cos(angle + Pi - Pi / 3) * size);

    QPolygonF flecha;
    flecha << QPointF(cx, cy) << arrowP1 << arrowP2;
    painter->setBrush(QBrush(QColor(0xFF, 0xD8, 0x00)));
    painter->drawPolygon(flecha);

    wp2->distanciaWPAnterior = calcularDistanciaWP(tipo, wp1, wp2);

    if (verDistanciasEntrePuntos) {
        //QRectF cajaTexto = QRectF(arrowP2.x(), arrowP2.y() + DIAMETRO_MARCA_WAYPOINT, DIAMETRO_MARCA_WAYPOINT + 20, DIAMETRO_MARCA_WAYPOINT);
        QRectF cajaTexto = QRectF(linea.center().x(), linea.center().y() + DIAMETRO_MARCA_WAYPOINT, DIAMETRO_MARCA_WAYPOINT + 20, DIAMETRO_MARCA_WAYPOINT);
        painter->setPen(QPen(QColor(0xFF, 0xFF, 0xFF), 2));
        painter->setFont(QFont("Arial", 7, QFont::Bold));
        if (distanciaTotalRecorrido) {
            painter->drawText(cajaTexto, QString::number(recorrido + wp2->distanciaWPAnterior, 'f', 0) + QString("m"),  Qt::AlignLeft | Qt::AlignVCenter);
        } else {
            painter->drawText(cajaTexto, QString::number(wp2->distanciaWPAnterior, 'f', 0) + QString("m"),  Qt::AlignLeft | Qt::AlignVCenter);
        }
    }
    painter->restore();
}

double GestorWayPoints::calcularDistanciaWP(Enumerados::Coordenadas tipo, WayPoint* wp1, WayPoint* wp2)
{
    double x1, x2, y1, y2, total;
    if (tipo == Enumerados::LatLng || tipo == Enumerados::UTM) {
        total = HerramientaConversorCoordenadas::getInstancia()->getDistanciaEntreCoordenadas(tipo, wp1->coordenadaX, wp1->coordenadaY, wp2->coordenadaX, wp2->coordenadaY);
        if (wp2->tipo == Enumerados::Circuit) {
            total += wp2->recorridoAterrizaje;
        }
        return total;
    } else if (tipo) {
        x1 = wp1->punto.x() - HerramientasVisor::getInstancia()->getDesplazamientoX();
        x2 = wp2->punto.x() - HerramientasVisor::getInstancia()->getDesplazamientoX();
        y1 = wp1->punto.y() - HerramientasVisor::getInstancia()->getDesplazamientoY();
        y2 = wp2->punto.y() - HerramientasVisor::getInstancia()->getDesplazamientoY();
    }
    return HerramientaConversorCoordenadas::getInstancia()->getDistanciaEntreCoordenadas(tipo, x1, y1, x2, y2);
}
