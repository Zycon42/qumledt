/**
 * @file operationdialog.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _OPERATIONDIALOG_H_
#define _OPERATIONDIALOG_H_

#include <QDialog>

#include "../members.h"

#include <QDialog>

namespace Ui {
    class OperationDialog;
}

class ParameterModel;
class OperationDialog : public QDialog
{
Q_OBJECT
public:
    explicit OperationDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~OperationDialog();

    void setOperation(const Operation& op);
    Operation getOperation() const;
private slots:
    void removeParameter();
    void newParameter();
private:
    Ui::OperationDialog* ui;
    ParameterModel* model;
};

#endif // _OPERATIONDIALOG_H_
