/**
 * @file modes.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _MODES_H_
#define _MODES_H_

#include <QCursor>
#include <QPixmap>

class QContextMenuEvent;
class DiagramView;
class Entity;
class QMouseEvent;
class QKeyEvent;
class Association;
class QGraphicsLineItem;

/**
 * Interface for diagram modes
 */
class IDiagramMode {
public:
    virtual ~IDiagramMode() { }
    /**
     * Called in diagram mouse press event handler
     * @return if diagram view should propagate event to scene
     */
    virtual bool onMousePress(DiagramView* view, QMouseEvent* event) = 0;
    /**
     * Called in diagram mouse move event handler
     * @return if diagram view should propagate event to scene
     */
    virtual bool onMouseMove(DiagramView* view, QMouseEvent* event) = 0;
    /**
     * Called in diagram mouse doubleclick event handler
     * @return if diagram view should propagate event to scene
     */
    virtual bool onMouseDoubleClick(DiagramView* view, QMouseEvent* event) = 0;
    /**
     * Called in diagram key press event handler
     * @return if diagram view should propagate event to scene
     */
    virtual bool onKeyPress(DiagramView* view, QKeyEvent* event) = 0;
    /**
     * Called in diagram context menu event handler
     * @return if diagram view should propagate event to scene
     */
    virtual bool onContextMenu(DiagramView* view, QContextMenuEvent* event) = 0;
    /**
     * Cursor used in this mode
     */
    virtual QCursor cursor() const = 0;
};

/**
 * Mode for selecting items
 */
class SelectMode : public IDiagramMode {
public:
    virtual QCursor cursor() const { return QCursor(Qt::ArrowCursor); }
    virtual bool onMousePress(DiagramView* view, QMouseEvent* event) { return true; }
    virtual bool onMouseMove(DiagramView* view, QMouseEvent* event) { return true; }
    virtual bool onMouseDoubleClick(DiagramView* view, QMouseEvent* event);
    virtual bool onKeyPress(DiagramView* view, QKeyEvent* event);
    virtual bool onContextMenu(DiagramView* view, QContextMenuEvent* event);
};

class CreationMode : public IDiagramMode {
public:
    virtual bool onKeyPress(DiagramView* view, QKeyEvent* event);
    virtual bool onMouseDoubleClick(DiagramView* view, QMouseEvent* event) { return true; }
    virtual bool onContextMenu(DiagramView* view, QContextMenuEvent* event) { return true; }
};

/**
 * Abstract Mode for creating entities
 */
class EntityCreationMode : public CreationMode {
public:
    virtual bool propagateEvents() const { return false; }
    virtual bool onMousePress(DiagramView* view, QMouseEvent* event);
    virtual bool onMouseMove(DiagramView* view, QMouseEvent* event) { return true; }
protected:
    virtual Entity* createEntity(qreal x, qreal y, int id) = 0;
};

/**
 * Mode for creating classes
 */
class ClassCreationMode : public EntityCreationMode {
public:
    virtual QCursor cursor() const { return QCursor(QPixmap(":/cursors/class.png"), 10, 10); }
protected:
    virtual Entity* createEntity(qreal x, qreal y, int id);
};

/**
 * Mode for creating interfaces
 */
class InterfaceCreationMode : public EntityCreationMode {
public:
    virtual QCursor cursor() const { return QCursor(QPixmap(":/cursors/interface.png"), 10, 10); }
protected:
    virtual Entity* createEntity(qreal x, qreal y, int id);
};

/**
 * Abstract mode for creating associations
 */
class AssociationMode : public CreationMode {
public:
    AssociationMode() : clickedOnItem(false), line(NULL) { }
    virtual ~AssociationMode();
    virtual bool propagateEvents() const { return false; }
    virtual bool onMousePress(DiagramView* view, QMouseEvent* event);
    virtual bool onMouseMove(DiagramView* view, QMouseEvent* event);
    virtual bool onKeyPress(DiagramView* view, QKeyEvent* event);
protected:
    virtual Association* createAssociation(Entity* A, Entity* B) = 0;
    bool clickedOnItem;
    Entity* clickedItems[2];
private:
    QGraphicsLineItem* line;
};

/**
 * Mode for creating bidirectional associations
 */
class BiAssociationMode : public AssociationMode {
public:
    BiAssociationMode() : AssociationMode() { }
    virtual QCursor cursor() const { return QCursor(QPixmap(":/cursors/association.png"), 10, 10); }
protected:
    virtual Association* createAssociation(Entity* A, Entity* B);
};

/**
 * Mode for creating unidirectional associations
 */
class UniAssociationMode : public AssociationMode {
public:
    UniAssociationMode() : AssociationMode() { }
    virtual QCursor cursor() const { return QCursor(QPixmap(":/cursors/uniassociation.png"), 10, 10); }
protected:
    virtual Association* createAssociation(Entity* A, Entity* B);
};

/**
 * Mode for creating aggregations
 */
class AggregationMode : public AssociationMode {
public:
    AggregationMode() : AssociationMode() { }
    virtual QCursor cursor() const { return QCursor(QPixmap(":/cursors/aggregation.png"), 10, 10); }
protected:
    virtual Association* createAssociation(Entity* A, Entity* B);
};

/**
 * Mode for creating compositions
 */
class CompositionMode : public AssociationMode {
public:
    CompositionMode() : AssociationMode() { }
    virtual QCursor cursor() const { return QCursor(QPixmap(":/cursors/composition.png"), 10, 10); }
protected:
    virtual Association* createAssociation(Entity* A, Entity* B);
};

/**
 * Mode for creating generalizations
 */
class GeneralizationMode : public AssociationMode {
public:
    GeneralizationMode() : AssociationMode() { }
    virtual QCursor cursor() const { return QCursor(QPixmap(":/cursors/generalisation.png"), 10, 10); }
protected:
    virtual Association* createAssociation(Entity* A, Entity* B);
};

#endif
