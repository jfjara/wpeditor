#include "waypointlistmodel.h"
#include <QDebug>

WayPointListModel::WayPointListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void WayPointListModel::addWP(WayPoint* wp)
{
    beginResetModel();
    listaWP.push_back(wp);
    insertRows(0, listaWP.length());
    endResetModel();
}

int WayPointListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return listaWP.size();

}

QVariant WayPointListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= listaWP.size()) return QVariant();
    //qDebug() << "indice : " << index.row() << " - valor : " << listaWP[index.row()]->toString();
    return QVariant(listaWP[index.row()]->toString());
}


bool WayPointListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0 || count < 0) {
           return false;
       }

       if (count == 0) {
           return true;
       }

       if (row > rowCount()) {
           row = rowCount();
       }

       beginInsertRows(QModelIndex(), row, row+count-1);
       endInsertRows();
       dataChanged(createIndex(0, 0), createIndex(rowCount()-1, 0));
       return true;
}
