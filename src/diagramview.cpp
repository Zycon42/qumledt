/**
 * @file diagramview.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#include "diagramview.h"
#include "modes.h"

#include <QMouseEvent>
#include <QDebug>

DiagramView::DiagramView(QWidget* parent):  QGraphicsView(parent), mode(NULL) {
    setMouseTracking(true);
    setSceneRect(0, 0, 500, 400);
}

void DiagramView::mousePressEvent(QMouseEvent* event) {
    if (mode == NULL) {
        QGraphicsView::mousePressEvent(event);
        return;
    }

    if (mode->onMousePress(this, event))
        QGraphicsView::mousePressEvent(event);
}

void DiagramView::mouseMoveEvent(QMouseEvent* event) {
    if (mode == NULL) {
        QGraphicsView::mouseMoveEvent(event);
        return;
    }

    if (mode->onMouseMove(this, event))
        QGraphicsView::mouseMoveEvent(event);
}

void DiagramView::mouseDoubleClickEvent(QMouseEvent* event) {
    if (mode == NULL) {
        QGraphicsView::mouseDoubleClickEvent(event);
        return;
    }

    if (mode->onMouseDoubleClick(this, event))
        QGraphicsView::mouseDoubleClickEvent(event);
}

void DiagramView::keyPressEvent(QKeyEvent* event) {
    if (mode == NULL) {
        QGraphicsView::keyPressEvent(event);
        return;
    }

    // HACK:
    // in non select mode theres posibility that mode will call view->setMode
    // and this temporaty variable will make sure that old mode will be destroyed
    // not inside its own method but at this method end
    QSharedPointer< IDiagramMode > hack = mode;
    if (mode->onKeyPress(this, event))
        QGraphicsView::keyPressEvent(event);
}

void DiagramView::contextMenuEvent(QContextMenuEvent* event) {
    if (mode == NULL) {
        QGraphicsView::contextMenuEvent(event);
        return;
    }

    if (mode->onContextMenu(this, event))
        QGraphicsView::contextMenuEvent(event);
}

void DiagramView::setMode(QSharedPointer< IDiagramMode > mode) {
    this->mode = mode;

    // seting cursor directly on viewport cuz if you set it only on this
    // and then some item changes cursor, this->setCursor will not work
    viewport()->setCursor(mode->cursor());
}

bool DiagramView::exportAsImage(const QString& fileName, const char* format, int quality) {
    QImage image(sceneRect().width(), sceneRect().height(), QImage::Format_ARGB32_Premultiplied);
    image.fill(0);
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    //painter.setBackground(QBrush(Qt::transparent));
    scene()->render(&painter);

    return image.save(fileName, format, quality);
}
