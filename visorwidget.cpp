#include "visorwidget.h"
#include <QDebug>

VisorWidget::VisorWidget(DatosProyecto* proyecto, GestorWayPoints* gestorWayPoints, GestorGrids* gestorGrids, QWidget *parent) : QWidget(parent)
{
    this->proyecto = proyecto;
    this->gestorWayPoints = gestorWayPoints;
    this->gestorGrids = gestorGrids;

    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::black);
    this->setPalette(pal);


#ifdef MODO_PORTATIL
    zoomMas = QImage(":iconos/images/zoomMas.png");
    zoomMenos = QImage(":iconos/images/zoomMenos.png");
#endif

    rosaDeLosVientos = QImage(":iconos/images/windrose.png");
    puntoAnclajeRaton = QPointF(0, 0);

    setAutoFillBackground(true);
    setMouseTracking(true);    

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(mostrarMenuContextual(const QPoint &)));
}


void VisorWidget::mostrarMenuContextual(const QPoint &punto)
{
    if (banderaOcultarMenuContextual) {
        banderaOcultarMenuContextual = false;
        return;
    }

    if (gestorWayPoints->getCircuit() != Q_NULLPTR) {
        return;
    }

    if (proyecto->imagenesMapa.size() == 0) {
        return;
    }

    puntoCrearWPMenuContextual = punto;

    QPoint globalPos = this->mapToGlobal(punto);
    QMenu contextMenu(QString("Context menu"), this);

    QAction accionCrearWPFromTo(Idioma::getInstancia()->getTexto("visor.menu.fromTo"), this);
    accionCrearWPFromTo.setIcon(QIcon(":iconos/images/fromto.png"));
    connect(&accionCrearWPFromTo, SIGNAL(triggered()), this, SLOT(crearWPFromToMenuContextual()));
    contextMenu.addAction(&accionCrearWPFromTo);

    QAction accionCrearWPFlyTo(Idioma::getInstancia()->getTexto("visor.menu.flyTo"), this);
    accionCrearWPFlyTo.setIcon(QIcon(":iconos/images/flyto.png"));
    connect(&accionCrearWPFlyTo, SIGNAL(triggered()), this, SLOT(crearWPFlyToMenuContextual()));
    contextMenu.addAction(&accionCrearWPFlyTo);

    QAction accionCrearCirculo(Idioma::getInstancia()->getTexto("visor.menu.circulo"), this);
    accionCrearCirculo.setIcon(QIcon(":iconos/images/circle.png"));
    connect(&accionCrearCirculo, SIGNAL(triggered()), this, SLOT(crearCircleMenuContextual()));
    contextMenu.addAction(&accionCrearCirculo);

    QAction accionCrearCircuit(Idioma::getInstancia()->getTexto("visor.menu.circuit"), this);
    accionCrearCircuit.setIcon(QIcon(":iconos/images/circuit.png"));
    connect(&accionCrearCircuit, SIGNAL(triggered()), this, SLOT(crearCircuitMenuContextual()));
    contextMenu.addAction(&accionCrearCircuit);
    contextMenu.exec(globalPos);
}


void VisorWidget::crearPuntoDesdeMenuContextual(QPointF punto, Enumerados::TipoWayPoint comando, Enumerados::ModoWayPoint tipo)
{
    gestorWayPoints->modoCrear = true;
    gestorWayPoints->crearWayPoint(punto);
    gestorWayPoints->modoCrear = false;
    WayPoint* wp = gestorWayPoints->getUltimoWayPoint();
    if (comando == Enumerados::WayPoint) {
        wp->modo = tipo;
    } else {
        wp->tipo = comando;
    }
    if (comando == Enumerados::Circuit) {
        wp->altura = 0;
        establecerAterrizaje();
    }
    emit waypointAdd(wp);
}

void VisorWidget::crearWPFromToMenuContextual()
{
    crearPuntoDesdeMenuContextual(puntoCrearWPMenuContextual, Enumerados::WayPoint, Enumerados::FromTo);
}

void VisorWidget::crearWPFlyToMenuContextual()
{
    crearPuntoDesdeMenuContextual(puntoCrearWPMenuContextual, Enumerados::WayPoint, Enumerados::FlyTo);
}

void VisorWidget::crearCircleMenuContextual()
{
    crearPuntoDesdeMenuContextual(puntoCrearWPMenuContextual, Enumerados::Circulo, Enumerados::FromTo);
}

void VisorWidget::crearCircuitMenuContextual()
{
    crearPuntoDesdeMenuContextual(puntoCrearWPMenuContextual, Enumerados::Circuit, Enumerados::FromTo);
}

void VisorWidget::marcarWayPoint(int orden)
{
    gestorWayPoints->marcaWayPoint(orden);
}

void VisorWidget::crearNuevoWPAntesDelMarcado()
{
    WayPoint* wp = gestorWayPoints->crearWayPointAnteriorAlMarcado();
    emit waypointAdd(wp);
}

void VisorWidget::activarCreacionWP(bool valor)
{
    gestorWayPoints->modoCrear = valor;
    gestorWayPoints->marcarTodosWayPoint(false);
}

void VisorWidget::activarMedirDistanciaYAngulo(bool valor)
{
    medirDistanciasYAngulos = valor;
    //medidores.clear();
}

void VisorWidget::activarCreacionGrid(bool valor)
{
    gestorWayPoints->marcarTodosWayPoint(false);
    gestorGrids->activarCreacionGrid(valor);
}

void VisorWidget::pintarLeyenda(QPainter* painter)
{
    painter->save();
    QRectF caja(this->width() - 150, 20, 140, 115);
    painter->setPen(QPen(QColor(0xFF, 0xFF, 0xFF, 0xAA), 1));
    painter->setBrush(QBrush(QColor(0x00, 0x00, 0x00, 0x66)));
    painter->drawRect(caja);

    painter->setFont(QFont("Arial", 8, QFont::Bold));

    painter->setPen(QPen(QColor(0x00, 0x7F, 0x0E), 2));
    painter->setBrush(QBrush(QColor(0x00, 0xFF, 0x00)));
    painter->drawEllipse(this->width() - 130 - DIAMETRO_MARCA_WAYPOINT/2,
                         40 - DIAMETRO_MARCA_WAYPOINT/2,
                         DIAMETRO_MARCA_WAYPOINT,
                         DIAMETRO_MARCA_WAYPOINT);
    painter->setPen(QPen(QColor(0x00, 0xFF, 0x00), 2));
    painter->drawText(QPoint(this->width() - 115, 45), "FromTo");

    painter->setPen(QPen(QColor(0xFF, 0xD8, 0x00), 2));
    painter->setBrush(QBrush(Qt::yellow));
    painter->drawEllipse(this->width() - 130 - DIAMETRO_MARCA_WAYPOINT/2,
                         65 - DIAMETRO_MARCA_WAYPOINT/2,
                         DIAMETRO_MARCA_WAYPOINT,
                         DIAMETRO_MARCA_WAYPOINT);
    painter->setPen(QPen(Qt::yellow, 2));
    painter->drawText(QPoint(this->width() - 115, 70), "FlyTo");

    painter->setPen(QPen(QColor(0x57, 0x00, 0x7F), 2));
    painter->setBrush(QBrush(QColor(0xB2, 0x00, 0xFF)));
    painter->drawEllipse(this->width() - 130 - DIAMETRO_MARCA_WAYPOINT/2,
                         90 - DIAMETRO_MARCA_WAYPOINT/2,
                         DIAMETRO_MARCA_WAYPOINT,
                         DIAMETRO_MARCA_WAYPOINT);
    painter->setPen(QPen(QColor(0xB2, 0x00, 0xFF), 2));
    painter->drawText(QPoint(this->width() - 115, 95), "Circle");

    painter->setPen(QPen(QColor(0x7F, 0x00, 0x00), 2));
    painter->setBrush(QBrush(QColor(0xFF, 0x00, 0x00)));
    painter->drawEllipse(this->width() - 130 - DIAMETRO_MARCA_WAYPOINT/2,
                         115 - DIAMETRO_MARCA_WAYPOINT/2,
                         DIAMETRO_MARCA_WAYPOINT,
                         DIAMETRO_MARCA_WAYPOINT);
    painter->setPen(QPen(QColor(0xFF, 0x00, 0x00), 2));
    painter->drawText(QPoint(this->width() - 115, 120), "Circuit");
    painter->restore();
}

void VisorWidget::pintarBarraAuxiliar(QPainter* painter)
{
    painter->save();
    QRectF barra(0, this->height() - 20, this->width(), this->height());
    painter->setPen(QPen(QColor(0x00, 0x00, 0x00, 0x66), 1));
    painter->setBrush(QBrush(QColor(0x00, 0x00, 0x00, 0x66)));
    painter->drawRect(barra);
    painter->restore();
}

void VisorWidget::crearImagen(QPixmap &pixmap)
{
    QPainter painter(&pixmap);
    if (proyecto->imagenesMapa.size() > 0){// && gestorTiles == Q_NULLPTR) {
        proyecto->imagenesMapa[0]->pintar(&painter);
    }
    //painter.drawImage(10, 10, rosaDeLosVientos);

    if (gestorTiles != Q_NULLPTR && verMosaico) {
        gestorTiles->pintar(&painter);
    }
    gestorGrids->pintar(&painter);
    gestorWayPoints->pintar(&painter, proyecto->tipoCoordenada, verDistanciasEntrePuntos, verLineasDireccion, distanciaTotalRecorrido);    

    cajaSeleccion.pintar(&painter);

    for (int i = 0; i < medidores.size(); i++) {
        medidores[i]->pintar(&painter);
    }



    if (proyecto->imagenesMapa.size() > 0) {
        medidorEscala.pintar(&painter, this->width(), this->height());
    }

#ifdef MODO_PORTATIL
    painter.save();
    painter.setPen(QPen(QColor(0xFF, 0xFF, 0xFF, 0x22), 2));
    painter.setBrush(QBrush(QColor(0xFF, 0xFF, 0xFF, 0x44)));
    painter.drawRect(QRectF(3, 3, 52, 102));
    painter.drawImage(5, 5, zoomMas);
    painter.drawImage(5, 55, zoomMenos);
    painter.restore();
#endif


    pintarBarraAuxiliar(&painter);
    pintarPosicionPuntero(&painter);
    if (mostrarLeyenda) {
        pintarLeyenda(&painter);
    }
}

void VisorWidget::paintEvent(QPaintEvent *)
{
    QPixmap pixmap(this->width(), this->height());
    crearImagen(pixmap);
    QPainter painter(this);
    painter.drawPixmap(0, 0, pixmap);
}

#ifdef MODO_PORTATIL
void VisorWidget::acercarVisor()
{
    realizarZoom(120, 0, 0);
}

void VisorWidget::alejarVisor()
{
    realizarZoom(-120, 0, 0);
}
#endif

void VisorWidget::realizarZoom(double delta, int pxMapa, int pyMapa)
{
    if (HerramientasVisor::getInstancia()->factorEscalado >= 2.0 && delta > 0) {
        return;
    }

    if (HerramientasVisor::getInstancia()->factorEscalado <= 0.1 && delta < 0) {
        return;
    }

    if (!desplazarVisor) {
        return;
    }

    if (proyecto->imagenesMapa.size() == 0) {
        return;
    }

    double curZoom = HerramientasVisor::getInstancia()->factorEscalado + delta / 4000.0;
    double ancho = 0.0;
    double alto = 0.0;
    if (proyecto->imagenesMapa.size() == 0) {
        ancho = this->width();
        alto = this->height();
    } else {
        ancho = proyecto->imagenesMapa[0]->imagen->width();
        alto = proyecto->imagenesMapa[0]->imagen->height();
    }

    int xReposicionada = (pxMapa * (ancho * curZoom)) / (ancho *  HerramientasVisor::getInstancia()->factorEscalado);
    int yReposicionada = (pyMapa * (alto * curZoom)) / (alto *  HerramientasVisor::getInstancia()->factorEscalado);

    HerramientasVisor::getInstancia()->acumularDesplazamiento(pxMapa - xReposicionada,  pyMapa - yReposicionada);
    HerramientasVisor::getInstancia()->factorEscalado = curZoom;

    HerramientaConversorCoordenadas::getInstancia()->establecerDatos(Enumerados::LatLng,
                HerramientaConversorCoordenadas::getInstancia()->yReferencia1,
                HerramientaConversorCoordenadas::getInstancia()->xReferencia1,
                HerramientaConversorCoordenadas::getInstancia()->yReferencia2,
                HerramientaConversorCoordenadas::getInstancia()->xReferencia2,
                ancho *  HerramientasVisor::getInstancia()->factorEscalado,
                alto *  HerramientasVisor::getInstancia()->factorEscalado
                );

    for (int i = 0; i < proyecto->imagenesMapa.size(); i++) {
        proyecto->imagenesMapa[i]->punto = QPointF(HerramientasVisor::getInstancia()->getDesplazamientoX(), HerramientasVisor::getInstancia()->getDesplazamientoY());
    }

    QImage imagenEscalada = proyecto->imagenesMapa[0]->imagen->scaled(proyecto->imagenesMapa[0]->imagen->width() * HerramientasVisor::getInstancia()->factorEscalado,
                   proyecto->imagenesMapa[0]->imagen->height() * HerramientasVisor::getInstancia()->factorEscalado,
                   Qt::KeepAspectRatio);

    HerramientaConversorCoordenadas::getInstancia()->setWidth(imagenEscalada.width());
    HerramientaConversorCoordenadas::getInstancia()->setHeight(imagenEscalada.height());
    HerramientaConversorCoordenadas::getInstancia()->imagenReescalada = imagenEscalada;
    proyecto->imagenesMapa[0]->punto = QPointF(HerramientasVisor::getInstancia()->getDesplazamientoX(), HerramientasVisor::getInstancia()->getDesplazamientoY());
    gestorWayPoints->refrescarWayPoints();

    if (gestorTiles != Q_NULLPTR) {
        for (int i = 0; i < gestorTiles->tiles.size(); i++) {
            gestorTiles->tiles[i]->imagenEscalada = gestorTiles->tiles[i]->imagen->scaled(gestorTiles->tiles[i]->imagen->width() * HerramientasVisor::getInstancia()->factorEscalado, gestorTiles->tiles[i]->imagen->width() * HerramientasVisor::getInstancia()->factorEscalado, Qt::KeepAspectRatio);
            QTransform rotating;
            rotating.rotate(90);
            gestorTiles->tiles[i]->imagenEscalada = gestorTiles->tiles[i]->imagenEscalada.transformed(rotating);
        }
    }

    update();
}

void VisorWidget::wheelEvent(QWheelEvent* event)
{
    double delta = event->delta();
    int pxMapa = event->pos().x() - HerramientasVisor::getInstancia()->getDesplazamientoX();
    int pyMapa = event->pos().y() - HerramientasVisor::getInstancia()->getDesplazamientoY();
    realizarZoom(delta, pxMapa, pyMapa);

}

void VisorWidget::mouseReleaseEvent(QMouseEvent*)
{
    desplazarVisor = true;
    clickIzquierdoPulsado = false;
    gestorWayPoints->mostrarLineas = true;
    puntoAnclajeRaton = QPointF(0, 0);
    if (seleccionarWaypoints) {
        cajaSeleccion.mostrar = false;
        gestorWayPoints->seleccionarPuntosPorCaja(cajaSeleccion.seleccion);
        gestorGrids->seleccionarGridsPorCaja(cajaSeleccion.seleccion);

        for (int i = 0; i < medidores.size(); i++) {
            medidores[i]->seleccionado = false;
            medidores[i]->marcar(cajaSeleccion.seleccion);
        }
    }
    if (waypointModificado && operacionModificacionWP != Q_NULLPTR) {
        GestorHistoricoOperaciones::getInstancia()->apilar(operacionModificacionWP);
        waypointModificado = false;
    }
}

void VisorWidget::mousePressEvent(QMouseEvent *ev)
{
    setFocus();
    puntoAnclajeRaton = ev->pos();
    puntoAnterior = puntoAnclajeRaton;
    if (ev->buttons() & Qt::LeftButton) {
        clickIzquierdoPulsado = true;
    }

    if ( ev->buttons() & Qt::MiddleButton) {
        clickIzquierdoPulsado = true;
        return;
    }

#ifdef MODO_PORTATIL

    if (cajaZoomMas.contains(puntoAnclajeRaton)) {
        acercarVisor();
    } else if (cajaZoomMenos.contains(puntoAnclajeRaton)) {
        alejarVisor();
    }

#endif

    if (seleccionarWaypoints) {
        cajaSeleccion.mostrar = !cajaSeleccion.mostrar;
        cajaSeleccion.seleccion.setX(puntoAnclajeRaton.x());
        cajaSeleccion.seleccion.setY(puntoAnclajeRaton.y());
        cajaSeleccion.seleccion.setWidth(0);
        cajaSeleccion.seleccion.setHeight(0);
        return;
    }

    if (gestorGrids->modoCrear && ev->buttons() & Qt::RightButton) {
        emit finalizarEdicion();
        banderaOcultarMenuContextual = true;
        return;
    }

    if (gestorWayPoints->modoCrear && ev->buttons() & Qt::RightButton) {
        emit finalizarEdicion();
        return;
    }

    if (medirDistanciasYAngulos) {
        if (medidores.size() > 0) {
            if (medidores[medidores.size() - 1]->finalizado) {
                MedidorDistanciaAngulo* medidor =new MedidorDistanciaAngulo();

                QPointF coordenada = HerramientaConversorCoordenadas::getInstancia()->convertirCoordenada(Enumerados::LatLng, puntoAnclajeRaton.x() - HerramientasVisor::getInstancia()->getDesplazamientoX(), puntoAnclajeRaton.y() - HerramientasVisor::getInstancia()->getDesplazamientoY());
                medidor->longitud1 = coordenada.x();
                medidor->latitud1 = coordenada.y();
                medidor->longitud2 = medidor->longitud1;
                medidor->latitud2 = medidor->latitud1;
                medidores.push_back(medidor);
            } else {
                medidores[medidores.size() - 1]->finalizado = true;
                QPointF coordenada = HerramientaConversorCoordenadas::getInstancia()->convertirCoordenada(Enumerados::LatLng, puntoAnclajeRaton.x() - HerramientasVisor::getInstancia()->getDesplazamientoX(), puntoAnclajeRaton.y() - HerramientasVisor::getInstancia()->getDesplazamientoY());
                medidores[medidores.size() - 1]->longitud2 = coordenada.x();
                medidores[medidores.size() - 1]->latitud2 = coordenada.y();
            }
        } else {
            MedidorDistanciaAngulo* medidor =new MedidorDistanciaAngulo();
            QPointF coordenada = HerramientaConversorCoordenadas::getInstancia()->convertirCoordenada(Enumerados::LatLng, puntoAnclajeRaton.x() - HerramientasVisor::getInstancia()->getDesplazamientoX(), puntoAnclajeRaton.y() - HerramientasVisor::getInstancia()->getDesplazamientoY());
            medidor->longitud1 = coordenada.x();
            medidor->latitud1 = coordenada.y();
            medidor->longitud2 = medidor->longitud1;
            medidor->latitud2 = medidor->latitud1;
            medidores.push_back(medidor);
        }
        return;
    }

    if (gestorGrids->modoCrear) {
        if (gestorWayPoints->getCircuit() == Q_NULLPTR) {
            gestorGrids->addPuntoGrid(puntoAnclajeRaton);
        }
        desplazarVisor = false;
        return;
    }

    if (gestorWayPoints->modoCrear) {
        if (gestorWayPoints->getCircuit() != Q_NULLPTR) {
            return;
        }

        gestorWayPoints->crearWayPoint(ev->pos());
        WayPoint* wp = gestorWayPoints->getUltimoWayPoint();
        emit waypointAdd(wp);
        desplazarVisor = false;
        return;
    }
    emit ocultarInformacion();
    gestorWayPoints->marcarTodosWayPoint(false);
    gestorGrids->marcarTodosGrids(false);
    int posicion = gestorWayPoints->marcaWayPoint(puntoAnclajeRaton);
    if (posicion != -1) {
        emit waypointVisorMarcado(posicion);
        WayPoint* wpClonado = gestorWayPoints->clonarWayPoint(gestorWayPoints->wayPoints[posicion]);
        operacionModificacionWP = new OperacionRealizada();
        operacionModificacionWP = new OperacionRealizada();
        operacionModificacionWP->accion = Enumerados::Modificar;
        operacionModificacionWP->tipoObjeto = Enumerados::ObjetoWayPoint;
        operacionModificacionWP->listaWaypoints.push_back(wpClonado);
        waypointModificado = true;

    }
    gestorGrids->marcarGrid(puntoAnclajeRaton);
    gestorGrids->marcarPuntoGrid(puntoAnclajeRaton);
    for (int i = 0; i < medidores.size(); i++) {
        medidores[i]->marcar(puntoAnclajeRaton);
    }


}

void VisorWidget::desplazarImagenesMapa(int dx, int dy)
{
    for (int i = 0; i < proyecto->imagenesMapa.size(); i++) {
        proyecto->imagenesMapa[i]->desplazar(dx, dy);
    }
}

void VisorWidget::mouseMoveEvent(QMouseEvent *ev)
{
    posicionPuntero = ev->pos();

    if (seleccionarWaypoints) {
        cajaSeleccion.seleccion.setX(puntoAnclajeRaton.x());
        cajaSeleccion.seleccion.setY(puntoAnclajeRaton.y());
        cajaSeleccion.seleccion.setWidth(posicionPuntero.x() - puntoAnclajeRaton.x());
        cajaSeleccion.seleccion.setHeight(posicionPuntero.y() - puntoAnclajeRaton.y());
        return;
    }

    if (gestorGrids->estaDesplazandoPuntoPoligono && clickIzquierdoPulsado && !gestorGrids->modoCrear) {
        gestorGrids->desplazaPuntoPoligono(posicionPuntero);
        return;
    }



    if (desplazarVisor && clickIzquierdoPulsado && !gestorWayPoints->hayWayPointMarcado()) {
        int dx = posicionPuntero.x() - puntoAnterior.x();
        int dy = posicionPuntero.y() - puntoAnterior.y();
        HerramientasVisor::getInstancia()->acumularDesplazamiento(dx, dy);
        puntoAnterior = posicionPuntero;
        desplazarImagenesMapa(dx, dy);
        gestorWayPoints->refrescarWayPoints();
    }

    if (medirDistanciasYAngulos) {
        if (medidores.size() > 0) {
            if (!medidores[medidores.size() - 1]->finalizado) {
                QPointF coordenada = HerramientaConversorCoordenadas::getInstancia()->convertirCoordenada(Enumerados::LatLng, posicionPuntero.x() - HerramientasVisor::getInstancia()->getDesplazamientoX(), posicionPuntero.y() - HerramientasVisor::getInstancia()->getDesplazamientoY());
                medidores[medidores.size() - 1]->longitud2 = coordenada.x();
                medidores[medidores.size() - 1]->latitud2 = coordenada.y();
            }
        }
        return;
    }

    if (gestorGrids->modoCrear) {
        //qDebug() << posicionPuntero;
        gestorGrids->addPuntoVirtual(posicionPuntero);
        return;
    }

    if (!gestorWayPoints->modoCrear) {
        if (clickIzquierdoPulsado) {
            gestorWayPoints->desplazarWayPoint(posicionPuntero);            
            gestorWayPoints->refrescarWayPoints();
            emit waypointUpdate();
        }
    } else {
        return;
    }

    if (desplazarVisor) {
        return;
    }

    if (gestorWayPoints->hayWayPoint(posicionPuntero)) {
        QApplication::setOverrideCursor(Qt::PointingHandCursor);
    } else {
        QApplication::setOverrideCursor(Qt::ArrowCursor);
    }

}

void VisorWidget::mouseDoubleClickEvent(QMouseEvent* e)
{
    puntoAnclajeRaton = e->pos();

    if (gestorWayPoints->modoCrear) {
        return;
    }

    if (gestorGrids->modoCrear) {
        return;
    }

    if (medirDistanciasYAngulos) {
        return;
    }

    if (e->button() == Qt::LeftButton)
    {
        gestorWayPoints->marcarTodosWayPoint(false);
        gestorGrids->marcarTodosGrids(false);
        if (gestorGrids->marcarGrid(e->pos())) {
            Grid* grid = gestorGrids->getGridMarcado();
            if (grid != Q_NULLPTR) {
                emit mostrarInformacionGrid(grid);
                return;
            } else {
                emit ocultarInformacion();
            }
        }        
    }

}


void VisorWidget::establecerAterrizajeDesdeFly()
{
    WayPoint* circuit = gestorWayPoints->getCircuit();
    if (circuit == Q_NULLPTR) {
        return;
    }
    if (circuit->aterrizaje != Q_NULLPTR) {
        delete circuit->aterrizaje;
    }
    circuit->aterrizaje = new LineaAterrizaje();
    circuit->recorridoAterrizaje = circuit->aterrizaje->recorrido * 2 + circuit->aterrizaje->base;
}
void VisorWidget::establecerAterrizaje()
{
    WayPoint* circuit = gestorWayPoints->getCircuit();
    if (circuit == Q_NULLPTR) {
        return;
    }
    if (circuit->aterrizaje != Q_NULLPTR) {
        delete circuit->aterrizaje;
    }
    circuit->aterrizaje = new LineaAterrizaje();
    if (proyecto->aterrizajeCirculo) {
        gestorWayPoints->crearProtocoloAterrizaje(circuit->aterrizaje);
    }
    circuit->recorridoAterrizaje = circuit->aterrizaje->recorrido * 2 + circuit->aterrizaje->base;

}

void VisorWidget::eliminarWPSeleccionados()
{
    QList<WayPoint*> wpEliminar = gestorWayPoints->eliminarWayPointMarcados();
    for (int i = 0; i < wpEliminar.size(); i++) {
        emit waypointDel(wpEliminar[i]);
    }
}

void VisorWidget::eliminarMarcadoresSeleccionados()
{
    for (int i = 0; i < medidores.size(); i++) {
        if (medidores[i]->seleccionado) {
            medidores.removeAt(i);
            i--;
        }
    }
}

void VisorWidget::keyPressEvent(QKeyEvent *ev)
{
    if (ev->key() == Qt::Key_Delete) {
        eliminarWPSeleccionados();
        gestorGrids->eliminarGridMarcados();
        emit ocultarInformacion();     
        eliminarMarcadoresSeleccionados();
    }
    if (ev->key() == Qt::Key_Left) {
        int posicion = gestorWayPoints->marcarAnteriorWayPoint();
        if (posicion >= 0) {
            emit waypointVisorMarcado(posicion);
        }

    }
    if (ev->key() == Qt::Key_Right) {
        int posicion = gestorWayPoints->marcarSiguienteWayPoint();
        if (posicion >= 0) {
            emit waypointVisorMarcado(posicion);
        }
    }
    if (ev->key() == Qt::Key_Escape) {
        emit teclaPulsada(ev);
    }
    //QApplication::setOverrideCursor(Qt::ArrowCursor);
}


void VisorWidget::modificaAterrizaje(bool derecha, int pitch)
{
     WayPoint* circuit = gestorWayPoints->getCircuit();
    if (circuit != Q_NULLPTR &&  circuit->aterrizaje != Q_NULLPTR) {
        circuit->aterrizaje->aterrizajeADerecha = derecha;
        circuit->headingCircuit = pitch;
    }
}

void VisorWidget::landingEstablecido()
{    
    int posicionLanding = gestorWayPoints->borrarPuntosDespuesLanding();
    WayPoint* circuit = gestorWayPoints->getCircuit();
    if (circuit!= Q_NULLPTR && circuit->aterrizaje == Q_NULLPTR) {
        circuit->aterrizaje = new LineaAterrizaje();
    }

    circuit->recorridoAterrizaje = circuit->aterrizaje->recorrido * 2 + circuit->aterrizaje->base;
    emit actualizarListaDespuesDeLanding(posicionLanding);
}

void VisorWidget::pintarPosicionPuntero(QPainter* painter)
{
    painter->save();
    painter->setPen(QColor(0xFF, 0xFF, 0xFF));
    if (proyecto->tipoCoordenada == Enumerados::Pixeles) {
        QPointF puntoReal(posicionPuntero.x() - HerramientasVisor::getInstancia()->getDesplazamientoX(),
                          posicionPuntero.y() - HerramientasVisor::getInstancia()->getDesplazamientoY());
        painter->drawText(this->width() - 150, this->height() - 20, 150, 20,Qt::AlignLeft, "X: " + QString::number(puntoReal.x()) + " Y: " + QString::number(puntoReal.y()));
    } else if (proyecto->tipoCoordenada == Enumerados::UTM) {
        QString zona;
        QPointF puntoReal = HerramientaConversorCoordenadas::getInstancia()->convertirCoordenada(Enumerados::LatLng, posicionPuntero.x() - HerramientasVisor::getInstancia()->getDesplazamientoX(), posicionPuntero.y() - HerramientasVisor::getInstancia()->getDesplazamientoY(), zona);
        zona = proyecto->numeroZona + proyecto->letraZona;
        QPointF utm = HerramientaConversorCoordenadas::getInstancia()->convertirLatLonAUTM(puntoReal.y(), puntoReal.x(), zona);

       // QPointF utm = HerramientaConversorCoordenadas::getInstancia()->convertirCoordenada(Enumerados::UTM, posicionPuntero.x() - HerramientasVisor::getInstancia()->getDesplazamientoX(), posicionPuntero.y() - HerramientasVisor::getInstancia()->getDesplazamientoY(), proyecto->letraZona + proyecto->numeroZona);
        painter->drawText(this->width() - 230, this->height() - 20, 40, 20, Qt::AlignLeft, proyecto->letraZona + " " + proyecto->numeroZona);
        painter->drawText(this->width() - 150, this->height() - 20, 180, 20,Qt::AlignLeft, QString::number(utm.x(), 'd', 0) + " m E    " + QString::number(utm.y(), 'd', 0) + " m N");
    } else if (proyecto->tipoCoordenada == Enumerados::LatLng) {
        QPointF puntoReal = HerramientaConversorCoordenadas::getInstancia()->convertirCoordenada(Enumerados::LatLng, posicionPuntero.x() - HerramientasVisor::getInstancia()->getDesplazamientoX(), posicionPuntero.y() - HerramientasVisor::getInstancia()->getDesplazamientoY());
        double lat = puntoReal.y();
        double lng = puntoReal.x();
        painter->drawText(this->width() - 150, this->height() - 20, 150, 20,Qt::AlignLeft, "Lat: " + QString::number(lat) + "  Lng: " + QString::number(lng));
    }
    painter->restore();
    update();
}
