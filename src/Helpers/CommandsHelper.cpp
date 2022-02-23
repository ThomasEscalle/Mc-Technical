#include "CommandsHelper.h"

#include "CommandParser.h"
#include "Minecraft/McFunction.h"


QString CommandsHelper::GetTellrawDebugCommand(const QString &message)
{
    QString GoodText = "tellraw @a {\"rawtext\":[{ \"text\":\"§e<Debug> §r" + message +"\"}]}";
    return GoodText;
}

QString CommandsHelper::GetTellrawCommand(const QString &message)
{
    QString GoodText = "tellraw @a {\"rawtext\":[{ \"text\":\"" + message +"\"}]}";
    return GoodText;
}

void CommandsHelper::ParseCommandListIntoFunction(const QStringList &list, McFunction *function, Project *project)
{
    // The parser
    CommandParser parser(project);
    // The list of parsed final commands
    QStringList parsedCommands = QStringList();

    // Loop throught all the commands of the list and parse them
    for(int i = 0 ; i < list.size() ; i++) {
        // Add the parsed command to the list of parsed command
        parsedCommands << parser.ParseCommand(list.at(i));
    }
    // Add the list of good commands to the mcfunction
    function->CommandList(parsedCommands);
}
