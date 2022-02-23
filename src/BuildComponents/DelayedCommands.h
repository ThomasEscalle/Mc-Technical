#ifndef DELAYEDCOMMANDS_H
#define DELAYEDCOMMANDS_H

#include <QObject>
#include <QMap>


#include "BuildComponentBase.h"
#include "Minecraft/BlockCoordinates.h"


// Represent one single delayed command
class DelayedCommand
{
public:
    // The list of commands
    QStringList m_commands;
    // The coordinates of the block
    BlockCoordinates m_blockCoordinate;
    // The delay before the command
    int m_delayInTicks = 0;

public:

    ///
    ///
    /// \brief FromJson Fill the data from a json Object
    /// \param object The object where the original data is
    /// \return If the operation was a success
    ///
    bool FromJson(const QJsonObject& object);

};


///
/// \brief The DelayedCommands class
/// Represent a container for all the delayed commands of the project
///
class DelayedCommands : public BuildComponentBase
{
    ///
    /// \brief m_delayedCommands The container that links the commands to their identifier
    ///
    QMap<QString, DelayedCommand> m_delayedCommands;

public:
    ///
    /// \brief DelayedCommands Construct the Delayed component
    /// \param project The project that are working on.
    ///
    DelayedCommands(Project* project);

    ///
    /// \brief Build_Pass_0 Build the pass 0 of the Delayed commands
    /// This is assigning the different coordinates of the delayed commands
    /// \return
    ///
    bool Build_Pass_0() override;


    ///
    /// \brief Build_Pass_1 Build the pass 1 of the delayed commands
    /// This is setting the blocks in the structure for the commands blocks area
    /// Adding the commands block
    /// And creating the function
    /// \return
    ///
    bool Build_Pass_1() override;


    public:

    ///
    /// \brief ContainsTick Say if the container contains a certain delayed command
    /// \return If it contains the command
    ///
    bool ContainsCommand(const QString CommandId);

    ///
    /// \brief GetCommand Return the commands corresponding to the given id
    /// \param CommandId The id of the delayed commands you want to get
    /// \return The command itself
    ///
    const DelayedCommand& GetCommand(const QString& CommandId);


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

#endif // DELAYEDCOMMANDS_H
