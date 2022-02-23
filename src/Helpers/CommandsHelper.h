#ifndef COMMANDSHELPER_H
#define COMMANDSHELPER_H


#include "QString"
class McFunction;
class Project;

class CommandsHelper
{
public:
    // Return a tellraw with the given message and a debug prefix.
    static QString GetTellrawDebugCommand(const QString& message);

    // Return a tellraw with the given text
    static QString GetTellrawCommand(const QString& message);


    // Add the list of commands to the mc function
    // THis is using the CommandParser to convert the commands
    // to mc commands
    static void ParseCommandListIntoFunction(const QStringList& list, McFunction* function, Project* project);


};

#endif // COMMANDSHELPER_H
