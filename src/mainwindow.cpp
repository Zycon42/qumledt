/**
 * @file mainwindow.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#include "mainwindow.h"
#include "diagramscene.h"
#include "diagramview.h"
#include "modes.h"

#include "ui_mainwindow.h"

#include <QGraphicsView>
#include <QFileDialog>
#include <QImageWriter>
#include <QDebug>
#include <QMessageBox>

template <class _Class>
QSharedPointer<_Class> makeShared() {
    return QSharedPointer<_Class>(new _Class());
}

MainWindow::MainWindow() : ui(new Ui::MainWindow()) {
    ui->setupUi(this);

    QCoreApplication::setOrganizationName("ITU");
    QCoreApplication::setApplicationName("UmlEditor");

    updateActions();
    createFactoryMapping();
    createConnections();

    newTab();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::createConnections() {
    connect(ui->action_New, SIGNAL(triggered()), this, SLOT(newTab()));
    connect(ui->action_Close, SIGNAL(triggered()), this, SLOT(closeCurTab()));
    connect(ui->action_Quit, SIGNAL(triggered()), this, SLOT(quit()));

    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

    connect(ui->action_Export, SIGNAL(triggered()), this, SLOT(exportAsImage()));
    connect(ui->action_SaveAs, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(ui->action_Save, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->action_About, SIGNAL(triggered()), this, SLOT(showAbout()));

    connect(ui->action_Select, SIGNAL(triggered()), this, SLOT(changeMode()));
    connect(ui->action_Class, SIGNAL(triggered()), this, SLOT(changeMode()));
    connect(ui->action_Interface, SIGNAL(triggered()), this, SLOT(changeMode()));
    connect(ui->action_Asociation, SIGNAL(triggered()), this, SLOT(changeMode()));
    connect(ui->action_UniAsociation, SIGNAL(triggered()), this, SLOT(changeMode()));
    connect(ui->action_Generalization, SIGNAL(triggered()), this, SLOT(changeMode()));
    connect(ui->action_Agregation, SIGNAL(triggered()), this, SLOT(changeMode()));
    connect(ui->action_Composition, SIGNAL(triggered()), this, SLOT(changeMode()));
}

void MainWindow::updateActions() {
    ui->action_New->setShortcut(QKeySequence::New);
    ui->action_Open->setShortcut(QKeySequence::Open);
    ui->action_Close->setShortcut(QKeySequence::Close);
    ui->action_Quit->setShortcut(QKeySequence::Quit);
    ui->action_Save->setShortcut(QKeySequence::Save);
    ui->action_SaveAs->setShortcut(QKeySequence::SaveAs);

    ui->action_New->setIcon(QIcon::fromTheme("document-new"));
    ui->action_Open->setIcon(QIcon::fromTheme("document-open"));
    ui->action_Save->setIcon(QIcon::fromTheme("document-save"));
}

void MainWindow::createFactoryMapping() {
    factoryMap.insert(ui->action_Select, makeShared< ModeFactory<SelectMode> >());
    factoryMap.insert(ui->action_Class, makeShared< ModeFactory<ClassCreationMode> >());
    factoryMap.insert(ui->action_Interface, makeShared< ModeFactory<InterfaceCreationMode> >());
    factoryMap.insert(ui->action_Agregation, makeShared< ModeFactory<AggregationMode> >());
    factoryMap.insert(ui->action_Asociation, makeShared< ModeFactory<BiAssociationMode> >());
    factoryMap.insert(ui->action_Composition, makeShared< ModeFactory<CompositionMode> >());
    factoryMap.insert(ui->action_UniAsociation, makeShared< ModeFactory<UniAssociationMode> >());
    factoryMap.insert(ui->action_Generalization, makeShared< ModeFactory<GeneralizationMode> >());
}

void MainWindow::newTab() {
    DiagramView* view = new DiagramView();
    DiagramScene* scene = new DiagramScene(view);
    view->setScene(scene);

    int index = ui->tabWidget->addTab(view, QString::fromUtf8("Bez názvu"));
    ui->tabWidget->setCurrentIndex(index);
}

void MainWindow::closeTab(int index) {
    Q_ASSERT(index >= 0 && index < ui->tabWidget->count());

    QWidget* widget = ui->tabWidget->widget(index);
    ui->tabWidget->removeTab(index);
    delete widget;
}

void MainWindow::closeCurTab() {
    int index = ui->tabWidget->currentIndex();
    if (index != -1)
        closeTab(index);
}

void MainWindow::quit() {
    // TODO ask user if save all unsaved opened files

    close();
}

void MainWindow::changeMode() {
    ui->tabWidget->currentWidget()->setFocus();

    QAction* act = qobject_cast<QAction*>(sender());
    Q_ASSERT(act != NULL);

    QMap< QAction*, QSharedPointer< IModeFactory > >::iterator modIt = factoryMap.find(act);
    Q_ASSERT(modIt != factoryMap.end());

    qobject_cast<DiagramView*>(ui->tabWidget->currentWidget())->setMode(QSharedPointer<IDiagramMode>((*modIt)->create()));
}

void MainWindow::exportAsImage() {
    DiagramView* view = qobject_cast<DiagramView*>(ui->tabWidget->currentWidget());
    Q_ASSERT(view != NULL);

    QList< QByteArray > formats = QImageWriter::supportedImageFormats();
    QString filter = "Images (";
    for (int i = 0; i < formats.size(); i++)
        filter += " *." + formats[i];
    filter += ")";

    QString selFilter;
    QString tabText = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    QString fileName = QFileDialog::getSaveFileName(this, "Exportovat", tabText + ".png", filter, &selFilter);
    if (fileName.isEmpty())
        return;

    if (view->exportAsImage(fileName))
        statusBar()->showMessage(QString::fromUtf8("Export se podařil"));
    else
        statusBar()->showMessage(QString::fromUtf8("Export selhal"));
}

void MainWindow::saveAs() {
    DiagramView* view = qobject_cast<DiagramView*>(ui->tabWidget->currentWidget());
    Q_ASSERT(view != NULL);

    DiagramScene* dia = qobject_cast<DiagramScene*>(view->scene());
    Q_ASSERT(dia != NULL);

    QString tabText = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    QString fileName = QFileDialog::getSaveFileName(this, "Uložit jako", tabText + ".xml");
    if (fileName.isEmpty())
        return;

    if (dia->saveToFile(fileName)) {
        statusBar()->showMessage(QString::fromUtf8("Soubor uložen"));
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), QFileInfo(fileName).baseName());
    } else
        statusBar()->showMessage(QString::fromUtf8("Uložení selhalo"));
}

void MainWindow::save() {
    DiagramView* view = qobject_cast<DiagramView*>(ui->tabWidget->currentWidget());
    Q_ASSERT(view != NULL);

    DiagramScene* dia = qobject_cast<DiagramScene*>(view->scene());
    Q_ASSERT(dia != NULL);

    if (dia->fileName().isEmpty()) {
        saveAs();
        return;
    }

    if (dia->saveToFile(dia->fileName()))
        statusBar()->showMessage(QString::fromUtf8("Soubor uložen"));
    else
        statusBar()->showMessage(QString::fromUtf8("Uložení selhalo"));
}

void MainWindow::open() {
    QString fileName = QFileDialog::getOpenFileName(this, QString::fromUtf8("Otevřít"));
    if (fileName.isEmpty())
        return;

    newTab();
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), QFileInfo(fileName).baseName());

    DiagramView* view = qobject_cast<DiagramView*>(ui->tabWidget->currentWidget());
    Q_ASSERT(view != NULL);

    DiagramScene* dia = qobject_cast<DiagramScene*>(view->scene());
    Q_ASSERT(dia != NULL);

    if (dia->loadFromFile(fileName)) {
        statusBar()->showMessage(QString::fromUtf8("Otevření bylo úspěšné"));
        ui->action_Select->trigger();
    } else {
        statusBar()->showMessage(QString::fromUtf8("Otevření selhalo"));
        closeCurTab();
    }
}

void MainWindow::showAbout() {
    QMessageBox::about(this, "O programu", QString::fromUtf8("<h1>Editor UML</h1>\nProjekt do předmětu ITU 2011<br><br>Ikony z Umbrello<br>Licence GPLv2"));
}
