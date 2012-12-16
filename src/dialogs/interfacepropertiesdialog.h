/**
 * @file interfacepropertiesdialog.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _INTERFACEPROPERTIESDIALOG_H_
#define _INTERFACEPROPERTIESDIALOG_H_

#include "basedialogs.h"

namespace Ui {
    class InterfacePropertiesDialog;
}

class InterfaceEntity;
class InterfacePropertiesDialog : public BaseDialog
{
Q_OBJECT
public:
    explicit InterfacePropertiesDialog(InterfaceEntity* entity, QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~InterfacePropertiesDialog();
private:
    Ui::InterfacePropertiesDialog* ui;
    InterfaceEntity* entity;
};

#endif // _INTERFACEPROPERTIESDIALOG_H_
