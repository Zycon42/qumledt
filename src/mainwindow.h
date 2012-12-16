/**
 * @file mainwindow.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _ITU_Proj_H_
#define _ITU_Proj_H_

#include "modefactory.h"

#include <QMainWindow>
#include <QMap>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow();
public slots:
    void newTab();
    void closeCurTab();
    void closeTab(int index);
    void quit();
    void exportAsImage();
    void showAbout();

    void save();
    void saveAs();

    void open();

    void changeMode();
private:
    void updateActions();
    void createConnections();
    void createFactoryMapping();

    Ui::MainWindow* ui;

    QMap<QAction*, QSharedPointer<IModeFactory> > factoryMap;
};

#endif // _ITU_Proj_H_
