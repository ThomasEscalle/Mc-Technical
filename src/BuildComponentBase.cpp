#include "BuildComponentBase.h"

#include <QJsonObject>
#include <QDebug>

#include "Helpers/FileHelper.h"


BuildComponentBase::BuildComponentBase(Project *project)
{
    m_project = project;
}

void BuildComponentBase::FromFile(const QString &path)
{
    QJsonObject object = FileHelper::OpenJsonObject(path);
    if(object.isEmpty()) {
        qWarning()<< "<BuildCategoryBase::FromFile> Impossible to get the Json object from the file : "<< path << ".";
    }
    else {
        // Fill the object
        FromJson(object);
    }
}

void BuildComponentBase::CreateTemplateFile(const QString &savePath)
{
    // Get the template file
    QJsonObject rootObject = GetJsonTemplateFile();
    FileHelper::SaveJsonToFile(savePath , rootObject);
}

bool BuildComponentBase::Build_Pass_0()
{
    return true;
}

bool BuildComponentBase::Build_Pass_1()
{
    return true;
}

bool BuildComponentBase::Build_Pass_2()
{
    return true;
}

