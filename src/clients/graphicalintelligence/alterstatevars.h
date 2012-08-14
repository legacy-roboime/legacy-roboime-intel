#ifndef ALTERSTATEVARS_H
#define ALTERSTATEVARS_H

#include <QDialog>

namespace Ui {
class AlterStateVars;
}

class AlterStateVars : public QDialog
{
    Q_OBJECT
    
public:
    explicit AlterStateVars(QWidget *parent = 0);
    ~AlterStateVars();
    Ui::AlterStateVars *ui;

private:



};

#endif // ALTERSTATEVARS_H
