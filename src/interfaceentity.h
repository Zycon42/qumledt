/**
 * @file interfaceentity.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _INTERFACEENTITY_H_
#define _INTERFACEENTITY_H_

#include "entity.h"
#include "members.h"

class DiagramScene;
/**
 * Interface entity. Has <<interface>> above name, its abstract class
 * and has no attributes
 */
class InterfaceEntity : public Entity
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    /**
     * Constructs interface on specified coordinates
     * @param x scene coordinate
     * @param y scene coordinate
     * @param id entity unique id
     */
    InterfaceEntity(qreal x, qreal y, int id);

    /**
     * Drawing
     */
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

    void setOperations(const QList<Operation>& op) { operationList = op; adjustSize();}
    QList<Operation>& getOperations() { return operationList; }

    virtual BaseDialog* settingsDialog();

    void setVisibility(Member::Visibility v) { _visibility = v; }
    Member::Visibility visibility() const { return _visibility; }

    virtual void serialize(QXmlStreamWriter& writer) const;

    static DiagramItem* deserialize(QXmlStreamReader& reader, DiagramScene* scene);
public slots:
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

    static Operation readOperation(QXmlStreamReader& reader);
    static Operation::Parameter readOperationParameter(QXmlStreamReader& reader);
private:
    int visibleOperationCount() const;

    static const int nameWidthMargin = 10;
    static const int nameSectionMargin = 2;
    static const int sectionMargin = 2;
    static const int minSectionHeight = 6;

    QList<Operation> operationList;

    Member::Visibility _visibility;
};

#endif // _INTERFACEENTITY_H_
