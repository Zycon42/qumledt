/**
 * @file uniassociationdialog.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _UNIASSOCIATIONDIALOG_H_
#define _UNIASSOCIATIONDIALOG_H_

#include "basedialogs.h"

namespace Ui {
    class UniAssociationDialog;
}

class UniAssociation;
class UniAssociationDialog : public BaseDialog
{
Q_OBJECT
public:
    explicit UniAssociationDialog(UniAssociation* asc, QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~UniAssociationDialog();

    virtual void accept();
private:
    Ui::UniAssociationDialog* ui;
    UniAssociation* asc;
};

#endif // _UNIASSOCIATIONDIALOG_H_
