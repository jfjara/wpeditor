#ifndef GESTIONARAVIONESDIALOG_H
#define GESTIONARAVIONESDIALOG_H

#include <QDialog>
#include <QList>
#include <QtGlobal>
#include <QRadioButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include "avion.h"
#include "simplecrypt.h"
#include "constantes.h"
#include "idioma.h"
#include "formateador.h"
#include "doublevalidator.h"


namespace Ui {
class GestionarAvionesDialog;
}

class GestionarAvionesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GestionarAvionesDialog(QString rutaHorizon, QList<Avion*> listaAviones, QWidget *parent = 0);
    ~GestionarAvionesDialog();

    QString rutaHorizon = "/home";
    bool aceptado = false;
    QList<Avion*> listaAviones;
    QList<QRadioButton*> listaRadioAviones;

    void addAvion(Avion* avion);
    void addRadio(QRadioButton* radio);
    void rellenarFormulario(Avion* avion);
    void crearValidadores();
    int getIndiceAvionSeleccionado();
    Avion* getAvionSeleccionado();
    void activarCampos(bool valor);
    void mostrarBotonesAceptacion(bool valor);
    void mostrarBotonesEdicion(bool valor);
    void guardarDatosAvion(Avion* avion);
    void quitarAvion(int indice);
    void traducir();

private:
    Ui::GestionarAvionesDialog *ui;
    bool esModificacion = false;

public slots:
     void cargarDatosAvion();
     void cancelarEdicion();
     void guardarAvion();
     void eliminarAvion();
     void modificarAvion();
     void exportar();
     void aceptar();
     void crearAvion();
     void seleccionarImagen();
     void seleccionarHeader();
     void seleccionarFooter();

};

#endif // GESTIONARAVIONESDIALOG_H
