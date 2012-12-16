/**
 * @file composition.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#include "composition.h"
#include "diagramscene.h"

#include <QPainter>

Composition::Composition(Entity* A, Entity* B): Aggregation(A, B) {
}

void Composition::paintEndA(QPainter* painter, const QLineF& segment) {
    brush = QBrush(painter->pen().color());
    Aggregation::paintEndA(painter, segment);
}

DiagramItem* Composition::deserialize(QXmlStreamReader& reader, DiagramScene* scene) {
    Association::DeserializationHelperOutput hep = Association::deserializationHelper(reader);
    Composition* assoc = new Composition(scene->entityById(hep.entityA), scene->entityById(hep.entityB));
    assoc->waypointList = hep.waypoints;

    deserializeRoles(reader, assoc);

    return assoc;
}
