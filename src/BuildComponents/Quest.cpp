#include "Quest.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QRandomGenerator>

#include "Project.h"
#include "Helpers/CommandsHelper.h"
#include "Helpers/JsonHelper.h"


QString RandomString() {
    int len = 12;
    QString str;
    str.resize(len);
    for (int s = 0; s < len ; ++s)
        str[s] = QChar('A' + char(QRandomGenerator::global()->generate()% ('Z' - 'A')));

    return str;
}



bool Quest::isUnlockable() const
{
    return m_isUnlockable;
}

const BlockCoordinates &Quest::unlockBlockCoordinates() const
{
    return m_unlockBlockCoordinates;
}

void Quest::setUnlockBlockCoordinates(const BlockCoordinates &newUnlockBlockCoordinates)
{
    m_unlockBlockCoordinates = newUnlockBlockCoordinates;
}

int Quest::questIndex() const
{
    return m_questIndex;
}

void Quest::setQuestIndex(int newQuestIndex)
{
    m_questIndex = newQuestIndex;
}

const QString &Quest::identifier() const
{
    return m_identifier;
}

bool Quest::FromJson(const QJsonObject &obj, const QString& identifier)
{
    // Set the identifier
    m_identifier = identifier;

    if(m_identifier.size() > 16) {
        qWarning()<< "<Quest::FromJson> The name of a quest must not be more than 16 characters (scoreboard maximum length). name : "<< m_identifier;
        qWarning()<< "<Quest::FromJson> Canceling the initialisation of the quest " << identifier<< ".";
        return false;
    }


    // IF the object contains a subquest id
    if(!obj.contains("subquests") && !obj["subquest"].isArray()) {
        qWarning()<< "<Quest::FromJson> The quest "<< identifier << " must contain a valid 'subquests' array.";
        qWarning()<< "<Quest::FromJson> Canceling the initialisation of the quest " << identifier<< ".";
        return false;
    }

    // Get the subquests array
    QJsonArray subquests = obj["subquests"].toArray();

    // IF the subquests are empty
    if(subquests.isEmpty()) {
        qWarning()<< "<Quest::FromJson> The subquest array from the quest "<< identifier << " is empty.";
        qWarning()<< "<Quest::FromJson> Canceling the initialisation of the quest " << identifier<< ".";
        return false;
    }

    // Loop through all the subquests
    for(int i = 0 ; i < subquests.size() ; i++) {
        if(!subquests[i].isObject()) {
            qWarning()<< "<Quest::FromJson> The subquest at index "<< i << "from the quest "<< identifier << " must be a json object.";
        }

        QJsonObject subquestObj = subquests[i].toObject();
        SubQuest subquest;

        // If we can parse the json object to a subquest
        if(subquest.FromJson(subquestObj, i)) {
            // Add the subquest to the list of subquests
            m_subquests.push_back(subquest);
        }
    }

    // Check if the quest is unlockable.
    // By default, this setting is set to true
    if(obj.contains("is_unlockable") && obj["is_unlockable"].isBool()) {
        m_isUnlockable = obj["is_unlockable"].toBool();
    }
    else m_isUnlockable = true;

    // Check if the quest is allow_chat_debug.
    // By default, this setting is set to true
    if(obj.contains("allow_chat_debug") && obj["allow_chat_debug"].isBool()) {
        allow_chat_debug = obj["allow_chat_debug"].toBool();
    }
    else m_isUnlockable = true;

    // Check if the quest contains a display_name.
    // By default, this setting is ""
    if(obj.contains("display_name") && obj["display_name"].isString()) {
        displayName = obj["display_name"].toString();
    }

    // Check if the quest contains a description.
    // By default, this setting is ""
    if(obj.contains("description") && obj["description"].isString()) {
        description = obj["description"].toString();
    }



    return true;
}

bool Quest::Build_Pass_1(Project *project)
{
    qDebug()<< "<Quest::Build_Pass_1> Building the pass 1 of the quest : "<< m_identifier << ".";

    // Add into the init function
    {
        // Create the scoreboard of the progress the quest
        project->InitFunction()->Command("scoreboard objectives add " +GetProgressScoreboardName() +" dummy " + GetProgressScoreboardName());
        // Set to the game controller object
        project->InitFunction()->Command("scoreboard players set " + project->GameControllerTarget() + " " + GetProgressScoreboardName() + " 0");
        project->InitFunction()->Space();
    }

    // Add into the reset function
    {
        project->ResetFunction()->Space();
        project->ResetFunction()->Comment("Reset the progress for the quest : " + m_identifier);
        project->ResetFunction()->Command("scoreboard players set " + project->GameControllerTarget() + " " + GetProgressScoreboardName() + " 0");
    }

    // Create the next.mcfunction
    McFunction next("quests/" + m_identifier + "/next" , project->BpPath());
    next.SetDescription("Select the next subquest from of the quest : "+ m_identifier + ".");
    next.Comment("Add one to the progress scoreboard of the current quest");
    next.Command("scoreboard players add " + project->GameControllerTarget() + " " + GetProgressScoreboardName() + " 1");
    next.Space();
    next.Comment("Debug the objective in the chat");
    next.Command("function quests/debug_only_desc");
    next.Space();



    // Debug in the chat
    McFunction debug("quests/" + m_identifier + "/debug" , project->BpPath());
    McFunction debug_only_desc("quests/" + m_identifier + "/debug_only_desc" , project->BpPath());
    debug.SetDescription("Debug the quest in the chat : "+ m_identifier + ".");
    debug_only_desc.SetDescription("Debug the quest in the chat (only the description of the subquest: "+ m_identifier + ".");

    if(displayName != "" && allow_chat_debug) {
        debug.Command(CommandsHelper::GetTellrawCommand("§e---------- §r" + project->Texts()->GetText(displayName) + " §e----------"));
        debug_only_desc.Command(CommandsHelper::GetTellrawCommand("§e---------- §r" + project->Texts()->GetText(displayName) + " §e----------"));
    }
    else if( allow_chat_debug){
        debug.Command(CommandsHelper::GetTellrawCommand("§e---------- §r" + project->Texts()->GetText(m_identifier) + " §e----------"));
        debug_only_desc.Command(CommandsHelper::GetTellrawCommand("§e---------- §r" + project->Texts()->GetText(m_identifier) + " §e----------"));
    }
    // If there is a description of the quest
    if(description != "" && allow_chat_debug) {
        debug.Command(CommandsHelper::GetTellrawCommand("§eDescription : §r" + project->Texts()->GetText(description)));
    }
    debug.Space();



    // Create the select.mcfunction
    McFunction select("quests/" + m_identifier + "/select" , project->BpPath());
    select.SetDescription("Set this quest to selected. Quest identifier : " + m_identifier);
    select.Comment("Cancel the current quest");
    select.Command("function quests/cancel_current");
    // Set the scoreboard to the select function
    select.Comment("Set the current_quest scoreboard to the index of this quest.");
    select.Command("scoreboard players set " + project->GameControllerTarget() + " current_quest " + QString::number(m_questIndex) );
    if(project->isDebugMode()) {
        select.Space();
        select.Comment("Debug mode");
        select.Command(CommandsHelper::GetTellrawDebugCommand("Quest selected : " + m_identifier));
    }
    select.Comment("Debug the objective in the chat");
    select.Command("function quests/debug");
    select.Space();

    select.Space();
    select.Comment("Select the good quest");



    // Create the function triggered when you have completed the quest
    McFunction completed("quests/" + m_identifier + "/completed" , project->BpPath());
    completed.SetDescription("Triggered when you have finished all the subquests. Quest identifier : " + m_identifier);
    if(project->isDebugMode()) {
        completed.Space();
        completed.Comment("Debug mode");
        completed.Command(CommandsHelper::GetTellrawDebugCommand("Quest completed : " + m_identifier));
    }


    // Create the function to trigger when you this quest is selected and you want to unselect it
    McFunction unselect("quests/" + m_identifier + "/uselect" , project->BpPath());
    unselect.SetDescription("Set this quest to un-selected. This needs to be triggered only when the quest is selected. Quest identifier : " + m_identifier);
    if(project->isDebugMode()) {
        unselect.Space();
        unselect.Comment("Debug mode");
        unselect.Command(CommandsHelper::GetTellrawDebugCommand("Quest unselected : " + m_identifier));
    }

    McFunction cancel_current("quests/" + m_identifier + "/cancel_current" , project->BpPath());
    cancel_current.SetDescription("Cancel the current subquest");

    // Loop through all the subquests of the quest
    for(int i = 0 ; i < m_subquests.size() ; i++) {
        // Get the subquest and build it
        SubQuest subquest = m_subquests[i];
        subquest.Build_Pass_1(project , m_identifier);


        // Select the corresponding file
        next.Command("execute " + project->GameControllerTarget(true) + ",scores={"+ GetProgressScoreboardName() + "=" + QString::number(i ) +"}] ~ ~ ~ function quests/" + m_identifier + "/" + subquest.identifier() + "/select");
        // Select the corresponding subquest when selecting the quest
        select.Command("execute " + project->GameControllerTarget(true) + ",scores={"+ GetProgressScoreboardName() + "=" + QString::number(i) +"}] ~ ~ ~ function quests/" + m_identifier + "/" + subquest.identifier() + "/select");

        cancel_current.Command("execute " + project->GameControllerTarget(true) + ",scores={"+ GetProgressScoreboardName() + "=" + QString::number(i) +"}] ~ ~ ~ function quests/" + m_identifier + "/" + subquest.identifier() + "/cancel");
        completed.Command("execute " + project->GameControllerTarget(true) + ",scores={"+ GetProgressScoreboardName() + "=" + QString::number(i) +"}] ~ ~ ~ function quests/" + m_identifier + "/" + subquest.identifier() + "/completed");


        // Cancel the current quest
        unselect.Command("execute " + project->GameControllerTarget(true) + ",scores={"+ GetProgressScoreboardName() + "=" + QString::number(i) +"}] ~ ~ ~ function quests/" + m_identifier + "/" + subquest.identifier() + "/cancel");

        // If we allowed the chat debuging
        if(allow_chat_debug) {
            debug.Command("execute " + project->GameControllerTarget(true) + ",scores={"+ GetProgressScoreboardName() + "=" + QString::number(i) +"}] ~ ~ ~ function quests/" + m_identifier + "/" + subquest.identifier() + "/debug");
            debug_only_desc.Command("execute " + project->GameControllerTarget(true) + ",scores={"+ GetProgressScoreboardName() + "=" + QString::number(i) +"}] ~ ~ ~ function quests/" + m_identifier + "/" + subquest.identifier() + "/debug");
        }

    }

    // Remove the current quest scoreboard to -1
    unselect.Space();
    unselect.Comment("Set the current quest scoreboard to -1 to say there there are no more quest selected.");
    unselect.Command("scoreboard players set " + project->GameControllerTarget() + " current_quest -1");


    debug.Space();
    // Add the end of the debug file
    if(displayName != "" && allow_chat_debug) {
        debug.Command(CommandsHelper::GetTellrawCommand("§e" + QString((10 + 10 + displayName.size()) , '-' )));
        debug_only_desc.Command(CommandsHelper::GetTellrawCommand("§e" + QString((10 + 10 + displayName.size()) , '-' )));
    }
    else if(allow_chat_debug){
        debug.Command(CommandsHelper::GetTellrawCommand("§e" + QString((10 + 10 + m_identifier.size()) , '-' )));
        debug_only_desc.Command(CommandsHelper::GetTellrawCommand("§e" + QString((10 + 10 + m_identifier.size()) , '-' )));
    }


    debug_only_desc.Save();
    qDebug()<< "<Quest::Build_Pass_1>     Saved the on debug_only_desc quest file from the quest " <<  m_identifier << " in "<< debug_only_desc.GetPath();
    debug.Save();
    qDebug()<< "<Quest::Build_Pass_1>     Saved the on debug quest file from the quest " <<  m_identifier << " in "<< debug.GetPath();
    completed.Save();
    qDebug()<< "<Quest::Build_Pass_1>     Saved the on completed quest file from the quest " <<  m_identifier << " in "<< completed.GetPath();
    select.Save();
    qDebug()<< "<Quest::Build_Pass_1>     Saved the quest select file from the quest " <<  m_identifier << " in "<< select.GetPath();
    unselect.Save();
    qDebug()<< "<Quest::Build_Pass_1>     Saved the unselect quest file from the quest " <<  m_identifier << " in "<< unselect.GetPath();
    next.Save();
    qDebug()<< "<Quest::Build_Pass_1>     Saved the quest next file from the quest " <<  m_identifier << " in "<< next.GetPath();
    cancel_current.Save();
    qDebug()<< "<Quest::Build_Pass_1>     Saved the cancel current file from the quest " <<  m_identifier << " in "<< cancel_current.GetPath();

    return true;
}

QString Quest::GetProgressScoreboardName()
{
    QString GoodIdentifier = m_identifier;
    GoodIdentifier = GoodIdentifier.replace(" ", "_");

    return GoodIdentifier;
}


int SubQuest::getSubquestIndex() const
{
    return subquestIndex;
}

void SubQuest::setSubquestIndex(int newSubquestIndex)
{
    subquestIndex = newSubquestIndex;
}

const QString &SubQuest::identifier() const
{
    return m_identifier;
}



bool SubQuest::FromJson(const QJsonObject &object, const int& questId)
{
    subquestIndex = questId;

    // Set the identifier if there is one
    if(object.contains("identifier") && object["identifier"].isString()){
        m_identifier = object["identifier"].toString();
        m_identifier = m_identifier.replace(" " , "_");
    }
    else {
        m_identifier = RandomString();
    }
    qDebug()<< "<SubQuest::FromJson> Identifier of the subquest at index " << subquestIndex << "was set to "<< m_identifier;


    // The on_selected events
    on_selected = JsonHelper::GetCommandsList(object , "on_selected");
    if(!on_selected.isEmpty()) {
        qDebug()<< "<SubQuest::FromJson> Found some commands for the 'on_selected' event in the subquest " << m_identifier;
    }

    // The on_unselected events
    on_unselected = JsonHelper::GetCommandsList(object , "on_unselected");
    if(!on_unselected.isEmpty()) {
        qDebug()<< "<SubQuest::FromJson> Found some commands for the 'on_unselected' event in the subquest " << m_identifier;
    }

    // The on_unselected events
    on_canceled = JsonHelper::GetCommandsList(object , "on_canceled");
    if(!on_canceled.isEmpty()) {
        qDebug()<< "<SubQuest::FromJson> Found some commands for the 'on_canceled' event in the subquest " << m_identifier;
    }

    // The on_completed events
    on_completed = JsonHelper::GetCommandsList(object , "on_completed");
    if(!on_completed.isEmpty()) {
        qDebug()<< "<SubQuest::FromJson> Found some commands for the 'on_completed' event in the subquest " << m_identifier;
    }

    // Check if the description contains a description
    if(object.contains("description") && object["description"].isString()) {
        description = object["description"].toString();
        qDebug()<< "<SubQuest::FromJson> Description found for the subquest " << m_identifier << "description :" << description;
    }

    return true;
}

bool SubQuest::Build_Pass_1(Project *project, const QString &ParentQuestId)
{
    qDebug()<< "<SubQuest::Build_Pass_1> Building the pass 1 of the subquest : "<< m_identifier << "from the quest"<< ParentQuestId<< ".";

    McFunction completed("quests/" + ParentQuestId + "/" + m_identifier + "/completed" , project->BpPath());
    McFunction selected("quests/" + ParentQuestId + "/" + m_identifier + "/select" , project->BpPath());
    McFunction canceled("quests/" + ParentQuestId + "/" + m_identifier + "/cancel" , project->BpPath());
    McFunction debug("quests/" + ParentQuestId + "/" + m_identifier + "/debug" , project->BpPath());

    // Add the text into the debug
    // If there is a description
    if(description != "") {
        debug.Comment("Debug the description of the quest");
        debug.Command(CommandsHelper::GetTellrawCommand("§aObjective: §r" +project->Texts()->GetText(description)));
    }




    // If there are some on_completed events
    // -> Merge the events into the mcfunction
    if(!on_completed.isEmpty()) {
        completed.Header("On completed events :");
        CommandsHelper::ParseCommandListIntoFunction(on_completed , &completed , project);
    }
    if(project->isDebugMode()) {
        completed.Space();
        completed.Comment("Debug mode");
        completed.Command(CommandsHelper::GetTellrawDebugCommand("SubQuest completed : " + m_identifier));
    }


    // If there are some on_canceled events
    // -> Merge the events into the mcfunction
    if(!on_canceled.isEmpty()) {
        canceled.Header("On canceled events :");
        CommandsHelper::ParseCommandListIntoFunction(on_canceled, &canceled , project);
    }
    if(project->isDebugMode()) {
        canceled.Space();
        canceled.Comment("Debug mode");
        canceled.Command(CommandsHelper::GetTellrawDebugCommand("SubQuest canceled : " + m_identifier));
    }


    // If there are some on_selected events
    // -> Merge the events into the mcfunction
    if(!on_selected.isEmpty()) {
        selected.Header("On selected events :");
        CommandsHelper::ParseCommandListIntoFunction(on_selected, &selected , project);
    }
    if(project->isDebugMode()) {
        selected.Space();
        selected.Comment("Debug mode");
        selected.Command(CommandsHelper::GetTellrawDebugCommand("SubQuest selected : " + m_identifier));
    }




    // If there are some on_unselected events
    // -> Merge the events into the mcfunction
    if(!on_unselected.isEmpty()) {
        // Add into the cancel
        canceled.Header("On un-selected events :");
        CommandsHelper::ParseCommandListIntoFunction(on_unselected, &canceled , project);
        // Add into the
        completed.Header("On un-selected events :");
        CommandsHelper::ParseCommandListIntoFunction(on_unselected, &completed , project);
    }




    //Save the functions
    completed.Save();
    selected.Save();
    canceled.Save();
    debug.Save();
    // Debug the functions paths
    qDebug()<< "<SubQuest::Build_Pass_1>          Saved the on canceled quest file from the quest" <<  ParentQuestId << "in the subquest"<< m_identifier <<"in" <<canceled.GetPath();
    qDebug()<< "<SubQuest::Build_Pass_1>          Saved the on completed quest file from the quest" <<  ParentQuestId << "in the subquest"<< m_identifier <<"in" <<completed.GetPath();
    qDebug()<< "<SubQuest::Build_Pass_1>          Saved the selected quest file from the quest" <<  ParentQuestId << "in the subquest"<< m_identifier <<"in" <<selected.GetPath();
    qDebug()<< "<SubQuest::Build_Pass_1>          Saved the debug quest file from the quest" <<  ParentQuestId << "in the subquest"<< m_identifier <<"in" << debug.GetPath();
    return true;
}
