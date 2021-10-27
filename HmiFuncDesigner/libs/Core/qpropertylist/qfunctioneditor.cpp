#include "qfunctioneditor.h"
#include "../../shared/property/qabstractproperty.h"
#include "functioneditordialog.h"
#include <QDialog>


QFunctionEditor::QFunctionEditor(QAbstractProperty *property, QUndoStack* stack, QWidget *parent) :
    QButtonCommonEditor(property, stack, parent)
{
}

void QFunctionEditor::onBtnClicked()
{
    QStringList szListFunc = property->get_value().toStringList();
    QString szSupportEvents = property->getAttribute("supportevents").toString();
    QStringList szListSupportEvents = szSupportEvents.split("|");
    FunctionEditorDialog dlg(this, szListSupportEvents);
    dlg.setFunctions(szListFunc);
    if(dlg.exec() == QDialog::Accepted) {
        QStringList szListFuncNew = dlg.getFunctions();
        QVariant v(szListFuncNew);
        property->notifyEditValue(v);
    }
}


