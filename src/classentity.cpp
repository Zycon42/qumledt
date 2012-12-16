/**
 * @file classentity.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#include "classentity.h"
#include "dialogs/classpropertiesdialog.h"
#include "dialogs/attributedialog.h"
#include "dialogs/operationdialog.h"

#include <QPainter>
#include <QMenu>

ClassEntity::ClassEntity(qreal x, qreal y, int id) : Entity(x, y, id), abstractFlag(false), _visibility(Member::Public) {
    adjustSize();
}

void ClassEntity::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Entity::paint(painter, option, widget);

    // set font
    QFont f = font();
    f.setBold(true);
    if (isAbstract())
        f.setItalic(true);
    painter->setFont(f);

    // draw name text
    painter->drawText(QRect(0, 0, size().width(), nameSectionSize().height()), Qt::AlignCenter, name());
    f.setBold(false);
    f.setItalic(false);
    painter->setFont(f);

    int lineHeight = painter->fontMetrics().height();

    // Draw attributes
    int i = 0;
    foreach (const Attribute& attr, attributeList) {
        if (attr.visibility() >= visibility()) {
            if (attr.isStatic()) {
                f.setUnderline(true);
                painter->setFont(f);
            }

            painter->drawText(QRect(0, nameSectionSize().height() + i * lineHeight, size().width(), lineHeight),
                              Qt::AlignLeft, attr.getUmlText());

            if (attr.isStatic()) {
                f.setUnderline(false);
                painter->setFont(f);
            }
            i++;
        }
    }

    // Draw operations
    int operationSectionStart = nameSectionSize().height() + attributeSectionSize().height();
    i = 0;
    foreach (const Operation& op, operationList) {
        if (op.visibility() >= visibility()) {
            if (op.isAbstract()) {
                f.setItalic(true);
                painter->setFont(f);
            }

            painter->drawText(QRect(0, operationSectionStart + i * lineHeight, size().width(), lineHeight),
                            Qt::AlignLeft, op.getUmlText());

            if (op.isAbstract()) {
                f.setItalic(false);
                painter->setFont(f);
            }
            i++;
        }
    }
}

void ClassEntity::setOperations(const QList< Operation >& op) {
    operationList = op;
    foreach (const Operation& o, operationList) {
        if (o.isAbstract())
            abstractFlag = true;
    }
    adjustSize();
}


QSize ClassEntity::nameSectionSize() const {
    return QSize(
        textSizeRequired(name()).width() + nameWidthMargin,
        textSizeRequired(name()).height() + nameSectionMargin
    );
}

QSize ClassEntity::attributeSectionSize() const {
    int height = visibleAttributesCount() * QFontMetrics(font()).height();
    if (height < minSectionHeight)
        height = minSectionHeight;
    height += sectionMargin;

    int width = 0;
    foreach (const Attribute& attr, attributeList) {
        if (attr.visibility() < visibility())
            continue;

        int attrWidth = textSizeRequired(attr.getUmlText()).width();
        if (attrWidth > width)
            width = attrWidth;
    }
    width += sectionMargin;

    return QSize(width, height);
}

QSize ClassEntity::operationSectionSize() const {
    int height = visibleOperationCount() * QFontMetrics(font()).height();
    if (height < minSectionHeight)
        height = minSectionHeight;
    height += sectionMargin;

    int width = 0;
    foreach (const Operation& op, operationList) {
        if (op.visibility() < visibility())
            continue;

        int opWidth = textSizeRequired(op.getUmlText()).width();
        if (opWidth > width)
            width = opWidth;
    }
    width += sectionMargin;

    return QSize(width, height);
}

BaseDialog* ClassEntity::settingsDialog() {
    return new ClassPropertiesDialog(this);
}

int ClassEntity::visibleAttributesCount() const {
    int count = 0;
    for (int i = 0; i < attributeList.count(); i++) {
        if (attributeList[i].visibility() >= visibility())
            count++;
    }
    return count;
}

int ClassEntity::visibleOperationCount() const {
    int count = 0;
    for (int i = 0; i < operationList.count(); i++) {
        if (operationList[i].visibility() >= visibility())
            count++;
    }
    return count;
}

void ClassEntity::fillMenu(QMenu* menu, const QPointF& pos) {
    menu->addAction(QIcon(":/icons/attribute.png"), QString::fromUtf8("Nový atribut"), this, SLOT(newAttributeFromDialog()))
        ->setIconVisibleInMenu(true);

    menu->addAction(QIcon(":/icons/operation.png"), QString::fromUtf8("Nová operace"), this, SLOT(newOperationFromDialog()))
        ->setIconVisibleInMenu(true);

    DiagramItem::fillMenu(menu, pos);
}

void ClassEntity::newAttributeFromDialog() {
    AttributeDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        attributeList.append(dialog.getAttribute());
    }
    adjustSize();
}

void ClassEntity::newOperationFromDialog() {
    OperationDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        operationList.append(dialog.getOperation());
    }
    adjustSize();
}

void ClassEntity::serialize(QXmlStreamWriter& writer) const {
    beginSerialization(writer, "class");

    writer.writeTextElement("visibility", Member::visibilityText(visibility()));
    writer.writeTextElement("isAbstract", QString(abstractFlag ? "true" : "false"));

    for (int i = 0; i < attributeList.count(); i++) {
        writer.writeStartElement("attribute");

        writer.writeTextElement("name", attributeList[i].name());
        writer.writeTextElement("type", attributeList[i].type());
        writer.writeTextElement("defaultValue", attributeList[i].defaultValue());
        writer.writeTextElement("isStatic", QString(attributeList[i].isStatic() ? "true" : "false"));
        writer.writeTextElement("visibility", Member::visibilityText(attributeList[i].visibility()));

        writer.writeEndElement();;
    }

    for (int i = 0; i < operationList.count(); i++) {
        writer.writeStartElement("operation");

        writer.writeTextElement("name", operationList[i].name());
        writer.writeTextElement("retType", operationList[i].returnType());
        writer.writeTextElement("isStatic", QString(operationList[i].isStatic() ? "true" : "false"));
        writer.writeTextElement("isAbstract", QString(operationList[i].isAbstract() ? "true" : "false"));
        writer.writeTextElement("visibility", Member::visibilityText(operationList[i].visibility()));

        foreach (const Operation::Parameter& p, operationList[i].getParameters()) {
            writer.writeStartElement("parameter");
            writer.writeTextElement("name", p.name);
            writer.writeTextElement("type", p.type);
            writer.writeEndElement();
        }

        writer.writeEndElement();
    }

    endSerialization(writer);
}

DiagramItem* ClassEntity::deserialize(QXmlStreamReader& reader, DiagramScene* scene) {
    Entity::DeserializationHelperOutput hel = Entity::deserializationHelper(reader);
    ClassEntity* ret = new ClassEntity(hel.pos.x(), hel.pos.y(), hel.id);
    ret->setName(hel.name);
    ret->setSize(hel.size);

    do {
        if (reader.name() == "visibility") {
            ret->setVisibility(Member::visibilityFromText(reader.readElementText()));
        } else if (reader.name() == "isAbstract") {
            ret->setAbstract(reader.readElementText() == "true" ? true : false);
        } else if (reader.name() == "attribute") {
            ret->attributeList.append(readAttribute(reader));
        } else if (reader.name() == "operation") {
            ret->operationList.append(readOperation(reader));
        }
    } while (reader.readNextStartElement());

    return ret;
}

Attribute ClassEntity::readAttribute(QXmlStreamReader& reader) {
    Attribute ret(QString(), Member::Public, QString());
    while (reader.readNextStartElement()) {
        if (reader.name() == "name")
            ret.setName(reader.readElementText());
        else if (reader.name() == "type")
            ret.setType(reader.readElementText());
        else if (reader.name() == "defaultValue")
            ret.setDefaultValue(reader.readElementText());
        else if (reader.name() == "isStatic")
            ret.setStatic(reader.readElementText() == "true" ? true : false);
        else if (reader.name() == "visibility")
            ret.setVisibility(Member::visibilityFromText(reader.readElementText()));
    }
    return ret;
}

Operation ClassEntity::readOperation(QXmlStreamReader& reader) {
    Operation ret(QString(), Member::Public, QString());
    while (reader.readNextStartElement()) {
        if (reader.name() == "name")
            ret.setName(reader.readElementText());
        else if (reader.name() == "retType")
            ret.setReturnType(reader.readElementText());
        else if (reader.name() == "isStatic")
            ret.setStatic(reader.readElementText() == "true" ? true : false);
        else if (reader.name() == "isAbstract")
            ret.setAbstract(reader.readElementText() == "true" ? true : false);
        else if (reader.name() == "visibility")
            ret.setVisibility(Member::visibilityFromText(reader.readElementText()));
        else if (reader.name() == "parameter")
            ret.addParameter(readOperationParameter(reader));
    }
    return ret;
}

Operation::Parameter ClassEntity::readOperationParameter(QXmlStreamReader& reader) {
    Operation::Parameter ret;
    while (reader.readNextStartElement()) {
        if (reader.name() == "name")
            ret.name = reader.readElementText();
        else if (reader.name() == "type")
            ret.type = reader.readElementText();
    }
    return ret;
}
