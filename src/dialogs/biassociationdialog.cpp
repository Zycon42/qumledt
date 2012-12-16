/**
 * @file biassociationdialog.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#include "biassociationdialog.h"
#include "ui_biassociationdialog.h"

#include "../biassociation.h"

BiAssociationDialog::BiAssociationDialog(BiAssociation* asc, QWidget* parent, Qt::WindowFlags f): BaseDialog(parent, f), asc(asc){
    ui = new Ui::BiAssociationDialog;
    ui->setupUi(this);

    ui->roleANameEdit->setText(asc->roleA().name);
    ui->roleAMultiplicity->setText(asc->roleA().multiplicity);

    switch (asc->roleA().visibility) {
        case Member::Public:
            ui->visiAPublicRadio->setChecked(true);
            break;
        case Member::Protected:
            ui->visiAProtectedRadio->setChecked(true);
            break;
        case Member::Private:
            ui->visiAPrivateRadio->setChecked(true);
            break;
    }

    ui->roleBNameEdit->setText(asc->roleB().name);
    ui->roleBMultiplicity->setText(asc->roleB().multiplicity);

    switch (asc->roleB().visibility) {
        case Member::Public:
            ui->visiBPublicRadio->setChecked(true);
            break;
        case Member::Protected:
            ui->visiBProtectedRadio->setChecked(true);
            break;
        case Member::Private:
            ui->visiBPrivateRadio->setChecked(true);
            break;
    }
}

BiAssociationDialog::~BiAssociationDialog() {
    delete ui;
}

void BiAssociationDialog::accept() {
    Member::Visibility visibilityA = ui->visiAPublicRadio->isChecked() ? Member::Public :
    (ui->visiAProtectedRadio->isChecked() ? Member::Protected : Member::Private);

    Association::Role roleA(ui->roleANameEdit->text(), ui->roleAMultiplicity->text(), visibilityA);
    asc->setRoleA(roleA);

    Member::Visibility visibilityB = ui->visiBPublicRadio->isChecked() ? Member::Public :
    (ui->visiBProtectedRadio->isChecked() ? Member::Protected : Member::Private);

    Association::Role roleB(ui->roleBNameEdit->text(), ui->roleBMultiplicity->text(), visibilityB);
    asc->setRoleB(roleB);

    QDialog::accept();
}


