/**
 * @file attributedialog.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#include "attributedialog.h"

#include "ui_attributedialog.h"

AttributeDialog::AttributeDialog(QWidget* parent, Qt::WindowFlags f) : QDialog(parent, f) {
    ui = new Ui::AttributeDialog;
    ui->setupUi(this);
}

AttributeDialog::~AttributeDialog() {
    delete ui;
}


void AttributeDialog::setAttribute(const Attribute& attr) {
    ui->nameEdit->setText(attr.name());
    ui->typeEdit->setText(attr.type());
    ui->defaultValEdit->setText(attr.defaultValue());

    ui->staticCheckBox->setChecked(attr.isStatic());

    if (attr.visibility() == Member::Public)
        ui->visiPublicRadio->setChecked(true);
    else if (attr.visibility() == Member::Protected)
        ui->visiProtectedRadio->setChecked(true);
    else
        ui->visiPrivateRadio->setChecked(true);
}

Attribute AttributeDialog::getAttribute() const {
    return Attribute(ui->nameEdit->text(),
        ui->visiPublicRadio->isChecked() ? Member::Public : (ui->visiProtectedRadio->isChecked() ? Member::Protected : Member::Private),
        ui->typeEdit->text(), ui->defaultValEdit->text(), ui->staticCheckBox->isChecked()
    );
}

