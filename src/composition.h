/**
 * @file composition.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _COMPOSITION_H_
#define _COMPOSITION_H_

#include "aggregation.h"

class Composition : public Aggregation
{
Q_OBJECT
Q_INTERFACES(QGraphicsItem)
public:
    Composition(Entity* A, Entity* B);

    static DiagramItem* deserialize(QXmlStreamReader& reader, DiagramScene* scene);
protected:
    virtual void paintEndA(QPainter* painter, const QLineF& segment);

    virtual QString typeString() const { return "composition"; }
};

#endif // _COMPOSITION_H_
