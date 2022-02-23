#ifndef VARIABLES_H
#define VARIABLES_H


#include <QObject>
#include <QMap>

#include "BuildComponentBase.h"
#include "Minecraft/BlockCoordinates.h"



enum VariableType {
    integer = 0,
    boolean = 1,
    unknown = 2
};


struct BoolVariable {
public:
    BlockCoordinates BlockCoord;
    bool defaultValue;
};

///
/// \brief The Variables class
/// Represent a container for all the variables of the prokect
///
class Variables : public BuildComponentBase
{
    ///
    /// \brief IntegersVariables A list that contain all the integers variables of the project linked to their Identifier
    ///
    QMap<QString, int> IntegersVariables;

    ///
    /// \brief BooleansVariables A list that contains all the booleans variables of the project linked to their identifier
    ///
    QMap<QString, BoolVariable> BooleansVariables;

public:
    ///
    /// \brief Variables Construct the variables Object
    /// \param project The project we are currently working on
    ///
    Variables(Project* project);

    ///
    /// \brief Build_Pass_0 Build the pass 0 of the Variables
    /// This is assigning the different coordinates of the booleans coordinates
    /// And creating the good scoreboards in the gameplay/init.mcfunction and gameplay/reset.mcfunction
    /// \return
    ///
    bool Build_Pass_0() override;

    ///
    /// \brief Build_Pass_1 Build the pass 1 of the variables
    /// This is setting the blocks in the structure for the commands block
    /// \return
    ///
    bool Build_Pass_1() override;



    // Get the variable type if this is stored in one of the map
    VariableType GetVariableType(const QString& key);


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

#endif // VARIABLES_H
