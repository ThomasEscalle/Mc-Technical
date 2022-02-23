#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>

#include "BuildComponents/Cinematics.h"
#include "BuildComponents/DelayedCommands.h"
#include "BuildComponents/Quests.h"
#include "BuildComponents/Ticks.h"
#include "BuildComponents/Variables.h"
#include "BuildComponents/Coordinates.h"
#include "BuildComponents/Texts.h"

#include "Minecraft/McFunction.h"
#include "Minecraft/CommandBlocksArea.h"

///
/// \brief Represent the project
///
class Project : public QObject
{
    Q_OBJECT
    // The Name of the project
    QString m_projectName = "";

    // The path of the project
    QString m_projectPath = "";

    // The path of the behavior pack
    QString m_BpPath = "";

    // The coordinates of the structure block that
    // Loads the commands blocks area
    int m_structureBlockCoordinates[3];

    // The game controller entity target
    QString m_gameControllerTarget = "@e[type=armor_stand,name=game_controller]";

    // If we are in debug mode
    bool m_isDebugMode = false;
private:
    // The cinematics contained in the project
    Cinematics m_cinematics;
    // The delayed commands contained in the project
    DelayedCommands m_delayedCommands;
    // The quests contained in the project
    Quests m_quests;
    // The ticks contained in the project
    Ticks m_ticks;
    // The variables contained in the project
    Variables m_variables;
    // The coordinates
    Coordinates m_coordinates;
    // The texts
    Texts m_texts;
private:
    // The function to reset everything
    McFunction m_resetFunction;
    // The function to initialise everything
    McFunction m_initFunction;
    // The function to store the informations
    // this contains no commands but all the informations necessary for latter
    McFunction m_informations;



private:
    // The commands blocks area
    CommandBlocksArea m_commandBlocksArea;

private:
    ///
    /// \brief m_isValid If the project is valid
    ///
    bool m_isValid = false;

public:
    // construct the project
    Project();
    ~Project();


    ///
    /// \brief FillFromFile Loads the project from a file
    /// \param path The path of the file to load
    ///
    void FillFromFile(const QString& path);

    ///
    /// \brief Build Build the project
    /// Call the different build passes.
    ///
    void Build();




private:
    ///
    /// \brief CreateComponents Create the different components from the project file
    /// This is called durring the FillFromFile and not durring the building process
    /// \return If the process was a success or if there was a fatal error
    ///
    bool CreateComponents(const QJsonObject& object);

    ///
    /// \brief ScanFolders Scan the files in a defined folder
    /// \param path The path of the folder to scan
    /// \return If everything was doing good
    ///
    bool ScanFolders(const QString& path);

    ///
    /// \brief Build_Pass_0 Build the first pass of the project
    /// The pass 0 correspond to the assignation of the coordinates
    /// \return  If the build operation was a success or if there was a fatal error
    ///
    bool Build_Pass_0();

    ///
    /// \brief Build_Pass_1 Build the second pass of the project
    /// The pass 1 correspond to the parse of the commands
    /// \return  If the build operation was a success or if there was a fatal error
    ///
    bool Build_Pass_1();

    ///
    /// \brief Build_Pass_2 Build the third pass of the project
    /// \return  If the build operation was a success or if there was a fatal error
    ///
    bool Build_Pass_2();


public:
    /// Getters :
    // If the project is valid
    bool isValid() const;
    // The name of the project
    const QString &ProjectName() const;
    // The path of the project
    const QString &ProjectPath() const;
    // The Behavior pack path
    const QString &BpPath() const;
    // The coordinates of the strucutre block
    const int& StructureBlockCoordinate(int index = 0) const;
    // The game controller target (where will be stored the scoreboards)
    QString GameControllerTarget( bool removeLastCharacter = false) const;




    // The cinematics contained in the project
    Cinematics* Cinematics();
    // The delayed commands contained in the project
    DelayedCommands* DelayedCommands();
    // The quests contained in the project
    Quests* Quests();
    // The ticks contained in the project
    Ticks* Ticks();
    // THe variables contained in the project
    Variables* Variables();
    // The coordinates of the project
    Coordinates* Coordinates();
    // The texts
    Texts* Texts();


    // The function to reset everything
    McFunction* ResetFunction();
    // The function to initialise everything
    McFunction* InitFunction();
    // The function to store the informations
    // this contains no commands but all the informations necessary for latter
    McFunction* InformationsFunction();

    // The commands blocks area
    CommandBlocksArea* CommandBlockArea();






    bool isDebugMode() const;
};

#endif // PROJECT_H
