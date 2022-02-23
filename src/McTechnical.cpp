#include "McTechnical.h"

#include "Project.h"
#include "Helpers/FileHelper.h"
#include "Helpers/JsonHelper.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QDir>

#include "QDebug"

int McTechnical::WarningNumber = 0;
int McTechnical::CriticalNumber = 0;
int McTechnical::FatalNumber = 0;

void McTechnical::CreateTemplateProject(const QString &directory)
{
    qDebug()<< "Creating a template project in : "<< directory;


    // Create the project path if it does not exits
    FileHelper::CreatePathIfDoesNotExist(directory);
    QString ProjectFilePath = directory + "/minecraft-technics.project.json";

    // Create the object of the project file and save it
    // The root object
    QJsonObject root; {

        // The description of the project
        QJsonObject description; {
            // The name of the project
            description.insert("project_name" , "Minecraft Technics");
            // The path of the behavior pack
            description.insert("behavior_pack_path" , QDir::homePath() + "/AppData/Local/Packages/Microsoft.MinecraftUWP_8wekyb3d8bbwe/LocalState/games/com.mojang");
            // If we want to build in debug mode
            description.insert("debug" , false);

            // The command blocks origins [0,0,0]
            QJsonArray commands_blocks_structure_origin = JsonHelper::ParseJsonArray("[0,0,0]");
            description.insert("commands_blocks_structure_origin", commands_blocks_structure_origin);

            description.insert("game_controller_entity_target" , "@e[type=armor_stand,name=game_controller]");

        }
        root.insert("description" , description);


        // The files of the project
        QJsonObject project_files; {

            // Create the ticks and the file template
            QJsonArray ticks;
            Ticks(nullptr).CreateTemplateFile(directory + "/ticks/ticks_00.json");
            ticks.append("ticks/ticks_00.json");
            project_files.insert("ticks" , ticks);

            // Create the variables and the file template
            QJsonArray variables;
            Variables(nullptr).CreateTemplateFile(directory + "/variables/variables.json");
            variables.append("variables/variables.json");
            project_files.insert("variables" , variables);

            // Create the quests and the file template
            QJsonArray quests;
            Quests(nullptr).CreateTemplateFile(directory + "/quests/quest_00.json");
            quests.append("quests/quest_00.json");
            project_files.insert("quests" , quests);

            // Create the cinematics and the file template
            QJsonArray cinematics;
            Cinematics(nullptr).CreateTemplateFile(directory + "/cinematics/cinematic_00.json" );
            cinematics.append("cinematics/cinematic_00.json");
            project_files.insert("cinematics" , cinematics);

            // Create the delayed commands and the file template
            QJsonArray delayed_commands;
            DelayedCommands(nullptr).CreateTemplateFile(directory + "/delayed_commands/delayed_commands_00.json");
            delayed_commands.append("delayed_commands/delayed_commands_00.json");
            project_files.insert("delayed_commands" , delayed_commands);

            // Create the delayed commands and the file template
            QJsonArray coordinates;
            Coordinates(nullptr).CreateTemplateFile(directory + "/coordinates.json");
            coordinates.append("coordinates.json");
            project_files.insert("coordinates" , coordinates);

            // Create the delayed commands and the file template
            QJsonArray texts;
            Texts(nullptr).CreateTemplateFile(directory + "/texts.json");
            coordinates.append("texts.json");
            project_files.insert("texts" , texts);


        }
        //root.insert("project_files" , project_files);

    }
    // Save the object
    FileHelper::SaveJsonToFile(ProjectFilePath , root);

    // Create the template file
    FileHelper::CopyFileContent(":/file_prefabs/ReadMe.md" , directory + "/ReadMe.md");


    qDebug()<< "Finished creating the template project.";
    qDebug()<< "";
}

void McTechnical::ComputeProject(const QString &projectPath)
{
    // Reset the number of errors
    McTechnical::WarningNumber = 0;
    McTechnical::CriticalNumber = 0;
    McTechnical::FatalNumber = 0;


    // Create the project
    Project project;

    // Fill the project from the content of the file
    qInfo()<< "Starting analysing the project file.";
    qDebug()<< "<McTechnical::ComputeProject> Starting to fill the project from : "<< projectPath;
    project.FillFromFile(projectPath);

    // If the project is valid
    if(project.isValid()) {
        // Build the project
        qDebug()<< "";
        qDebug()<< "<McTechnical::ComputeProject> The project is valid.";
        qDebug()<< "";
        project.Build();
    }

    qInfo()<< "Number of warnings errors : "<< WarningNumber;
    qInfo()<< "Number of critical errors : "<< CriticalNumber;
    qInfo()<< "Number of fatal errors : "<< FatalNumber;



}
