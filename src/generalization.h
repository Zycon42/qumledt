/**
 * @file generalization.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _GENERALIZATION_H_
#define _GENERALIZATION_H_

#include "association.h"

class DiagramScene;

class Generalization : public Association
{
Q_OBJECT
Q_INTERFACES(QGraphicsItem)
public:
    Generalization(Entity* A, Entity* B);

    virtual BaseDialog* settingsDialog() { return NULL; }
    virtual void serialize(QXmlStreamWriter& writer) const;

    static DiagramItem* deserialize(QXmlStreamReader& reader, DiagramScene* scene);
protected:
    virtual void intersectionPointsChanged();
    virtual void paintEndB(QPainter* painter, const QLineF& segment);
private:
    static const qreal arrowLength;
    static const qreal arrowAngle;
};

#endif // _GENERALIZATION_H_
