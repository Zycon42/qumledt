/**
 * @file association.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#include "association.h"
#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QMenu>

#include <limits>

Association::Association(Entity* A, Entity* B) : entityA(A), entityB(B), pen(Qt::red), wpMoveId(-1) {
    // create intersectionPoints
    waypointList.resize(2);

    entityA->registerAssociation(this);
    entityB->registerAssociation(this);

    setZValue(-1.0);

    setAcceptHoverEvents(true);
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
}

Association::~Association() {
    entityA->unregisterAssociation(this);
    entityB->unregisterAssociation(this);
}

void Association::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    painter->setRenderHint(QPainter::Antialiasing, true);

    painter->setPen(pen);
    painter->drawPolyline(QPolygonF(waypointList));

    if (isSelected()) {
        foreach (const QPointF& waypoint, waypointList) {
            painter->fillRect(waypoint.x() - waypointSize / 2, waypoint.y()  - waypointSize / 2, waypointSize, waypointSize, Qt::magenta);
        }
    }

    paintEndA(painter, firstSegment);
    paintEndB(painter, lastSegment);
}

QRectF Association::boundingRect() const {
    return shape().controlPointRect();
}

QPainterPath Association::shape() const {
    bool first = true;
    QPainterPath path;
    foreach (const QPointF& waypoint, waypointList) {
        if (first)
            path.moveTo(waypoint);
        else
            path.lineTo(waypoint);
        first = false;
    }

    return shapeFromPath(path);
}

QPainterPath Association::shapeFromPath(const QPainterPath& path) const {
    QPainterPathStroker ps;
    ps.setCapStyle(pen.capStyle());
    ps.setWidth(pen.widthF() + 20);
    ps.setJoinStyle(pen.joinStyle());
    ps.setMiterLimit(pen.miterLimit());
    QPainterPath p = ps.createStroke(path);
    p.addPath(path);
    return p;
}

QPointF Association::mapPointToLine(const QLineF& line, const QPointF& point) const {
    QPointF vec = line.p2() - line.p1();
    qreal c = -(vec.x() * point.x() + vec.y() * point.y());
    QLineF perpendicular(point, QPointF(0, vec.y() ? -c / vec.y() : 1.0));
    QPointF in;
    //qDebug() << perpendicular << vec;
    QLineF::IntersectType res = perpendicular.intersect(line, &in);
    if (res == QLineF::NoIntersection)
        return QPointF();
    else if (res == QLineF::UnboundedIntersection) {
        qreal t = (in.x() - line.p1().x()) / line.dx();
        if (t < 0.0)
            return line.pointAt(0.0);
        else if (t > 1.0)
            return line.pointAt(1.0);
        else
            return in;
    } else
        return in;
}

qreal Association::pointDistanceOnLine(const QLineF& line, const QPointF& point) const {
    QPointF vec = line.p2() - line.p1();
    return ((point.x() - line.p1().x()) / vec.x()) * line.length();
}

int Association::segmentAt(const QPointF& point) {
    for (int i = 1; i < waypointList.size(); i++) {
        // get line path
        QPainterPath linePath;
        linePath.moveTo(waypointList[i - 1]);
        linePath.lineTo(waypointList[i]);

        if (shapeFromPath(linePath).contains(point)) {
            return i - 1;   // segment id == waypoint id - 1
        }
    }
    return -1;
}

int Association::waypointAt(const QPointF& point) {
    int seg = segmentAt(point);
    if (seg == -1)
        return -1;
    QLineF line(waypointList[seg], waypointList[seg + 1]);
    qreal distA = pointDistanceOnLine(line, mapPointToLine(line, point));
    qreal distB = line.length() - distA;
    if (distA < 10.0)
        return seg;
    else if (distB < 10.0)
        return seg + 1;
    else
        return -1;
}

void Association::resetWaypoints() {
    waypointList.clear();
    waypointList.resize(2);
    intPointA = QPointF();
    intPointB = QPointF();
    recomputeIntersectionPoints();
}

void Association::recomputeIntersectionPoints() {
    prepareGeometryChange();

    QRectF rectA = entityA->mapRectToScene(entityA->boundingRect());
    QRectF rectB = entityB->mapRectToScene(entityB->boundingRect());
    QPointF centerA = intPointA.isNull() ? rectA.center() : entityA->mapToScene(intPointA);
    QPointF centerB = intPointB.isNull() ? rectB.center() : entityB->mapToScene(intPointB);

    QVector<QPointF> insA, insB;
    QLineF baseLine(centerA, centerB);
    entityA->intersects(waypointList.count() > 2 ? QLineF(centerA, waypointList[1]) : baseLine, insA);
    entityB->intersects(waypointList.count() > 2 ? QLineF(waypointList[waypointList.count() - 2], centerB) : baseLine, insB);

    qreal minlen = std::numeric_limits<qreal>::max();
    for (int i = 0; i < insA.count(); i++) {
        for (int j = 0; j < insB.count(); j++) {
            qreal len = QLineF(insA[i], insB[j]).length();
            if (len < minlen) {
                waypointList.first() = insA[i];
                waypointList.last() = insB[j];
                minlen = len;
            }
        }
    }

    firstSegment.setPoints(waypointList[0], waypointList[1]);
    lastSegment.setPoints(waypointList.last(), waypointList[waypointList.count() - 2]);

    intersectionPointsChanged();
}

void Association::addWaypoint(qreal distance) {
    for (int i = 1; i < waypointList.size(); i++) {
        QLineF line(waypointList[i - 1], waypointList[i]);
        if (distance < line.length()) {
            waypointList.insert(i, line.pointAt(distance / line.length()));
            break;
        }
        distance -= line.length();
    }
    update();
}

void Association::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    int wp = waypointAt(event->pos());
    if (wp != -1)
        wpMoveId = wp;

    QGraphicsItem::mousePressEvent(event);
}

void Association::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    //qDebug() << "Association::mouseMoveEvent";
    if (wpMoveId != -1) {
        prepareGeometryChange();
        if (wpMoveId == 0 || wpMoveId == waypointList.count() - 1) {
            Entity* ent = wpMoveId == 0 ? entityA : entityB;
            QLineF l = ent->boundingLineAt(waypointList[wpMoveId]);
            QPointF p = mapPointToLine(l, event->pos());
            if (!p.isNull() && ent->boundingRect().contains(ent->mapFromScene(p))) {
                if (wpMoveId == 0)
                    intPointA = ent->mapFromScene(p);
                else
                    intPointB = ent->mapFromScene(p);
            }
        } else
            waypointList[wpMoveId] = event->pos();
        recomputeIntersectionPoints();
    }

    QGraphicsItem::mouseMoveEvent(event);
}

void Association::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (wpMoveId != -1)
        wpMoveId = -1;
    QGraphicsItem::mouseReleaseEvent(event);
}

void Association::hoverMoveEvent(QGraphicsSceneHoverEvent* event) {
    if (!isSelected())
        return;

    if (waypointAt(event->pos()) != -1)
        setCursor(QCursor(Qt::SizeAllCursor));
    else
        unsetCursor();
    QGraphicsItem::hoverMoveEvent(event);
}

QVariant Association::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value) {
    if (change == ItemSelectedHasChanged) {
        if (value.toBool())
            setZValue(1.0);     // When item is selected move to foreground
        else
            setZValue(-1.0);    // when deselected move to background
    }
    return QGraphicsItem::itemChange(change, value);
}

void Association::paintEndA(QPainter* painter, const QLineF& segment) {
    Q_UNUSED(segment)
    Q_UNUSED(painter)
}

void Association::paintEndB(QPainter* painter, const QLineF& segment) {
    Q_UNUSED(segment)
    Q_UNUSED(painter)
}

void Association::fillMenu(QMenu* menu, const QPointF& pos) {
    QAction* act = menu->addAction(QIcon(), QString::fromUtf8("Přidat bod"), this, SLOT(createWaypointViaMenu()));
    act->setIconVisibleInMenu(true);
    act->setData(QVariant(pos));

    menu->addAction(QIcon(), QString::fromUtf8("Resetovat čáru"), this, SLOT(resetWaypoints()));

    DiagramItem::fillMenu(menu, pos);
}

void Association::createWaypointViaMenu() {
    QAction* act = qobject_cast<QAction*>(sender());
    Q_ASSERT(act != NULL);

    QPointF pos = act->data().toPointF();
    Q_ASSERT(!pos.isNull());

    //ignore double click if we are above waypoint
    if (waypointAt(pos) != -1)
        return;

    qreal distance = 0.0;
    for (int i = 1; i < waypointList.size(); i++) {
        QLineF line(waypointList[i - 1], waypointList[i]);
        // get line path
        QPainterPath linePath;
        linePath.moveTo(waypointList[i - 1]);
        linePath.lineTo(waypointList[i]);

        if (shapeFromPath(linePath).contains(pos)) {
            QPointF in = mapPointToLine(line, pos);
            qreal dist = pointDistanceOnLine(line, in);
            distance += dist;
            addWaypoint(distance);
            setSelected(true);
            break;
        }
        distance += line.length();
    }
}

void Association::beginSerialization(QXmlStreamWriter& writer, const QString& assocType) const {
    writer.writeStartElement("association");
    writer.writeAttribute("type", assocType);

    writer.writeTextElement("entityA", QString::number(entityA->id()));
    writer.writeTextElement("entityB", QString::number(entityB->id()));

    writer.writeStartElement("wpList");
    foreach (const QPointF& p, waypointList) {
        writer.writeStartElement("wp");
        writer.writeAttribute("x", QString::number(p.x()));
        writer.writeAttribute("y", QString::number(p.y()));
        writer.writeEndElement();
    }
    writer.writeEndElement();
}

void Association::endSerialization(QXmlStreamWriter& writer) const {
    writer.writeEndElement();
}

Association::DeserializationHelperOutput Association::deserializationHelper(QXmlStreamReader& reader) {
    Association::DeserializationHelperOutput out;
    while (reader.readNextStartElement()) {
        //qDebug() << reader.name();
        if (reader.name() == "entityA")
            out.entityA = reader.readElementText().toInt();
        else if (reader.name() == "entityB")
            out.entityB = reader.readElementText().toInt();
        else if (reader.name() == "wpList") {
            while (reader.readNextStartElement()) {
                if (reader.name() == "wp")
                    out.waypoints.append(QPointF(
                        reader.attributes().value("", "x").toString().toDouble(),
                        reader.attributes().value("", "y").toString().toDouble()
                    ));
                reader.readNext();
            }
        } else
            break;
    }
    return out;
}
