/**
 * @file attributemodel.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _OPERATIONMODEL_H_
#define _OPERATIONMODEL_H_

#include "../members.h"

#include <QAbstractListModel>

class OperationModel : public QAbstractListModel
{
    Q_OBJECT
public:
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;

    void insertRow(int row, const Operation& attr);
    void insertRows(int row, const QList<Operation>& attrs);
    void removeRow(int row);
    void removeRows(int row, int count);

    const Operation& operation(int row) const { return operationList[row]; }
    const QList<Operation>& operations() const { return operationList; }
private:
    QList<Operation> operationList;
};

#endif // _OPERATIONMODEL_H_