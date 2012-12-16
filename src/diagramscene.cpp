/**
 * @file diagramscene.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#include "diagramscene.h"
#include "diagramitem.h"
#include "entity.h"
#include "association.h"
#include "interfaceentity.h"
#include "uniassociation.h"
#include "biassociation.h"
#include "generalization.h"
#include "aggregation.h"
#include "composition.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QFile>
#include "classentity.h"

DiagramScene::DiagramScene(QObject* parent): QGraphicsScene(parent) {
}

DiagramScene::~DiagramScene() {
    while(items().count()) {
        delete items().first();
    }
}

void DiagramScene::addDiagramItem(DiagramItem* item) {
    addItem(item);
}

bool DiagramScene::saveToFile(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
        return false;

    _fileName = fileName;

    // sort to entities and associations cuz when deserializing entities must go first
    QList<Entity*> entities;
    QList<Association*> associations;
    foreach (QGraphicsItem* item, items()) {
        Entity* ent = dynamic_cast<Entity*>(item);
        if (ent)
            entities.append(ent);
        Association* assoc = dynamic_cast<Association*>(item);
        if (assoc)
            associations.append(assoc);
    }

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);

    writer.writeStartDocument();

    writer.writeStartElement("diagram");

    foreach (Entity* ent, entities) {
        ent->serialize(writer);
    }
    foreach (Association* assoc, associations) {
        assoc->serialize(writer);
    }

    writer.writeEndElement();

    writer.writeEndDocument();

    return true;
}

bool DiagramScene::loadFromFile(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QXmlStreamReader reader(&file);
    // read root elm
    if (reader.readNextStartElement()) {
        // its name must be diagram
        if (reader.name() == "diagram") {
            parseDiagram(reader);
        } else
            reader.raiseError("Invalid diagram file");
    }

    if (reader.hasError()) {
        qWarning() << reader.errorString();
        return false;
    }

    _fileName = fileName;
    return true;
}

void DiagramScene::parseDiagram(QXmlStreamReader& reader) {
    QMap<QString, DiagramItem* (*)(QXmlStreamReader&, DiagramScene*)> mapping;
    mapping["class"] = ClassEntity::deserialize;
    mapping["interface"] = InterfaceEntity::deserialize;
    mapping["unidirection"] = UniAssociation::deserialize;
    mapping["bidirectional"] = BiAssociation::deserialize;
    mapping["generalization"] = Generalization::deserialize;
    mapping["aggregation"] = Aggregation::deserialize;
    mapping["composition"] = Composition::deserialize;

    // read inner elements
    while (reader.readNextStartElement()) {
        if (reader.name() == "entity" || reader.name() == "association") {
            addDiagramItem(mapping[reader.attributes().value("", "type").toString()](reader, this));
        }
    }
}

Entity* DiagramScene::entityById(int id) const {
    foreach (QGraphicsItem* item, items()) {
        Entity* ent = dynamic_cast<Entity*>(item);
        if (ent && ent->id() == id)
            return ent;
    }
    return NULL;
}
