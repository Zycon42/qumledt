/**
 * @file attributemodel.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _ATTRIBUTEMODEL_H_
#define _ATTRIBUTEMODEL_H_

#include "../members.h"

#include <QAbstractListModel>

class AttributeModel : public QAbstractListModel
{
Q_OBJECT
public:
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;

    void insertRow(int row, const Attribute& attr);
    void insertRows(int row, const QList<Attribute>& attrs);
    void removeRow(int row);
    void removeRows(int row, int count);

    const Attribute& attribute(int row) const { return attributeList[row]; }
    const QList<Attribute>& attributes() const { return attributeList; }
private:
    QList<Attribute> attributeList;
};

#endif // _ATTRIBUTEMODEL_H_
