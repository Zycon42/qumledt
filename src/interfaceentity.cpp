/**
 * @file interfaceentity.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#include "interfaceentity.h"
#include "dialogs/interfacepropertiesdialog.h"
#include "dialogs/operationdialog.h"

#include <QPainter>
#include <QMenu>

InterfaceEntity::InterfaceEntity(qreal x, qreal y, int id) : Entity(x, y, id), _visibility(Member::Public) {
    adjustSize();
}

void InterfaceEntity::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Entity::paint(painter, option, widget);

    // set font
    QFont f = font();
    f.setBold(true);
    f.setItalic(true);
    painter->setFont(f);

    // draw name text
    painter->drawText(QRect(0, 0, size().width(), nameSectionSize().height()), Qt::AlignCenter, "<<interface>>\n" + name());
    f.setBold(false);
    painter->setFont(f);

    int operationSectionStart = nameSectionSize().height() + attributeSectionSize().height();
    int lineHeight = painter->fontMetrics().height();
    int i = 0;
    foreach (const Operation& op, operationList) {
        if (op.visibility() >= visibility()) {
            painter->drawText(QRect(0, operationSectionStart + i * lineHeight, size().width(), lineHeight), Qt::AlignLeft, op.getUmlText());
            i++;
        }
    }
}

QSize InterfaceEntity::nameSectionSize() const {
    return QSize(
        textSizeRequired("<<interface>>\n" + name()).width() + nameWidthMargin,
                 textSizeRequired("<<interface>>\n" + name()).height() + nameSectionMargin
    );
}

QSize InterfaceEntity::attributeSectionSize() const {
    return QSize(0, minSectionHeight + sectionMargin);
}

QSize InterfaceEntity::operationSectionSize() const {
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

BaseDialog* InterfaceEntity::settingsDialog() {
    return new InterfacePropertiesDialog(this);
}

int InterfaceEntity::visibleOperationCount() const {
    int count = 0;
    for (int i = 0; i < operationList.count(); i++) {
        if (operationList[i].visibility() >= visibility())
            count++;
    }
    return count;
}

void InterfaceEntity::fillMenu(QMenu* menu, const QPointF& pos) {
    menu->addAction(QIcon(":/icons/operation.png"), QString::fromUtf8("Nová operace"), this, SLOT(newOperationFromDialog()))
        ->setIconVisibleInMenu(true);

    DiagramItem::fillMenu(menu, pos);
}

void InterfaceEntity::newOperationFromDialog() {
    OperationDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        operationList.append(dialog.getOperation());
    }
    adjustSize();
}

void InterfaceEntity::serialize(QXmlStreamWriter& writer) const {
    beginSerialization(writer, "interface");

    writer.writeTextElement("visibility", Member::visibilityText(visibility()));

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

DiagramItem* InterfaceEntity::deserialize(QXmlStreamReader& reader, DiagramScene* scene) {
    Entity::DeserializationHelperOutput hel = Entity::deserializationHelper(reader);
    InterfaceEntity* ret = new InterfaceEntity(hel.pos.x(), hel.pos.y(), hel.id);
    ret->setName(hel.name);
    ret->setSize(hel.size);

    do {
        if (reader.name() == "visibility") {
            ret->setVisibility(Member::visibilityFromText(reader.readElementText()));
        } else if (reader.name() == "operation") {
            ret->operationList.append(readOperation(reader));
        }
    } while (reader.readNextStartElement());

    return ret;
}

Operation InterfaceEntity::readOperation(QXmlStreamReader& reader) {
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

Operation::Parameter InterfaceEntity::readOperationParameter(QXmlStreamReader& reader) {
    Operation::Parameter ret;
    while (reader.readNextStartElement()) {
        if (reader.name() == "name")
            ret.name = reader.readElementText();
        else if (reader.name() == "type")
            ret.type = reader.readElementText();
    }
    return ret;
}
