/**
 * @file operationdialog.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#include "operationdialog.h"
#include "parametermodel.h"

#include "ui_operationdialog.h"

#include <QDebug>

OperationDialog::OperationDialog(QWidget* parent, Qt::WindowFlags f): QDialog(parent, f) {
    ui = new Ui::OperationDialog;
    ui->setupUi(this);

    model = new ParameterModel();
    ui->parameterView->setModel(model);
    ui->parameterView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    connect(ui->removeParamBtn, SIGNAL(clicked()), this, SLOT(removeParameter()));
    connect(ui->newParamBtn, SIGNAL(clicked()), this, SLOT(newParameter()));
}

OperationDialog::~OperationDialog() {
    delete model;
    delete ui;
}

void OperationDialog::setOperation(const Operation& op) {
    ui->abstractCheckBox->setChecked(op.isAbstract());
    ui->staticCheckBox->setChecked(op.isStatic());
    ui->nameEdit->setText(op.name());
    ui->returnTypeEdit->setText(op.returnType());

    if (op.visibility() == Member::Public)
        ui->visiPublicRadio->setChecked(true);
    else if (op.visibility() == Member::Protected)
        ui->visiProtectedRadio->setChecked(true);
    else
        ui->visiPrivateRadio->setChecked(true);

    model->setParameters(op.getParameters());
}

Operation OperationDialog::getOperation() const {
    Operation op(ui->nameEdit->text(),
        ui->visiPublicRadio->isChecked() ? Member::Public : (ui->visiProtectedRadio->isChecked() ? Member::Protected : Member::Private),
        ui->returnTypeEdit->text(), ui->abstractCheckBox->isChecked(), ui->staticCheckBox->isChecked()
    );
    op.addParameters(model->parameters());
    return op;
}

void OperationDialog::newParameter() {
    model->insertRows(model->rowCount(), 1, Operation::Parameter("", ""));
    QModelIndex index = model->index(model->rowCount() - 1, 0);
    ui->parameterView->setCurrentIndex(index);
    ui->parameterView->edit(index);
}

void OperationDialog::removeParameter() {
    QItemSelectionModel* selection = ui->parameterView->selectionModel();
    QModelIndexList indexes = selection->selectedIndexes();

    foreach (const QModelIndex& index, indexes) {
        model->removeRow(index.row());
    }
}
