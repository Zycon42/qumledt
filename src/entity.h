/**
 * @file entity.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "diagramitem.h"

#include <QFont>

class Association;

/**
 * Diagram entity eq. Class, Interface
 * Handles moving, selecting, resizing and basic painting
 */
class Entity : public DiagramItem
{
Q_OBJECT
Q_INTERFACES(QGraphicsItem)
public:
    virtual ~Entity();
    /**
     * Constructs entity on given coordinates
     * @param x x scene coordinate
     * @param y y scene coordinate
     * @param id entity unique id
     */
    Entity(qreal x, qreal y, int id);

    int id() const { return _id; }

    /**
     * Drawing.
     * Reimplement this method to add name, attributes and operations painting
     */
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
    /**
     * Entity outer bounding rectangle
     * @return bounding rect
     */
    virtual QRectF boundingRect() const;

    /**
     * Sets new size.
     * @param size new size if some dimension is lesser than minimumSize its cliped to that value
     */
    void setSize(const QSizeF& size);
    /**
     * Gets entity size
     * @return entity size
     */
    const QSizeF& size() const { return _size; }

    /**
     * Sets entity name
     * @param name
     */
    void setName(const QString& name);
    /**
     * Gets entity name
     * @return entity name
     */
    const QString& name() const { return _name; }

    /**
     * Gets font used to draw text
     * @return font
     */
    const QFont& font() const { return _font; }

    /**
     * Minimal entity size
     * @return size
     */
    QSizeF minimumSize() const;

    /**
     * Resizes entity to fit minimum size,
     * so if (size < minimum size) size = minimum size
     */
    void adjustSize();

    /**
     * Registers association so entity can tell association about important events such as moving
     */
    void registerAssociation(Association* assoc);
    /**
     * Unregisters association
     */
    void unregisterAssociation(Association* assoc);

    /**
     * Intersection of Entity with line
     * @param line line to intersect
     * @param intersections appends to this vector intersection points
     * @return QLineF::BoundedIntersection if theres intersection QLineF::NoIntersection if theres none
     */
    QLineF::IntersectType intersects(const QLineF& line, QVector<QPointF>& intersections);

    QLineF boundingLineAt(const QPointF& p);
protected:
    struct DeserializationHelperOutput {
        int id;
        QString name;
        QPointF pos;
        QSizeF size;
    };
    static DeserializationHelperOutput deserializationHelper(QXmlStreamReader& reader);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

    void beginSerialization(QXmlStreamWriter& writer, const QString& entityType) const;
    void endSerialization(QXmlStreamWriter& writer) const;

    /**
     * Paint entity decoration when selected
     * @param painter QPainter used for painting
     */
    void paintSelected(QPainter* painter);
    /**
     * Paints entity base rect. Its yellow with red edges
     * @param painter QPainter used for painting
     */
    void paintBaseRect(QPainter* painter);

    /**
     * Size of name section
     * Implement this method in subclass
     */
    virtual QSize nameSectionSize() const = 0;
    /**
     * Size of attribute section
     * Implement this method in subclass
     */
    virtual QSize attributeSectionSize() const = 0;
    /**
     * Size of operation section
     * Implement this method in subclass
     */
    virtual QSize operationSectionSize() const = 0;

    /**
     * Returns size in pixels required for given text drawed with font()
     * @param text input text for metrics
     * @return size
     */
    QSize textSizeRequired(const QString& text) const;

    /**
     * Overriden QGraphicsItem::itemChange
     */
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);
private:
    bool pointOnLine(const QLineF& line, const QPointF& point) const;

    /// Size of corner rect shown when entity is selected
    static const int selectedIndicatorSize = 5;
    /// Size of corner resize indicator
    static const int resizeIndicatorSize = 12;

    QSizeF _size;
    QString _name;
    int _id;
    QFont _font;
    bool resizingFlag;

    /// registered associations
    QList<Association*> associationList;
    bool deleting;
};

#endif // _ENTITY_H_
