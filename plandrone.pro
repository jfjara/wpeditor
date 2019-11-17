#-------------------------------------------------
#
# Project created by QtCreator 2017-03-01T11:39:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = plandrone
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    visorwidget.cpp \
    waypoint.cpp \
    gestorwaypoints.cpp \
    grid.cpp \
    gestorgrids.cpp \
    waypointdockwidget.cpp \
    enumerados.cpp \
    waypointlistmodel.cpp \
    visoralturasdockwidget.cpp \
    visoralturaswidget.cpp \
    gestorxml.cpp \
    datosproyecto.cpp \
    gestorfly.cpp \
    waypointaltura.cpp \
    lineaaltura.cpp \
    lineasalturadialog.cpp \
    lineaalturawidget.cpp \
    configuracionvisordialog.cpp \
    configuracionvisorwaypointsdialog.cpp \
    imagenmapa.cpp \
    configuracionimagenesmapadialog.cpp \
    imagenmapawidget.cpp \
    herramientasvisor.cpp \
    herramientaconversorcoordenadas.cpp \
    mapashorizondialog.cpp \
    logomenuwidget.cpp \
    acercadedialog.cpp \
    griddockwidget.cpp \
    camara.cpp \
    camarasdialog.cpp \
    simplecrypt.cpp \
    gestorcamaras.cpp \
    medidordistanciaangulo.cpp \
    lineaaterrizaje.cpp \
    gestionaravionesdialog.cpp \
    avion.cpp \
    gestoraviones.cpp \
    properties.cpp \
    idioma.cpp \
    etiquetawidget.cpp \
    configuraciondialog.cpp \
    log.cpp \
    gestorconfiguracion.cpp \
    gestorultimasmapas.cpp \
    cajaseleccion.cpp \
    importarflydialog.cpp \
    medidorescala.cpp \
    gestorhistoricooperaciones.cpp \
    operacionrealizada.cpp \
    comandoextra.cpp \
    gestorcomandosextra.cpp \
    objetocomandoextraencapsulado.cpp \
    utmconversion.cpp \
    formateador.cpp \
    doublevalidator.cpp \
    parametrosobligatoriosdialog.cpp \
    tile.cpp \
    gestortiles.cpp \
    mosaicodialog.cpp \
    barraprogresomosaicoform.cpp \
    operadormosaico.cpp \
    ventanavaciadialog.cpp \
    mosaicodockwidget.cpp


HEADERS  += mainwindow.h \
    visorwidget.h \
    waypoint.h \
    gestorwaypoints.h \
    grid.h \
    gestorgrids.h \
    waypointdockwidget.h \
    enumerados.h \
    waypointlistmodel.h \
    visoralturasdockwidget.h \
    visoralturaswidget.h \
    gestorxml.h \
    datosproyecto.h \
    gestorfly.h \
    waypointaltura.h \
    lineaaltura.h \
    lineasalturadialog.h \
    lineaalturawidget.h \
    configuracionvisordialog.h \
    configuracionvisorwaypointsdialog.h \
    imagenmapa.h \
    configuracionimagenesmapadialog.h \
    imagenmapawidget.h \
    herramientasvisor.h \
    herramientaconversorcoordenadas.h \
    mapashorizondialog.h \
    logomenuwidget.h \
    acercadedialog.h \
    griddockwidget.h \
    camara.h \
    camarasdialog.h \
    simplecrypt.h \
    constantes.h \
    gestorcamaras.h \
    medidordistanciaangulo.h \
    lineaaterrizaje.h \
    gestionaravionesdialog.h \
    avion.h \
    gestoraviones.h \
    properties.h \
    idioma.h \
    etiquetawidget.h \
    configuraciondialog.h \
    log.h \
    gestorconfiguracion.h \
    gestorultimasmapas.h \
    cajaseleccion.h \
    importarflydialog.h \
    medidorescala.h \
    gestorhistoricooperaciones.h \
    operacionrealizada.h \
    comandoextra.h \
    gestorcomandosextra.h \
    objetocomandoextraencapsulado.h \
    utmconversion.h \
    conversorcoordenadas.h \
    formateador.h \
    doublevalidator.h \
    parametrosobligatoriosdialog.h \
    tile.h \
    gestortiles.h \
    mosaicodialog.h \
    barraprogresomosaicoform.h \
    operadormosaico.h \
    ventanavaciadialog.h \
    mosaicodockwidget.h


FORMS    += mainwindow.ui \
    waypointwidget.ui \
    visoralturasdockwidget.ui \
    lineasalturadialog.ui \
    lineaalturawidget.ui \
    configuracionvisordialog.ui \
    configuracionvisorwaypointsdialog.ui \
    configuracionimagenesmapadialog.ui \
    imagenmapawidget.ui \
    mapashorizondialog.ui \
    logomenuwidget.ui \
    acercadedialog.ui \
    griddockwidget.ui \
    camarasdialog.ui \
    gestionaravionesdialog.ui \
    configuraciondialog.ui \
    importarflydialog.ui \
    parametrosobligatoriosdialog.ui \
    mosaicodialog.ui \
    barraprogresomosaicoform.ui \
    ventanavaciadialog.ui \
    mosaicodockwidget.ui

RESOURCES += \
    recursos.qrc
