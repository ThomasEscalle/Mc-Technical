#include "Project.h"

#include "Helpers/FileHelper.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QDebug>
#include <QDirIterator>

#include "Helpers/JsonHelper.h"




bool Project::isDebugMode() const
{
    return m_isDebugMode;
}

Project::Project():
      QObject(nullptr),
      m_cinematics(this),
      m_delayedCommands(this),
      m_quests(this),
      m_ticks(this),
      m_variables(this),
      m_coordinates(this),
      m_texts(this),

      m_resetFunction("", ""),
      m_initFunction ("", ""),
      m_informations ("", "")
{
    // Initialise the arrays :
    // The coordinates
    m_structureBlockCoordinates[0] = 0;
    m_structureBlockCoordinates[1] = 0;
    m_structureBlockCoordinates[2] = 0;
}

Project::~Project()
{

}

void Project::FillFromFile(const QString &path)
{

    // Open the project object
    QJsonObject ProjectObj = FileHelper::OpenJsonObject(path);
    // If the returned project object is empty
    if(ProjectObj.isEmpty()) {
        qWarning()<< "<Project::FillFromFile> Impossible to open the file as a JsonDocument. path: " << path;
        m_isValid = false;
        return;
    }


    // Get the description from the root json object
    if(!ProjectObj.contains("description") || !ProjectObj["description"].isObject()) {
        qWarning()<< "<Project::FillFromFile> The project Json Object does not contains a 'description' child object.";
        qWarning()<< "<Project::FillFromFile> Canceling the operation.";
        m_isValid = false;
        return;
    }
    QJsonObject description = ProjectObj["description"].toObject();


    // Get the project name from the description json object
    if(!description.contains("project_name") || !description["project_name"].isString()) {
        qWarning()<< "<Project::FillFromFile> The description Json Object does not contains a 'project_name' child string.";
        qWarning()<< "<Project::FillFromFile> Canceling compute the operation.";
        m_isValid = false;
        return;
    }
    m_projectName = description["project_name"].toString();
    qDebug()<<"<Project::FillFromFile> Project name set to : " << m_projectName;



    // Get the project behavior pack path from the description json object
    if(!description.contains("debug") || !description["debug"].isBool()) {

    }
    m_isDebugMode = description["debug"].toBool();
    qDebug()<<"<Project::FillFromFile> Debug mode set to : " << (m_isDebugMode ? "true" : "false");



    // Get if we want to build in debug mode
    if(!description.contains("behavior_pack_path") || !description["behavior_pack_path"].isString()) {
        qWarning()<< "<Project::FillFromFile> The description Json Object does not contains a 'behavior_pack_path' child string." ;
        qWarning()<< "<Project::FillFromFile> Canceling compute the operation." ;
        m_isValid = false;
        return;
    }
    m_BpPath = description["behavior_pack_path"].toString();
    qDebug()<<"<Project::FillFromFile> Behavior pack path set to : " << m_BpPath;





    // Get the world coordinates of the structure block that will load the commands blocks area
    if(!description.contains("commands_blocks_structure_origin") || !description["commands_blocks_structure_origin"].isArray()) {
        qWarning()<< "<Project::FillFromFile> The description Json Object does not contains a 'commands_blocks_structure_origin' child int array.";
        qWarning()<< "<Project::FillFromFile> Canceling compute the operation.";
        m_isValid = false;
        return;
    }
    QJsonArray commands_blocks_structure_origin = description["commands_blocks_structure_origin"].toArray();
    m_structureBlockCoordinates[0] = commands_blocks_structure_origin.at(0).toInt(0);
    m_structureBlockCoordinates[1] = commands_blocks_structure_origin.at(1).toInt(0);
    m_structureBlockCoordinates[2] = commands_blocks_structure_origin.at(2).toInt(0);
    qDebug()<<"<Project::FillFromFile> Structure block coordinates set to : ["
           << QString::number(m_structureBlockCoordinates[0]) << ", "
           << QString::number(m_structureBlockCoordinates[1]) << ", "
           << QString::number(m_structureBlockCoordinates[2]) << "].";


    // Get the project behavior pack path from the description json object
    if(!description.contains("behavior_pack_path") || !description["behavior_pack_path"].isString()) {
        qWarning()<< "<Project::FillFromFile> The description Json Object does not contains a 'game_controller_entity_target' child string.";
    }
    m_gameControllerTarget = description["game_controller_entity_target"].toString("@e[type=armor_stand,name=game_controller]");
    qDebug()<<"<Project::FillFromFile> Game controller entity target set to : " << m_gameControllerTarget;





    // Check if the behavior pack path exists
    if(!QDir().exists(BpPath())) {
        qWarning()<< "<Project::FillFromFile> The given behavior pack path does not exits! Trying to create it...";
        if(QDir().mkpath(BpPath()) == false) {
            qWarning()<< "<Project::FillFromFile> An error happened when creating the behavior pack path.";
            qWarning()<< "<Project::FillFromFile> Canceling compute the operation.";
            m_isValid = false;
            return;
        }
        else {
            qDebug()<<"<Project::FillFromFile> Behavior pack path succesfully created.";
        }
    }

    // Set the project path.
    qDebug()<<"<Project::FillFromFile> Project path set to : "<< path;
    m_projectPath = path;

    /*
    // Get the project_files from the root json object
    if(!ProjectObj.contains("project_files") || !ProjectObj["project_files"].isObject()) {
        qWarning()<< "<Project::FillFromFile> The project Json Object does not contains a 'project_files' child object." ;
        qWarning()<< "<Project::FillFromFile> Canceling compute the operation.</b>" ;
        m_isValid = false;
        return;
    }
    */

    /*
    // Create the different components from json
    if(!CreateComponents(ProjectObj["project_files"].toObject())) {
        qWarning()<< "<Project::FillFromFile> Canceling compute the operation.</b>" ;
        m_isValid = false;
        return;
    }
    */
    QString scan_path = QFileInfo(ProjectPath()).dir().path();
    ScanFolders(scan_path);


    // Create the functions from the Behavior pack path
    m_resetFunction = McFunction("gameplay/reset", m_BpPath);
    m_initFunction  = McFunction("gameplay/init" , m_BpPath);
    m_informations  = McFunction("gameplay/informations", m_BpPath);

    // Create the commands blocks area object
    m_commandBlocksArea = CommandBlocksArea();

    // Set the document to valid
    m_isValid = true;
}



void Project::Build()
{
    qInfo()<< "Building the project...";
    if(!Build_Pass_0()) return;
    if(!Build_Pass_1()) return;
    if(!Build_Pass_2()) return;
}

bool Project::CreateComponents(const QJsonObject &object)
{
    qDebug()<< "<Project::CreateComponents> Starting to create the different build components...";

    // Get the project directory path
    QString ProjectDirectory = QFileInfo(m_projectPath).dir().path();

    // Create a new variables handler
    qDebug()<< "";
    qInfo()<< "Starting analysing project variables.";
    if(object.contains("variables") && object["variables"].isArray()) {
        QJsonArray variables = object["variables"].toArray();
        // Loop throught all the variables entries
        for(int i = 0 ; i < variables.size() ; i++) {
            if(variables[i].isString()) {
                QString varPath = ProjectDirectory + "/" + variables[i].toString();
                qDebug()<< "<Project::CreateComponents> Variable entry found : " << varPath;
                m_variables.FromFile(varPath);
            }
            else {
                qWarning()<< "<Project::CreateComponents> The 'variables' childs must be a string. Error at 'variables[" << QString::number(i) << "]'";
            }
        }
    }

    // Create a new ticks handler
    qDebug()<< "";
    qInfo()<< "Starting analysing project ticks.";
    if(object.contains("ticks") && object["ticks"].isArray()) {
        QJsonArray ticks = object["ticks"].toArray();
        // Loop throught all the variables entries
        for(int i = 0 ; i < ticks.size() ; i++) {
            if(ticks[i].isString()) {
                QString varPath = ProjectDirectory + "/" + ticks[i].toString();
                qDebug()<< "<Project::CreateComponents> Tick entry found : " << varPath;
                m_ticks.FromFile(varPath);
            }
            else {
                qWarning()<< "<Project::CreateComponents> The 'ticks' childs must be a string. Error at 'ticks[" << QString::number(i) << "]'" ;
            }
        }
    }


    // Create a new coordinates handler
    qDebug()<< "";
    qInfo()<< "Starting analysing project coordinates.";
    if(object.contains("coordinates") && object["coordinates"].isArray()) {
        QJsonArray coordinates = object["coordinates"].toArray();
        // Loop throught all the variables entries
        for(int i = 0 ; i < coordinates.size() ; i++) {
            if(coordinates[i].isString()) {
                QString varPath = ProjectDirectory + "/" + coordinates[i].toString();
                qDebug()<< "<Project::CreateComponents> Coordinate entry found : " << varPath;
                m_coordinates.FromFile(varPath);
            }
            else {
                qWarning()<< "<Project::CreateComponents> The 'coordinates' childs must be a string. Error at 'coordinates[" << QString::number(i) << "]'" ;
            }
        }
    }


    // Create a new texts handler
    qDebug()<< "";
    qInfo()<< "Starting analysing project texts.";
    if(object.contains("texts") && object["texts"].isArray()) {
        QJsonArray coordinates = object["texts"].toArray();
        // Loop throught all the variables entries
        for(int i = 0 ; i < coordinates.size() ; i++) {
            if(coordinates[i].isString()) {
                QString varPath = ProjectDirectory + "/" + coordinates[i].toString();
                qDebug()<< "<Project::CreateComponents> Texts entry found : " << varPath;
                m_texts.FromFile(varPath);
            }
            else {
                qWarning()<< "<Project::CreateComponents> The 'texts' childs must be a string. Error at 'texts[" << QString::number(i) << "]'" ;
            }
        }
    }




    // Create the delayed commands handler
    qDebug()<< "";
    qInfo()<< "Starting analysing project delayed commands.";
    if(object.contains("delayed_commands") && object["delayed_commands"].isArray()) {
        QJsonArray delayed_commands = object["delayed_commands"].toArray();
        // Loop throught all the variables entries
        for(int i = 0 ; i < delayed_commands.size() ; i++) {
            if(delayed_commands[i].isString()) {
                QString varPath = ProjectDirectory + "/" + delayed_commands[i].toString();
                qDebug()<< "<Project::CreateComponents> Delayed commands entry found : " << varPath;
                m_delayedCommands.FromFile(varPath);
            }
            else {
                qWarning()<< "<Project::CreateComponents> The 'delayed_commands' childs must be a string. Error at 'delayed_commands[" << QString::number(i) << "]'" ;
            }
        }
    }


    // Create a new cinematics handler
    qDebug()<< "";
    qInfo()<< "Starting analysing project cinematics.";
    if(object.contains("cinematics") && object["cinematics"].isArray()) {
        QJsonArray cinematics = object["cinematics"].toArray();
        // Loop throught all the variables entries
        for(int i = 0 ; i < cinematics.size() ; i++) {
            if(cinematics[i].isString()) {
                QString varPath = ProjectDirectory + "/" + cinematics[i].toString();
                qDebug()<< "<Project::CreateComponents> Cinematics entry found : " << varPath;
                m_cinematics.FromFile(varPath);
            }
            else {
                qWarning()<< "<Project::CreateComponents> The 'cinematics' childs must be a string. Error at 'ticks[" << QString::number(i)<< "]'";
            }
        }
    }

    // Create a new quests handler
    qDebug()<< "";
    qInfo()<< "Starting analysing project quests.";
    if(object.contains("quests") && object["quests"].isArray()) {
        QJsonArray quests = object["quests"].toArray();
        // Loop throught all the variables entries
        for(int i = 0 ; i < quests.size() ; i++) {
            if(quests[i].isString()) {
                QString varPath = ProjectDirectory + "/" + quests[i].toString();
                qDebug()<< "<Project::CreateComponents> Quests entry found : " << varPath;
                m_quests.FromFile(varPath);
            }
            else {
                qWarning()<< "<Project::CreateComponents> The 'quests' childs must be a string. Error at 'ticks[" << QString::number(i) << "]'" ;
            }
        }
    }

    return true;
}

bool Project::ScanFolders(const QString &path)
{

    qDebug()<< "";

    // An iterator that loops throught all the files and the sub file
    QDirIterator iterator(path, QDir::Files | QDir::NoDotAndDotDot , QDirIterator::Subdirectories);
    while(iterator.hasNext()) {
        // The path of the file to scan
        QString filePath = iterator.next();
        QFileInfo info(filePath);


        qDebug()<< "<Project::ScanFolders> Scanning the file "<< filePath;

        // If the path of the file is different than
        // the project that we are currently computin

        // If the file is a json file
        if(filePath != ProjectPath() && info.suffix().toLower() == "json")
        {
            // Get the file as a json object
            QJsonObject obj = FileHelper::OpenJsonObject(filePath);
            if(!obj.isEmpty()) {

                // If the object contains ticks
                if(obj.contains("booleans")&& obj["booleans"].isObject()) {
                    qDebug()<< "<Project::ScanFolders> Booleans were found in the file "<< filePath;
                    Variables()->FromJson(obj);
                }
                // If the object contains ticks
                else if(obj.contains("integers")&& obj["integers"].isObject()) {
                    qDebug()<< "<Project::ScanFolders> Integers were found in the file "<< filePath;
                    Variables()->FromJson(obj);
                }
                // If the object contains ticks
                if(obj.contains("ticks")&& obj["ticks"].isObject()) {
                    qDebug()<< "<Project::ScanFolders> Ticks were found in the file "<< filePath;
                    Ticks()->FromJson(obj);
                }
                // If the object contains texts
                if(obj.contains("texts")&& obj["texts"].isObject()) {
                    qDebug()<< "<Project::ScanFolders> Texts were found in the file "<< filePath;
                    Texts()->FromJson(obj);
                }

                // If the object contains coordinates
                if(obj.contains("delayed_commands")&& obj["delayed_commands"].isObject()) {
                    qDebug()<< "<Project::ScanFolders> Delayed commands were found in the file "<< filePath;
                    DelayedCommands()->FromJson(obj);
                }

                // If the object contains coordinates
                if(obj.contains("coordinates")&& obj["coordinates"].isObject()) {
                    qDebug()<< "<Project::ScanFolders> coordinates were found in the file "<< filePath;
                    Coordinates()->FromJson(obj);
                }

                // If the object contains quests
                if(obj.contains("quests")&& obj["quests"].isObject()) {
                    qDebug()<< "<Project::ScanFolders> Quests were found in the file "<< filePath;
                    Quests()->FromJson(obj);
                }

                // If the object contains cinematics
                if(obj.contains("cinematics")&& obj["cinematics"].isObject()) {
                    qDebug()<< "<Project::ScanFolders> Cinematics were found in the file "<< filePath;
                    Cinematics()->FromJson(obj);
                }


            }
        }
        qDebug()<< "";
    }

    return true;
}

bool Project::Build_Pass_0()
{
    qInfo()<< "Building the pass 0 of the project...";

    // Add the structure to load in the init function
    m_initFunction.Comment("Load the commands blocks structure");
    m_initFunction.Command("structure load commands_blocks_area " + QString::number(m_structureBlockCoordinates[0]) + " " + QString::number(m_structureBlockCoordinates[1]) + " " + QString::number(m_structureBlockCoordinates[2]));
    m_initFunction.Space();

    // Build all the components here
    if(!m_variables.Build_Pass_0()) return false;
    if(!m_ticks.Build_Pass_0()) return false;
    if(!m_delayedCommands.Build_Pass_0()) return false;
    if(!m_cinematics.Build_Pass_0()) return false;
    if(!m_quests.Build_Pass_0()) return false;

    qDebug()<< "<Project::Build_Pass_0> Finished building the pass 0s of the project.";
    qDebug()<< "";
    return true;
}

bool Project::Build_Pass_1()
{
    qInfo()<< "Building the pass 1 of the project...";

    // Build all the components here
    if(!m_variables.Build_Pass_1()) return false;
    if(!m_ticks.Build_Pass_1()) return false;
    if(!m_delayedCommands.Build_Pass_1()) return false;
    if(!m_cinematics.Build_Pass_1()) return false;
    if(!m_quests.Build_Pass_1()) return false;

    qDebug()<< "<Project::Build_Pass_1> Finished building the pass 1 of the project.";
    qDebug()<< "";
    return true;
}

bool Project::Build_Pass_2()
{
    qInfo()<< "Building the pass 2 of the project...";

    // Build all the components here
    if(!m_variables.Build_Pass_2()) return false;
    if(!m_ticks.Build_Pass_2()) return false;
    if(!m_delayedCommands.Build_Pass_2()) return false;
    if(!m_cinematics.Build_Pass_2()) return false;
    if(!m_quests.Build_Pass_2()) return false;

    // Save the functions
    m_resetFunction.Save();
    m_initFunction.Save();
    m_informations.Save();

    // Save the commands blocks area
    m_commandBlocksArea.Save(m_BpPath + "/structures/commands_blocks_area.mcstructure");

    qDebug()<< "<Project::Build_Pass_2> Finished building the pass 2 of the project.";
    qDebug()<< "";
    return true;
}

// Getters
bool Project::isValid() const { return m_isValid; }
const QString &Project::ProjectName() const { return m_projectName; }
const QString &Project::ProjectPath() const { return m_projectPath; }
const QString &Project::BpPath() const { return m_BpPath; }
const int &Project::StructureBlockCoordinate(int index) const { return m_structureBlockCoordinates[index]; }
QString Project::GameControllerTarget(bool removeLastCharacter) const {
    if(removeLastCharacter == false) {
        return m_gameControllerTarget;
    }
    else {
        QString base_target = GameControllerTarget();
        if(base_target[base_target.size()-1] == "]"){
            base_target = base_target.remove(base_target.size()-1, 1);
        }
        return base_target;
    }
}

Cinematics *Project::Cinematics() { return &m_cinematics; }
DelayedCommands *Project::DelayedCommands() { return &m_delayedCommands; }
Quests *Project::Quests() { return &m_quests; }
Ticks *Project::Ticks() { return &m_ticks; }
Variables *Project::Variables() { return &m_variables; }
Coordinates *Project::Coordinates() { return &m_coordinates; }
Texts *Project::Texts() { return &m_texts; }

McFunction *Project::ResetFunction() { return &m_resetFunction; }
McFunction *Project::InitFunction() { return &m_initFunction; }
McFunction *Project::InformationsFunction() { return &m_informations; }

CommandBlocksArea *Project::CommandBlockArea() { return &m_commandBlocksArea; }


