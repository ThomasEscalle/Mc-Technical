#include "Quests.h"

#include "Helpers/FileHelper.h"
#include "Project.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>


Quests::Quests(Project *project)
    :BuildComponentBase(project)
{

}

bool Quests::Build_Pass_0()
{
    qInfo()<< "Starting Building the pass 0 of the quests.";
    int unlockX = 0;
    int unlockZ = 0;

    // Loop throught all the keys of the quests
    QStringList keys = m_quests.keys();
    for(int i = 0 ; i < keys.size() ; i++) {
        QString key = keys[i];

        // Get the quest
        Quest quest = m_quests[key];

        // If the quest is unlockable
        if(quest.isUnlockable()) {
            // The new coordinates of the unlock block
            BlockCoordinates coord;
            coord.x = unlockX;
            coord.y = 5;
            coord.z = unlockZ;

            // Apply the coordinates to the quest
            quest.setUnlockBlockCoordinates(coord);

            // Debug
            qDebug()<< "<Quests::Build_Pass_0> The quest "<< key << " was assigned the unlock coordinates "<< coord.ToString();

            // Add one on the coordinates
            unlockX++;
            if(unlockX >= 16) {
                unlockZ++;
                unlockX=0;
            }
        }

        // Set the index of the quest
        quest.setQuestIndex(i);

        // Set the quest back to the map
        m_quests[key] = quest;
    }


    // Set the scoreboard to define the current quest
    // Create the objective
    m_project->InitFunction()->Command("scoreboard objectives add current_quest dummy current_quest");
    // Set to the game controller object
    // -1 means there is no quest selected
    m_project->InitFunction()->Command("scoreboard players set " + m_project->GameControllerTarget() + " current_quest -1");
    m_project->InitFunction()->Space();


    // Add into the reset function
    m_project->ResetFunction()->Space();
    m_project->ResetFunction()->Comment("Set that there is no more current quest selected");
    m_project->ResetFunction()->Command("scoreboard players set " + m_project->GameControllerTarget() + " current_quest -1");

    return true;
}

bool Quests::Build_Pass_1()
{
    qInfo()<< "Starting Building the pass 0 of the quests.";


    // Create the quests/next.mcfunction
    McFunction next("quests/next" , m_project->BpPath());
    next.SetDescription("Root function used to select the next subquest from the selected quest.");
    next.Comment("Cancel the current subquest");
    next.Command("function quests/cancel_current");
    next.Space();

    McFunction cancel_current("quests/cancel_current", m_project->BpPath());
    cancel_current.SetDescription("When you want to cancel the current quest. Used when canceling, or when triggering the next quest");

    McFunction debug("quests/debug", m_project->BpPath());
    McFunction debug_only_desc("quests/debug_only_desc", m_project->BpPath());

    // Loop throught all the keys of the quests
    QStringList keys = m_quests.keys();
    for(int i = 0 ; i < keys.size() ; i++) {
        QString key = keys[i];

        // Get the quest
        Quest quest = m_quests[key];

        // Build the quest itself
        quest.Build_Pass_1(m_project);

        // Trigger the corresponding next quest
        next.Comment("If the current quest is equal to " + QString::number(i));
        next.Command("execute " + m_project->GameControllerTarget(true) + ",scores={current_quest=" + QString::number(i) +"}] ~ ~ ~ function quests/" + key + "/next");

        // Trigger the corresponding cancel quest command
        cancel_current.Comment("If the current quest is equal to " + QString::number(i));
        cancel_current.Command("execute " + m_project->GameControllerTarget(true) + ",scores={current_quest=" + QString::number(i) +"}] ~ ~ ~ function quests/" + key + "/cancel_current");

        debug.Comment("If the current quest is equal to " + QString::number(i));
        debug.Command("execute " + m_project->GameControllerTarget(true) + ",scores={current_quest=" + QString::number(i) +"}] ~ ~ ~ function quests/" + key + "/debug");

        debug_only_desc.Comment("If the current quest is equal to " + QString::number(i));
        debug_only_desc.Command("execute " + m_project->GameControllerTarget(true) + ",scores={current_quest=" + QString::number(i) +"}] ~ ~ ~ function quests/" + key + "/debug_only_desc");

    }

    // Save the debug.mcfunction
    debug_only_desc.Save();
    qDebug()<< "<Quests::Build_Pass_1> Saved the global debug_only_desc file in "<< debug_only_desc.GetPath();
    // Save the debug.mcfunction
    debug.Save();
    qDebug()<< "<Quests::Build_Pass_1> Saved the global debug file in "<< debug.GetPath();
    // Save the next.mcfunction
    next.Save();
    qDebug()<< "<Quests::Build_Pass_1> Saved the global next file in "<< next.GetPath();
    // Save the cancel_current.mcfunction
    cancel_current.Save();
    qDebug()<< "<Quests::Build_Pass_1> Saved the global cancel_current file in "<< cancel_current.GetPath();

    return true;
}

bool Quests::ContainsQuest(const QString &questIdentifier)
{
    return m_quests.contains(questIdentifier);
}

Quest Quests::GetQuest(const QString &questIdentifier)
{
    return m_quests[questIdentifier];
}

void Quests::FromJson(const QJsonObject &obj)
{
    // Get the object where the ticks are
    QJsonObject QuestsObj = obj["quests"].toObject();


    // Add the integers to the map
    QStringList QuestsKeys = QuestsObj.keys();
    for(int i = 0 ; i < QuestsKeys.size() ; i++) {
        QString key = QuestsKeys[i];
        if(m_quests.contains(key))    qWarning()<< "<Quests::FromJson> There is already a quest with the name : " + key + ".";
        else if(!QuestsObj[key].isObject()) qWarning()<< "<Quests::FromJson> The 'Quests' childs must be JsonObject. Key : "+ key;
        else {
            // Get one single quest
            QJsonObject questJs = QuestsObj[key].toObject();
            // Create this quest from json
            Quest quest;
            if(quest.FromJson(questJs, key)) {
                m_quests.insert(key, quest);
                qDebug()<< "<Quests::FromJson> Registered the quest: " << key << ".";
            }
        }
    }
}

QJsonObject Quests::GetJsonTemplateFile()
{
    QJsonObject root;
    QJsonObject quests;

    QJsonObject quest_tuto;
    // Items in the quest
    quest_tuto.insert("is_unlockable" , true);
    quest_tuto.insert("description" , "");
    quest_tuto.insert("description" , "");
    quest_tuto.insert("allow_chat_debug" , true);

    QJsonArray subquests;
    QJsonObject subquest;
    // Items in the subquest
    subquest.insert("identifier" , "talk_to_the_guard");
    subquest.insert("on_selected" , QJsonArray());
    subquest.insert("on_unlocked" , QJsonArray());
    subquest.insert("on_completed" , QJsonArray());
    subquest.insert("on_canceled" , QJsonArray());
    subquest.insert("description" , "");

    subquests.push_back(subquest);
    quest_tuto.insert("subquests", subquests);
    quests.insert("tuto", quest_tuto);
    root.insert("quests" , quests);
    return root;
}


