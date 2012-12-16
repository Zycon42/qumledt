/**
 * @file biassociation.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _BIASSOCIATION_H_
#define _BIASSOCIATION_H_

#include "association.h"
#include "roletextitem.h"

#include <QSharedPointer>

class DiagramScene;
class BiAssociation : public Association
{
Q_OBJECT
Q_INTERFACES(QGraphicsItem)
public:
    BiAssociation(Entity* A, Entity* B);
    virtual ~BiAssociation();

    void setRoleA(const Role& role);
    void setRoleB(const Role& role);

    const Role& roleA() const { return _roleA; }
    const Role& roleB() const { return _roleB; }

    virtual BaseDialog* settingsDialog();

    virtual void serialize(QXmlStreamWriter& writer) const;

    static DiagramItem* deserialize(QXmlStreamReader& reader, DiagramScene* scene);
protected:
    enum Entities { EntityA, EntityB };

    virtual void intersectionPointsChanged();
    virtual QString typeString() const { return "bidirectional"; }

    static void deserializeRoles(QXmlStreamReader& reader, BiAssociation* assoc);
private:
    static const qreal defaultTranslation;
    static const qreal defaultRoleRotation;
    static const qreal defaultMultiplicityRotation;

    QSharedPointer< RoleTextItem > createTextItem(const QString& text, qreal translation, qreal rotation, BiAssociation::Entities entity);

    Role _roleA;
    Role _roleB;

    QMultiMap<Entities, QSharedPointer<RoleTextItem> > items;
};

#endif // _BIASSOCIATION_H_
