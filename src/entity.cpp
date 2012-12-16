/**
 * @file entity.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#include "entity.h"
#include "association.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QDebug>


Entity::Entity(qreal x, qreal y, int id) : _size(60, 60), _name("Class"), _id(id), _font(), resizingFlag(false), deleting(false) {
    setPos(x, y);

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges | QGraphicsItem::ItemIsFocusable);
}

Entity::~Entity() {
    deleting = true;
    for (int i = 0; i < associationList.size(); i++) {
        delete associationList[i];
    }
}

void Entity::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    paintBaseRect(painter);

    // draw line uder name
    painter->drawLine(0, nameSectionSize().height(), size().width(), nameSectionSize().height());

    int attributeSectionEnd = nameSectionSize().height() + attributeSectionSize().height();
    painter->drawLine(0, attributeSectionEnd, size().width(), attributeSectionEnd);

    if (isSelected())
        paintSelected(painter);

    _font = painter->font();
    painter->setPen(Qt::black);
}

void Entity::paintSelected(QPainter* painter) {
    // draw squares in corners
    painter->fillRect(0, 0, selectedIndicatorSize, selectedIndicatorSize, Qt::magenta);
    painter->fillRect(0, size().height() - selectedIndicatorSize, selectedIndicatorSize, selectedIndicatorSize, Qt::magenta);
    painter->fillRect(size().width() - selectedIndicatorSize, 0, selectedIndicatorSize, selectedIndicatorSize, Qt::magenta);

    // draw resize indicator in right bottom corner
    int x = size().width() - resizeIndicatorSize;
    int y = size().height() - resizeIndicatorSize;
    int step = resizeIndicatorSize / 3;
    while (x < size().width() && y < size().height()) {
        painter->drawLine(x, size().height(), size().width(), y);
        x += step;
        y += step;
    }
}

void Entity::paintBaseRect(QPainter* painter) {
    painter->fillRect(boundingRect(), Qt::yellow);
    painter->setPen(Qt::red);
    painter->drawRect(boundingRect());
}

QRectF Entity::boundingRect() const {
    return QRectF(0, 0, size().width(), size().height());
}

QSize Entity::textSizeRequired(const QString& text) const {
    return QFontMetrics(font()).size(0, text);
}

void Entity::setName(const QString& name) {
    _name = name;

    adjustSize();
    update();
}

void Entity::setSize(const QSizeF& size) {
    QSizeF newSize = size;
    // truncate given size to minimumSize
    if (newSize.width() < minimumSize().width())
        newSize.rwidth() = minimumSize().width();
    if (newSize.height() < minimumSize().height())
        newSize.rheight() = minimumSize().height();

    if (_size != newSize) {
        prepareGeometryChange();
        _size = newSize;
    }
}

QSizeF Entity::minimumSize() const {
    return QSizeF(
        qMax(nameSectionSize().width(), qMax(attributeSectionSize().width(), operationSectionSize().width())),
        nameSectionSize().height() + attributeSectionSize().height() + operationSectionSize().height()
    );
}

void Entity::adjustSize() {
    setSize(size());
}

void Entity::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QRect resizeIndicatorRect(
        QPoint(size().width() - resizeIndicatorSize, size().height() - resizeIndicatorSize),
        QPoint(size().width(), size().height())
    );
    resizingFlag = false;
    if (resizeIndicatorRect.contains(event->pos().toPoint())) {
        setCursor(QCursor(Qt::SizeFDiagCursor));
        resizingFlag = true;
    } else
        QGraphicsItem::mousePressEvent(event);
}

void Entity::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (resizingFlag) {
        setSize(QSizeF(event->pos().x(), event->pos().y()));
    } else
        QGraphicsItem::mouseMoveEvent(event);
}

void Entity::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    unsetCursor();
    QGraphicsItem::mouseReleaseEvent(event);
}

void Entity::registerAssociation(Association* assoc) {
    associationList.append(assoc);
}

void Entity::unregisterAssociation(Association* assoc) {
    // entity and association has circular reference when deleting entity assoc
    // will call this method to unregister but if we would remove assoc
    // from list then we corrupt loop in dtor
    if (!deleting) {
        associationList.removeOne(assoc);
    }
}

QVariant Entity::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value) {
    // When position changed notify Association
    if (change == ItemPositionHasChanged) {
        foreach (Association* assoc, associationList) {
            assoc->recomputeIntersectionPoints();
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

QLineF::IntersectType Entity::intersects(const QLineF& line, QVector< QPointF >& intersections) {
    int oldsize = intersections.size();
    QRectF rect = mapRectToScene(boundingRect());

    QPointF intersection;
    if (line.intersect(QLineF(rect.topLeft(), rect.bottomLeft()), &intersection) == QLineF::BoundedIntersection)
        intersections.append(intersection);
    if (line.intersect(QLineF(rect.topLeft(), rect.topRight()), &intersection) == QLineF::BoundedIntersection)
        intersections.append(intersection);
    if (line.intersect(QLineF(rect.topRight(), rect.bottomRight()), &intersection) == QLineF::BoundedIntersection)
        intersections.append(intersection);
    if (line.intersect(QLineF(rect.bottomLeft(), rect.bottomRight()), &intersection) == QLineF::BoundedIntersection)
        intersections.append(intersection);

    if (intersections.size() == oldsize)
        return QLineF::NoIntersection;
    else
        return QLineF::BoundedIntersection;
}

QLineF Entity::boundingLineAt(const QPointF& p) {
    QRectF rect = mapRectToScene(boundingRect());

    QLineF rl(rect.topLeft(), rect.bottomLeft());
    if (pointOnLine(rl, p))
        return rl;

    QLineF tl(rect.topLeft(), rect.topRight());
    if (pointOnLine(tl, p))
        return tl;

    QLineF ll(rect.topRight(), rect.bottomRight());
    if (pointOnLine(ll, p))
        return ll;

    QLineF bl(rect.bottomLeft(), rect.bottomRight());
    if (pointOnLine(bl, p))
        return bl;

    return QLineF();
}

bool Entity::pointOnLine(const QLineF& line, const QPointF& point) const {
    QLineF normalLine = line.normalVector();
    QPointF normal = normalLine.p2() - normalLine.p1();
    qreal cInv = line.p1().x() * normal.x() + line.p1().y() * normal.y();
    return (point.x() * normal.x() + point.y() * normal.y()) == cInv;
}


void Entity::beginSerialization(QXmlStreamWriter& writer, const QString& entityType) const {
    writer.writeStartElement("entity");
    writer.writeAttribute("type", entityType);

    writer.writeTextElement("id", QString::number(id()));
    writer.writeTextElement("name", name());
    writer.writeTextElement("xpos", QString::number(pos().x()));
    writer.writeTextElement("ypos", QString::number(pos().y()));
    writer.writeTextElement("width", QString::number(size().width()));
    writer.writeTextElement("height", QString::number(size().height()));
}

void Entity::endSerialization(QXmlStreamWriter& writer) const {
    writer.writeEndElement();
}

Entity::DeserializationHelperOutput Entity::deserializationHelper(QXmlStreamReader& reader) {
    Entity::DeserializationHelperOutput out;
    bool idFlag = false, nameFlag = false, xposFlag = false, yposFlag = false, widthFlag = false, heightFlag = false;
    // read inner elements
    while (reader.readNextStartElement()) {
        if (reader.name() == "id") {
            if (idFlag)
                reader.raiseError("Multiple entity id");
            else {
                bool ok;
                out.id = reader.readElementText().toInt(&ok);
                if (!ok)
                    reader.raiseError("Entity id has to be integer");
                idFlag = true;
            }
        } else if (reader.name() == "name") {
            if (nameFlag)
                reader.raiseError("Multiple entity name");
            else {
                out.name = reader.readElementText();
                nameFlag = true;
            }
        } else if (reader.name() == "xpos") {
            if (xposFlag)
                reader.raiseError("Multiple entity xpos");
            else {
                bool ok;
                out.pos.rx() = reader.readElementText().toDouble(&ok);
                if (!ok)
                    reader.raiseError("Entity x coord has to be double");
                xposFlag = true;
            }
        } else if (reader.name() == "ypos") {
            if (yposFlag)
                reader.raiseError("Multiple entity ypos");
            else {
                bool ok;
                out.pos.ry() = reader.readElementText().toDouble(&ok);
                if (!ok)
                    reader.raiseError("Entity y coord has to be double");
                yposFlag = true;
            }
        } else if (reader.name() == "width") {
            if (widthFlag)
                reader.raiseError("Multiple entity width");
            else {
                bool ok;
                out.size.rwidth() = reader.readElementText().toDouble(&ok);
                if (!ok)
                    reader.raiseError("Entity width has to be double");
                widthFlag = true;
            }
        } else if (reader.name() == "height") {
            if (heightFlag)
                reader.raiseError("Multiple entity height");
            else {
                bool ok;
                out.size.rheight() = reader.readElementText().toDouble(&ok);
                if (!ok)
                    reader.raiseError("Entity height has to be double");
                heightFlag = true;
            }
        } else
            break;
    }
    if (!idFlag || !nameFlag || !xposFlag || !yposFlag || !widthFlag || !heightFlag)
        reader.raiseError("Enity attribute missing");

    return out;
}
