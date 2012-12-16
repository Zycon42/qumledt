/**
 * @file diagramscene.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _DIAGRAMSCENE_H_
#define _DIAGRAMSCENE_H_

#include <QGraphicsScene>

class QXmlStreamReader;
class DiagramItem;
class Entity;

/**
 * Scene representing UML diagram
 */
class DiagramScene : public QGraphicsScene
{
Q_OBJECT
public:
    DiagramScene(QObject* parent = 0);
    virtual ~DiagramScene();

    /**
     * Load diagram from XML file
     * @param fileName input file
     * @return success
     */
    bool loadFromFile(const QString& fileName);
    /**
     * Saves diagram to XML file
     * @param fileName output file
     * @return success
     */
    bool saveToFile(const QString& fileName);

    /**
     * Add item to diagram
     * @param item diagram item
     */
    void addDiagramItem(DiagramItem* item);

    const QString& fileName() { return _fileName; }

    Entity* entityById(int id) const;
private:
    void parseDiagram(QXmlStreamReader& reader);

    QString _fileName;
};

#endif // _DIAGRAMSCENE_H_
