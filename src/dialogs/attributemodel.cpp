/**
 * @file attributemodel.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#include "attributemodel.h"

QVariant AttributeModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        return QVariant(attributeList[index.row()].getUmlText());
    }
    return QVariant();
}

int AttributeModel::rowCount(const QModelIndex& parent) const {
    return attributeList.count();
}

void AttributeModel::insertRow(int row, const Attribute& attr) {
    beginInsertRows(QModelIndex(), row, row);
    attributeList.insert(row, attr);
    endInsertRows();
}

void AttributeModel::insertRows(int row, const QList< Attribute >& attrs) {
    beginInsertRows(QModelIndex(), row, row + attrs.count() - 1);
    for (int i = 0; i < attrs.size(); i++)
        attributeList.insert(row + i, attrs[i]);
    endInsertRows();
}

void AttributeModel::removeRow(int row) {
    beginRemoveRows(QModelIndex(), row, row);
    attributeList.removeAt(row);
    endRemoveRows();
}

void AttributeModel::removeRows(int row, int count) {
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; i++)
        attributeList.removeAt(row);
    endRemoveRows();
}
