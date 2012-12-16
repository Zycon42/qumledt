/**
 * @file diagramitem.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _DIAGRAMITEM_H_
#define _DIAGRAMITEM_H_

#include <QObject>
#include <QGraphicsItem>
#include <QXmlStreamReader>

class BaseDialog;

/**
 * Base class representing Item in DiagramScene
 * Base class for entities and associations
 */
class DiagramItem : public QObject, public QGraphicsItem
{
Q_OBJECT
Q_INTERFACES(QGraphicsItem)
public:
    virtual ~DiagramItem() { }
    virtual BaseDialog* settingsDialog() = 0;
    /**
     * Creates context menu for item
     * @param pos scene coordinates where menu will be executed
     * @return created menu
     */
    virtual QMenu* menu(const QPointF& pos);

    virtual void serialize(QXmlStreamWriter& writer) const = 0;
public slots:
    void killSelf();
    void showSettings();
protected:
    virtual void fillMenu(QMenu* menu, const QPointF& pos);
};

#endif // _DIAGRAMITEM_H_
