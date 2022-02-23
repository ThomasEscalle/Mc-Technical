#include "Cinematics.h"

#include "Helpers/FileHelper.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

#include "Project.h"





Cinematics::Cinematics(Project *project)
    :BuildComponentBase(project)
{

}

bool Cinematics::Build_Pass_0()
{
    qInfo()<< "Starting Building the pass 0 of the cinematics.";
    int x = 0;
    int z = 0;

    // Add to the informations
    m_project->InformationsFunction()->Header("Cinematics coordinates :");



    // Loop throught all the commands
    QStringList keys = m_cinematics.keys();
    for(int i = 0 ; i < keys.size() ; i++) {
        // The key of the command
        QString key = keys[i];

        // One delayed command
        Cinematic cine = m_cinematics[key];

        // Set the coordinates
        BlockCoordinates coordinates;
        coordinates.x = x;
        coordinates.y = 7;
        coordinates.z = z;

        // Set the coordinate into delayed commands object
        cine.BlockCoord = coordinates;
        // Set the delayed commands object into the map
        m_cinematics[key] = cine;

        qDebug()<< "<Cinematics::Build_Pass_0> The cinematic "<< key << " was assigned the coordinates "<< m_cinematics[key].BlockCoord.ToString();

        m_project->InformationsFunction()->Comment("    " + key + " has the coordinate : " + coordinates.ToString() );

        // Add one on the coordinates
        x++;
        if(x >= 16) {
            z++;
            x=0;
        }
    }


    if(keys.size() > 0) {
        // Create the objective
        m_project->InitFunction()->Command("scoreboard objectives add cine dummy cine");
        // Set to the game controller object
        m_project->InitFunction()->Command("scoreboard players set " + m_project->GameControllerTarget() + " cine 0");
        m_project->InitFunction()->Space();
    }


    return true;
}

bool Cinematics::Build_Pass_1()
{
    // Loop throught all the commands
    QStringList keys = m_cinematics.keys();

    qInfo()<< "Starting Building the pass 1 of the cinematics.";

    // Loop throught the cinematics
    for(int i = 0 ; i < keys.size() ; i++) {
        // The key of the command
        QString key = keys[i];
        // The cinematic
        Cinematic cine = m_cinematics[key];

        // Build the cinematic
        cine.Build_Pass_1(m_project);


        // Create the command blocks
        m_project->CommandBlockArea()->SetBlockAt(cine.BlockCoord.x , cine.BlockCoord.y , cine.BlockCoord.z , "stone");
        m_project->CommandBlockArea()->SetCommandBlockAt(cine.BlockCoord.x , cine.BlockCoord.y - 1 , cine.BlockCoord.z , "function cinematics/" + key + "/start", 0);
        m_project->CommandBlockArea()->SetTickingCommandBlockAt(cine.BlockCoord.x , cine.BlockCoord.y + 1 , cine.BlockCoord.z , "function cinematics/" + key + "/exec", 0);

        qDebug()<< "<Cinematics::Build_Pass_1> Commands blocks set for the cinematic "<< key;
    }
    return true;
}

bool Cinematics::ContainsCinematic(const QString &cineIdentifier)
{
    return m_cinematics.contains(cineIdentifier);
}

Cinematic Cinematics::GetCinematic(const QString &cineIdentifier)
{
    return m_cinematics[cineIdentifier];
}

void Cinematics::FromJson(const QJsonObject &obj)
{
    QJsonObject CinematicsObj = obj["cinematics"].toObject();



    // Add the integers to the map
    QStringList CinematicsKeys = CinematicsObj.keys();
    // Loop throught all the cinematics in the container object
    for(int i = 0 ; i < CinematicsKeys.size() ; i++) {
        // Get the key
        QString key = CinematicsKeys[i];
        qDebug()<< "<Cinematics::FromJson> Cinematic found : " << key << ".";
        // If there is already a cinematic with this name
        if(m_cinematics.contains(key)) qWarning()<< "<Cinematics::FromJson> There is already a cinematic with the name : " + key + ".";
        else if(!CinematicsObj[key].isObject())  qWarning()<< "<Cinematics::FromJson> The 'cinematics' childs must be JsonObject. Key : "+ key;
        else {
            QJsonObject cinematicJs = CinematicsObj[key].toObject();
            // Create the cinematic object
            Cinematic cinematic;
            // Set the identifier of the sub cinematic
            cinematic.identifier = key;
            // FIll the cinematic from the subcinematic
            if(cinematic.FromJson(cinematicJs)) {
                qDebug()<< "<Cinematics::FromJson> Registered the cinematic " << key << ".";
                m_cinematics.insert(key, cinematic);
            }
        }
    }
}

QJsonObject Cinematics::GetJsonTemplateFile()
{
    QJsonObject root;
    QJsonObject cinematics = QJsonObject();
    QJsonObject cinematic;
    cinematic.insert("length" , 12);
    QJsonObject keys;
    keys.insert("from" , "125 30 25 90 0");
    keys.insert("to", "121 15 25 90 0");
    QJsonArray on_start;
    QJsonArray on_end;
    cinematic.insert("on_start_commands" , on_start);
    cinematic.insert("on_start_commands" , on_end);
    QJsonObject runtime_commands;
    QJsonArray command_3;
    command_3.push_back("say hello");
    runtime_commands.insert("3", command_3);
    cinematic.insert("runtime_commands", runtime_commands);
    cinematic.insert("keys" , keys);
    cinematic.insert("take_shortcut", true);
    cinematics.insert("cine_0" , cinematic);
    root.insert("cinematics" , cinematics);
    return root;
}

