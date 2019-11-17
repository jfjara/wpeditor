#ifndef WAYPOINTLISTMODEL_H
#define WAYPOINTLISTMODEL_H

#include <QAbstractListModel>
#include <QMap>
#include <QList>
#include "waypoint.h"

class WayPointListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit WayPointListModel(QObject *parent = 0);

    QList<WayPoint*> listaWP;


    void addWP(WayPoint* wp);

    // Header:
    //QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

private:
};

#endif // WAYPOINTLISTMODEL_H
