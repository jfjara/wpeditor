#ifndef CAMARASDIALOG_H
#define CAMARASDIALOG_H

#include <QDialog>
#include <QList>
#include <QtGlobal>
#include <QRadioButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include "camara.h"
#include "simplecrypt.h"
#include "constantes.h"
#include "idioma.h"
#include "doublevalidator.h"
#include "formateador.h"

namespace Ui {
class CamarasDialog;
}

class CamarasDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CamarasDialog(QList<Camara*> camaras, QWidget *parent = 0);
    ~CamarasDialog();

    bool aceptado = false;
    QList<Camara*> camaras;
    QList<QRadioButton*> listaRadioCamara;

    void traducir();

public slots:
    void cargarDatosCamara();
    void crearCamara();
    void cancelarEdicion();
    void guardarCamara();
    void eliminarCamara();
    void modificarCamara();
    void exportar();
    void aceptar();

private:

    Ui::CamarasDialog *ui;
    bool esModificacion = false;

    Camara* getCamaraSeleccionada();
    int getIndiceCamaraSeleccionada();
    void addRadio(QRadioButton* radio);
    void alinear();
    void mostrarBotonesAceptacion(bool valor);
    void mostrarBotonesEdicion(bool valor);
    void rellenarFormulario(Camara* camara);
    void activarCampos(bool valor);
    void guardarDatosCamara(Camara* camara);
    void addCamara(Camara* camara);
    void quitarCamara(int indice);
    void crearValidadores();

};

#endif // CAMARASDIALOG_H
