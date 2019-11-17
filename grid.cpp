#include "grid.h"
#include <QDebug>

Grid::Grid()
{
}

void Grid::addPunto(QPointF punto)
{
    //QPointF puntoReal(punto.x() - HerramientasVisor::getInstancia()->getDesplazamientoX(),
    //                          punto.y() - HerramientasVisor::getInstancia()->getDesplazamientoY());
    QPointF puntoReal = HerramientaConversorCoordenadas::getInstancia()->convertirCoordenada(Enumerados::LatLng, punto.x() - HerramientasVisor::getInstancia()->getDesplazamientoX(), punto.y() - HerramientasVisor::getInstancia()->getDesplazamientoY());
    double latitud = puntoReal.y();
    double longitud= puntoReal.x();
    //double latitud = HerramientaConversorCoordenadas::getInstancia()->convertirPixelYALatitud(puntoReal.y());
    //double longitud= HerramientaConversorCoordenadas::getInstancia()->convertirPixelXALongitud(puntoReal.x());
    QPointF coordenada;
    coordenada.setX(longitud);
    coordenada.setY(latitud);
    coordenadas.push_back(coordenada);
}

void Grid::addPunto(QPointF punto, int posicion)
{
    //QPointF puntoReal(punto.x() - HerramientasVisor::getInstancia()->getDesplazamientoX(),
    //                          punto.y() - HerramientasVisor::getInstancia()->getDesplazamientoY());
    QPointF puntoReal = HerramientaConversorCoordenadas::getInstancia()->convertirCoordenada(Enumerados::LatLng, punto.x() - HerramientasVisor::getInstancia()->getDesplazamientoX(), punto.y() - HerramientasVisor::getInstancia()->getDesplazamientoY());
    double latitud = puntoReal.y();
    double longitud= puntoReal.x();
    //double latitud = HerramientaConversorCoordenadas::getInstancia()->convertirPixelYALatitud(puntoReal.y());
    //double longitud= HerramientaConversorCoordenadas::getInstancia()->convertirPixelXALongitud(puntoReal.x());
    QPointF coordenada;
    coordenada.setX(longitud);
    coordenada.setY(latitud);
    coordenadas.removeAt(posicion);
    coordenadas.insert(posicion, coordenada);
}

void Grid::addPuntoVirtual(QPointF punto)
{    
    if (coordenadas.size() == 0) return;
    if (coordenadas.size() == 1) {
        coordenadas.push_back(punto);
        return;
    }
    quitarPuntoVirtual();
    addPunto(punto);
}

void Grid::quitarPuntoVirtual()
{
    if (coordenadas.size() > 1) {
        coordenadas.pop_back();
    }
}

int Grid::getNumeroPuntos()
{
    return coordenadas.size();
}

bool Grid::esPuntoDePoligono(QPointF p)
{
    for (int i = 0; i < poligono.size(); i++) {
        QRectF cajaPunto = QRectF(poligono.at(i).x() - 5, poligono.at(i).y() - 5, 10 , 10);
        if (cajaPunto.contains(p)) {
            posicionPuntoRedimension = i;
            return true;
        }
    }
    posicionPuntoRedimension = -1;
    return false;
}

bool Grid::estaPuntoContenido(QPointF punto)
{    
    return poligono.containsPoint(punto, Qt::WindingFill);
}

void Grid::predibujarRecorrido(QPainter* painter)
{
    painter->save();
    painter->setPen(QPen(QColor(0xFF, 0xFF, 0xFF, 0xFF), 2));
    painter->setBrush(QBrush(QColor(0xFF, 0xFF, 0xFF, 0xFF)));
    for (int i = 1; i < puntosRecorridoCalculado.size(); i++) {

        QPointF puntoReal1 = HerramientaConversorCoordenadas::getInstancia()->getPixel(Enumerados::LatLng, puntosRecorridoCalculado[i - 1].x(), puntosRecorridoCalculado[i - 1].y());
        puntoReal1.setX(HerramientasVisor::getInstancia()->getDesplazamientoX() + puntoReal1.x());
        puntoReal1.setY(HerramientasVisor::getInstancia()->getDesplazamientoY() +  puntoReal1.y());

        QPointF puntoReal2 = HerramientaConversorCoordenadas::getInstancia()->getPixel(Enumerados::LatLng, puntosRecorridoCalculado[i].x(), puntosRecorridoCalculado[i].y());;
        puntoReal2.setX(HerramientasVisor::getInstancia()->getDesplazamientoX() + puntoReal2.x());
        puntoReal2.setY(HerramientasVisor::getInstancia()->getDesplazamientoY() +  puntoReal2.y());

        QLineF linea;
        linea.setP1(puntoReal1);
        linea.setP2(puntoReal2);        
        painter->drawLine(linea);

        int cx = 0;
        int cy = 0;
        if (puntoReal1.x() < puntoReal2.x()) {
            cx = puntoReal1.x() + ((puntoReal2.x() - puntoReal1.x()) / 2);
        } else {
            cx = puntoReal1.x() - ((puntoReal1.x() - puntoReal2.x()) / 2);
        }
        if (puntoReal1.y() < puntoReal2.y()) {
            cy = puntoReal1.y() + ((puntoReal2.y() - puntoReal1.y()) / 2);
        } else {

            cy = puntoReal1.y() - ((puntoReal1.y() - puntoReal2.y()) / 2);
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
        painter->drawPolygon(flecha);
    }

    painter->restore();
}

void Grid::crearRecorridoHorizontalHaciaDerechaYArriba(QRectF caja, int numeroIteraciones, int espacioEntreLineas, int overshot, int overshotLat)
{
    bool irArriba = true;

    for (int i = 0; i < numeroIteraciones; i++) {
        int x =  caja.bottomLeft().x() + (i * espacioEntreLineas);
        if (i == 0) {
            int yInicial = caja.bottomLeft().y();
            if (!irArriba) {
                yInicial = caja.topLeft().y();
            }

            QPointF puntoInicial(x, yInicial);
            puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoInicial);
        }
        int yFinal = caja.topLeft().y();
        if (!irArriba) {
            yFinal = caja.bottomLeft().y();
        }

        QPointF puntoFinal(x, yFinal);
        puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoFinal);

        if (i + 1 < numeroIteraciones) {            
            if (overshot > 0) {
                int direccionOvershot = 1;
                if (irArriba) {
                    direccionOvershot = -1;
                }
                QPointF puntoOvershot(puntoFinal.x() + overshotLat, puntoFinal.y() + (direccionOvershot * overshot));
                puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoOvershot);
            }

            QPointF puntoGiro(puntoFinal.x() + espacioEntreLineas, puntoFinal.y());
            puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoGiro);
        }
        irArriba = !irArriba;
    }
}

void Grid::crearRecorridoHorizontalHaciaDerechaYAbajo(QRectF caja, int numeroIteraciones, int espacioEntreLineas, int overshot, int overshotLat)
{
    bool irArriba = false;

    for (int i = 0; i < numeroIteraciones; i++) {

        int x =  caja.topLeft().x() + (i * espacioEntreLineas);
        if (i == 0) {
            int yInicial = caja.topLeft().y();
            if (irArriba) {
                yInicial = caja.bottomLeft().y();
            }

            QPointF puntoInicial(x, yInicial);
            puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoInicial);
        }
        int yFinal = caja.bottomLeft().y();
        if (irArriba) {
            yFinal = caja.topLeft().y();
        }

        QPointF puntoFinal(x, yFinal);
        puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoFinal);

        if (i + 1 < numeroIteraciones) {

            //overshoot
            if (overshot > 0) {
                int direccionOvershot = 1;
                if (irArriba) {
                    direccionOvershot = -1;
                }
                QPointF puntoOvershot(puntoFinal.x() + (overshotLat), puntoFinal.y() + (direccionOvershot * overshot));
                puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoOvershot);
            }

            QPointF puntoGiro(puntoFinal.x() + espacioEntreLineas, puntoFinal.y());
            puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoGiro);
        }
        irArriba = !irArriba;
    }
}

void Grid::crearRecorridoHorizontalHaciaIzquierdaYArriba(QRectF caja, int numeroIteraciones, int espacioEntreLineas, int overshot, int overshotLat)
{
    bool irArriba = true;

    for (int i = 0; i < numeroIteraciones; i++) {

        int x =  caja.bottomRight().x() - (i * espacioEntreLineas);
        if (i == 0) {
            int yInicial = caja.bottomRight().y();
            if (!irArriba) {
                yInicial = caja.topRight().y();
            }

            QPointF puntoInicial(x, yInicial);
            puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoInicial);
        }
        int yFinal = caja.topRight().y();
        if (!irArriba) {
            yFinal = caja.bottomRight().y();
        }

        QPointF puntoFinal(x, yFinal);
        puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoFinal);

        if (i + 1 < numeroIteraciones) {

            //overshoot
            if (overshot > 0) {
                int direccionOvershot = 1;
                if (irArriba) {
                    direccionOvershot = -1;
                }
                QPointF puntoOvershot(puntoFinal.x() - (overshotLat), puntoFinal.y() + (direccionOvershot * overshot));
                puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoFinal);
            }

            QPointF puntoGiro(puntoFinal.x() - espacioEntreLineas, puntoFinal.y());
            puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoGiro);
        }
        irArriba = !irArriba;
    }
}

void Grid::crearRecorridoHorizontalHaciaIzquierdaYAbajo(QRectF caja, int numeroIteraciones, int espacioEntreLineas, int overshot, int overshotLat)
{
    bool irArriba = false;

    for (int i = 0; i < numeroIteraciones; i++) {
        int x =  caja.topRight().x() - (i * espacioEntreLineas);
        if (i == 0) {
            int yInicial = caja.topRight().y();
            if (irArriba) {
                yInicial = caja.bottomRight().y();
            }

            QPointF puntoInicial(x, yInicial);
            puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoInicial);
        }

        int yFinal = caja.bottomRight().y();
        if (irArriba) {
            yFinal = caja.topRight().y();
        }

        QPointF puntoFinal(x, yFinal);
        puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoFinal);

        if (i + 1 < numeroIteraciones) {
            //overshoot
            if (overshot > 0) {
                int direccionOvershot = 1;
                if (irArriba) {
                    direccionOvershot = -1;
                }
                QPointF puntoOvershot(puntoFinal.x() - (overshotLat), puntoFinal.y() + (direccionOvershot * overshot));
                puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoOvershot);
            }


            QPointF puntoGiro(puntoFinal.x() - espacioEntreLineas, puntoFinal.y());
            puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoGiro);
        }
        irArriba = !irArriba;
    }
}

void Grid::crearRecorridoVerticalHaciaDerechaYArriba(QRectF caja, int numeroIteraciones, int espacioEntreLineas, int overshot, int overshotLat)
{
    bool irDerecha = true;

    for (int i = 0; i < numeroIteraciones; i++) {

        int y =  caja.bottomLeft().y() - (i * espacioEntreLineas);
        if (i == 0) {
            int xInicial = caja.bottomLeft().x();
            if (!irDerecha) {
                xInicial = caja.bottomLeft().x();
            }

            QPointF puntoInicial(xInicial, y);
            puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoInicial);
        }
        int xFinal = caja.bottomRight().x();
        if (!irDerecha) {
            xFinal = caja.bottomLeft().x();
        }

        QPointF puntoFinal(xFinal, y);
        puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoFinal);

        if (i + 1 < numeroIteraciones) {

            //overshoot
            if (overshot > 0) {
                int direccionOvershot = 1;
                if (!irDerecha) {
                    direccionOvershot = -1;
                }
                QPointF puntoOvershot(puntoFinal.x() + (direccionOvershot * overshot), puntoFinal.y() - (overshotLat));
                puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoOvershot);
            }


            QPointF puntoGiro(puntoFinal.x(), puntoFinal.y() - espacioEntreLineas);
            puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoGiro);
        }
        irDerecha = !irDerecha;
    }
}

void Grid::crearRecorridoVerticalHaciaDerechaYAbajo(QRectF caja, int numeroIteraciones, int espacioEntreLineas, int overshot, int overshotLat)
{
    bool irDerecha = true;

    for (int i = 0; i < numeroIteraciones; i++) {

        int y =  caja.topLeft().y() + (i * espacioEntreLineas);
        if (i == 0) {
            int xInicial = caja.bottomLeft().x();
            if (!irDerecha) {
                xInicial = caja.bottomLeft().x();
            }

            QPointF puntoInicial(xInicial, y);
            puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoInicial);
        }
        int xFinal = caja.bottomRight().x();
        if (!irDerecha) {
            xFinal = caja.bottomLeft().x();
        }

        QPointF puntoFinal(xFinal, y);
        puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoFinal);

        if (i + 1 < numeroIteraciones) {

            //overshoot
            if (overshot > 0) {
                int direccionOvershot = 1;
                if (!irDerecha) {
                    direccionOvershot = -1;
                }
                QPointF puntoOvershot(puntoFinal.x() + (direccionOvershot * overshot), puntoFinal.y() + (overshotLat));
                puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoOvershot);
            }

            QPointF puntoGiro(puntoFinal.x(), puntoFinal.y() + espacioEntreLineas);
            puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoGiro);
        }
        irDerecha = !irDerecha;
    }
}

void Grid::crearRecorridoVerticalHaciaIzquierdaYArriba(QRectF caja, int numeroIteraciones, int espacioEntreLineas, int overshot, int overshotLat)
{
    bool irDerecha = false;    
    for (int i = 0; i < numeroIteraciones; i++) {

        int y =  caja.bottomRight().y() - (i * espacioEntreLineas);
        if (i == 0) {
            int xInicial = caja.bottomLeft().x();
            if (!irDerecha) {
                xInicial = caja.bottomRight().x();
            }

            QPointF puntoInicial(xInicial, y);
            puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoInicial);
        }
        int xFinal = caja.bottomRight().x();
        if (!irDerecha) {
            xFinal = caja.bottomLeft().x();
        }

        QPointF puntoFinal(xFinal, y);
        puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoFinal);
        //puntosRecorridoCalculado << puntoFinal;

        if (i + 1 < numeroIteraciones) {
            //overshoot
            if (overshot > 0) {
                int direccionOvershot = 1;
                if (!irDerecha) {
                    direccionOvershot = -1;
                }
                QPointF puntoOvershot(puntoFinal.x() + (direccionOvershot * overshot), puntoFinal.y() - (overshotLat));
                puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoOvershot);
            }

            QPointF puntoGiro(puntoFinal.x(), puntoFinal.y() - espacioEntreLineas);
            puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoGiro);
        }
        irDerecha = !irDerecha;
    }
}

void Grid::crearRecorridoVerticalHaciaIzquierdaYAbajo(QRectF caja, int numeroIteraciones, int espacioEntreLineas, int overshot, int overshotLat)
{
    bool irDerecha = false;    

    for (int i = 0; i < numeroIteraciones; i++) {

        int y =  caja.topRight().y() + (i * espacioEntreLineas);
        if (i == 0) {
            int xInicial = caja.topRight().x();
            if (irDerecha) {
                xInicial = caja.topLeft().x();
            }

            QPointF puntoInicial(xInicial, y);
            puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoInicial);
            //puntosRecorridoCalculado << puntoInicial;
        }
        int xFinal = caja.topLeft().x();
        if (irDerecha) {
            xFinal = caja.topRight().x();
        }

        QPointF puntoFinal(xFinal, y);       
        puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoFinal);
        //puntosRecorridoCalculado << puntoFinal;

        if (i + 1 < numeroIteraciones) {

            //overshoot
            if (overshot > 0) {
                int direccionOvershot = 1;
                if (!irDerecha) {
                    direccionOvershot = -1;
                }
                QPointF puntoOvershot(puntoFinal.x() + (direccionOvershot * overshot), puntoFinal.y() + (overshotLat));                
                puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoOvershot);

                //puntosRecorridoCalculado << puntoOvershot;
            }

            QPointF puntoGiro(puntoFinal.x(), puntoFinal.y() + espacioEntreLineas);            
            puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(puntoGiro);

            //puntosRecorridoCalculado << puntoGiro;
        }
        irDerecha = !irDerecha;
    }
}

void Grid::crearRecorridoCajaEnvolvente(Enumerados::RecorridoGrid recorrido, Enumerados::DireccionHorizontalGrid direccionHorizontal, Enumerados::DireccionVerticalGrid direccionVertical, int espacioEntreLineas, int overshot, int overshotLat = 0)
{
    puntosRecorridoCalculado.clear();
    QRectF caja = poligono.boundingRect();

    if (recorrido == Enumerados::Horizontal) {
        int numeroIteraciones = (abs(caja.bottomRight().x() - caja.bottomLeft().x()) / espacioEntreLineas) + 1;
        if (direccionHorizontal == Enumerados::DDerecha && direccionVertical == Enumerados::DArriba) {
            crearRecorridoHorizontalHaciaDerechaYArriba(caja, numeroIteraciones, espacioEntreLineas, overshot, overshotLat);
        } else  if (direccionHorizontal == Enumerados::DDerecha && direccionVertical == Enumerados::DAbajo) {
            crearRecorridoHorizontalHaciaDerechaYAbajo(caja, numeroIteraciones, espacioEntreLineas, overshot, overshotLat);
        } else if (direccionHorizontal == Enumerados::DIzquierda && direccionVertical == Enumerados::DArriba) {
            crearRecorridoHorizontalHaciaIzquierdaYArriba(caja, numeroIteraciones, espacioEntreLineas, overshot, overshotLat);
        } else {
            crearRecorridoHorizontalHaciaIzquierdaYAbajo(caja, numeroIteraciones, espacioEntreLineas, overshot, overshotLat);
        }
    } else {
        int numeroIteraciones = (abs(caja.bottomRight().y() - caja.topRight().y()) / espacioEntreLineas) + 1;
        if (direccionHorizontal == Enumerados::DDerecha && direccionVertical == Enumerados::DArriba) {
            crearRecorridoVerticalHaciaDerechaYArriba(caja, numeroIteraciones, espacioEntreLineas, overshot, overshotLat);
        } else  if (direccionHorizontal == Enumerados::DDerecha && direccionVertical == Enumerados::DAbajo) {
            crearRecorridoVerticalHaciaDerechaYAbajo(caja, numeroIteraciones, espacioEntreLineas, overshot, overshotLat);
        } else if (direccionHorizontal == Enumerados::DIzquierda && direccionVertical == Enumerados::DArriba) {
            crearRecorridoVerticalHaciaIzquierdaYArriba(caja, numeroIteraciones, espacioEntreLineas, overshot, overshotLat);
        } else {
            crearRecorridoVerticalHaciaIzquierdaYAbajo(caja, numeroIteraciones, espacioEntreLineas, overshot, overshotLat);
        }
    }
}

int Grid::obtenerAnguloRecorridoMasOptimo()
{
    int distanciaMinima = 999999999;
    int anguloOptimo = 0;
    for (int i = 0; i < 360; i++) {
        puntosRecorridoCalculado.clear();
        crearRecorrido(i);
        int distancia = calcularDistanciaRecorrido();
        if (distancia < distanciaMinima) {
            anguloOptimo = i;
            distanciaMinima = distancia;
        }
    }
    return anguloOptimo;
}

void Grid::crearRecorrido()
{
    puntosRecorridoCalculado.clear();
    crearRecorrido(angulo);
}

void Grid::crearRecorrido(int anguloRotacion)
{
    QRectF cajaEnvolvente = poligono.boundingRect();
    QPointF centro = cajaEnvolvente.center();
    QPolygonF cajaGirada;
    cajaGirada << rotarPunto(anguloRotacion, cajaEnvolvente.topLeft(), centro);
    cajaGirada << rotarPunto(anguloRotacion, cajaEnvolvente.topRight(), centro);
    cajaGirada << rotarPunto(anguloRotacion, cajaEnvolvente.bottomRight(), centro);
    cajaGirada << rotarPunto(anguloRotacion, cajaEnvolvente.bottomLeft(), centro);
    cajaGirada << cajaGirada[0];

    QList<QLineF> lineList;

    float x = cajaGirada.boundingRect().topLeft().x() + (HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng,espacioLineas)/2);// * HerramientasVisor::getInstancia()->factorEscalado / 2);
    while (x < cajaGirada.boundingRect().bottomRight().x() ) {
        float yTop =    cajaGirada.boundingRect().topLeft().y() - 100.0;
        float yBottom = cajaGirada.boundingRect().bottomRight().y() + 100.0;
        lineList += QLineF(rotarPunto(anguloRotacion, QPointF(x, yTop), centro), rotarPunto(anguloRotacion, QPointF(x, yBottom), centro));
        x += HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, espacioLineas);// * HerramientasVisor::getInstancia()->factorEscalado;
    }

    QList<QLineF> intersectLines;
    interseccionLineasConPoligono(lineList, poligono, intersectLines);
   // if (poligono.count() > 5) {
   //     intersectLines = lineList;
   // }
    QList<QLineF> resultLines;
    ajustarDireccionLinea(intersectLines, resultLines);    
    float turnaroundDist = HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, overshot);
    float turnaroundLatDist = HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, overshotLateral);

    puntosRecorridoCalculado.clear();
    for (int i=0; i<resultLines.count(); i++) {
        const QLineF& line = resultLines[i];


        QPointF turnaroundOffset = line.p2() - line.p1();
        turnaroundOffset = turnaroundOffset * turnaroundDist / sqrt(pow(turnaroundOffset.x(),2.0) + pow(turnaroundOffset.y(),2.0));

        if (i & 1) {
            if (turnaroundDist > 0.0) {
               if (overshotLateral > 0) {
                   double anguloOpuesto = atan(turnaroundLatDist / turnaroundDist);
                   anguloOpuesto = 90 - (anguloOpuesto * 180.0) / PI;
                   double offset = 90 - line.angle();
                   double anguloLinea = 360 - anguloOpuesto - offset;
                   QLineF lineaOvershot;
                   lineaOvershot.setP1(line.p1());
                   lineaOvershot.setAngle(anguloLinea);
                   lineaOvershot.setLength(sqrt(pow(turnaroundDist,2.0) + pow(turnaroundLatDist,2.0)));
                   puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(line.p2()) << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(line.p1()) << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(lineaOvershot.p2());
               } else {
                    double anguloLinea = line.angle();
                    QLineF lineaOvershot;
                    lineaOvershot.setP1(line.p1());
                    lineaOvershot.setAngle(anguloLinea + 180);
                    lineaOvershot.setLength(sqrt(pow(turnaroundDist,2.0) + pow(turnaroundLatDist,2.0)));
                    puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(line.p2()) << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(line.p1()) << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(lineaOvershot.p2());
               }
            }
        } else {
            if (turnaroundDist > 0.0) {
                if (overshotLateral > 0) {
                    double anguloOpuesto = atan(turnaroundLatDist / turnaroundDist);
                    anguloOpuesto = 90 - (anguloOpuesto * 180.0) / PI;
                    double offset = 90 - line.angle();
                    double anguloLinea = anguloOpuesto - offset;
                    QLineF lineaOvershot;
                    lineaOvershot.setP1(line.p2());
                    lineaOvershot.setAngle(anguloLinea);
                    lineaOvershot.setLength(sqrt(pow(turnaroundDist,2.0) + pow(turnaroundLatDist,2.0)));
                    //puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(line.p1()) << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(line.p2()) << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(lineaOvershot.p2());
                    puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(line.p1()) << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(line.p2()) << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(lineaOvershot.p2());
                } else {
                    double anguloLinea = line.angle();
                    QLineF lineaOvershot;
                    lineaOvershot.setP1(line.p2());
                    lineaOvershot.setAngle(anguloLinea);
                    lineaOvershot.setLength(sqrt(pow(turnaroundDist,2.0) + pow(turnaroundLatDist,2.0)));
                    puntosRecorridoCalculado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(line.p1()) << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(line.p2()) << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(lineaOvershot.p2());
                }
            }
        }
    }
}

void Grid::desplazaPunto(QPointF punto)
{
    if (posicionPuntoRedimension < 0 || posicionPuntoRedimension >= coordenadas.size()) {
        return;
    }
    addPunto(punto, posicionPuntoRedimension);
}

void Grid::pintar(QPainter* painter)
{
    painter->save();
    if (!marcado) {
        painter->setPen(QPen(QColor(0x00, 0x13, 0x7F), 2));
        painter->setBrush(QBrush(QColor(0x00, 0x94, 0xFF, 0x66)));
    } else {
        painter->setPen(QPen(QColor(0xFF, 0xD8, 0x00), 2));
        painter->setBrush(QColor(0xFF, 0xD8, 0x00, 0x66));
    }

    poligono.clear();
    for (int i = 0; i < coordenadas.size(); i++) {
        QPointF punto = HerramientaConversorCoordenadas::getInstancia()->getPixel(Enumerados::LatLng, coordenadas[i].x(), coordenadas[i].y());
        punto.setX(HerramientasVisor::getInstancia()->getDesplazamientoX() + punto.x());
        punto.setY(HerramientasVisor::getInstancia()->getDesplazamientoY() +  punto.y());
        poligono.push_back(punto);
    }

    painter->drawPolygon(poligono);

    for (int i = 0; i < poligono.size(); i++) {

        QRectF cajaPunto = QRectF(poligono.at(i).x() - 5, poligono.at(i).y() - 5, 10 , 10);

        if (i != posicionPuntoRedimension) {
            painter->setPen(QPen(QColor(0x00, 0x13, 0x7F), 2));
            painter->setBrush(QBrush(QColor(0x00, 0x13, 0x7F, 0xFF)));
        } else {
            painter->setPen(QPen(QColor(0xFF, 0x55, 0x00), 2));
            painter->setBrush(QColor(0xFF, 0x55, 0x00, 0xFF));
        }

        painter->drawRect(cajaPunto);

    }


    painter->restore();
    if (mostrarPrevia) {
        crearRecorrido();
        predibujarRecorrido(painter);
    }

//    painter->save();
//    QRectF cajaEnvolvente = poligono.boundingRect();
//    QPointF centro = cajaEnvolvente.center();
//    QPolygonF cajaGirada;
//    cajaGirada << rotarPunto(cajaEnvolvente.topLeft(), centro);
//    cajaGirada << rotarPunto(cajaEnvolvente.topRight(), centro);
//    cajaGirada << rotarPunto(cajaEnvolvente.bottomRight(), centro);
//    cajaGirada << rotarPunto(cajaEnvolvente.bottomLeft(), centro);
//    cajaGirada << cajaGirada[0];

//    painter->setPen(QPen(QColor(0xFF, 0x08, 0x00), 2));
//    painter->setBrush(QColor(0xFF, 0x08, 0x00, 0x66));
//    painter->drawRect(cajaEnvolvente);

//    painter->setPen(QPen(QColor(0xFF, 0x08, 0xF0), 2));
//    painter->setBrush(QColor(0xFF, 0x08, 0xF0, 0x66));
//    painter->drawPolygon(cajaGirada);


//    painter->restore();
}

void Grid::ajustarDireccionLinea(const QList<QLineF>& lineList, QList<QLineF>& resultLines)
{
    for (int i=0; i<lineList.count(); i++) {
        const QLineF& line = lineList[i];
        QLineF adjustedLine;

        if (line.angle() > 180.0) {
            adjustedLine.setP1(line.p2());
            adjustedLine.setP2(line.p1());
        } else {
            adjustedLine = line;
        }

        resultLines += adjustedLine;
    }
}

bool Grid::hayInterseccionPoligonoConCaja(QRectF caja)
{
    return caja.intersects(poligono.boundingRect());
}

void Grid::interseccionLineasConPoligono(const QList<QLineF>& lineList, const QPolygonF& polygon, QList<QLineF>& resultLines)
{
    for (int i=0; i<lineList.count(); i++) {
        int foundCount = 0;
        QLineF intersectLine;
        const QLineF& line = lineList[i];

        for (int j=0; j<polygon.count(); j++) {
            QPointF intersectPoint;
            QLineF polygonLine;
            if (j < polygon.count() - 1) {
                polygonLine = QLineF(polygon[j], polygon[j+1]);
            } else {
                polygonLine = QLineF(polygon[j], polygon[0]);
            }
            if (line.intersect(polygonLine, &intersectPoint) == QLineF::BoundedIntersection) {
                if (foundCount == 0) {
                    foundCount++;
                    intersectLine.setP1(intersectPoint);
                } else {
                    foundCount++;
                    intersectLine.setP2(intersectPoint);
                    break;
                }
            }
        }

        if (foundCount == 2) {
            resultLines += intersectLine;
        }
    }
}

QPointF Grid::rotarPunto(int anguloRotacion, const QPointF& point, const QPointF& origin)
{
    QPointF rotated;
    double radians = (PI / 180.0) * anguloRotacion; //* HerramientaConversorCoordenadas::getInstancia()->convertirAngulo(angulo);

    rotated.setX(((point.x() - origin.x()) * cos(radians)) - ((point.y() - origin.y()) * sin(radians)) + origin.x());
    rotated.setY(((point.x() - origin.x()) * sin(radians)) + ((point.y() - origin.y()) * cos(radians)) + origin.y());

    return rotated;
}

QList<QPointF> Grid::getRutaCalculada()
{
    QList<QPointF> resultado;
    for (int i = 0; i < gridPoints.size(); i++) {
        resultado << HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(gridPoints[i]);
    }
    return resultado;
}

qreal Grid::getArea()
{
    QPainterPath path;
    path.addPolygon(poligono);

    QPointF a,b;
    qreal len;
    qreal area=0;
    for(len=0; len<path.length(); len+=1)
    {
        a=path.pointAtPercent(path.percentAtLength(len));
        b=path.pointAtPercent(path.percentAtLength(len+1>path.length()?path.length():len+1));
        area+=(b.x()-a.x())*(a.y()+b.y());
    }
    area = abs(area/qreal(2));
    //esto es en pixeles, ahora calcularlo en hectareas
    return HerramientaConversorCoordenadas::getInstancia()->convertirAreaPixelesEnHectareas(poligono) / 1000;
}

double Grid::calcularDistanciaRecorrido()
{
    double distancia = 0;
    for (int i = 1; i < puntosRecorridoCalculado.size(); i++) {
        distancia += HerramientaConversorCoordenadas::getInstancia()->getDistanciaEntreCoordenadas(Enumerados::LatLng, puntosRecorridoCalculado[i - 1].x(), puntosRecorridoCalculado[i - 1].y(), puntosRecorridoCalculado[i].x(), puntosRecorridoCalculado[i].y());
    }
    return distancia;
}
