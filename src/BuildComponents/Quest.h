#ifndef QUEST_H
#define QUEST_H

#include <QObject>
#include <QMap>

#include "Minecraft/BlockCoordinates.h"


// Represent one single subquest
class SubQuest {
    // The identifier of the quest
    QString m_identifier;

    // The index of the subquest
    int subquestIndex = 0;

    // The list of commands to trigger when the subquest is selected
    QStringList on_selected;

    // The list of commands to trigger when the quest is unselected
    QStringList on_unselected;

    // The list of commands to trigger when the quest is canceled
    QStringList on_canceled;

    // The list of commands to trigger when the quest is completed
    QStringList on_completed;

    // The description of the subquest quest (displayed when debugging)
    QString description = "";

public:
    // Initialise the quest from a json object
    bool FromJson(const QJsonObject& object, const int& questId);


    // Build the second pass of the quest
    bool Build_Pass_1(Project* project, const QString& ParentQuestId);
    int getSubquestIndex() const;
    void setSubquestIndex(int newSubquestIndex);
    const QString &identifier() const;
};




// Represent a quest that is containing multiple subquests
class Quest
{
    // The identifier of the quest
    QString m_identifier = "";
    // If the quest is unlockable
    bool m_isUnlockable = true;

    // The map that contains all the subquests
    QList<SubQuest> m_subquests;

    // The coordinates of the block if the quest is unlocked or not
    BlockCoordinates m_unlockBlockCoordinates;

    // The index of the quest
    // This is used for the current_quest storage
    int m_questIndex = 0;

    // The display name of the quest
    QString displayName = "";

    // The description of the quest
    QString description = "";

    // Allow if we allow the debuging of the quest
    bool allow_chat_debug = true;


public:
    // Initialise the quest from a json object
    bool FromJson(const QJsonObject& obj, const QString& identifier);


    // Build the second pass of the quest
    bool Build_Pass_1(Project* project);


private:
    // Get the name of the scoraboard that define the
    // progress of the current quest
    QString GetProgressScoreboardName();


public:
    // Getters and setters
    bool isUnlockable() const;
    const BlockCoordinates &unlockBlockCoordinates() const;
    void setUnlockBlockCoordinates(const BlockCoordinates &newUnlockBlockCoordinates);
    int questIndex() const;
    void setQuestIndex(int newQuestIndex);
    const QString &identifier() const;
};


#endif // QUEST_H
