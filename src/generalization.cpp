/**
 * @file generalization.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#include "generalization.h"
#include <QPainter>
#include "diagramscene.h"

const qreal Generalization::arrowLength = 12.0;
const qreal Generalization::arrowAngle = 37.0;

void Generalization::intersectionPointsChanged() {
}

Generalization::Generalization(Entity* A, Entity* B) : Association(A, B) {
    recomputeIntersectionPoints();
}

void Generalization::paintEndB(QPainter* painter, const QLineF& segment) {
    QPointF v1 = QLineF::fromPolar(arrowLength, segment.angle() + arrowAngle).p2();
    QPointF end1 = v1 + segment.p1();

    QPointF v2 = QLineF::fromPolar(arrowLength, segment.angle() + -arrowAngle).p2();
    QPointF end2 = v2 + segment.p1();

    QVector<QPointF> points;
    points << end1 << end2 << segment.p1();

    QBrush oldBrush = painter->brush();
    QBrush brush(QColor(255, 255, 110));
    painter->setBrush(brush);
    painter->drawPolygon(QPolygonF(points));

    painter->setBrush(oldBrush);
}

void Generalization::serialize(QXmlStreamWriter& writer) const {
    beginSerialization(writer, "generalization");
    endSerialization(writer);
}

DiagramItem* Generalization::deserialize(QXmlStreamReader& reader, DiagramScene* scene) {
    Association::DeserializationHelperOutput hep = Association::deserializationHelper(reader);
    Generalization* gen = new Generalization(scene->entityById(hep.entityA), scene->entityById(hep.entityB));
    gen->waypointList = hep.waypoints;
    return gen;
}
