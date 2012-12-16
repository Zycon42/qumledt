/**
 * @file modes.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#include "modes.h"
#include "diagramview.h"
#include "diagramscene.h"

#include "classentity.h"
#include "interfaceentity.h"
#include "biassociation.h"
#include "uniassociation.h"
#include "generalization.h"
#include "aggregation.h"
#include "composition.h"

#include "dialogs/basedialogs.h"

#include <QMouseEvent>
#include <QDebug>
#include <QMenu>

bool SelectMode::onMouseDoubleClick(DiagramView* view, QMouseEvent* event) {
    if (view->scene()) {
        DiagramItem* item = dynamic_cast<DiagramItem*>(view->itemAt(event->pos()));
        if (item) {
            item->showSettings();
        }
    }
    return true;
}

bool SelectMode::onKeyPress(DiagramView* view, QKeyEvent* event) {
    if (view->scene() && view->scene()->isActive()) {
        DiagramItem* item = dynamic_cast<DiagramItem*>(view->scene()->focusItem());
        if (item && event->key() == Qt::Key_Delete) {
            item->killSelf();
            return false;
        }
    }
    return true;
}

bool SelectMode::onContextMenu(DiagramView* view, QContextMenuEvent* event) {
    if (view->scene()) {
        DiagramItem* item = dynamic_cast<DiagramItem*>(view->itemAt(event->pos()));
        if (item) {
            QMenu* menu = item->menu(view->mapToScene(event->pos()));
            menu->exec(view->viewport()->mapToGlobal(event->pos()));
            delete menu;
            return false;
        }
    }
    return true;
}

bool CreationMode::onKeyPress(DiagramView* view, QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        view->setMode(QSharedPointer<IDiagramMode>(new SelectMode()));
        return false;
    }
    return true;
}

bool EntityCreationMode::onMousePress(DiagramView* view, QMouseEvent* event) {
    static int idCounter = 0;
    if (view->scene()) {
        QPointF p = view->mapToScene(event->pos());
        qobject_cast<DiagramScene*>(view->scene())->addDiagramItem(createEntity(p.x(), p.y(), idCounter++));
    }
    return false;
}

Entity* ClassCreationMode::createEntity(qreal x, qreal y, int id) {
    return new ClassEntity(x, y, id);
}

Entity* InterfaceCreationMode::createEntity(qreal x, qreal y, int id) {
    return new InterfaceEntity(x, y, id);
}

AssociationMode::~AssociationMode() {
    if (line) {
        line->scene()->removeItem(line);
        delete line;
    }
}

bool AssociationMode::onMousePress(DiagramView* view, QMouseEvent* event) {
    if (view->scene()) {
        QPointF p = event->pos();
        if (clickedOnItem) {
            clickedItems[1] = dynamic_cast<Entity*>(view->itemAt(p.toPoint()));
            if (clickedItems[1] && clickedItems[1] != clickedItems[0]) {
                clickedOnItem = false;
                qobject_cast<DiagramScene*>(view->scene())->addDiagramItem(createAssociation(clickedItems[0], clickedItems[1]));

                view->scene()->removeItem(line);
                delete line;
                line = NULL;
            }
        } else {
            clickedItems[0] = dynamic_cast<Entity*>(view->itemAt(p.toPoint()));
            if (clickedItems[0]) {
                QGraphicsItem* item = view->itemAt(event->pos());
                line = view->scene()->addLine(QLineF(item->pos() + item->boundingRect().center(),
                                                     view->mapToScene(event->pos())), QPen(Qt::red, 0.0, Qt::DashDotLine));
                line->setZValue(-1.0);
                clickedOnItem = true;
            }
        }
    }
    return false;
}

bool AssociationMode::onMouseMove(DiagramView* view, QMouseEvent* event) {
    if (clickedOnItem) {
        QLineF l = line->line();
        l.setP2(view->mapToScene(event->pos()));
        line->setLine(l);
    }
    return true;
}

bool AssociationMode::onKeyPress(DiagramView* view, QKeyEvent* event) {
    if (clickedOnItem && event->key() == Qt::Key_Escape) {
        view->scene()->removeItem(line);
        delete line;
        line = NULL;
        clickedOnItem = false;
        return false;
    }
    return CreationMode::onKeyPress(view, event);
}

Association* AggregationMode::createAssociation(Entity* A, Entity* B) {
    return new Aggregation(A, B);
}

Association* BiAssociationMode::createAssociation(Entity* A, Entity* B) {
    return new BiAssociation(A, B);
}

Association* CompositionMode::createAssociation(Entity* A, Entity* B) {
    return new Composition(A, B);
}

Association* GeneralizationMode::createAssociation(Entity* A, Entity* B) {
    return new Generalization(A, B);
}

Association* UniAssociationMode::createAssociation(Entity* A, Entity* B) {
    return new UniAssociation(A, B);
}
