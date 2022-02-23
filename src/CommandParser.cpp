#include "CommandParser.h"

#include "Project.h"
#include "QRegularExpression"


#include <QDebug>

CommandParser::CommandParser(Project *project)
{
    m_project = project;
}

QString CommandParser::ParseCommand(const QString &command)
{
    qDebug()<< "    <CommandParser::ParseCommand> Parsing the command" << command;

    // Get the first character
    QString firstCharacter = command[0];

    // Search for the coordinates regular expression
    QString finalCommand = PreprocessCommand(command);

    // If it is a minecraft command
    if(firstCharacter == "/") {
        finalCommand.remove(0,1);
        return ParseMcCommand(finalCommand);
    }
    // If it is a variable modification
    else if(firstCharacter == "$") {
        finalCommand.remove(0,1);
        return ParseVariableModification(finalCommand);
    }
    // If the command is a conditional event
    else if(firstCharacter == "?") {
        finalCommand.remove(0,1);
        return ParseConditional(finalCommand);
    }
    // If the command is a tick
    else if(firstCharacter == "*") {
        finalCommand.remove(0,1);
        return ParseTick(finalCommand);
    }
    // If the command is a delayed command
    else if(firstCharacter == "-") {
        finalCommand.remove(0,1);
        return ParseDelayedCommand(finalCommand);
    }
    // If the command is a comment
    else if(firstCharacter == "#") {
        // Return it
        return command;
    }


    // Split the command on every space
    QStringList splitedCommand = command.split(" ");
    QString action = splitedCommand.takeFirst();

    // If the command is a variable modification
    if(action == "var") {
        return ParseVariableModification(splitedCommand.join(" "));
    }
    // If the command is a conditional variable
    else if(action == "if") {
        return ParseConditional(splitedCommand.join(" "));
    }
    // If the command is a ticking
    else if(action == "tick") {
        return ParseTick(splitedCommand.join(" "));
    }
    // If the command is a delayed command
    else if(action == "delayed") {
        return ParseDelayedCommand(splitedCommand.join(" "));
    }
    // if the command is a cinematic command
    else if(action == "cine") {
        return ParseCinematicCommand(splitedCommand.join(" "));
    }
    // If the command is a quest command
    else if(action == "quest") {
        return ParseQuestCommand(splitedCommand.join(" "));
    }

    return command;
}



QString CommandParser::PreprocessCommand(const QString &command)
{
    QString goodCommand = command;

    // Check for the "coord" arguments
    {
        QRegularExpression coord_exp("(?<={coord:)(.*)(?=})", QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatchIterator i = coord_exp.globalMatch(command);
        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            QString word = match.captured(1);
            QString replaced = "{coord:" + word + "}";
            goodCommand.replace(replaced, m_project->Coordinates()->GetCoordinates(word));
        }
    }

    // Check for the "coord_rot" arguments
    {
        QRegularExpression coord_exp("(?<={coord_rot:)(.*)(?=})", QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatchIterator i = coord_exp.globalMatch(command);
        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            QString word = match.captured(1);
            QString replaced = "{coord_rot:" + word + "}";
            goodCommand.replace(replaced, m_project->Coordinates()->GetCoordinatesRot(word));
        }
    }

    // Check for the "coord_target" arguments
    {
        QRegularExpression coord_exp("(?<={coord_target:)(.*)(?=})", QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatchIterator i = coord_exp.globalMatch(command);
        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            QString word = match.captured(1);
            QString replaced = "{coord_target:" + word + "}";
            goodCommand.replace(replaced, m_project->Coordinates()->GetCoordintatesTarget(word));
        }
    }



    // Check for the "text" arguments
    {
        QRegularExpression coord_exp("(?<={text:)(.*)(?=})", QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatchIterator i = coord_exp.globalMatch(command);
        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            QString word = match.captured(1);
            QString replaced = "{text:" + word + "}";
            goodCommand.replace(replaced, m_project->Texts()->GetText(word));
        }
    }

    return goodCommand;
}

QString CommandParser::ParseMcCommand(const QString &command)
{
    qDebug()<< "        <CommandParser::ParseMcCommand> The command is a minecraft command : " << command;
    return command;
}

QString CommandParser::ParseVariableModification(const QString &command)
{
    qDebug()<< "        <CommandParser::ParseVariableModification> The command is a variable modification command : " << command;


    return command;
}

QString CommandParser::ParseConditional(const QString &command)
{
    qDebug()<< "        <CommandParser::ParseConditional> The command is a conditional command : " << command;
    QStringList splited = command.split(" ");

    if(splited.size() >= 2) {
        QString ModifiedVariable = splited.takeFirst();
        QString Operator = splited.takeFirst();
    }
    return "";
}


QString CommandParser::ParseTick(const QString &command)
{
    qDebug()<< "        <CommandParser::ParseTick> The command is a tick command : " << command;
    QStringList splitedCommand = command.split(" ");
    if(splitedCommand.size()>= 2) {
        QString action = splitedCommand.takeFirst();
        QString tickId = splitedCommand.takeFirst();

        if(m_project->Ticks()->ContainsTick(tickId)) {
            Tick t = m_project->Ticks()->GetTick(tickId);
            if(action == "start") {
                return "setblock "+ t.BlockCoord.ToStringInvertedXZ(true, m_project) + " redstone_block 0";
            }
            else if(action == "finish" || action == "end") {
                return "setblock "+ t.BlockCoord.ToStringInvertedXZ(true, m_project) + " stone 0";
            }
            else {
                qWarning()<< "        <CommandParser::ParseCommand> The tick action does not correspond to anything : "<< action;
                return "setblock 0 0 0 air 0";
            }
        }
        else {
            qWarning()<< "        <CommandParser::ParseCommand> Impossible to find a declared tick with the identifier : "<< tickId;
            return "setblock 0 0 0 air 0";
        }

    }
    qWarning()<< "    <CommandParser::ParseCommand> Not enought arguments were passed to the tick command : "<< command;
    return "setblock 0 0 0 air 0";
}

QString CommandParser::ParseDelayedCommand(const QString &command)
{
    qDebug()<< "        <CommandParser::ParseDelayedCommand> The command is a delayed command : " << command;

    if(m_project->DelayedCommands()->ContainsCommand(command)) {
        DelayedCommand DelayedCommand = m_project->DelayedCommands()->GetCommand(command);
        return "setblock "+ DelayedCommand.m_blockCoordinate.ToStringInvertedXZ(true, m_project) + " redstone_block 0";
    }
    else {
        qWarning()<< "        <CommandParser::ParseDelayedCommand> Impossible to find a declared delayed command with the identifier : "<< command;
        return "setblock 0 0 0 air 0";
    }

    return command;
}

QString CommandParser::ParseCinematicCommand(const QString &command)
{
    qDebug()<< "        <CommandParser::ParseCinematicCommand> The command is a cinematic command : " << command;

    // Split the command into substrings
    QStringList splitedCommand = command.split(" ");
    if(splitedCommand.size()> 0){

        // Get the action and the name of the cinematic
        QString Action = splitedCommand.at(0);
        QString CinematicName = splitedCommand.at(1);

        if(m_project->Cinematics()->ContainsCinematic(CinematicName)) {
            // Get the cinematic itself
            Cinematic cine = m_project->Cinematics()->GetCinematic(CinematicName);

            // If this is the start action
            if(Action == "start") {
                return "setblock " + cine.BlockCoord.ToStringInvertedXZ(true, m_project) + " redstone_block 0";
            }

            else {
                qWarning()<< "        <CommandParser::ParseCinematicCommand> There is no cinematic action named : "<< Action << "from the command : "<< command;
                return "setblock 0 0 0 air 0";
            }
        }
        else {
            qWarning()<< "        <CommandParser::ParseCinematicCommand> There is no cinematic with the given identifier : "<< CinematicName << "from the command : "<< command;
            return "setblock 0 0 0 air 0";
        }
    }
    else {
        qWarning()<< "    <CommandParser::ParseCinematicCommand> Not enought arguments were passed to the cinematic command : "<< command;
        return "setblock 0 0 0 air 0";
    }

    return command;
}

QString CommandParser::ParseQuestCommand(const QString &command)
{
    qDebug()<< "        <CommandParser::ParseQuestCommand> The command is a quest command : " << command;

    // Split the command into substrings
    QStringList splitedCommand = command.split(" ");
    if(splitedCommand.size()>0 ) {
        // Get the action and the name of the quest
        QString Action = splitedCommand.at(0);

        if(Action == "next") {
            return "function quests/next";
        }
        if(Action == "debug") {
            return "function quests/debug";
        }
    }

    // If there is an action and a quest name
    if(splitedCommand.size()> 1){

        // Get the action and the name of the quest
        QString Action = splitedCommand.at(0);
        QString QuestName = splitedCommand.at(1);

        if(m_project->Quests()->ContainsQuest(QuestName)) {
            // Get the quest itself
            Quest quest = m_project->Quests()->GetQuest(QuestName);

            // If this is the start action
            if(Action == "unlock") {
                return "function quests/" + quest.identifier() + "/unlock";
            }
            else if(Action == "select") {
                return "function quests/" + quest.identifier() + "/select";
            }

            else {
                qWarning()<< "        <CommandParser::ParseQuestCommand> There is no quest action named : "<< Action << "from the command : "<< command;
                return "setblock 0 0 0 air 0";
            }
        }
        else {
            qWarning()<< "        <CommandParser::ParseQuestCommand> There is no quest with the given identifier : "<< QuestName << "from the command : "<< command;
            return "setblock 0 0 0 air 0";
        }
    }
    else {
        qWarning()<< "    <CommandParser::ParseQuestCommand> Not enought arguments were passed to the quest command : "<< command;
        return "setblock 0 0 0 air 0";
    }

    return command;
}



