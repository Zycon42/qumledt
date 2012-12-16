/**
 * @file attributemodel.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#include "operationmodel.h"

QVariant OperationModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        return QVariant(operationList[index.row()].getUmlText());
    }
    return QVariant();
}

int OperationModel::rowCount(const QModelIndex& parent) const {
    return operationList.count();
}

void OperationModel::insertRow(int row, const Operation& attr) {
    beginInsertRows(QModelIndex(), row, row);
    operationList.insert(row, attr);
    endInsertRows();
}

void OperationModel::insertRows(int row, const QList< Operation >& attrs) {
    beginInsertRows(QModelIndex(), row, row + attrs.count() - 1);
    for (int i = 0; i < attrs.size(); i++)
        operationList.insert(row + i, attrs[i]);
    endInsertRows();
}

void OperationModel::removeRow(int row) {
    beginRemoveRows(QModelIndex(), row, row);
    operationList.removeAt(row);
    endRemoveRows();
}

void OperationModel::removeRows(int row, int count) {
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; i++)
        operationList.removeAt(row);
    endRemoveRows();
}
