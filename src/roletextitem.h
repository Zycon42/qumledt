/**
 * @file roletextitem.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _ROLETEXTITEM_H_
#define _ROLETEXTITEM_H_

#include <QGraphicsTextItem>

class RoleTextItem : public QGraphicsTextItem
{
Q_INTERFACES(QGraphicsItem)
public:
    RoleTextItem(const QString& text, qreal trans, qreal rot, const QLineF* origin, QGraphicsItem* parent = 0, QGraphicsScene* scene = 0);

    qreal translation() const { return _translation; }
    qreal rotation() const { return _rotation; }
    const QLineF* origin() const { return _origin; }
protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
private:
    qreal _translation;
    qreal _rotation;
    const QLineF* _origin;
};

#endif // _ROLETEXTITEM_H_
