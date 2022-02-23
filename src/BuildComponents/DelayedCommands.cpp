#include "DelayedCommands.h"

#include "Project.h"
#include <QJsonObject>
#include <QJsonArray>
#include "CommandParser.h"

DelayedCommands::DelayedCommands(Project *project)
    :BuildComponentBase(project)
{

}

bool DelayedCommands::Build_Pass_0()
{
    qInfo()<< "Starting Building the pass 0 of the delayed commands.";
    int x = 0;
    int z = 0;

    // Add to the informations
    m_project->InformationsFunction()->Header("Delayed Commands coordinates :");


    // Loop throught all the commands
    QStringList keys = m_delayedCommands.keys();
    for(int i = 0 ; i < keys.size() ; i++) {
        // The key of the command
        QString key = keys[i];

        // One delayed command
        DelayedCommand command = m_delayedCommands[key];
        // Set the coordinates
        BlockCoordinates coordinates;
        coordinates.x = x;
        coordinates.y = 0;
        coordinates.z = z;

        // Set the coordinate into delayed commands object
        command.m_blockCoordinate = coordinates;
        // Set the delayed commands object into the map
        m_delayedCommands[key] = command;

        qDebug()<< "<DelayedCommands::Build_Pass_0> The command "<< key << " was assigned the coordinates "<< m_delayedCommands[key].m_blockCoordinate.ToString();

        m_project->InformationsFunction()->Comment("    " + key + " has the coordinate : " + coordinates.ToString() );


        // Add one on the coordinates
        x++;
        if(x >= 16) {
            z++;
            x=0;
        }
    }

    return true;
}

bool DelayedCommands::Build_Pass_1()
{
    qInfo()<< "Starting Building the pass 1 of the delayed commands.";

    // Loop throught all the commands
    QStringList keys = m_delayedCommands.keys();
    for(int i = 0 ; i < keys.size() ; i++) {
        // The key of the command
        QString key = keys[i];

        // One delayed command
        DelayedCommand command = m_delayedCommands[key];

        // The path of the command
        QString functionPath = "delayed_commands/" + key;


        // Create the function
        McFunction delayedFunction(functionPath , m_project->BpPath());
        delayedFunction.SetDescription("Delayed command function : " + key);

        // Command parser
        CommandParser commandParser(m_project);
        for(int i = 0 ; i < command.m_commands.size(); i++) {
            QString commandStr = command.m_commands[i];
            QString goodCommand = commandParser.ParseCommand(commandStr);
            delayedFunction.Command(goodCommand);
        }
        // Add at the end of the ticking command the clear of the redstone block that triggered it
        delayedFunction.Space();
        delayedFunction.Comment("Remove the activator redstone block");
        delayedFunction.Command("setblock " + command.m_blockCoordinate.ToStringInvertedXZ(true, m_project) + " stone 0");


        // Save the function
        delayedFunction.Save();
        qDebug()<< "<DelayedCommands::Build_Pass_1> Created the function "<< functionPath;

        // Add the stone block to the command block area
        m_project->CommandBlockArea()->SetBlockAt(command.m_blockCoordinate.x , command.m_blockCoordinate.y , command.m_blockCoordinate.z , "stone");
        m_project->CommandBlockArea()->SetCommandBlockAt(command.m_blockCoordinate.x , command.m_blockCoordinate.y + 1 , command.m_blockCoordinate.z , "function " + functionPath , command.m_delayInTicks);

        qDebug()<< "<DelayedCommands::Build_Pass_1> Set the stone block at "<< command.m_blockCoordinate.ToString();
        BlockCoordinates cbCommand = command.m_blockCoordinate; cbCommand.y += 1;
        qDebug()<< "<DelayedCommands::Build_Pass_1> Set the delayed command block at "<< cbCommand.ToString();
    }

    return true;
}

bool DelayedCommands::ContainsCommand(const QString CommandId)
{
    return m_delayedCommands.contains(CommandId);
}

const DelayedCommand &DelayedCommands::GetCommand(const QString &CommandId)
{
    return m_delayedCommands[CommandId];
}

void DelayedCommands::FromJson(const QJsonObject &obj)
{
    QJsonObject delayed_commands_obj = obj["delayed_commands"].toObject();
    QStringList keys = delayed_commands_obj.keys();

    // Add the integers to the map
    for(int i = 0 ; i < keys.size() ; i++) {
        QString key = keys[i];
        if(m_delayedCommands.contains(key))    qWarning()<< "<DelayedCommands::FromJson> There is already a delayed command with the name : " + key + ".";
        else if(!delayed_commands_obj[key].isObject()) qWarning()<< "<DelayedCommands::FromJson> The 'delayed_commands' childs must be JsonObject. Key : "+ key;
        else if(m_delayedCommands.size() >= 256)  qWarning() << "<DelayedCommands::FromJson> You have reached the maximum number of delayed commands which is 256. Skiping the delayed command : " << key;
        else {
            // Get one single quest
            QJsonObject delayed_command_js = delayed_commands_obj[key].toObject();
            // Create this quest from json
            DelayedCommand delayed_command;
            if(delayed_command.FromJson(delayed_command_js)) {
                m_delayedCommands.insert(key, delayed_command);
                qDebug()<< "<DelayedCommands::FromJson> Registered the delayed command: " << key << ".";
            }
        }
    }
}

QJsonObject DelayedCommands::GetJsonTemplateFile()
{
    QJsonObject rootObj;
    QJsonObject delayed_commands;

    QJsonObject delayed_command;
    delayed_command.insert("delay_in_ticks" , 60);
    QJsonArray commands;
    commands.push_back("/say hello");

    delayed_command.insert("commands", commands);
    delayed_commands.insert("wait_3_seconds", delayed_command);
    rootObj.insert("delayed_commands", delayed_commands);
    return rootObj;
}

bool DelayedCommand::FromJson(const QJsonObject &object)
{
    // Get the delay in ticks
    // Default value is 0
    m_delayInTicks = object["delay_in_ticks"].toInt(0);

    // Get the commands
    QJsonArray commands = object["commands"].toArray();

    // In case there are no commands
    if(commands.isEmpty()){
        qWarning() << "<DelayedCommand::FromJson> The delayed command must contains a 'commands' child array";
        return false;
    }
    for(int i = 0 ; i < commands.size() ; i++) {
        if(commands[i].isString()) {
            m_commands.push_back(commands[i].toString());
        }
        else {
            qWarning() << "<DelayedCommand::FromJson> The commands must be a string json value.";
        }
    }

    // Return if there was some valid commands found or not
    return !m_commands.isEmpty();
}
