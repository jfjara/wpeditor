#include "mainwindow.h"
#include <QApplication>
#include <QImageReader>
#include <QDebug>
#include "log.h"
#include "formateador.h"
#include "herramientaconversorcoordenadas.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::addLibraryPath(QLatin1String("./plugins"));

    //qDebug() << QImageReader::supportedImageFormats();
    /*QList<QByteArray> lista = QImageReader::supportedImageFormats ();
    for (int i = 0; i < lista.size(); i++) {
        Log::getInstancia()->info(QString(lista[i]));
    }*/
//    QString utmZona;
//    qDebug() << HerramientaConversorCoordenadas::getInstancia()->convertirLatLonAUTM(37.420345, -6.064453, utmZona);
//    qDebug() << HerramientaConversorCoordenadas::getInstancia()->convertirUTMALatLon(687461.03, 6090710.05, utmZona);

//    utmZona = "";
//    qDebug() << HerramientaConversorCoordenadas::getInstancia()->convertirLatLonAUTM(37.146918, -5.788141, utmZona);
//    qDebug() << HerramientaConversorCoordenadas::getInstancia()->convertirUTMALatLon(252372.75, 4114810.15, utmZona);



    Formateador::separadorDecimal = ',';
    Formateador::separadorMillar = '.';

    MainWindow w;
    w.show();

//    a.setStyleSheet(
//                QString("QMainWindow { background-color: #FFFFFF ; }").append(
//                QString("QPushButton { color: #404040;  background-color: #C0C0C0;   border-width: 1px;    border-color: #C0C0C0;    border-style: solid;    border-radius: 2;    padding: 3px;    font-size: 16px;    padding-left: 5px;    padding-right: 5px;    min-width: 15px;    max-width: 350px;    min-height: 15px;    max-height: 100px;} QPushButton:hover:!pressed     {       background-color: #3A7DBC; color: white;    }   ")).append(
//                QString("QLineEdit { padding: 1px; border-style: solid; border: 1px solid #4472ce; border-radius: 8px;}")).append(
//                QString("QDialog {  background-color:white;}")).append(
//                QString("QGroupBox { padding: 10px; border-style: solid; border: 1px solid #4472ce;}")).append(
//                QString("QScrollArea QWidget { background-color: white;  }")).append(
//                QString("QMenuBar { color: white; background-color: #1A004A ;} QMenuBar::item { spacing: 3px; padding: 3px 10px;background: transparent;}QMenuBar::item:selected {  color: black;   background: #FFFFFF;}QMenuBar::item:pressed {   background: #FFFFFF;}")).append(
//                //QString("QToolBar { min-height: 64px; color: blue; background-color: QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #FFFFFF, stop: 1 #B2C1FF);}")).append(
//                QString("QToolBar { min-height: 64px; color: blue; background-color: white;}")).append(
//                QString("QTabWidget QWidget { background-color: white; } ")).append(
//                QString("QDockWidget { background-color: white; }"))
//                );




    //a.setStyleSheet("QMenuBar { background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 lightgray, stop:1 darkgray);}QMenuBar::item { spacing: 3px; padding: 1px 4px;background: transparent;border-radius: 4px;}QMenuBar::item:selected {     background: #a8a8a8;}QMenuBar::item:pressed {   background: #888888;}");
    return a.exec();
}
