﻿#include "ScriptManageChild.h"
#include <QApplication>
#include <QCoreApplication>
#include <QDataStream>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonDocument>
#include <QJsonObject>
#include <QListWidgetItem>
#include <QMenu>
#include <QProcess>
#include "ProjectData.h"
#include "ConfigUtils.h"
#include "ScriptConditionConfigForm.h"
#include "ScriptEditorDlg.h"

QList<ScriptObject *> ScriptFileManage::m_listScriptInfo = QList<ScriptObject *>();

ScriptObject::ScriptObject()
{

}

ScriptObject::~ScriptObject()
{

}

void ScriptObject::load(QJsonObject &json)
{
    m_strName = json["Name"].toString();
    m_bInUse = json["InUse"].toBool();
    m_strDescription = json["Description"].toString();
    m_strRunMode = json["RunMode"].toString();
    m_strRunModeArgs = json["RunModeArgs"].toString();
}

void ScriptObject::save(QJsonObject &json)
{
    json["Name"] = m_strName;
    json["InUse"] = m_bInUse;
    json["Description"] = m_strDescription;
    json["RunMode"] = m_strRunMode;
    json["RunModeArgs"] = m_strRunModeArgs;
}

void ScriptFileManage::AddScriptInfo(ScriptObject *obj)
{
    int pos = m_listScriptInfo.indexOf(obj);
    if (pos == -1) m_listScriptInfo.append(obj);
}

void ScriptFileManage::ModifyScriptInfo(ScriptObject *oldobj, ScriptObject *newobj)
{
    int pos = m_listScriptInfo.indexOf(oldobj);
    if (pos == -1) return;
    m_listScriptInfo.replace(pos, newobj);
}

void ScriptFileManage::DeleteScriptInfo(ScriptObject *obj)
{
    m_listScriptInfo.removeOne(obj);
}

ScriptObject *ScriptFileManage::GetScriptObject(const QString &name)
{
    foreach (ScriptObject *pobj, m_listScriptInfo) {
        if (pobj->m_strName == name) return pobj;
    }
    return NULL;
}

void ScriptFileManage::load(const QString &filename, SaveFormat saveFormat)
{
    QFile loadFile(filename);

    if (!loadFile.exists()) return;

    if (!loadFile.open(QIODevice::ReadOnly)) return;

    m_listScriptInfo.clear();
    QByteArray loadData = loadFile.readAll();
    QJsonDocument loadDoc(saveFormat == Json ? QJsonDocument::fromJson(loadData) : QJsonDocument::fromBinaryData(loadData));
    const QJsonObject json = loadDoc.object();

    QJsonArray scriptInfoArray = json["ScriptInfos"].toArray();
    for (int i = 0; i < scriptInfoArray.size(); ++i) {
        QJsonObject jsonObj = scriptInfoArray[i].toObject();
        ScriptObject *pObj = new ScriptObject();
        pObj->load(jsonObj);
        m_listScriptInfo.append(pObj);
    }

    loadFile.close();
}

void ScriptFileManage::save(const QString &filename, SaveFormat saveFormat)
{
    QString strPath = ProjectData::getInstance()->getProjectPath(filename);
    QDir dir(strPath);
    if (!dir.exists()) {
        dir.mkpath(strPath);
    }

    QFile saveFile(filename);
    QJsonObject obj;
    QJsonArray scriptInfosArray;

    saveFile.open(QFile::WriteOnly);

    for (int i = 0; i < m_listScriptInfo.size(); i++) {
        QJsonObject jsonObj;
        ScriptObject *pObj = m_listScriptInfo.at(i);
        pObj->save(jsonObj);
        scriptInfosArray.append(jsonObj);
    }

    obj["ScriptInfos"] = scriptInfosArray;

    QJsonDocument saveDoc(obj);
    saveFile.write(saveFormat == Json ? saveDoc.toJson() : saveDoc.toBinaryData());
    saveFile.close();
}

/////////////////////////////////////////////////////////////////////////////////////

ScriptManageChild::ScriptManageChild(QWidget *parent) : QWidget(parent)
{
    m_pListWidgetObj = new QListWidget(this);
    m_pListWidgetObj->setViewMode(QListView::IconMode);
    m_pListWidgetObj->setIconSize(QSize(32, 32));
    m_pListWidgetObj->setGridSize(QSize(100, 100));
    m_pListWidgetObj->setWordWrap(true);
    m_pListWidgetObj->setSpacing(20);
    m_pListWidgetObj->setResizeMode(QListView::Adjust);
    m_pListWidgetObj->setMovement(QListView::Static);
    connect(m_pListWidgetObj, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
            this, SLOT(ListWidgetClicked(QListWidgetItem *)));
    m_pVLayoutObj = new QVBoxLayout();
    m_pVLayoutObj->addWidget(m_pListWidgetObj);
    m_pVLayoutObj->setContentsMargins(1, 1, 1, 1);
    this->setLayout(m_pVLayoutObj);
    setContextMenuPolicy(Qt::DefaultContextMenu);
}

ScriptManageChild::~ScriptManageChild()
{
    DEL_OBJ(m_pListWidgetObj);
    DEL_OBJ(m_pVLayoutObj);
}


/*
* 插槽：列表视图控件单击
*/
void ScriptManageChild::ListWidgetClicked(QListWidgetItem *item)
{
    if (m_szProjectName == "") return;

    if (item->text() == "新建脚本") {
        NewScript();
    } else {
        ModifyScript();
    }
}

/*
* 右键菜单生成
*/
void ScriptManageChild::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);

    QMenu *pMenu = new QMenu(this);

    QAction *pNewAct = new QAction(QIcon(":/images/icon_new.png"), tr("新建"), this);
    pNewAct->setStatusTip(tr("新建脚本"));
    connect(pNewAct, SIGNAL(triggered()), this, SLOT(NewScript()));
    pMenu->addAction(pNewAct);

    QAction *pModifyAct = new QAction(QIcon(":/images/icon_modify.png"), tr("修改"), this);
    pModifyAct->setStatusTip(tr("修改脚本"));
    connect(pModifyAct, SIGNAL(triggered()), this, SLOT(ModifyScript()));
    pMenu->addAction(pModifyAct);

    QAction *pDeleteAct = new QAction(QIcon(":/images/icon_delete.png"), tr("删除"), this);
    pDeleteAct->setStatusTip(tr("删除脚本"));
    connect(pDeleteAct, SIGNAL(triggered()), this, SLOT(DeleteScript()));
    pMenu->addAction(pDeleteAct);

    if(!pMenu->isEmpty()) {
        pMenu->move(cursor().pos());
        pMenu->exec();
        pMenu->clear();
    }
    delete pMenu;
}

/*
* 插槽：新建
*/
void ScriptManageChild::NewScript()
{
    if (m_szProjectName == "") return;

    QString strProjectPath = ProjectData::getInstance()->getProjectPath(m_szProjectName);
    QListWidgetItem *pCurItem = m_pListWidgetObj->currentItem();

    /////////////////////////////////////////////////////////////////////////////

    ScriptConditionConfigForm *pDlg = new ScriptConditionConfigForm(strProjectPath, this);
    pDlg->setWindowTitle(tr("脚本属性"));
    pDlg->SetName(pCurItem->text());
    if (pDlg->exec() == QDialog::Accepted) {
        ScriptObject *pObj = new ScriptObject();
        pObj->m_strName = pDlg->GetName();
        pObj->m_bInUse = pDlg->isInUse();
        pObj->m_strDescription = pDlg->GetDescription();
        pObj->m_strRunMode = pDlg->GetRunMode();
        pObj->m_strRunModeArgs = pDlg->GetRunModeArgs();
        ScriptFileManage::AddScriptInfo(pObj);
        save();
        open();
    }
    delete pDlg;

    /////////////////////////////////////////////////////////////////////////////
}

/*
* 插槽：修改
*/
void ScriptManageChild::ModifyScript()
{
    if (m_szProjectName == "") return;

    QString strProjectPath = ProjectData::getInstance()->getProjectPath(m_szProjectName);
    QListWidgetItem *pCurItem = m_pListWidgetObj->currentItem();
    QString scriptFileName = strProjectPath + "/Scripts/" + pCurItem->text() + ".js";

    /////////////////////////////////////////////////////////////////////////////

    ScriptConditionConfigForm *pDlg = new ScriptConditionConfigForm(strProjectPath, this);
    pDlg->setWindowTitle(tr("脚本属性"));
    ScriptObject *pObj = ScriptFileManage::GetScriptObject(pCurItem->text());
    pDlg->SetName(pObj->m_strName);
    pDlg->SetInUse(pObj->m_bInUse);
    pDlg->SetDescription(pObj->m_strDescription);
    pDlg->SetRunMode(pObj->m_strRunMode);
    pDlg->SetRunModeArgs(pObj->m_strRunModeArgs);
    if (pDlg->exec() == QDialog::Accepted) {
        pObj->m_strName = pDlg->GetName();
        pObj->m_bInUse = pDlg->isInUse();
        pObj->m_strDescription = pDlg->GetDescription();
        pObj->m_strRunMode = pDlg->GetRunMode();
        pObj->m_strRunModeArgs = pDlg->GetRunModeArgs();
        if (pObj->m_strName != pCurItem->text()) {
            QString oldScriptFileName = strProjectPath + "/Scripts/" + pCurItem->text() + ".js";
            QString newScriptFileName = strProjectPath + "/Scripts/" + pObj->m_strName + ".js";
            pCurItem->setText(pObj->m_strName);
            QFile::rename(oldScriptFileName, newScriptFileName);
            scriptFileName = newScriptFileName;
        }

        save();
        open();

        /////////////////////////////////////////////////////////////////////////////

        ScriptEditorDlg *pScriptEditorDlg = new ScriptEditorDlg(strProjectPath, this);
        // qDebug() << "scriptFileName: " << scriptFileName;
        pScriptEditorDlg->load(scriptFileName);
        if (pScriptEditorDlg->exec() == QDialog::Accepted) {
            pScriptEditorDlg->save(scriptFileName);
        }
        delete pScriptEditorDlg;
    }
    delete pDlg;
}

/*
* 插槽：删除
*/
void ScriptManageChild::DeleteScript()
{
    QListWidgetItem *pCurItem = m_pListWidgetObj->currentItem();
    ScriptObject *pObj = ScriptFileManage::GetScriptObject(pCurItem->text());
    ScriptFileManage::DeleteScriptInfo(pObj);

    QString scriptFileName = ProjectData::getInstance()->getProjectPath(m_szProjectName) + "/Scripts/" + pCurItem->text() + ".js";
    QFile scriptFile(scriptFileName);
    if (scriptFile.exists()) scriptFile.remove();

    m_pListWidgetObj->removeItemWidget(pCurItem);

    save();
    open();
}


bool ScriptManageChild::open()
{
    QString fileDes = ProjectData::getInstance()->getProjectPath(m_szProjectName) + "/Scripts/Script.info";
    ScriptFileManage::load(fileDes, DATA_SAVE_FORMAT);
    m_pListWidgetObj->clear();
    QListWidgetItem *pNewItemObj = new QListWidgetItem(QIcon(":/images/pm_script.png"), tr("新建脚本"));
    m_pListWidgetObj->addItem(pNewItemObj);
    for (int i = 0; i < ScriptFileManage::m_listScriptInfo.count(); i++) {
        ScriptObject *pObj = ScriptFileManage::m_listScriptInfo.at(i);
        QString scriptName = pObj->m_strName;
        QListWidgetItem *pItemObj = new QListWidgetItem(QIcon(":/images/pm_script.png"), scriptName);
        m_pListWidgetObj->addItem(pItemObj);
    }
    return true;
}

bool ScriptManageChild::save()
{
    QString fileDes = ProjectData::getInstance()->getProjectPath(m_szProjectName) + "/Scripts/Script.info";
    ScriptFileManage::save(fileDes, DATA_SAVE_FORMAT);
    return true;
}

void ScriptManageChild::buildUserInterface(QMainWindow* pMainWin)
{
    Q_UNUSED(pMainWin)
    open();
}

void ScriptManageChild::removeUserInterface(QMainWindow* pMainWin)
{
    Q_UNUSED(pMainWin)
}

QString ScriptManageChild::wndTitle() const
{
    return this->windowTitle();
}




