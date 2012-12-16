/**
 * @file classpropertiesdialog.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#include "classpropertiesdialog.h"
#include "attributemodel.h"
#include "attributedialog.h"

#include "operationmodel.h"

#include "ui_classdialog.h"
#include "../classentity.h"
#include "operationdialog.h"

ClassPropertiesDialog::ClassPropertiesDialog(ClassEntity* entity, QWidget* parent, Qt::WindowFlags f): BaseDialog(parent, f), entity(entity) {
    ui = new Ui::ClassPropertiesDialog;
    ui->setupUi(this);

    attrModel = new AttributeModel();
    ui->attributesView->setModel(attrModel);

    attrModel->insertRows(0, entity->getAttributes());

    opModel = new OperationModel();
    ui->operationsView->setModel(opModel);

    opModel->insertRows(0, entity->getOperations());

    ui->classNameEdit->setText(entity->name());
    ui->abstractCheckBox->setChecked(entity->isAbstract());

    if (entity->visibility() == Member::Public)
        ui->visiPublicRadio->setChecked(true);
    else if (entity->visibility() == Member::Protected)
        ui->visiProtectedRadio->setChecked(true);
    else
        ui->visiPrivateRadio->setChecked(true);

    connect(ui->removeAttrBtn, SIGNAL(clicked()), this, SLOT(removeAttributes()));
    connect(ui->newAttributeBtn, SIGNAL(clicked()), this, SLOT(createAttribute()));
    connect(ui->attrPropertiesbtn, SIGNAL(clicked()), this, SLOT(modifyAttribute()));

    connect(ui->removeOperationBtn, SIGNAL(clicked()), this, SLOT(removeOperations()));
    connect(ui->newOperationBtn, SIGNAL(clicked()), this, SLOT(createOperation()));
    connect(ui->operPropertiesBtn, SIGNAL(clicked()), this, SLOT(modifyOperation()));
}

ClassPropertiesDialog::~ClassPropertiesDialog() {
    delete ui;
    delete attrModel;
    delete opModel;
}

void ClassPropertiesDialog::accept() {
    entity->setName(ui->classNameEdit->text());
    entity->setAbstract(ui->abstractCheckBox->isChecked());
    entity->setVisibility(ui->visiPublicRadio->isChecked() ? Member::Public :
        (ui->visiProtectedRadio->isChecked() ? Member::Protected : Member::Private));

    entity->setAttributes(attrModel->attributes());
    entity->setOperations(opModel->operations());

    QDialog::accept();
}


void ClassPropertiesDialog::removeAttributes() {
    QItemSelectionModel* selection = ui->attributesView->selectionModel();
    QModelIndexList indexes = selection->selectedRows();

    foreach (const QModelIndex& index, indexes) {
        attrModel->removeRow(index.row());
    }
}

void ClassPropertiesDialog::createAttribute() {
    AttributeDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        attrModel->insertRow(attrModel->rowCount(), dialog.getAttribute());
    }
}

void ClassPropertiesDialog::modifyAttribute() {
    QItemSelectionModel* selection = ui->attributesView->selectionModel();
    QModelIndexList indexes = selection->selectedRows();
    if (indexes.count() != 1)
        return;

    int selected = indexes.first().row();

    AttributeDialog dialog;
    dialog.setAttribute(attrModel->attribute(selected));
    if (dialog.exec() == QDialog::Accepted) {
        attrModel->removeRow(selected);
        attrModel->insertRow(selected, dialog.getAttribute());

        selection->clearSelection();
        selection->select(indexes.first(), QItemSelectionModel::Select);
    }
}

void ClassPropertiesDialog::removeOperations() {
    QItemSelectionModel* selection = ui->operationsView->selectionModel();
    QModelIndexList indexes = selection->selectedRows();

    foreach (const QModelIndex& index, indexes) {
        opModel->removeRow(index.row());
    }
}

void ClassPropertiesDialog::createOperation() {
    OperationDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        opModel->insertRow(opModel->rowCount(), dialog.getOperation());
    }
}

void ClassPropertiesDialog::modifyOperation() {
    QItemSelectionModel* selection = ui->operationsView->selectionModel();
    QModelIndexList indexes = selection->selectedRows();
    if (indexes.count() != 1)
        return;

    int selected = indexes.first().row();

    OperationDialog dialog;
    dialog.setOperation(opModel->operation(selected));
    if (dialog.exec() == QDialog::Accepted) {
        opModel->removeRow(selected);
        opModel->insertRow(selected, dialog.getOperation());

        selection->clearSelection();
        selection->select(indexes.first(), QItemSelectionModel::Select);
    }
}
