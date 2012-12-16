/**
 * @file basedialogs.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _BASEDIALOGS_H_
#define _BASEDIALOGS_H_

#include <QDialog>

class BaseDialog : public QDialog {
Q_OBJECT
public:
    explicit BaseDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);
};

#endif // _BASEDIALOGS_H_
