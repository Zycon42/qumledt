/**
 * @file classentity.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _CLASSENTITY_H_
#define _CLASSENTITY_H_

#include "entity.h"
#include "members.h"

class DiagramScene;

/**
 * Entity representing Class
 */
class ClassEntity : public Entity
{
Q_OBJECT
Q_INTERFACES(QGraphicsItem)
public:
    /**
     * Constructs class on specified coordinates
     * @param x scene coordinate
     * @param y scene coordinate
     * @param id entity unique id
     */
    ClassEntity(qreal x, qreal y, int id);

    /**
     * Drawing
     */
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

    void setAttributes(const QList<Attribute>& attrs) { attributeList = attrs; adjustSize();}
    QList<Attribute>& getAttributes() { return attributeList; }

    void setOperations(const QList<Operation>& op);
    QList<Operation>& getOperations() { return operationList; }

    virtual BaseDialog* settingsDialog();

    void setAbstract(bool flag) { abstractFlag = flag; update(); }
    bool isAbstract() const { return abstractFlag; }

    void setVisibility(Member::Visibility v) { _visibility = v; }
    Member::Visibility visibility() const { return _visibility; }

    virtual void serialize(QXmlStreamWriter& writer) const;

    static DiagramItem* deserialize(QXmlStreamReader& reader, DiagramScene* scene);
public slots:
    void newAttributeFromDialog();
    void newOperationFromDialog();
protected:
    /**
     * Size of name section
     * Implementation of Enity::nameSectionSize
     */
    virtual QSize nameSectionSize() const;
    /**
     * Size of attribute section
     * Implementation of Enity::attributeSectionSize
     */
    virtual QSize attributeSectionSize() const;
    /**
     * Size of operation section
     * Implementation of Enity::attributeSectionSize
     */
    virtual QSize operationSectionSize() const;

    virtual void fillMenu(QMenu* menu, const QPointF& pos);

    static Attribute readAttribute(QXmlStreamReader& reader);
    static Operation readOperation(QXmlStreamReader& reader);
    static Operation::Parameter readOperationParameter(QXmlStreamReader& reader);
private:
    int visibleAttributesCount() const;
    int visibleOperationCount() const;

    static const int nameWidthMargin = 10;
    static const int nameSectionMargin = 2;
    static const int sectionMargin = 2;
    static const int minSectionHeight = 6;

    QList<Attribute> attributeList;
    QList<Operation> operationList;

    bool abstractFlag;
    Member::Visibility _visibility;
};

#endif // _CLASSENTITY_H_
