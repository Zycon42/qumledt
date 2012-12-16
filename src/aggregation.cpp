/**
 * @file aggregation.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#include "aggregation.h"
#include "diagramscene.h"
#include <QPainter>

const qreal Aggregation::arrowLength = 9.0;
const qreal Aggregation::arrowAngle = 32.0;
const qreal Aggregation::diamondLength = 17.0;
const qreal Aggregation::diamondWidth = 10.0;

Aggregation::Aggregation(Entity* A, Entity* B) : BiAssociation(A, B), brush(QColor(255, 255, 110)) {
}

void Aggregation::paintEndB(QPainter* painter, const QLineF& segment) {
    QPointF v1 = QLineF::fromPolar(arrowLength, segment.angle() + arrowAngle).p2();
    QPointF end1 = v1 + segment.p1();
    painter->drawLine(QLineF(segment.p1(), end1));

    QPointF v2 = QLineF::fromPolar(arrowLength, segment.angle() + -arrowAngle).p2();
    QPointF end2 = v2 + segment.p1();
    painter->drawLine(QLineF(segment.p1(), end2));
}

void Aggregation::paintEndA(QPainter* painter, const QLineF& segment) {
    QPointF center = segment.pointAt((diamondLength / 2) / segment.length());
    QLineF normalLine = segment.normalVector();
    normalLine.setLength(diamondWidth / 2);
    QPointF normal = normalLine.p2() - normalLine.p1();

    QVector<QPointF> points;
    points << segment.p1() << center + normal << segment.pointAt(diamondLength / segment.length()) << center - normal;

    QBrush oldBrush = painter->brush();
    painter->setBrush(brush);
    painter->drawPolygon(QPolygonF(points));

    painter->setBrush(oldBrush);
}

DiagramItem* Aggregation::deserialize(QXmlStreamReader& reader, DiagramScene* scene) {
    Association::DeserializationHelperOutput hep = Association::deserializationHelper(reader);
    Aggregation* assoc = new Aggregation(scene->entityById(hep.entityA), scene->entityById(hep.entityB));
    assoc->waypointList = hep.waypoints;

    deserializeRoles(reader, assoc);

    return assoc;
}

