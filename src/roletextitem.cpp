/**
 * @file roletextitem.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#include "roletextitem.h"

#include <QFont>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

RoleTextItem::RoleTextItem(const QString& text, qreal trans, qreal rot, const QLineF* origin, QGraphicsItem* parent, QGraphicsScene* scene)
    : QGraphicsTextItem(text, parent, scene), _translation(trans), _rotation(rot), _origin(origin) {

    QFont font = this->font();
    font.setPointSize(font.pointSize() * 0.8);
    setFont(font);
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | ItemSendsGeometryChanges);
}

void RoleTextItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    QPointF pos = this->pos() + boundingRect().center();
    QLineF line(_origin->p1(), pos);
    _rotation = _origin->angleTo(line);
    _translation = line.length();


    QGraphicsTextItem::mouseMoveEvent(event);
}
