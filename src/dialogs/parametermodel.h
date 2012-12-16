/**
 * @file parametermodel.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _PARAMETERMODEL_H_
#define _PARAMETERMODEL_H_

#include "../members.h"

#include <QAbstractItemModel>

class ParameterModel : public QAbstractItemModel
{
Q_OBJECT
public:
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    virtual QModelIndex parent(const QModelIndex& child) const;
    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;

    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

    virtual bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());
    virtual bool insertRows(int row, int count, const Operation::Parameter& p, const QModelIndex& parent = QModelIndex());

    const QList<Operation::Parameter>& parameters() const { return parameterList; }
    void setParameters(const QList<Operation::Parameter>& p);
private:
    QList<Operation::Parameter> parameterList;
};

#endif // _PARAMETERMODEL_H_
