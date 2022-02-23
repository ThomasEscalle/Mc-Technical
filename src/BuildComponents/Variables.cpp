#include "Variables.h"

#include "Project.h"
#include <QJsonObject>
#include "Helpers/FileHelper.h"


Variables::Variables(Project *project)
    :BuildComponentBase(project)
{

}

bool Variables::Build_Pass_0()
{

    qInfo()<< "Starting Building the pass 0 of the variables.";

    // The coordinates
    int x = 0;
    int z = 0;

    // Add to the informations
    m_project->InformationsFunction()->Header("Booleans variable coordinates");

    // The keys of the booleans
    QStringList booleanKeys = BooleansVariables.keys();
    for(int i = 0 ; i < booleanKeys.size() ; i ++) {
        QString key = booleanKeys[i];

        // Get the boolean variable from the map
        BoolVariable var = BooleansVariables[key];
        // Assign  the coordinates
        var.BlockCoord.x = x;
        var.BlockCoord.y = 2;
        var.BlockCoord.z = z;
        // Set the variable into the map again
        BooleansVariables[key] = var;

        qDebug()<< "<Variables::Build_Pass_0> The Boolean Variable "<< key << " was assigned the coordinates "<< BooleansVariables[key].BlockCoord.ToString();

        // Add the coordinates to the informations
        m_project->InformationsFunction()->Comment("    " + key + " has the coordinates : " + BooleansVariables[key].BlockCoord.ToString());


        m_project->ResetFunction()->Comment("Reset the boolean variable : " + key);
        if(var.defaultValue == true) {
            m_project->ResetFunction()->Command("setblock " + var.BlockCoord.ToString() + " lapis_block 0");
        }
        else {
            m_project->ResetFunction()->Command("setblock " + var.BlockCoord.ToString()+ " stone 0");
        }
        m_project->ResetFunction()->Space();
        x++;
        if(x >= 16) {
            x = 0;
            z ++;
        }
    }


    // Add to the informations
    m_project->InformationsFunction()->Header("Integers variables names :");

    QStringList integerKeys = IntegersVariables.keys();
    for(int i = 0 ; i < integerKeys.size() ; i++) {
        // The key of the integer
        // This will be the name of the scoreboard
        QString key = integerKeys[i];

        // Add to the init function
        m_project->InitFunction()->Comment("Creating the integer variable : " + key);
        // Create the objective
        m_project->InitFunction()->Command("scoreboard objectives add " + key + " dummy " + key);
        // Set to the game controller object
        m_project->InitFunction()->Command("scoreboard players set " + m_project->GameControllerTarget() + " " + key + " " + QString::number(IntegersVariables[key]));
        m_project->InitFunction()->Space();
        qDebug()<< "<Variables::Build_Pass_0> The Integer Variable "<< key << " was registered in gameplay/init.mcfunction";


        // Add to the reset function
        m_project->ResetFunction()->Comment("Reset the integer variable : " + key);
        m_project->ResetFunction()->Command("scoreboard players set " + m_project->GameControllerTarget() + " " + key + " " + QString::number(IntegersVariables[key]));
        m_project->ResetFunction()->Space();
        qDebug()<< "<Variables::Build_Pass_0> The Integer Variable "<< key << " was reset in gameplay/reset.mcfunction";

        // Add the coordinates to the informations
        m_project->InformationsFunction()->Comment("    " + key + " has the scoreboard : " + key );

    }
    return true;
}

bool Variables::Build_Pass_1()
{
    qInfo()<< "Starting Building the pass 1 of the variables.";

    // The keys of the booleans
    QStringList booleanKeys = BooleansVariables.keys();

    for(int i = 0 ; i < booleanKeys.size() ; i ++) {
        QString key = booleanKeys[i];
        // Get the boolean variable
        BoolVariable variable = BooleansVariables[key];

        if(variable.defaultValue == true) {
            m_project->CommandBlockArea()->SetBlockAt(variable.BlockCoord.x , variable.BlockCoord.y , variable.BlockCoord.z , "lapis_lazuli");
        }
        else {
            m_project->CommandBlockArea()->SetBlockAt(variable.BlockCoord.x , variable.BlockCoord.y , variable.BlockCoord.z , "stone");
        }
        qDebug()<< "<Variables::Build_Pass_1> The Boolean variable "<< key << " was registered in the commands blocks structure.";
    }
    return true;
}


VariableType Variables::GetVariableType(const QString &key)
{
    if(IntegersVariables.contains(key)) return VariableType::integer;
    else if(BooleansVariables.contains(key)) return VariableType::boolean;
    else return VariableType::unknown;
}

void Variables::FromJson(const QJsonObject &obj)
{
    // Get the Integers sub keys
    QJsonObject Integers = obj["integers"].toObject();

    // Add the integers to the map
    QStringList IntKeys = Integers.keys();
    for(int i = 0 ; i < IntKeys.size() ; i++) {
        QString key = IntKeys[i];
        // If there is already an int with this name
        if(IntegersVariables.contains(key)) qWarning() << "<Variables::FromJson> Impossible to add the integer variable : " << key << ". This variable name already exists in the integer variables.";
        // IF there is already an int with this name
        else if(BooleansVariables.contains(key)) qWarning() << "<Variables::FromJson> Impossible to add the integer variable : <b>" << key << "</b>. This variable name already exists in the booleans variables.";
        // If you have reached the maximum of 256 integers
        else if(IntegersVariables.size() >= 256)  qWarning() << "<Variables::FromJson> You have reached the maximum number of Integers which is 256. Skiping the integer : " << key;
        // Add the integer variable to the list
        else {
            IntegersVariables.insert(key, Integers[key].toInt(0));
            qDebug()<< "<Variables::FromJson> Registered the integer variable " << key << ".";
        }
    }

    // Get the booleans sub object
    QJsonObject Booleans = obj["booleans"].toObject();

    // Loop throught all the booleans keys
    QStringList BoolKeys = Booleans.keys();
    for(int i = 0 ; i < BoolKeys.size() ; i++) {
        QString key = BoolKeys[i];
        // If there is already a boolean with this name
        if(BooleansVariables.contains(key)) qWarning() << "<Variables::FromJson> Impossible to add the boolean variable : " << key << ". This variable name already exists in the booleans variables.";
        // If there is already an integer variable with this name
        else if(IntegersVariables.contains(key)) qWarning() << "<Variables::FromJson> Impossible to add the boolean variable : " << key << ". This variable name already exists in the integers variables.";
        // If you have reached the maximum number of booleans variables
        else if(BooleansVariables.size() >= 512) qWarning() << "<Variables::FromJson> You have reached the maximum number of Booleans which is 512. Skiping the boolean : " << key;
        // Create the boolean variable and add it to the list
        else {
            BoolVariable var;
            var.defaultValue = Booleans[key].toBool(false);
            BooleansVariables.insert(key, var );
            qDebug()<< "<Variables::FromJson> Registered the boolean variable " << key << ".";
        }
    }
}

QJsonObject Variables::GetJsonTemplateFile()
{
    QJsonObject root;

    QJsonObject booleans;
    booleans.insert("is_in_cinematic" , true);
    root.insert("booleans" , booleans);

    QJsonObject integers;
    integers.insert("quest_progress" , 0);
    root.insert("integers" , integers);

    return root;
}
