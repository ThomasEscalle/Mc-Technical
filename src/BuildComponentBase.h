#ifndef BUILDCOMPONENTBASE_H
#define BUILDCOMPONENTBASE_H

#include <QObject>
class Project;

class BuildComponentBase
{
protected:
    // The project that we are working on
    // Allows the component to know about the others components
    Project* m_project;

public:
    // Needs to be constructed from
    BuildComponentBase(Project* project);

public:


    // Fill the data from a file path (open and read)
    void FromFile(const QString& path);

    // Create a template file and save it
    void CreateTemplateFile(const QString& savePath);

    // Build the first pass of the component
    virtual bool Build_Pass_0();

    // Build the second pass of the component
    virtual bool Build_Pass_1();

    // Build the third pass of the component
    virtual bool Build_Pass_2();


protected:
    // Load the category from json
    virtual void FromJson(const QJsonObject& obj) = 0;

    // Get a template of the category to json
    virtual QJsonObject GetJsonTemplateFile() = 0;

};

#endif // BUILDCOMPONENTBASE_H
