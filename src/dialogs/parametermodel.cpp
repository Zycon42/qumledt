/**
 * @file parametermodel.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#include "parametermodel.h"

#include <QStringList>

QVariant ParameterModel::data(const QModelIndex& index, int role) const {
    if (index.row() >= rowCount() || index.column() >= columnCount())
        return QVariant();

    if (role == Qt::DisplayRole) {
        Operation::Parameter param = parameterList[index.row()];
        if (index.column() == 0)
            return QVariant(param.name);
        else
            return QVariant(param.type);
    }
    return QVariant();
}

QVariant ParameterModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section == 0)
            return QVariant(QString::fromUtf8("Název"));
        else if (section == 1)
            return QVariant(QString::fromUtf8("Typ"));
        else
            return QVariant();
    }
    return QVariant();
}

int ParameterModel::rowCount(const QModelIndex& parent) const {
    return parameterList.count();
}

int ParameterModel::columnCount(const QModelIndex& parent) const {
    return 2;
}

QModelIndex ParameterModel::parent(const QModelIndex& child) const {
    return QModelIndex();
}

QModelIndex ParameterModel::index(int row, int column, const QModelIndex& parent) const {
    return hasIndex(row, column, parent) ? createIndex(row, column, 0) : QModelIndex();
}

Qt::ItemFlags ParameterModel::flags(const QModelIndex& index) const {
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool ParameterModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (role == Qt::EditRole && !value.toString().isEmpty()) {
        QString val = value.toString();
        if (index.column() == 0)
            parameterList[index.row()].name = val;
        else if (index.column() == 1)
            parameterList[index.row()].type = val;
        else
            return false;
        return true;
    }
    return false;
}

bool ParameterModel::removeRows(int row, int count, const QModelIndex& parent) {
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; i++)
        parameterList.removeAt(row);
    endRemoveRows();
    return true;
}

bool ParameterModel::insertRows(int row, int count, const Operation::Parameter& p, const QModelIndex& parent) {
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; i++)
        parameterList.insert(row + i, p);
    endInsertRows();
    return true;
}

void ParameterModel::setParameters(const QList< Operation::Parameter >& p) {
    removeRows(0, rowCount());
    beginInsertRows(QModelIndex(), 0, p.count() - 1);
    parameterList = p;
    endInsertRows();
}
