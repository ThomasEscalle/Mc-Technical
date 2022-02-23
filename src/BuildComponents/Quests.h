#ifndef QUESTS_H
#define QUESTS_H

#include "BuildComponentBase.h"

#include <QObject>
#include <QMap>

#include "Quest.h"




///
/// \brief The Quests class
/// Represent a container for all the quests of the project
///
class Quests : public BuildComponentBase
{
    ///
    /// \brief m_quests The list of all the quests of the project linked to their identifier
    ///
    QMap<QString, Quest> m_quests;

public:
    ///
    /// \brief Quests Construct the Quest Object
    /// \param project The project that we are working on at the moment
    ///
    Quests(Project* project);


    ///
    /// \brief Build_Pass_0 Build the pass 0 of the quests
    /// This is assigning the different coordinates unlocked quests
    /// \return
    ///
    bool Build_Pass_0() override;

    ///
    /// \brief Build_Pass_1 Build the pass 1 of the quests
    /// This is building the cinematic itself and placing the command blocks
    /// \return
    ///
    bool Build_Pass_1() override;
public:

    ///
    /// \brief ContainsQuest Return if the quest contains a given quest
    /// \param questIdentifier The identifier of the quest to check
    /// \return If it exist or not
    ///
    bool ContainsQuest(const QString& questIdentifier);


    ///
    /// \brief GetQuest Get the quest with the given identifier
    /// \param questIdentifier The identifier of the quest
    /// \return The quest itself
    ///
    Quest GetQuest(const QString& questIdentifier);

public:
    ///
    /// \brief FromJson Fill the Object from a JsonObject
    /// \param obj The Object we are using for filling
    ///
    void FromJson(const QJsonObject &obj) override;

    ///
    /// \brief GetJsonTemplateFile Get the Json Prefab that will be saved in a template file.
    /// \return The generated Json Object
    ///
    QJsonObject GetJsonTemplateFile() override;
};

#endif // QUESTS_H
