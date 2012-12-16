/**
 * @file aggregation.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _AGGREGATION_H_
#define _AGGREGATION_H_

#include "biassociation.h"

class Aggregation : public BiAssociation
{
Q_OBJECT
Q_INTERFACES(QGraphicsItem)
public:
    Aggregation(Entity* A, Entity* B);

    static DiagramItem* deserialize(QXmlStreamReader& reader, DiagramScene* scene);
protected:
    virtual void paintEndA(QPainter* painter, const QLineF& segment);
    virtual void paintEndB(QPainter* painter, const QLineF& segment);

    virtual QString typeString() const { return "aggregation"; }

    QBrush brush;
private:
    static const qreal arrowLength;
    static const qreal arrowAngle;
    static const qreal diamondLength;
    static const qreal diamondWidth;
};

#endif // _AGGREGATION_H_
