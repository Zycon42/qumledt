/**
 * @file attributedialog.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _ATTRIBUTEDIALOG_H_
#define _ATTRIBUTEDIALOG_H_

#include "../members.h"

#include <QDialog>

namespace Ui {
    class AttributeDialog;
}

class AttributeDialog : public QDialog
{
Q_OBJECT
public:
    explicit AttributeDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~AttributeDialog();

    void setAttribute(const Attribute& attr);
    Attribute getAttribute() const;
private:
    Ui::AttributeDialog* ui;
};

#endif // _ATTRIBUTEDIALOG_H_
