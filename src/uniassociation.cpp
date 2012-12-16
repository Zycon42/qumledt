/**
 * @file uniassociation.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#include "uniassociation.h"
#include "dialogs/uniassociationdialog.h"
#include "diagramscene.h"

#include <QPainter>

const qreal UniAssociation::defaultTranslation = 15.0;
const qreal UniAssociation::defaultRoleRotation = 40.0;
const qreal UniAssociation::defaultMultiplicityRotation = -40.0;
const qreal UniAssociation::arrowLength = 9.0;
const qreal UniAssociation::arrowAngle = 32.0;


UniAssociation::UniAssociation(Entity* A, Entity* B): Association(A, B) {
    recomputeIntersectionPoints();
}

void UniAssociation::setRole(const Association::Role& role) {
    //items.clear();
    QList< QSharedPointer< RoleTextItem > > newItems;

    if (!role.name.isEmpty()) {
        qreal trans = !_role.name.isEmpty() ? items[0]->translation() : defaultTranslation;
        qreal rot = !_role.name.isEmpty() ? items[0]->translation() : defaultRoleRotation;
        QSharedPointer< RoleTextItem > item = createTextItem(Member::visibilityText(role.visibility) + " " + role.name,
                    trans, rot);
        newItems.append(item);
    }

    if (!role.multiplicity.isEmpty()) {
        int oldItemIndex = _role.name.isEmpty() ? 0 : 1;
        qreal trans = !_role.name.isEmpty() ? items[oldItemIndex]->translation() : defaultTranslation;
        qreal rot = !_role.name.isEmpty() ? items[oldItemIndex]->translation() : defaultMultiplicityRotation;
        QSharedPointer< RoleTextItem > item = createTextItem(role.multiplicity, trans, rot);

        newItems.append(item);
    }

    items.clear();
    items.append(newItems);

    intersectionPointsChanged();

    _role = role;
}

void UniAssociation::intersectionPointsChanged() {
    QList< QSharedPointer< RoleTextItem > >::const_iterator item = items.begin();
    while (item != items.end()) {
        QPointF v = QLineF::fromPolar((*item)->translation(),
            (*item)->origin()->angle() + (*item)->rotation()).p2();

        QPointF center = v + (*item)->origin()->p1();
        (*item)->setPos(center - (*item)->boundingRect().center());

        ++item;
    }
}

QSharedPointer<RoleTextItem> UniAssociation::createTextItem(const QString& text, qreal translation, qreal rotation) {
    RoleTextItem* textItem = new RoleTextItem(text, translation, rotation, segmentEntityB(), this);

    return QSharedPointer<RoleTextItem>(textItem);
    //items.append(QSharedPointer<RoleTextItem>(textItem));
}

void UniAssociation::paintEndB(QPainter* painter, const QLineF& segment) {
    QPointF v1 = QLineF::fromPolar(arrowLength, segment.angle() + arrowAngle).p2();
    QPointF end1 = v1 + segment.p1();
    painter->drawLine(QLineF(segment.p1(), end1));

    QPointF v2 = QLineF::fromPolar(arrowLength, segment.angle() + -arrowAngle).p2();
    QPointF end2 = v2 + segment.p1();
    painter->drawLine(QLineF(segment.p1(), end2));
}

BaseDialog* UniAssociation::settingsDialog() {
    return new UniAssociationDialog(this);
}

void UniAssociation::serialize(QXmlStreamWriter& writer) const {
    beginSerialization(writer, "unidirection");

    if (items.count()) {
        writer.writeStartElement("role");

        if (!_role.name.isEmpty()) {
            writer.writeStartElement("name");
            writer.writeAttribute("trans", QString::number(items[0]->translation()));
            writer.writeAttribute("rot", QString::number(items[0]->rotation()));

            writer.writeCharacters(_role.name);

            writer.writeEndElement();
        }
        if (!_role.multiplicity.isEmpty()) {
            writer.writeStartElement("multiplicity");
            int index = _role.name.isEmpty() ? 0 : 1;
            writer.writeAttribute("trans", QString::number(items[index]->translation()));
            writer.writeAttribute("rot", QString::number(items[index]->rotation()));

            writer.writeCharacters(_role.multiplicity);

            writer.writeEndElement();
        }

        writer.writeTextElement("visibility", Member::visibilityText(_role.visibility));

        writer.writeEndElement();
    }

    endSerialization(writer);
}

DiagramItem* UniAssociation::deserialize(QXmlStreamReader& reader, DiagramScene* scene) {
    Association::DeserializationHelperOutput hep = Association::deserializationHelper(reader);
    UniAssociation* assoc = new UniAssociation(scene->entityById(hep.entityA), scene->entityById(hep.entityB));
    assoc->waypointList = hep.waypoints;

    if (reader.tokenType() == QXmlStreamReader::EndElement)
        return assoc;

    if (reader.name() == "role") {
        Association::Role role;
        QSharedPointer<RoleTextItem> nameItem, multiplicityItem;
        while (reader.readNextStartElement()) {
            if (reader.name() == "name") {
                nameItem = assoc->createTextItem(reader.readElementText(), reader.attributes().value("", "trans").toString().toDouble(),
                    reader.attributes().value("", "rot").toString().toDouble());
                role.name = reader.readElementText();
            } else if (reader.name() == "multiplicity") {
                multiplicityItem = assoc->createTextItem(reader.readElementText(), reader.attributes().value("", "trans").toString().toDouble(),
                                          reader.attributes().value("", "rot").toString().toDouble());
                role.multiplicity = reader.readElementText();
            } else if (reader.name() == "visibility") {
                role.visibility = Member::visibilityFromText(reader.readElementText());
            }
        }

        assoc->_role = role;
        if (!nameItem.isNull())
            assoc->items.append(nameItem);
        if (!multiplicityItem.isNull())
            assoc->items.append(multiplicityItem);
    }

    return assoc;
}
