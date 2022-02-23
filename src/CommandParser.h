#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H


#include <QObject>

class Project;

class CommandParser
{
    // The project we are working on
    Project* m_project;

public:
    // Construct the parser with the current project we are working on
    CommandParser(Project* project);

    // Parse a command
    QString ParseCommand(const QString& command);


private:
    // Compute the Preprocessing of a command
    QString PreprocessCommand(const QString& command);



private:
    // Parse a minecraft command
    QString ParseMcCommand(const QString& command);
    // Parse a variable modification command
    QString ParseVariableModification(const QString& command);
    // Parse a conditional command
    QString ParseConditional(const QString& command);
    // Parse a tick command
    QString ParseTick(const QString& command);
    // Parse a delayed command
    QString ParseDelayedCommand(const QString& command);
    // Parse a cinematic command
    QString ParseCinematicCommand(const QString& command);
    // Parse a quest command
    QString ParseQuestCommand(const QString& command);
};


#endif // COMMANDPARSER_H
