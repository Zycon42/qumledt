/**
 * @file uniassociation.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _UNIASSOCIATION_H_
#define _UNIASSOCIATION_H_

#include "association.h"
#include "roletextitem.h"

class DiagramScene;

class UniAssociation : public Association
{
Q_OBJECT
Q_INTERFACES(QGraphicsItem)
public:
    UniAssociation(Entity* A, Entity* B);

    void setRole(const Role& role);
    const Role& role() const { return _role; }

    virtual BaseDialog* settingsDialog();

    virtual void serialize(QXmlStreamWriter& writer) const;

    static DiagramItem* deserialize(QXmlStreamReader& reader, DiagramScene* scene);
protected:
    virtual void intersectionPointsChanged();

    virtual void paintEndB(QPainter* painter, const QLineF& segment);
private:
    static const qreal defaultTranslation;
    static const qreal defaultRoleRotation;
    static const qreal defaultMultiplicityRotation;
    static const qreal arrowLength;
    static const qreal arrowAngle;

    QSharedPointer< RoleTextItem > createTextItem(const QString& text, qreal translation, qreal rotation);

    Role _role;
    QList<QSharedPointer<RoleTextItem> > items;
};

#endif // _UNIASSOCIATION_H_
