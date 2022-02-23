#ifndef CINEMATIC_H
#define CINEMATIC_H

#include <QObject>
#include <QMap>

#include "Minecraft/BlockCoordinates.h"

// Define the project to use it when building the first pass
class Project;

// The number of decimals after the double when converting to string
const int decimalsDouble = 3;

///
/// \brief The Cinematic class
/// Describe one single cinematic
///
class Cinematic
{
    // Represent the positions of the player at a given time
    struct CinematicPositions
    {
    public:
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float xRot = 0.0f;
        float yRot = 0.0f;
    public:
        // Fill the data from a string coordinate
        void FromStringCoordinates(const QString& coordinates);

        // Get the
        static CinematicPositions LerpPosition(CinematicPositions from, CinematicPositions to , double t);

        QString ToString();

    };

public:
    // The default length of a cinematic (in seconds)
    double length = 12;

    // The starting position
    CinematicPositions startPosition;
    // The ending position
    CinematicPositions endPosition;

    // The list of events to execute on start of the cinematic
    QStringList start_commands;
    // The list of events to execute on the end of a cinematic
    QStringList end_commands;

    // The container of the commands linked to their keyframe position (in tick)
    QMap<int, QStringList> m_runtimeCommands;

    // If we want to take a shortcut or not
    bool take_shortcut = true;

    // The identifier of the cinematic
    QString identifier = "";

public:
    // The coordinates of the block to start
    BlockCoordinates BlockCoord;

    ///
    /// \brief FromJson Setup the object from a json object
    /// \param obj The object to parse
    /// \return If the action was a success or not
    ///
    bool FromJson(const QJsonObject& obj);

    ///
    /// \brief Build_Pass_1 Build the pass 1 of the delayed commands
    /// This is building the cinematic itself and placing the command blocks
    /// \return
    ///
    bool Build_Pass_1(Project* project);

};

#endif // CINEMATIC_H
