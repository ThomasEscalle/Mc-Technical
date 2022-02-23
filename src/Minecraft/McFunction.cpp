#include "McFunction.h"

#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>

#include <QDebug>




McFunction::McFunction(const QString &functionPath, const QString &pBpPath)
{
    FunctionPath = functionPath;
    this->BpPath = pBpPath;
}

void McFunction::SetDescription(const QString &description)
{
    Description = description;
}

void McFunction::Header(const QString &CommentText)
{
    functions.push_back("");
    functions.push_back("### ========================================");
    functions.push_back("### " + CommentText);
    functions.push_back("### ========================================");
}

void McFunction::Comment(const QString &CommentText)
{
    functions.push_back("### " + CommentText);

}

void McFunction::Space(const int &number)
{
    for(int i = 0 ; i < number ; i++) {
        functions.push_back("");
    }
}

void McFunction::Command(const QString &Command)
{
    functions.push_back(Command);
    isEmpty = false;
}

void McFunction::CommandList(const QStringList &pCommandList)
{
    for(int i = 0 ; i < pCommandList.size() ; i++ ) {
        functions.push_back(pCommandList.at(i));
    }
}

void McFunction::ExecCommand(const QString &target, const QString Command)
{
    QString FinalCommand = "execute " + target + " ~ ~ ~ " + Command;
    functions.push_back(FinalCommand);
}

void McFunction::DebugFunctionName(const QString &BeforeText, const QString &AfterText)
{
    Q_UNUSED(BeforeText);
    Q_UNUSED(AfterText);
    qDebug()<< "        Created the function " <<"/functions/" + FunctionPath  + ".mcfunction";
}

QString McFunction::GetPath()
{
    return BpPath + "/functions/" + FunctionPath + ".mcfunction";
}

const bool &McFunction::IsEmpty() const
{
    return isEmpty;
}


void McFunction::Save()
{
    // Split the function only to get the relative path and not the name of the function itself;
    QStringList splitedBpPath = FunctionPath.split("/");
    QString FunctionName = splitedBpPath.takeLast();

    // Add the path of the function to the path
    QString DirPath = BpPath;
    if(splitedBpPath.size() >= 1) {
        DirPath =DirPath + "/functions/" + splitedBpPath.join("/");
    }

    // Create the path if it does not exits
    if(!QDir().exists(DirPath)) {
        QDir().mkpath(DirPath);
    }

    // The path of the file
    QString FilePath = DirPath + "/" + FunctionName + ".mcfunction" ;
    // The file itself that we want to fil.
    QFile file(FilePath);
    if(file.open(QFile::WriteOnly)) {

        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        // Fill the stream

        // If there is a description
        if(Description != "" ) {
            stream << "### " + Description + "\n";
            stream << "###\n";
        }
        stream << "### /function " << FunctionPath+ "\n";
        stream << "### " + FunctionName + ".mcfunction\n";
        stream << "###\n";
        stream << "\n";

        for(int i = 0 ; i < functions.size() ; i++) {
            stream << functions.at(i) + "\n";
        }

        file.flush();
        file.close();
    }
    else {
        qWarning() << "<McFunction::Save> Warning : Impossible to open the file as WriteOnly. path : "<< FilePath;
        return;
    }
}


