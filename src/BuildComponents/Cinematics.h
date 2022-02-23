#ifndef CINEMATICS_H
#define CINEMATICS_H

#include <QObject>
#include <QMap>

#include "BuildComponentBase.h"
#include "Cinematic.h"


///
/// \brief The Cinematics class
/// Represent a container for all the cinematics of the project
///
class Cinematics : public BuildComponentBase
{
    // Define the global cinematic settings
    // that are applied on every cinematic
    //
    class GlobalCinematicsSettings {
    public:
        // The list of events to execute on start of the cinematic
        QStringList start_commands;
        // The list of events to execute on the end of a cinematic
        QStringList end_commands;

        // The container of the commands linked to their keyframe position (in tick)
        QMap<int, QStringList> m_runtimeCommands;
    };


    ///
    /// \brief m_cinematics The list of all the cinematics linked to their Identifiers
    ///
    QMap<QString, Cinematic> m_cinematics;

public:
    ///
    /// \brief Cinematics Construc the cinematics component
    /// \param project The project we are working on
    ///
    Cinematics(Project* project);

    ///
    /// \brief Build_Pass_0 Build the pass 0 of the cinematics
    /// This is assigning the different coordinates of the commands blocks
    /// \return
    ///
    bool Build_Pass_0() override;

    ///
    /// \brief Build_Pass_1 Build the pass 1 of the cinematics
    /// This is building the cinematic itself and placing the command blocks
    /// \return
    ///
    bool Build_Pass_1() override;



public:

    ///
    /// \brief ContainsCinematic Return if there is a cinematic with the given name
    /// \param cineIdentifier The identifier of the cinematic to check
    /// \return If it exitsts
    ///
    bool ContainsCinematic(const QString& cineIdentifier);

    ///
    /// \brief GetCinematic Get the cinematic linked to the identifier
    /// \param cineIdentifier The identifier of the cinematic to get
    /// \return The cinematic
    ///
    Cinematic GetCinematic(const QString& cineIdentifier);

public:
    ///
    /// \brief FromJson Fill the Object from a JsonObject
    /// \param obj The Object we are using for filling
    ///
    void FromJson(const QJsonObject &obj) override;

    ///
    /// \brief GetJsonTemplateFile Get the Json Prefab that will be saved in a template file.
    /// \return The generated Json Object
    ///
    QJsonObject GetJsonTemplateFile() override;
};

#endif // CINEMATICS_H
