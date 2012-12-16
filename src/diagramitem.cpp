/**
 * @file diagramitem.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#include "diagramitem.h"
#include "dialogs/basedialogs.h"

#include <QMenu>
#include <QDebug>
#include <QGraphicsScene>

QMenu* DiagramItem::menu(const QPointF& pos) {
    QMenu* m = new QMenu();
    fillMenu(m, pos);
    return m;
}

void DiagramItem::fillMenu(QMenu* menu, const QPointF& pos) {
    menu->addAction(QIcon::fromTheme("document-properties"), "Upravit", this, SLOT(showSettings()))->setIconVisibleInMenu(true);
    menu->addAction(QIcon::fromTheme("edit-delete"), "Odstranit", this, SLOT(killSelf()))->setIconVisibleInMenu(true);
}

void DiagramItem::killSelf() {
    scene()->removeItem(this);
    delete this;
}

void DiagramItem::showSettings() {
    BaseDialog* dialog = settingsDialog();
    if (dialog) {
        dialog->exec();
        delete dialog;
    }
}
