#include "ProjectData.h"
#include "XMLObject.h"
#include "Helper.h"
#include <QFileInfo>
#include <QDebug>


ProjectData::ProjectData() : szProjVersion_("V1.0.0")
{
    this->pImplGraphPageSaveLoadObj_ = Q_NULLPTR;
    szProjPath_ = "";
    szProjName_ = "";
}

ProjectData::~ProjectData()
{

}


ProjectData* ProjectData::getInstance()
{
    static ProjectData instance_;
    return &instance_;
}


bool ProjectData::openFromXml(const QString &szProjFile)
{
    QString buffer = Helper::readString(szProjFile);
    XMLObject xml;
    if(!xml.load(buffer, 0)) return false;

    QList<XMLObject*> projObjs = xml.getChildren();
    foreach(XMLObject* pProjObj, projObjs) {
        // 工程信息管理
        projInfoMgr_.openFromXml(pProjObj);
        // 网络配置
        netSetting_.openFromXml(pProjObj);
        // 数据库配置
        dbSetting_.openFromXml(pProjObj);
        // 用户权限
        userAuthority_.openFromXml(pProjObj);
        // 设备配置信息
        deviceInfo_.openFromXml(pProjObj);

        XMLObject *pTagsObj = pProjObj->getCurrentChild("tags");
        if(pTagsObj != Q_NULLPTR) {
            // 设备标签变量组
            tagIOGroup_.openFromXml(pTagsObj);
            // 设备标签变量
            tagIO_.openFromXml(pTagsObj);
            // 中间标签变量
            tagTmp_.openFromXml(pTagsObj);
            // 系统标签变量
            tagSys_.openFromXml(pTagsObj);
        }

        // 加载画面
        XMLObject *pPagesObj = pProjObj->getCurrentChild("pages");
        if(pPagesObj != Q_NULLPTR) {
            if(pImplGraphPageSaveLoadObj_) {
                pImplGraphPageSaveLoadObj_->openFromXml(pPagesObj);
            }
        }

        // 脚本
        script_.openFromXml(pProjObj);
        // 图片资源管理
        pictureResourceMgr_.openFromXml(pProjObj);

    }

    return true;
}


bool ProjectData::saveToXml(const QString &szProjFile)
{
    XMLObject projObjs;
    projObjs.setTagName("projects");

    XMLObject *pProjObj = new XMLObject(&projObjs);
    pProjObj->setTagName("project");
    pProjObj->setProperty("application_version", szProjVersion_);

    // 工程信息管理
    projInfoMgr_.saveToXml(pProjObj);
    // 网络配置
    netSetting_.saveToXml(pProjObj);
    // 数据库配置
    dbSetting_.saveToXml(pProjObj);
    // 用户权限
    userAuthority_.saveToXml(pProjObj);
    // 设备配置信息
    deviceInfo_.saveToXml(pProjObj);

    XMLObject *pTagsObj = new XMLObject(pProjObj);
    pTagsObj->setTagName("tags");
    // 设备标签变量组
    tagIOGroup_.saveToXml(pTagsObj);
    // 设备标签变量
    tagIO_.saveToXml(pTagsObj);
    // 中间标签变量
    tagTmp_.saveToXml(pTagsObj);
    // 系统标签变量
    tagSys_.saveToXml(pTagsObj);

    // 保存画面
    XMLObject *pPagesObj = new XMLObject(pProjObj);
    pPagesObj->setTagName("pages");
    if(pImplGraphPageSaveLoadObj_) {
        pImplGraphPageSaveLoadObj_->saveToXml(pPagesObj);
    }

    // 脚本
    script_.saveToXml(pProjObj);
    // 图片资源管理
    pictureResourceMgr_.saveToXml(pProjObj);

    Helper::writeString(szProjFile, projObjs.write());

    return true;
}


/**
 * @brief ProjectData::GetAllProjectVariableName
 * @details 获取工程所有变量的名称
 * @param varList 存储变量列表
 * @param type IO, TMP, SYS, ALL
 */
void ProjectData::getAllTagName(QStringList &varList, const QString &type)
{
    varList.clear();
    QString szType = type.toUpper();

    //-------------设备变量------------------//
    if(szType == "ALL" || szType == "IO") {
        for(int i=0; i<tagIO_.listTagIODBItem_.count(); i++) {
            TagIODBItem *pObj = tagIO_.listTagIODBItem_.at(i);
            varList << (QObject::tr("设备变量.") + pObj->m_szName + "[" + pObj->m_szTagID + "]");
        }
    }

    //-------------中间变量------------------//
    if(szType == "ALL" || szType == "TMP") {
        for(int i=0; i<tagTmp_.listTagTmpDBItem_.count(); i++) {
            TagTmpDBItem *pObj = tagTmp_.listTagTmpDBItem_.at(i);
            varList << (QObject::tr("中间变量.") + pObj->m_szName + "[" + pObj->m_szTagID + "]");
        }
    }

    //-------------系统变量------------------//
    if(szType == "ALL" || szType == "SYS") {
        for(int i=0; i<tagSys_.listTagSysDBItem_.count(); i++) {
            TagSysDBItem *pObj = tagSys_.listTagSysDBItem_.at(i);
            varList << (QObject::tr("系统变量.") + pObj->m_szName + "[" + pObj->m_szTagID + "]");
        }
    }
}

/**
 * @brief ProjectData::getProjectPath
 * @details 获取工程路径
 * @param projectName 工程名称全路径
 * @return 工程路径
 */
QString ProjectData::getProjectPath(const QString &projectName) {
    QString path = QString();
    int pos = projectName.lastIndexOf("/");
    if (pos != -1) {
        path = projectName.left(pos);
    }
    return path;
}

/**
 * @brief ProjectData::getProjectNameWithSuffix
 * @details 获取包含后缀工程名称
 * @param projectName 工程名称全路径
 * @return 工程名称包含后缀
 */
QString ProjectData::getProjectNameWithSuffix(const QString &projectName) {
    QFileInfo projFileInfo(projectName);
    return projFileInfo.fileName();
}

/**
 * @brief ProjectData::getProjectNameWithOutSuffix
 * @details 获取不包含后缀工程名称
 * @param projectName 工程名称全路径
 * @return 工程名称不包含后缀
 */
QString ProjectData::getProjectNameWithOutSuffix(const QString &projectName) {
    QFileInfo projFileInfo(projectName);
    return projFileInfo.baseName();
}
