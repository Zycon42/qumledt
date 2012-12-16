/**
 * @file diagramview.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _DIAGRAMVIEW_H_
#define _DIAGRAMVIEW_H_

#include <QGraphicsView>

class IDiagramMode;
class Entity;
class DiagramView : public QGraphicsView
{
Q_OBJECT
public:
    DiagramView(QWidget* parent = 0);

    void setMode(QSharedPointer<IDiagramMode> mode);

    /**
     * Exports scene as image
     * @param fileName resulting image file name
     * @param format one of QImageWriter::supportedImageFormats() or 0 then format will be guesed according to fileName suffix
     * @param quality 0..100 -1 == default settings
     * @return success
     */
    bool exportAsImage(const QString& fileName, const char* format = 0, int quality = -1);
protected:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseDoubleClickEvent(QMouseEvent* event);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void contextMenuEvent(QContextMenuEvent* event);
private:
    QSharedPointer<IDiagramMode> mode;
};

#endif // _DIAGRAMVIEW_H_
