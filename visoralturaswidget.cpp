#include "visoralturaswidget.h"
#include <QDebug>

double VisorAlturasWidget::offsetX = 0.0;

VisorAlturasWidget::VisorAlturasWidget(DatosProyecto* datosProyecto, GestorWayPoints* gestorWayPoints, QWidget *parent) : QWidget(parent)
{
    this->datosProyecto = datosProyecto;
    this->gestorWayPoints = gestorWayPoints;

    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::black);
    this->setPalette(pal);

    setAutoFillBackground(true);
    setMouseTracking(true);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(mostrarMenuContextual(const QPoint &)));
}

void VisorAlturasWidget::mostrarMenuContextual(const QPoint &punto) {

    QPoint globalPos = this->mapToGlobal(punto);
    QMenu contextMenu(QString("Context menu"), this);

    accionLineasAlturas = new QAction(Idioma::getInstancia()->getTexto("visoralturas.menu.lineasaltura"), this);
    connect(accionLineasAlturas, SIGNAL(triggered()), this, SLOT(mostrarVentanaLineas()));
    contextMenu.addAction(accionLineasAlturas);

    accionConfiguracionVisor = new QAction(Idioma::getInstancia()->getTexto("visoralturas.menu.configuracion"), this);
    connect(accionConfiguracionVisor, SIGNAL(triggered()), this, SLOT(mostrarConfiguracionVisor()));
    contextMenu.addAction(accionConfiguracionVisor);

    contextMenu.exec(globalPos);
}

void VisorAlturasWidget::traducir()
{
    if (accionConfiguracionVisor != Q_NULLPTR) {
        accionConfiguracionVisor->setText(Idioma::getInstancia()->getTexto("visoralturas.menu.configuracion"));
    }
    if (accionLineasAlturas != Q_NULLPTR) {
        accionLineasAlturas->setText(Idioma::getInstancia()->getTexto("visoralturas.menu.lineasaltura"));
    }
}

void VisorAlturasWidget::mostrarConfiguracionVisor()
{
    ConfiguracionVisorDialog dialogo(verNombres, verAlturas, distanciasReales);
    dialogo.setModal(true);
    dialogo.exec();

    if (!dialogo.aceptado) {
        return;
    }
    verNombres = dialogo.verNombres;
    verAlturas = dialogo.verAlturas;
    distanciasReales = dialogo.distanciasReales;
    if (dialogo.establecerAlturas) {
        int altura = dialogo.altura;

        int inicio = 1;
        int fin = listaWPAlturas.size();
        if (dialogo.rangoEstablecido) {
            inicio = dialogo.inicioRango;
            fin = dialogo.finRango;
            if (inicio == 0) {
                inicio = 1;
            }
            if (fin > listaWPAlturas.size()) {
                fin = listaWPAlturas.size();
            }
        }

        for (int i = inicio - 1; i < fin; i++) {
            if (listaWPAlturas[i]->wayPoint->tipo != Enumerados::Circuit) {
                listaWPAlturas[i]->wayPoint->altura = altura;
                listaWPAlturas[i]->recalcularPosicionEnVisor(this->height() - 60, 150);
            }
        }
    }
}

void VisorAlturasWidget::mostrarVentanaLineas()
{
    LineasAlturaDialog dialogo(datosProyecto);
    dialogo.setModal(true);
    dialogo.exec();
}

void VisorAlturasWidget::setWayPoints(QList<WayPoint*> lista)
{
    int height = this->height() - (MARGENES_VISOR_ALTURAS * 2);
    listaWP = lista;
    for (int i = 0; i < lista.size(); i++) {
        WayPointAltura* wpAltura = new WayPointAltura();
        wpAltura->setWayPoint(lista[i], height, datosProyecto->alturaMaximaVisor);
        listaWPAlturas.push_back(wpAltura);
    }
}

void VisorAlturasWidget::addWayPoint(WayPoint* wp)
{
    int height = this->height() - (MARGENES_VISOR_ALTURAS * 2);

    listaWP.push_back(wp);
    WayPointAltura* wpAltura = new WayPointAltura();
    wpAltura->setWayPoint(wp, height, datosProyecto->alturaMaximaVisor);
    listaWPAlturas.push_back(wpAltura);
}

void VisorAlturasWidget::limpiarWayPoints()
{
    listaWP.clear();
    listaWPAlturas.clear();
}

void VisorAlturasWidget::borrarWP(WayPoint* wp)
{
    for (int i = 0; i < listaWP.size(); i++) {
        if (wp->orden == listaWP[i]->orden) {
            listaWP.removeAt(i);
            break;
        }
    }    
}

void VisorAlturasWidget::borrarWPAltura(WayPoint* wp)
{
    for (int i = 0; i < listaWPAlturas.size(); i++) {
        if (wp->orden == listaWPAlturas[i]->wayPoint->orden) {
            listaWPAlturas.removeAt(i);
            break;
        }
    }
}

void VisorAlturasWidget::desmarcarWayPoints()
{
    for (int i = 0; i < listaWPAlturas.size(); i++) {
        listaWPAlturas[i]->wayPoint->marcado = false;
    }
}

void VisorAlturasWidget::mousePressEvent(QMouseEvent *ev)
{
    puntoAnclajeRaton = ev->pos();
    if (ev->buttons() & Qt::LeftButton) {
        clickIzquierdoPulsado = true;
        WayPointAltura* wp = getWayPointEnPunto(puntoAnclajeRaton);
        if (wp != Q_NULLPTR) {
            wayPointSeleccionado = wp;
            desmarcarWayPoints();
            wayPointSeleccionado->wayPoint->marcado = true;
            wayPointSeleccionado->verDistancia = true;
            emit wapypointSeleccionado(wp->wayPoint->orden - 1);
        }
    }
}

WayPointAltura* VisorAlturasWidget::getWayPointEnPunto(QPointF punto)
{
    for (int i = listaWPAlturas.size() - 1; i >= 0; i--) {
        if (listaWPAlturas[i]->cajaEnvolvente.contains(punto)) {
            return listaWPAlturas[i];
        }
    }
    return Q_NULLPTR;
}

void VisorAlturasWidget::ocultarDistanciaDeWayPointsAltura()
{
    for (int i = listaWPAlturas.size() - 1; i >= 0; i--) {
        listaWPAlturas[i]->verDistancia = false;
    }
}

void VisorAlturasWidget::mouseMoveEvent(QMouseEvent *ev)
{
    posicionPuntero = ev->pos();

    //WayPointAltura* wp = getWayPointEnPunto(posicionPuntero);
    if (ev->buttons() & Qt::LeftButton && wayPointSeleccionado != Q_NULLPTR) {        
        int dy = posicionPuntero.y() - puntoAnclajeRaton.y();
        int height = this->height() - (MARGENES_VISOR_ALTURAS * 2);
        int altura = wayPointSeleccionado->getAltura(dy, height, datosProyecto->alturaMaximaVisor);
        if (altura > datosProyecto->alturaMaximaVisor || altura < 0) return;
        if (dy > 15 || dy < -15) return;
        if (wayPointSeleccionado->wayPoint->altura < 0 && dy > 0) {
            return;
        }
        if (wayPointSeleccionado->wayPoint->altura > datosProyecto->alturaMaximaVisor && dy < 0) {
            return;
        }
        if (wayPointSeleccionado->wayPoint->bloqueado) {
            return;
        }
        wayPointSeleccionado->desplazarEjeY(dy, height, datosProyecto->alturaMaximaVisor);

    } else if (clickIzquierdoPulsado) {
        int dx = posicionPuntero.x() - puntoAnclajeRaton.x();
        offsetX += dx;
        if (offsetX > 0 && dx > 0) {
            offsetX = 0;
        } else if (offsetX < this->width() - widthVisor - 60 && dx < 0) {
            offsetX = this->width() - widthVisor - 60;
        }
    }
    puntoAnclajeRaton = posicionPuntero;
}

void VisorAlturasWidget::mouseReleaseEvent(QMouseEvent*)
{
    clickIzquierdoPulsado = false;
    puntoAnclajeRaton = QPointF(0, 0);
    wayPointSeleccionado = Q_NULLPTR;
    ocultarDistanciaDeWayPointsAltura();
}


double VisorAlturasWidget::getMaximaAlturaWP()
{
    double maximaAltura = 0;
    for (int i = 0; i < listaWP.size(); i++) {        
        if (maximaAltura < listaWP[i]->altura) {
            maximaAltura = listaWP[i]->altura;
        }
    }
    return maximaAltura;
}

float VisorAlturasWidget::getAngulo(float x1, float y1, float x2, float y2)
{
    float Theta;
    if (x2 - x1 == 0 ) {
        if ( y2 > y1 ) {
            Theta = 0;
        } else {
            Theta = static_cast<float>( 3.1415 );
        }
    } else     {
        Theta = std::atan( (y2 - y1) / (x2 - x1) );
        if ( x2 > x1 ) {
            Theta = static_cast<float>( 3.1514 ) / 2.0f - Theta;
        } else {
            Theta = static_cast<float>( 3.1415 ) * 1.5f - Theta;
        }
    }
    return Theta;
}

void VisorAlturasWidget::pintarLineasUnionWayPoints(QPainter* painter,  WayPointAltura* wp1, WayPointAltura* wp2)
{
    int x1, y1, x2, y2;
    x1 = wp1->cajaEnvolvente.x();
    x2 = wp2->cajaEnvolvente.x();
    y1 = wp1->cajaEnvolvente.y();
    y2 = wp2->cajaEnvolvente.y();

    QLineF linea(x1, y1, x2, y2);


    double angulo = atan( abs(wp1->wayPoint->altura - wp2->wayPoint->altura) / wp2->wayPoint->distanciaWPAnterior);
    angulo = angulo * 180 / PI;
    if (angulo < 0) { angulo = 360 + angulo; }

    painter->save();
    if (angulo <= 90) {        
        if (angulo > datosProyecto->maximaInclinacion) {
            painter->setPen(QPen(QColor(0xAA, 0x11, 0x00), 3));
        } else {
            painter->setPen(QPen(QColor(0x11, 0xD8, 0x00, 0x66), 2));
        }

    } else {
         angulo = 360 - angulo;
         if (angulo > datosProyecto->maximaDeclinacion) {
             painter->setPen(QPen(QColor(0xAA, 0x11, 0x00), 3));
         } else {
             painter->setPen(QPen(QColor(0x11, 0xD8, 0x00, 0x66), 2));
         }
    }


    painter->drawLine(x1 + DIAMETRO_MARCA_WAYPOINT / 2, y1 + DIAMETRO_MARCA_WAYPOINT / 2,  x2 + DIAMETRO_MARCA_WAYPOINT / 2, y2 + DIAMETRO_MARCA_WAYPOINT / 2);
    painter->restore();
}

void VisorAlturasWidget::pintarLineasAltura(QPainter* painter, int width, int height, int alturaMaxima)
{
    for (int i = 0; i < datosProyecto->lineasAltura.size(); i++) {        
        datosProyecto->lineasAltura[i]->pintar(painter, width, height, alturaMaxima, verNombres);
    }
}

void VisorAlturasWidget::actualizarElementosVisor()
{
    int height = this->height() - (MARGENES_VISOR_ALTURAS * 2);
    for (int i = 0; i < listaWPAlturas.size(); i++) {
        listaWPAlturas[i]->recalcularPosicionEnVisor(height, datosProyecto->alturaMaximaPermitida);
    }
}

void VisorAlturasWidget::wheelEvent(QWheelEvent* event)
{
    if (event->delta() > 0) {
        factorEscala += 0.1;
    } else {
        factorEscala -= 0.1;
    }
    if (factorEscala < 1) {
        factorEscala = 1.0;
    }
    widthVisorAnterior = widthVisor;
    widthVisor = (this->width() - (MARGENES_VISOR_ALTURAS * 2)) * factorEscala;
    if (offsetX + widthVisor + MARGENES_VISOR_ALTURAS < widthVisor + MARGENES_VISOR_ALTURAS) {
        offsetX = offsetX + (widthVisorAnterior - widthVisor);
        if (offsetX > 0) offsetX = 0;
    }
}

void VisorAlturasWidget::pintarGrafica(QPainter* painter)
{
    painter->save();
    painter->setPen(QPen(QColor(0xFF, 0xD8, 0x00), 2));
    painter->drawLine(offsetX + MARGENES_VISOR_ALTURAS, MARGENES_VISOR_ALTURAS, offsetX + MARGENES_VISOR_ALTURAS, this->height() - MARGENES_VISOR_ALTURAS);
    painter->drawLine(offsetX + MARGENES_VISOR_ALTURAS, this->height() - MARGENES_VISOR_ALTURAS, offsetX + widthVisor + MARGENES_VISOR_ALTURAS, this->height() - MARGENES_VISOR_ALTURAS);
    painter->setPen(QPen(QColor(0xFF, 0xD8, 0x00), 2));
    painter->drawLine(offsetX + MARGENES_VISOR_ALTURAS, MARGENES_VISOR_ALTURAS, offsetX + widthVisor + MARGENES_VISOR_ALTURAS, MARGENES_VISOR_ALTURAS);
    painter->drawLine(offsetX + widthVisor + MARGENES_VISOR_ALTURAS, MARGENES_VISOR_ALTURAS, offsetX + widthVisor + MARGENES_VISOR_ALTURAS, this->height() - MARGENES_VISOR_ALTURAS);
    painter->restore();
}

void VisorAlturasWidget::pintarLineaMaximaAltura(QPainter* painter)
{
    painter->save();
    int pxLimite = datosProyecto->alturaMaximaPermitida * heightVisor / datosProyecto->alturaMaximaVisor;
    QRectF caja = QRectF(offsetX + 2, this->height() - (45 + pxLimite), 28, 30);
    painter->setPen(QPen(QColor(0xFF, 0x00, 0x00), 1, Qt::DashLine));
    painter->setFont(QFont("Arial", 7, QFont::Bold));
    painter->drawText(caja, QString::number(datosProyecto->alturaMaximaPermitida) + "m",  Qt::AlignLeft | Qt::AlignVCenter);
    painter->drawLine(offsetX + MARGENES_VISOR_ALTURAS, this->height() - (MARGENES_VISOR_ALTURAS + pxLimite), offsetX + widthVisor + MARGENES_VISOR_ALTURAS, this->height() - (MARGENES_VISOR_ALTURAS + pxLimite));
    painter->restore();
}

void VisorAlturasWidget::paintEvent(QPaintEvent *)
{
    heightVisor = this->height() - (MARGENES_VISOR_ALTURAS * 2);
    widthVisor = this->width() - (MARGENES_VISOR_ALTURAS * 2);
    widthVisor = widthVisor * factorEscala;

    QPainter painter(this);
    pintarGrafica(&painter);
    pintarLineasAltura(&painter, widthVisor, heightVisor, datosProyecto->alturaMaximaVisor);
    pintarLineaMaximaAltura(&painter);

    if (listaWP.size() <= 0) {
        return;
    }

    painter.save();
    painter.setPen(QPen(QColor(0xFF, 0xD8, 0x00), 1));


    if (!distanciasReales) {        
            listaWPAlturas[0]->pintar(&painter, MARGENES_VISOR_ALTURAS, verAlturas);
            double anchoSubtramo = widthVisor / (listaWPAlturas.size() - 1);
            int suma = 0;
            int resto = (widthVisor % (listaWPAlturas.size() - 1));

            for (int i = 0; i < listaWPAlturas.size(); i++) {
                double inicioSubtramo;

                inicioSubtramo = (i * anchoSubtramo) + MARGENES_VISOR_ALTURAS + suma;
                if (suma < resto) {
                    suma++;
                }
                if (i + 1 < listaWPAlturas.size()) {
                    pintarLineasUnionWayPoints(&painter, listaWPAlturas[i], listaWPAlturas[i+1]);
                }
                listaWPAlturas[i]->pintar(&painter, inicioSubtramo, verAlturas);
            }        
    } else {
        //pinta con la distancia real
        double distanciaTotal = gestorWayPoints->getDistanciaTotalVuelo();
        if (distanciaTotal == 0) {
            distanciaTotal = 1;
        }
        double totalAcumuladoDistanciaPosicion = MARGENES_VISOR_ALTURAS;
        double totalAcumuladoDistancia = 0.0;


            for (int i = 0; i < listaWPAlturas.size(); i++) {
                double distanciaTramo = listaWPAlturas[i]->wayPoint->distanciaWPAnterior;
                double inicioSubtramo = distanciaTramo * widthVisor / distanciaTotal;
                totalAcumuladoDistancia += distanciaTramo;
                listaWPAlturas[i]->distanciaAcumulada = totalAcumuladoDistancia;
                totalAcumuladoDistanciaPosicion += inicioSubtramo;
                if (i + 1 < listaWPAlturas.size()) {
                    pintarLineasUnionWayPoints(&painter, listaWPAlturas[i], listaWPAlturas[i + 1]);
                }
                listaWPAlturas[i]->pintar(&painter, totalAcumuladoDistanciaPosicion, verAlturas);
                QRectF caja = QRectF(listaWPAlturas[i]->cajaEnvolvente.x(), heightVisor + MARGENES_VISOR_ALTURAS, MARGENES_VISOR_ALTURAS + 20, 30);
                painter.setPen(QColor(0xFF, 0xD8, 0x00));
                painter.setFont(QFont("Arial", 7, QFont::Bold));
                painter.drawText(caja, Formateador::formatearEntero(totalAcumuladoDistancia) + "m",  Qt::AlignLeft | Qt::AlignVCenter);
            }        
    }

    if (clickIzquierdoPulsado && wayPointSeleccionado != Q_NULLPTR) {
        //pintar los metros al lado del punto
        painter.setPen(QPen(QColor(0xF0, 0x30, 0x30), 2));
        painter.setFont(QFont("Arial", 8, QFont::Bold));
        QRectF caja = QRectF(wayPointSeleccionado->cajaEnvolvente.x() + 25,
                             wayPointSeleccionado->cajaEnvolvente.y(),
                             30,
                             wayPointSeleccionado->cajaEnvolvente.height()
                             );
        painter.drawText(caja, QString::number(wayPointSeleccionado->wayPoint->altura) + "m",  Qt::AlignHCenter | Qt::AlignVCenter);
    }

    painter.restore();
    update();
}



