#ifndef MCTECHNICAL_H
#define MCTECHNICAL_H

#include <QString>

///
/// \brief The McTechnical class
/// The class is managing everything about the project
/// creating and the projects computing
///
class McTechnical
{
public:

    static int WarningNumber;
    static int CriticalNumber;
    static int FatalNumber;


public:
    ///
    /// \brief CreateTemplateProject Create a template project in the given file.
    /// \param directory Path to the directory where to export the template project.
    ///
    void CreateTemplateProject(const QString& directory);

    ///
    /// \brief ComputeProject Export the given project to file.
    /// \param projectPath The path of the project file to compute.
    ///
    void ComputeProject(const QString& projectPath);





private:

};

#endif // MCTECHNICAL_H
