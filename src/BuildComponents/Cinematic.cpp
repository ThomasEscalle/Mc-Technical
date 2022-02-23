#include "Cinematic.h"

#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>

#include "CommandParser.h"
#include "Minecraft/McFunction.h"
#include "Project.h"


bool Cinematic::FromJson(const QJsonObject &obj)
{
    // if the object is empty
    if(obj.isEmpty()) {
        qWarning()<< "<Cinematic::FromJson> The Json Object should not be empty. cinematic id : "<< identifier;
        return false;
    }

    // Set the length of the cinematic
    if(obj.contains("length") && obj["length"].isDouble()) {
        length = obj["length"].toDouble(12);
    }

    // Set the from and two keys
    if(obj.contains("keys") && obj["keys"].isObject()) {
        QJsonObject keys = obj["keys"].toObject();
        // Get the 'from' coordinate and parse it to the start position as string
        if(keys.contains("from")&& keys["from"].isString()) {
            startPosition.FromStringCoordinates(keys["from"].toString());
        }
        else {
            qWarning()<< "<Cinematic::FromJson> The 'keys' object from the cinematics must contain a 'from' sub string that represend the start coordinates or the cutscene. cinematic id : "<< identifier;
            return false;
        }

        // Get the 'to' coordinate and parse it to the start position as string
        if(keys.contains("to")&& keys["to"].isString()) {
            endPosition.FromStringCoordinates(keys["to"].toString());
        }
        else {
            qWarning()<< "<Cinematic::FromJson> The 'keys' object from the cinematics must contain a 'to' sub string that represend the end coordinates of the cutscene. cinematic id : "<< identifier;
            return false;
        }
    }
    else {
        qWarning()<< "<Cinematic::FromJson> The Json Object has to contain a 'keys' sub object. cinematic id : "<< identifier;
        return false;
    }

    // Get the take shortcut boolean
    take_shortcut = obj["take_shortcut"].toBool(true);

    // Get the start events
    if(obj.contains("on_start") && obj["on_start"].isArray()) {
        QJsonArray on_start = obj["on_start"].toArray();
        for(int i = 0 ; i < on_start.size(); i++){
            QString command = on_start[i].toString("");
            if(command != "") start_commands.push_back(command);
        }
    }

    // Get the end events
    if(obj.contains("on_end") && obj["on_end"].isArray()) {
        QJsonArray on_end = obj["on_end"].toArray();
        for(int i = 0 ; i < on_end.size(); i++){
            QString command = on_end[i].toString("");
            if(command != "") end_commands.push_back(command);
        }
    }

    // Get the runtime events
    if(obj.contains("runtime_commands")&& obj["runtime_commands"].isObject() ) {
        QJsonObject runtime_commands = obj["runtime_commands"].toObject();
        QStringList runtime_commands_keys = runtime_commands.keys();
        for(int i = 0 ; i < runtime_commands_keys.size() ; i++) {
            QString key = runtime_commands_keys[i];
            bool isConverted = false;
            int position = key.toInt(&isConverted);
            if(isConverted) {
                QJsonArray array = runtime_commands[key].toArray();
                if(array.size() == 0) {

                }
                else {

                    QStringList commands;
                    for(int j = 0 ; j < array.size() ; j++) {
                        if(array[j].isString()) {
                            commands.push_back(array[j].toString());
                        }
                    }
                    if(commands.size() >= 1) {
                        m_runtimeCommands.insert(position, commands);
                    }
                }
            }
            else {
                qWarning()<< "<Cinematic::FromJson> Impossible to read the runtime position "<< key<< " as an integer. cinematic id : "<< identifier;
            }
        }
    }
    return true;
}

bool Cinematic::Build_Pass_1(Project *project)
{
    qDebug()<< "<Cinematic::Build_Pass_1> Starting building the cinematic "<< identifier;

    int number_of_ticks = length * 20;
    qDebug()<< "<Cinematic::Build_Pass_1> The cinematic "<< identifier<< "contains "<< QString::number(number_of_ticks)<< "ticks.";

    // Get the base target
    QString base_target = project->GameControllerTarget();
    if(base_target[base_target.size()-1] == "]"){
        base_target = base_target.remove(base_target.size()-1, 1);
    }


    // Building the exec commands
    {
        // The number of the file that we have executed
        int execute_index = 0;
        // The current number of functions in the current file
        int functions_in_file = 0;
        int old_i = 0;

        // The execute function
        McFunction execFunction("cinematics/" + identifier + "/exec", project->BpPath());
        execFunction.Comment("Redirect to the correct teleport function.");
        execFunction.SetDescription("Function executed every ticks of the cinematic: " + identifier);

        McFunction currentExecFunction("cinematics/" + identifier + "/exec/" + QString::number(execute_index) , project->BpPath());

        // Loop through all the ticks
        for(int i = 0 ; i <= number_of_ticks ; i++) {
            // Get the t factor for the linear interpolation
            double t = (double)i / (double)number_of_ticks;

            // Add one to the functions in file
            functions_in_file ++;

            // Get the interpolated position
            CinematicPositions pos = CinematicPositions::LerpPosition(startPosition , endPosition , t);

            // Add the command into the current executing function
            currentExecFunction.Command("execute " + base_target + ",scores={cine="+QString::number(i) + "}] ~ ~ ~ tp @a " + pos.ToString());

            // If there is a runtime command at this tick
            if(m_runtimeCommands.contains(i)) {
                QStringList commands = m_runtimeCommands[i];
                CommandParser parser(project);
                // Loop throught all the command and parse them
                for(int j=0; j < commands.size() ; j++) {
                    QString goodCommand = parser.ParseCommand(commands[j]);
                    if(goodCommand != "" ) {
                        // Add the command into the current executing function
                        currentExecFunction.Command("execute " + base_target + ",scores={cine="+QString::number(i) + "}] ~ ~ ~ " + goodCommand);
                    }
                }
            }

            // If there are more than 80 functions in the current function
            if(functions_in_file >= 80) {
                old_i++;
                // Save the execute function
                currentExecFunction.Save();
                qDebug()<< "<Cinematic::Build_Pass_1> Saved the execution file in "<< execFunction.GetPath();
                execFunction.Command("execute " + base_target + ",scores={cine="+QString::number(old_i) + ".." + QString::number(i) + "}] ~ ~ ~ function cinematics/" + identifier + "/exec/" + QString::number(execute_index));

                // Set a new exec function
                execute_index ++;
                functions_in_file = 0;

                currentExecFunction = McFunction("cinematics/" + identifier + "/exec/" + QString::number(execute_index) , project->BpPath());

                old_i = i;
            }
        }

        if(!currentExecFunction.IsEmpty()) {
            qDebug()<< "<Cinematic::Build_Pass_1> Saved the execution file in "<< execFunction.GetPath();
            execFunction.Command("execute " + base_target + ",scores={cine="+QString::number(old_i + 1 ) + ".." + QString::number(number_of_ticks) + "}] ~ ~ ~ function cinematics/" + identifier + "/exec/" + QString::number(execute_index));
            currentExecFunction.Save();
        }

        execFunction.Space();
        execFunction.Comment("Add the scoreboard to the game controller");
        execFunction.Command("scoreboard players add " + project->GameControllerTarget() + " cine 1");
        execFunction.Space();
        execFunction.Comment("Test for the last frame ");
        execFunction.Command("execute " + base_target + ",scores={cine=" + QString::number(number_of_ticks) +"}] ~ ~ ~ function cinematics/" + identifier + "/finish");

        execFunction.Save();
        qDebug()<< "<Cinematic::Build_Pass_1> Saved the main execution file in "<< execFunction.GetPath();
    }


    // Building the start commands
    {
        McFunction start("cinematics/" + identifier + "/start", project->BpPath());
        start.SetDescription("Function automaticaly executed when the cinematic is starting to initialise everything");

        start.Comment("Set the redstone block to start the execution");
        start.Command("setblock "+ BlockCoord.ToString(true , project) + " redstone_block 0");

        start.Space();
        start.Comment("Scoreboards initialisation.");
        start.Command("scoreboard players set " + project->GameControllerTarget() + " cine 0");

        start.Space();
        start.Comment("Add effects to the players");
        start.Command("effect @a invisibility 600 30 true");
        start.Command("effect @a resistance 255 255 true");

        // Add the start commands
        if(start_commands.size() > 0) {
            CommandParser parser(project);

            start.Space(2);
            start.Header("on_start commands : ");
            start.Space(1);

            // Add the on_start events
            for(int i = 0 ; i < start_commands.size() ; i++) {
                QString goodCommand = parser.ParseCommand(start_commands[i]);
                start.Command(goodCommand);
            }
        }

        start.Save();
        qDebug()<< "<Cinematic::Build_Pass_1> Saved the start cinematic file in "<< start.GetPath();
    }


    // Create the finish.mcfunction
    {
        McFunction finish("cinematics/" + identifier + "/finish", project->BpPath());
        finish.SetDescription("Function automaticaly executed when the cinematic " + identifier+" is finished.");

        finish.Comment("Set the stone block to finish the ticking");
        finish.Command("setblock "+ BlockCoord.ToString(true , project) + " stone 0");

        finish.Space();
        finish.Comment("Remove the effects from the player");
        finish.Command("effect @a clear");

        finish.Space();
        finish.Comment("Reset the scoreboards");
        finish.Command("scoreboard players reset " + project->GameControllerTarget() + " cine");

        // Add the finish commands
        if(end_commands.size() > 0) {
            CommandParser parser(project);
            finish.Space(2);
            finish.Header("on_end commands : ");
            finish.Space(1);

            // Add the on_start events
            for(int i = 0 ; i < end_commands.size() ; i++) {
                QString goodCommand = parser.ParseCommand(end_commands[i]);
                finish.Command(goodCommand);
            }
        }

        finish.Save();
        qDebug()<< "<Cinematic::Build_Pass_1> Saved the finish cinematic file in "<< finish.GetPath();
    }


    return true;
}

void Cinematic::CinematicPositions::FromStringCoordinates(const QString &coordinates)
{
    // Split the coordinates
    QStringList splitedCoordinates = coordinates.split(" ");

    // If the size is over 1
    // Fill the x
    if(splitedCoordinates.size() >= 1) {
        x = splitedCoordinates.takeFirst().toFloat();
    }
    // If the size is over 1
    // Fill the y
    if(splitedCoordinates.size() >= 1) {
        y = splitedCoordinates.takeFirst().toFloat();
    }
    // If the size is over 1
    // Fill the z
    if(splitedCoordinates.size() >= 1) {
        z = splitedCoordinates.takeFirst().toFloat();
    }
    // If the size is over 1
    // Fill the xRot
    if(splitedCoordinates.size() >= 1) {
        xRot = splitedCoordinates.takeFirst().toFloat();
    }
    // If the size is over 1
    // Fill the yRot
    if(splitedCoordinates.size() >= 1) {
        yRot = splitedCoordinates.takeFirst().toFloat();
    }
}

Cinematic::CinematicPositions Cinematic::CinematicPositions::LerpPosition(CinematicPositions from, CinematicPositions to, double t)
{
    // Lerp = double result = a + (t * ( b - a));
    CinematicPositions result;
    result.x = from.x + (t * (to.x - from.x));
    result.y = from.y + (t * (to.y - from.y));
    result.z = from.z + (t * (to.z - from.z));

    result.xRot = from.xRot + (t * (to.xRot - from.xRot));
    result.yRot = from.yRot + (t * (to.yRot - from.yRot));

    return result;
}


QString Cinematic::CinematicPositions::ToString()
{
    return QString::number(x , 'f', decimalsDouble)  + " " +
            QString::number(y , 'f', decimalsDouble) + " " +
            QString::number(z , 'f', decimalsDouble) + " " +
            QString::number(xRot , 'f', decimalsDouble) + " " +
            QString::number(yRot , 'f', decimalsDouble);
}











