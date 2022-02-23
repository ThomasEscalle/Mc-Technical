#ifndef MCFUNCTION_H
#define MCFUNCTION_H


#include <QObject>

class McFunction
{
    QString FunctionPath;
    QString BpPath;
    QString Description;

    bool isEmpty = true;

    // The entries of the mcfunction
    QStringList functions;

public:
    // Save the function
    void Save();



public:
    ///
    /// \brief McFunction Constructor of the function itself
    /// \param functionPath
    /// \param BpPath
    ///
    McFunction(const QString& functionPath, const QString& BpPath);

public :


    ///
    /// \brief SetDescription Set the description of the function
    /// \param description
    ///
    void SetDescription(const QString& description);


    ///
    /// \brief Header Add a header to the function
    /// \param CommentText
    ///
    void Header(const QString& CommentText);

    ///
    /// \brief comment Add a comment to the function
    /// \param CommentText
    ///
    void Comment(const QString& CommentText);

    ///
    /// \brief space Add a space in the function
    ///
    void Space(const int& number = 1);

    ///
    /// \brief Command Add a command to the function
    /// \param Command The command itself
    ///
    void Command(const QString& Command);


    ///
    /// \brief CommandList Add a list of commands to the list
    /// \param CommandList The list of commands to add
    ///
    void CommandList(const QStringList& CommandList);


    ///
    /// \brief ExecCommand Add a command executed by another target than @s
    /// \param target
    /// \param Command
    ///
    void ExecCommand(const QString& target, const QString Command);


    ///
    /// \brief DebugFunctionName
    /// \param BeforeText
    /// \param AfterText
    ///
    void DebugFunctionName(const QString& BeforeText = "" , const QString& AfterText = "");


    ///
    /// \brief GetPath Get the path of the function
    /// \return the path as a string
    ///
    QString GetPath();

    ///
    /// \brief IsEmpty If there are any commands in the function
    /// \return
    ///
    const bool& IsEmpty() const;




};

#endif // MCFUNCTION_H
