#include "Ticks.h"
#include "Helpers/FileHelper.h"

#include "CommandParser.h"
#include "Minecraft/McFunction.h"
#include "CommandParser.h"

#include "Project.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>


Ticks::Ticks(Project *project)
    :BuildComponentBase(project)
{

}


void Ticks::Compile()
{
    qDebug()<< "Starting the compilation of the ticks.";

    CommandParser commandParser(m_project);

    // Get all the keys from the map of ticks
    QStringList keys = m_ticks.keys();
    for(int i = 0 ; i < keys.size() ; i++) {
        QString key = keys[i];

        // Get the tick itself
        Tick tick = m_ticks[key];

        // Create the function name
        QString functionName = "ticks/" + key;
        McFunction function(functionName , m_project->BpPath());

        // Loop throught the commands of the tick and parse them into mc commands
        for(int j = 0 ; j < tick.commands.size(); j++) {
            QString command = tick.commands[j];
            QString parsedCommand = commandParser.ParseCommand(command);
            if(parsedCommand != "") {
                function.Command(parsedCommand);
            }
        }
        function.Save();
        //OutputConsole::Instance()->Log("From Ticks : Compilation of function <b>" + functionName + "</b> was a success.",4 );

        // Assign the command block
        //Builder::Instance()->CommandBlocksArea()->SetBlockAt(tick.BlockCoord.x , tick.BlockCoord.y , tick.BlockCoord.z , "stone");
        //Builder::Instance()->CommandBlocksArea()->SetTickingCommandBlockAt(tick.BlockCoord.x , tick.BlockCoord.y + 1 , tick.BlockCoord.z , "function " + functionName , tick.delayInTicks);
        //OutputConsole::Instance()->Log("From Ticks : Commands blocks created for the tick : <b>" + key + "</b> at : " + tick.BlockCoord.ToString() + ".",4 );

    }
}

bool Ticks::Build_Pass_0()
{
    qInfo()<< "Starting Building the pass 0 of the ticks.";

    int x = 0;
    int z = 0;

    // Add to the informations
    m_project->InformationsFunction()->Header("Ticks coordinates :");


    // Loop throught all the commands
    QStringList keys = m_ticks.keys();
    for(int i = 0 ; i < keys.size() ; i++) {
        // The key of the command
        QString key = keys[i];

        // One delayed command
        Tick tick = m_ticks[key];

        // Set the coordinates
        BlockCoordinates coordinates;
        coordinates.x = x;
        coordinates.y = 3;
        coordinates.z = z;

        // Set the coordinate into delayed commands object
        tick.BlockCoord = coordinates;
        // Set the delayed commands object into the map
        m_ticks[key] = tick;

        qDebug()<< "<Ticks::Build_Pass_0> The tick "<< key << " was assigned the coordinates "<< m_ticks[key].BlockCoord.ToString();

        m_project->InformationsFunction()->Comment("    " + key + " has the coordinate : " + coordinates.ToString() );

        m_project->ResetFunction()->Comment("Reset the tick : " + key);
        m_project->ResetFunction()->Command("setblock " + coordinates.ToString()+ " stone 0");
        m_project->ResetFunction()->Space();

        // Add one on the coordinates
        x++;
        if(x >= 16) {
            z++;
            x=0;
        }
    }

    return true;
}

bool Ticks::Build_Pass_1()
{
    qInfo()<< "Starting Building the pass 1 of the ticks.";

    // Loop throught all the commands
    QStringList keys = m_ticks.keys();
    for(int i = 0 ; i < keys.size() ; i++) {
        // The key of the command
        QString key = keys[i];

        // One delayed command
        Tick tick = m_ticks[key];

        // The path of the command
        QString functionPath = "ticks/" + key;


        // Create the function
        McFunction tickFunction(functionPath , m_project->BpPath());
        tickFunction.SetDescription("Tick function : " + key);
        // Command parser
        CommandParser commandParser(m_project);
        for(int i = 0 ; i < tick.commands.size(); i++) {
            QString command = tick.commands[i];
            QString goodCommand = commandParser.ParseCommand(command);
            tickFunction.Command(goodCommand);
        }
        // Save the function
        tickFunction.Save();
        qDebug()<< "<Ticks::Build_Pass_1> Created the function "<< functionPath;

        // Add the stone block to the command block area
        m_project->CommandBlockArea()->SetBlockAt(tick.BlockCoord.x , tick.BlockCoord.y , tick.BlockCoord.z , "stone");
        m_project->CommandBlockArea()->SetTickingCommandBlockAt(tick.BlockCoord.x , tick.BlockCoord.y + 1 , tick.BlockCoord.z , "function " + functionPath , tick.delayInTicks);

        qDebug()<< "<Ticks::Build_Pass_1> Set the stone block at "<< tick.BlockCoord.ToString();
        BlockCoordinates cbCommand = tick.BlockCoord; cbCommand.y += 1;
        qDebug()<< "<Ticks::Build_Pass_1> Set the ticking command block at "<< cbCommand.ToString();
    }

    return true;
}

bool Ticks::ContainsTick(const QString TickId)
{
    return m_ticks.contains(TickId);
}

const Tick &Ticks::GetTick(const QString &TickId)
{
    return m_ticks[TickId];
}

void Ticks::FromJson(const QJsonObject &obj)
{
    // Get the object where the ticks are
    QJsonObject TicksObj = obj["ticks"].toObject();

    // Get all the ticking keys
    QStringList TicksKeys = TicksObj.keys();

    // Loop throught the ticks contained in the file
    for(int i = 0 ; i < TicksKeys.size() ; i++) {
        QString key = TicksKeys[i];
        // If there is already this tick in the map
        if(m_ticks.contains(key)) qWarning()<< "<Ticks::FromJson> There is already a tick with the name : " << key << ".";
        // If the tick is not a json object
        else if(!TicksObj[key].isObject()) qWarning()<< "<Ticks::FromJson> The 'ticks' childs must be JsonObject. Key : " << key << ".";
        // If you have more than 255 ticks
        else if(m_ticks.size() >= 256) qWarning()<< "You have reached the maximum number of ticks which is 256. Skiping the tick : " << key;
        else {
            // Get the tick it self and build a tick object
            QJsonObject TickObj = TicksObj[key].toObject();
            Tick tick;
            tick.BlockCoord.x = 0;
            tick.BlockCoord.y = 0;
            tick.BlockCoord.z = 0;
            tick.delayInTicks = TickObj["delay_in_ticks"].toInt(0);

            // If there is no commands array as a child
            if(!TickObj.contains("commands") || !TickObj["commands"].isArray() || TickObj["commands"].toArray().isEmpty()) {
                qWarning()<< "<Ticks::FromJson> Could not find a valid 'commands' JsonArray in the tick : Key : "<< key <<".";
            }
            else {
                // Loop throught all the items of the array
                QJsonArray commands = TickObj["commands"].toArray();
                // Loop throught all the commands
                for(int j = 0 ; j < commands.size() ; j++) {
                    // If the command is a valid string
                    if(commands[j].isString()) { tick.commands.push_back(commands[j].toString()); }
                    // If it is not valid
                    else  qWarning()<<"<Ticks::FromJson> Commands must be a string value : Key : "<< key <<".";
                }

                m_ticks.insert(key , tick);
                qDebug()<< "<Ticks::FromJson> Registed the tick " << key << ".";
            }
        }
    }
}

QJsonObject Ticks::GetJsonTemplateFile()
{
    QJsonObject root;

    QJsonObject ticks = QJsonObject();
    QJsonObject tick;
    QJsonArray commands;
    commands.push_back("/effect @a clear");
    commands.push_back("/say Hello");
    tick.insert("commands" , commands);
    tick.insert("delay_in_ticks" , 0);
    ticks.insert("check_for_player" , tick);
    root.insert("ticks" , ticks);

    return root;
}






