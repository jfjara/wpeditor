#ifndef ENUMERADOS_H
#define ENUMERADOS_H

#include <QObject>
#include <QMetaEnum>

class Enumerados : public QObject
{
    Q_OBJECT
    Q_ENUMS(TipoWayPoint)
    Q_ENUMS(DireccionCirculo)
    Q_ENUMS(ModoWayPoint)
    Q_ENUMS(Coordenadas)
    Q_ENUMS(IdiomasAplicacion)
    Q_ENUMS(ModoParametroComandoExtra)
public:
    explicit Enumerados(QObject *parent = 0);

    enum TipoWayPoint {      
      WayPoint = 0,
      Circulo = 1,
      Circuit = 2
    };

    enum DireccionCirculo {
        Izquierda = 0,
        Derecha = 1
    };

    enum ModoWayPoint {
        Ninguno = 0,
        FromTo = 1,
        FlyTo = 2
    };

    enum Coordenadas {
        LatLng = 0,
        UTM = 1,
        Pixeles = 2
    };

    enum RecorridoGrid {
        Vertical = 0,
        Horizontal = 1
    };

    enum DireccionVerticalGrid {
        DArriba = 0,
        DAbajo = 1
    };

    enum DireccionHorizontalGrid {
        DIzquierda = 0,
        DDerecha = 1
    };

    enum TiposRecorridoGridCajasEnvolventes {
        HorizontalArribaDerecha,
        HorizontalArribaIzquierda,
        HorizontalAbajoDerecha,
        HorizontalAbajoIzquierda,
        VerticalArribaDerecha,
        VerticalArribaIzquierda,
        VerticalAbajoDerecha,
        VerticalAbajoIzquierda
    };

    enum IdiomasAplicacion {
        Castellano,
        English
    };

    enum AccionRealizada {
        Crear,
        Modificar,
        Borrar
    };

    enum Objeto {
        ObjetoWayPoint,
        ObjetoGrid
    };

    enum ModoParametroComandoExtra {
        Disabled,
        FixedParams,
        SetParam1,
        SetParam2,
        SetParams1And2
    };

signals:

public slots:
};



#endif // ENUMERADOS_H
