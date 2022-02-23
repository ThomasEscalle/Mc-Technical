#ifndef TICKS_H
#define TICKS_H

#include "BuildComponentBase.h"
#include "Minecraft/BlockCoordinates.h"

#include <QObject>
#include <QMap>


class Tick {
public:
    BlockCoordinates BlockCoord;
    QStringList commands;
    int delayInTicks;
};


class Ticks : public BuildComponentBase
{
    ///
    /// \brief m_ticks All the ticks contained in the project linked to their identifier
    ///
    QMap<QString, Tick> m_ticks;


public:
    ///
    /// \brief Ticks Contruct the Ticks object
    /// \param project The project that we are working on
    ///
    Ticks(Project* project);


    // Compile the ticks
    void Compile();

    ///
    /// \brief Build_Pass_0 Build the pass 0 of the Ticks
    /// This is assigning the different coordinates of the commands blocks
    /// \return
    ///
    bool Build_Pass_0() override;



    ///
    /// \brief Build_Pass_1 Build the pass 1 of the variables
    /// This is setting the blocks in the structure for the commands blocks area
    /// Adding the commands blocks themself
    /// And creating the functions
    /// \return
    ///
    bool Build_Pass_1() override;

public:
    ///
    /// \brief ContainsTick Say if the container contains a certain tick
    /// \return If it contains the tick
    ///
    bool ContainsTick(const QString TickId);

    ///
    /// \brief GetTick Return the tick corresponding to the given id
    /// \param TickId The id of the tick you want to get
    /// \return The tick itself
    ///
    const Tick& GetTick(const QString& TickId);


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

#endif // TICKS_H
