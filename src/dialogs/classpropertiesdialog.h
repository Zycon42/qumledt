/**
 * @file classpropertiesdialog.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _CLASSPROPERTIESDIALOG_H_
#define _CLASSPROPERTIESDIALOG_H_

#include "basedialogs.h"

namespace Ui {
    class ClassPropertiesDialog;
}

class AttributeModel;
class OperationModel;
class ClassEntity;
class ClassPropertiesDialog : public BaseDialog
{
Q_OBJECT
public:
    explicit ClassPropertiesDialog(ClassEntity* entity, QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~ClassPropertiesDialog();

    virtual void accept();
private slots:
    void removeAttributes();
    void createAttribute();
    void modifyAttribute();

    void removeOperations();
    void createOperation();
    void modifyOperation();
private:
    Ui::ClassPropertiesDialog* ui;
    ClassEntity* entity;
    AttributeModel* attrModel;
    OperationModel* opModel;
};

#endif // _CLASSPROPERTIESDIALOG_H_
