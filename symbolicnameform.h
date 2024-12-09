#ifndef SYMBOLICNAMEFORM_H
#define SYMBOLICNAMEFORM_H

#include <QWidget>

namespace Ui {
class SymbolicNameForm;
}

class SymbolicNameForm : public QWidget
{
    Q_OBJECT

public:
    explicit SymbolicNameForm(QWidget *parent = nullptr);
    ~SymbolicNameForm();

private:
    Ui::SymbolicNameForm *ui;
};

#endif // SYMBOLICNAMEFORM_H
