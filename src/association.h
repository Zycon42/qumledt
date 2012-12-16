/**
 * @file association.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _ASSOCIATION_H_
#define _ASSOCIATION_H_

#include "diagramitem.h"
#include "entity.h"
#include "members.h"

#include <QPen>

class Association : public DiagramItem
{
Q_OBJECT
Q_INTERFACES(QGraphicsItem)
public:
    struct Role {
        Role() : visibility(Member::Public) { }
        explicit Role(const QString& name, Member::Visibility visibility = Member::Public)
            : name(name), visibility(visibility) { }
        explicit Role(const QString& multiplicity) : multiplicity(multiplicity), visibility(Member::Public) { }
        Role(const QString& name, const QString& multiplicity, Member::Visibility visibility = Member::Public)
            : name(name), multiplicity(multiplicity), visibility(visibility) { }
        QString name;
        QString multiplicity;
        Member::Visibility visibility;
    };

    Association(Entity* A, Entity* B);

    virtual ~Association();

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;

    void recomputeIntersectionPoints();

    void addWaypoint(qreal distance);
protected:
    struct DeserializationHelperOutput {
        int entityA;
        int entityB;
        QVector<QPointF> waypoints;
    };
    static DeserializationHelperOutput deserializationHelper(QXmlStreamReader& reader);

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event);

    const QLineF* segmentEntityA() const { return &firstSegment; }
    const QLineF* segmentEntityB() const { return &lastSegment; }

    virtual void intersectionPointsChanged() = 0;
    virtual void paintEndA(QPainter* painter, const QLineF& segment);
    virtual void paintEndB(QPainter* painter, const QLineF& segment);

    virtual void fillMenu(QMenu* menu, const QPointF& pos);

    void beginSerialization(QXmlStreamWriter& writer, const QString& assocType) const;
    void endSerialization(QXmlStreamWriter& writer) const;

    QVector<QPointF> waypointList;
protected slots:
    void createWaypointViaMenu();
    void resetWaypoints();
private:
    static const int waypointSize = 10;

    /**
     * Returns pen stroked path
     */
    QPainterPath shapeFromPath(const QPainterPath& path) const;
    /**
     * Maps point to line.
     * @param line where resulting point will lie
     * @param point point not on line
     * @return point that lies on line and has shortest distance to given point
     */
    QPointF mapPointToLine(const QLineF& line, const QPointF& point) const;
    /**
     * Gets point distance from line start
     * @param line
     * @param point must be on line! else result undefined
     * @return distance
     */
    qreal pointDistanceOnLine(const QLineF& line, const QPointF& point) const;
    /**
     * Returns id of segment in which point lies
     * @param point
     * @return id of segment -1 if not in any segment
     */
    int segmentAt(const QPointF& point);
    /**
     * Returns waypoint id of waypoint located at given point
     * @param point given point
     * @return waypoint id -1 if point in any waypoint
     */
    int waypointAt(const QPointF& point);

    Entity* entityA;
    Entity* entityB;

    QPointF intPointA;
    QPointF intPointB;

    QLineF firstSegment;
    QLineF lastSegment;

    QPen pen;
    int wpMoveId;
};

#endif // _ASSOCIATION_H_
