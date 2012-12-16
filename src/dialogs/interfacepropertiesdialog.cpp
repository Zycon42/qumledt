/**
 * @file interfacepropertiesdialog.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#include "interfacepropertiesdialog.h"
#include "ui_interfacedialog.h"

InterfacePropertiesDialog::InterfacePropertiesDialog(InterfaceEntity* entity, QWidget* parent, Qt::WindowFlags f)
    : BaseDialog(parent, f), entity(entity) {

    ui = new Ui::InterfacePropertiesDialog;
    ui->setupUi(this);
}

InterfacePropertiesDialog::~InterfacePropertiesDialog() {
    delete ui;
}

