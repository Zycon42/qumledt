/**
 * @file uniassociationdialog.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#include "uniassociationdialog.h"
#include "ui_uniassociationdialog.h"

#include "../uniassociation.h"

UniAssociationDialog::UniAssociationDialog(UniAssociation* asc, QWidget* parent, Qt::WindowFlags f): BaseDialog(parent, f), asc(asc) {
    ui = new Ui::UniAssociationDialog;
    ui->setupUi(this);

    ui->roleNameEdit->setText(asc->role().name);
    ui->roleMultiplicity->setText(asc->role().multiplicity);

    switch (asc->role().visibility) {
        case Member::Public:
            ui->visiPublicRadio->setChecked(true);
            break;
        case Member::Protected:
            ui->visiProtectedRadio->setChecked(true);
            break;
        case Member::Private:
            ui->visiPrivateRadio->setChecked(true);
            break;
    }
}

UniAssociationDialog::~UniAssociationDialog() {
    delete ui;
}

void UniAssociationDialog::accept() {
    Member::Visibility visibility = ui->visiPublicRadio->isChecked() ? Member::Public :
        (ui->visiProtectedRadio->isChecked() ? Member::Protected : Member::Private);

    Association::Role role(ui->roleNameEdit->text(), ui->roleMultiplicity->text(), visibility);
    asc->setRole(role);

    QDialog::accept();
}
