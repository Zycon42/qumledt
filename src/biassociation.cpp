/**
 * @file biassociation.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#include "biassociation.h"
#include "dialogs/biassociationdialog.h"
#include "diagramscene.h"

#include <QGraphicsScene>
#include <QDebug>

const double BiAssociation::defaultTranslation = 15.0;
const double BiAssociation::defaultRoleRotation = 40.0;
const double BiAssociation::defaultMultiplicityRotation = -40.0;

BiAssociation::BiAssociation(Entity* A, Entity* B) : Association(A, B) {
    recomputeIntersectionPoints();
}

BiAssociation::~BiAssociation() {
}

void BiAssociation::setRoleA(const Association::Role& role) {
    //items.remove(EntityA);
    QList< QSharedPointer< RoleTextItem > > newItems;
    QMap< Entities, QSharedPointer< RoleTextItem > >::iterator oldItems = items.find(EntityA);

    if (!role.name.isEmpty()) {
        qreal trans = defaultTranslation, rot = defaultRoleRotation;
        if (oldItems != items.end() && !_roleA.name.isEmpty()) {
            QMap< Entities, QSharedPointer< RoleTextItem > >::iterator tmp = oldItems;
            if (!_roleA.multiplicity.isEmpty())
                ++tmp;

            trans = tmp.value()->translation();
            rot = tmp.value()->rotation();
        }

        QSharedPointer< RoleTextItem > item = createTextItem(Member::visibilityText(role.visibility) + " " + role.name,
                       trans, rot, EntityA);

        newItems.append(item);
    }

    if (!role.multiplicity.isEmpty()) {
        qreal trans = defaultTranslation, rot = defaultMultiplicityRotation;
        if (oldItems != items.end() && !_roleA.multiplicity.isEmpty()) {
            trans = oldItems.value()->translation();
            rot = oldItems.value()->rotation();
        }

        QSharedPointer< RoleTextItem > item = createTextItem(role.multiplicity, trans, rot, EntityA);

        newItems.append(item);
    }

    items.remove(EntityA);
    foreach (const QSharedPointer< RoleTextItem >& it, newItems) {
        items.insert(EntityA, it);
    }

    intersectionPointsChanged();

    _roleA = role;
}

void BiAssociation::setRoleB(const Association::Role& role) {
    QList< QSharedPointer< RoleTextItem > > newItems;
    QMap< Entities, QSharedPointer< RoleTextItem > >::iterator oldItems = items.find(EntityB);

    if (!role.name.isEmpty()) {
        qreal trans = defaultTranslation, rot = defaultRoleRotation;
        if (oldItems != items.end() && !_roleB.name.isEmpty()) {
            QMap< Entities, QSharedPointer< RoleTextItem > >::iterator tmp = oldItems;
            if (!_roleB.multiplicity.isEmpty())
                ++tmp;

            trans = tmp.value()->translation();
            rot = tmp.value()->rotation();
        }

        QSharedPointer< RoleTextItem > item = createTextItem(Member::visibilityText(role.visibility) + " " + role.name,
                                                             trans, rot, EntityB);

        newItems.append(item);
    }

    if (!role.multiplicity.isEmpty()) {
        qreal trans = defaultTranslation, rot = defaultMultiplicityRotation;
        if (oldItems != items.end() && !_roleB.multiplicity.isEmpty()) {
            trans = oldItems.value()->translation();
            rot = oldItems.value()->rotation();
        }

        QSharedPointer< RoleTextItem > item = createTextItem(role.multiplicity, trans, rot, EntityB);

        newItems.append(item);
    }

    items.remove(EntityB);
    foreach (const QSharedPointer< RoleTextItem >& it, newItems) {
        items.insert(EntityB, it);
    }

    intersectionPointsChanged();

    _roleB = role;
}

QSharedPointer<RoleTextItem> BiAssociation::createTextItem(const QString& text, qreal translation, qreal rotation, BiAssociation::Entities entity) {
    const QLineF* segment = entity == EntityA ? segmentEntityA() : (entity == EntityB ? segmentEntityB() : NULL);
    RoleTextItem* textItem = new RoleTextItem(text, translation, rotation, segment, this);

    //items.insert(entity, QSharedPointer<RoleTextItem>(textItem));
    return QSharedPointer<RoleTextItem>(textItem);
}

void BiAssociation::intersectionPointsChanged() {
    QMap<Entities, QSharedPointer<RoleTextItem> >::iterator item = items.begin();
    while (item != items.end()) {
        QPointF v = QLineF::fromPolar(item.value()->translation(),
                                      item.value()->origin()->angle() + item.value()->rotation()).p2();

        QPointF center = v + item.value()->origin()->p1();
        item.value()->setPos(center - item.value()->boundingRect().center());

        ++item;
    }
}

BaseDialog* BiAssociation::settingsDialog() {
    return new BiAssociationDialog(this);
}

void BiAssociation::serialize(QXmlStreamWriter& writer) const {
    beginSerialization(writer, typeString());

    if (items.count(EntityA)) {
        writer.writeStartElement("role");
        writer.writeAttribute("side", "A");

        QMap< Entities, QSharedPointer< RoleTextItem > >::const_iterator it = items.find(EntityA);

        if (!_roleA.name.isEmpty()) {
            QMap< Entities, QSharedPointer< RoleTextItem > >::const_iterator tmp = it;
            if (!_roleA.multiplicity.isEmpty())
                ++tmp;

            writer.writeStartElement("name");
            writer.writeAttribute("trans", QString::number(tmp.value()->translation()));
            writer.writeAttribute("rot", QString::number(tmp.value()->rotation()));

            writer.writeCharacters(_roleA.name);

            writer.writeEndElement();
        }
        if (!_roleA.multiplicity.isEmpty()) {
            writer.writeStartElement("multiplicity");
            writer.writeAttribute("trans", QString::number(it.value()->translation()));
            writer.writeAttribute("rot", QString::number(it.value()->rotation()));

            writer.writeCharacters(_roleA.multiplicity);

            writer.writeEndElement();
        }

        writer.writeTextElement("visibility", Member::visibilityText(_roleA.visibility));

        writer.writeEndElement();
    }

    if (items.count(EntityB)) {
        writer.writeStartElement("role");
        writer.writeAttribute("side", "B");

        QMap< Entities, QSharedPointer< RoleTextItem > >::const_iterator it = items.find(EntityB);

        if (!_roleB.name.isEmpty()) {
            QMap< Entities, QSharedPointer< RoleTextItem > >::const_iterator tmp = it;
            if (!_roleB.multiplicity.isEmpty())
                ++tmp;

            writer.writeStartElement("name");
            writer.writeAttribute("trans", QString::number(tmp.value()->translation()));
            writer.writeAttribute("rot", QString::number(tmp.value()->rotation()));

            writer.writeCharacters(_roleB.name);

            writer.writeEndElement();
        }
        if (!_roleB.multiplicity.isEmpty()) {
            writer.writeStartElement("multiplicity");
            writer.writeAttribute("trans", QString::number(it.value()->translation()));
            writer.writeAttribute("rot", QString::number(it.value()->rotation()));

            writer.writeCharacters(_roleB.multiplicity);

            writer.writeEndElement();
        }

        writer.writeTextElement("visibility", Member::visibilityText(_roleB.visibility));

        writer.writeEndElement();
    }

    endSerialization(writer);
}

DiagramItem* BiAssociation::deserialize(QXmlStreamReader& reader, DiagramScene* scene) {
    Association::DeserializationHelperOutput hep = Association::deserializationHelper(reader);
    BiAssociation* assoc = new BiAssociation(scene->entityById(hep.entityA), scene->entityById(hep.entityB));
    assoc->waypointList = hep.waypoints;

    deserializeRoles(reader, assoc);

    return assoc;
}

void BiAssociation::deserializeRoles(QXmlStreamReader& reader, BiAssociation* assoc) {
    if (reader.tokenType() == QXmlStreamReader::EndElement)
        return;

    if (reader.name() == "role" && reader.attributes().value("", "side") == "A") {
        Association::Role role;
        QSharedPointer<RoleTextItem> nameItem, multiplicityItem;
        while (reader.readNextStartElement()) {
            if (reader.name() == "name") {
                nameItem = assoc->createTextItem(reader.readElementText(), reader.attributes().value("", "trans").toString().toDouble(),
                                                 reader.attributes().value("", "rot").toString().toDouble(), EntityA);
                role.name = reader.readElementText();
            } else if (reader.name() == "multiplicity") {
                multiplicityItem = assoc->createTextItem(reader.readElementText(), reader.attributes().value("", "trans").toString().toDouble(),
                                                         reader.attributes().value("", "rot").toString().toDouble(), EntityA);
                role.multiplicity = reader.readElementText();
            } else if (reader.name() == "visibility") {
                role.visibility = Member::visibilityFromText(reader.readElementText());
            }
        }

        assoc->_roleA = role;
        if (!nameItem.isNull())
            assoc->items.insert(EntityA, nameItem);
        if (!multiplicityItem.isNull())
            assoc->items.insert(EntityA, multiplicityItem);
    }

    if (!reader.readNextStartElement())
        return;

    if (reader.name() == "role" && reader.attributes().value("", "side") == "B") {
        Association::Role role;
        QSharedPointer<RoleTextItem> nameItem, multiplicityItem;
        while (reader.readNextStartElement()) {
            if (reader.name() == "name") {
                nameItem = assoc->createTextItem(reader.readElementText(), reader.attributes().value("", "trans").toString().toDouble(),
                                                 reader.attributes().value("", "rot").toString().toDouble(), EntityB);
                role.name = reader.readElementText();
            } else if (reader.name() == "multiplicity") {
                multiplicityItem = assoc->createTextItem(reader.readElementText(), reader.attributes().value("", "trans").toString().toDouble(),
                                                         reader.attributes().value("", "rot").toString().toDouble(), EntityB);
                role.multiplicity = reader.readElementText();
            } else if (reader.name() == "visibility") {
                role.visibility = Member::visibilityFromText(reader.readElementText());
            }
        }

        assoc->_roleB = role;
        if (!nameItem.isNull())
            assoc->items.insert(EntityB, nameItem);
        if (!multiplicityItem.isNull())
            assoc->items.insert(EntityB, multiplicityItem);
    }
}
