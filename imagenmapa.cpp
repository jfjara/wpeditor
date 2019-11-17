#include "imagenmapa.h"
#include <QDebug>
#include "log.h"

ImagenMapa::ImagenMapa()
{
}

void ImagenMapa::desplazar(int dx, int dy)
{    
    punto.setX(punto.x() + dx);
    punto.setY(punto.y() + dy);
}

void ImagenMapa::setImagen(QString pathFichero)
{
    this->pathFichero = pathFichero;
    imagen = new QImage(pathFichero);
//    Log::getInstancia()->info("Tamaño de la imagen -> w: " + QString::number(imagen->width()) + ", h: " + QString::number(imagen->height()));
    widthOriginal = imagen->width();
    heigthOriginal = imagen->height();
}

void ImagenMapa::pintar(QPainter* painter)
{
    painter->save();
    painter->drawImage(punto, HerramientaConversorCoordenadas::getInstancia()->imagenReescalada); 
    painter->restore();
}

QString ImagenMapa::rellenarLineaPlantilla(QString linea)
{
    QString resultado = linea;
    if (resultado.contains("[xsize]")) {
        resultado = resultado.replace("[xsize]", QString::number(widthOriginal));
    } else if (resultado.contains("[ysize]")) {
        resultado = resultado.replace("[ysize]",  QString::number(heigthOriginal));
    } else if (resultado.contains("[north]")) {
        resultado = resultado.replace("[north]",  QString::number(latitud1, 'f', 6));
    } else if (resultado.contains("[south]")) {
        resultado = resultado.replace("[south]",  QString::number(latitud2, 'f', 6));
    } else if (resultado.contains("[east]")) {
        resultado = resultado.replace("[east]",  QString::number(longitud2, 'f', 6));
    } else if (resultado.contains("[west]")) {
        resultado = resultado.replace("[west]",  QString::number(longitud1, 'f', 6));
    }
    return resultado;
}

QString ImagenMapa::getNombreFicheroInfo()
{
    QStringList cachos = pathFichero.split("/");
    QString nombreImagen = cachos[cachos.size() - 1];
    QString nombre = nombreImagen.split(".")[0];
    return nombre + ".info";
}

bool ImagenMapa::exportarAHorizon(QString rutaHorizon)
{
    QString nombreInfo = rutaHorizon + "\\" + getNombreFicheroInfo();
    QFile file(nombreInfo);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream streamWrite(&file);

        QFile templateFile("./templates/plantillaMapa.txt");
        if (templateFile.open(QIODevice::ReadOnly))
        {
            QTextStream stream( &templateFile );
            while (!stream.atEnd()) {
                QString linea = stream.readLine();
                linea = rellenarLineaPlantilla(linea);
                streamWrite << linea << endl;
            }
            templateFile.close();
            file.close();
        } else {
           return false;
        }
    } else {
        return false;
    }

    QStringList cachos = pathFichero.split("/");
    QString nombreImagen = rutaHorizon + "\\" + cachos[cachos.size() - 1];
    QFile::copy(pathFichero, nombreImagen);
    return true;
}
