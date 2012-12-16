/**
 * @file biassociationdialog.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _BIASSOCIATIONDIALOG_H_
#define _BIASSOCIATIONDIALOG_H_

#include "basedialogs.h"

namespace Ui {
    class BiAssociationDialog;
}

class BiAssociation;
class BiAssociationDialog : public BaseDialog
{
Q_OBJECT
public:
    explicit BiAssociationDialog(BiAssociation* asc, QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~BiAssociationDialog();

    virtual void accept();
private:
    Ui::BiAssociationDialog* ui;
    BiAssociation* asc;
};

#endif // _BIASSOCIATIONDIALOG_H_
