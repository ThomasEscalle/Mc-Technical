#include <QCoreApplication>
#include <QFileInfo>
#include <QDebug>

#include "McTechnical.h"
#include "iostream"
#include "windows.h"

//#define TESTS

void errorHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();

    if(msg == "") {
        std::cerr << std::endl;
        return;
    }

#ifdef _WIN32
    switch (type) {
    case QtDebugMsg:
        SetConsoleTextAttribute(GetStdHandle( STD_ERROR_HANDLE),6);
        std::cerr << "Debug :     ";
        SetConsoleTextAttribute(GetStdHandle( STD_ERROR_HANDLE),15);
        break;

    case QtMsgType::QtInfoMsg:
        SetConsoleTextAttribute(GetStdHandle( STD_ERROR_HANDLE),10);
        std::cerr << "Info :      " << msg.toStdString()<< std::endl;
        SetConsoleTextAttribute(GetStdHandle( STD_ERROR_HANDLE),15);
        return;
        break;
    case QtMsgType::QtWarningMsg:
        SetConsoleTextAttribute(GetStdHandle( STD_ERROR_HANDLE),12);
        std::cerr << "Warning :   ";
        McTechnical::WarningNumber ++;
        SetConsoleTextAttribute(GetStdHandle( STD_ERROR_HANDLE),15);
        break;
    case QtMsgType::QtCriticalMsg:
        SetConsoleTextAttribute(GetStdHandle( STD_ERROR_HANDLE),12);
        std::cerr << "Critical :  " << msg.toStdString()<< std::endl;
        McTechnical::CriticalNumber ++;
        SetConsoleTextAttribute(GetStdHandle( STD_ERROR_HANDLE),15);
        return;
        break;
    case QtMsgType::QtFatalMsg:
        SetConsoleTextAttribute(GetStdHandle( STD_ERROR_HANDLE),12);
        std::cerr << "Fatal :     " << msg.toStdString()<< std::endl;
        McTechnical::FatalNumber ++;
        SetConsoleTextAttribute(GetStdHandle( STD_ERROR_HANDLE),15);
        return;
        break;
    default:
        std::cerr << "Message :   ";
        SetConsoleTextAttribute(GetStdHandle( STD_ERROR_HANDLE),15);
        return;
        break;
    }
    std::cout<< msg.toStdString()<< std::endl;
#else

    switch (type) {
    case QtDebugMsg:
        std::cerr << "Debug :     ";
        break;
    case QtMsgType::QtInfoMsg:
        std::cerr << "Info :      " << msg.toStdString()<< std::endl;
        break;
    case QtMsgType::QtWarningMsg:
        std::cerr << "Warning :   ";
        break;
    case QtMsgType::QtCriticalMsg:
        std::cerr << "Critical :  ";
        break;
    case QtMsgType::QtFatalMsg:
        std::cerr << "Fatal :     " ;
        break;
    default:
        std::cerr << "Message :   ";
        return;
        break;
    }
    std::cout<< msg.toStdString()<< std::endl;
#endif

}


int main(int argc, char *argv[])
{
    qInstallMessageHandler(errorHandler);
    QCoreApplication a(argc, argv);


#ifdef TESTS
    qDebug()<< "Testing mode activated";
    qDebug()<< "";


    QString projectPath = "C:/Users/Thoma/OneDrive/Bureau/PinkCastle/minecraft-technics.project.json";
    McTechnical tec;
    tec.ComputeProject(projectPath);

    /**
    QString projectPath = "C:/Users/Thoma/OneDrive/Bureau/McTechnicalTests";
    McTechnical tec;
    tec.CreateTemplateProject(projectPath);
    */

    #else

    // Get all the given arguments as a string list
    QStringList arguments = a.arguments();

    // Remove the first argument that correspond to the path of the application
    arguments.removeFirst();

    if(arguments.size() == 0) {
        qDebug()<< "No arguments was passed to the application.";
    }


    // Loop throught all the given arguments
    for(int i = 0 ; i < arguments.size() ; i++) {
        // The argument we are analysing
        QString argument = arguments.takeFirst();
        argument.replace("//" , "/");

        // Check if the argument is a file or a directory
        QFileInfo info(argument);

        // If the given argument is a file
        if(info.isFile() && info.exists()) {
            // Compute the file
            McTechnical computer;
            computer.ComputeProject(argument);
        }
        // If the argument is an existing directory
        else if(info.isDir() && info.exists()) {
            // Create a template project in the folder
            McTechnical computer;
            computer.CreateTemplateProject(argument);
        }
        // If we dont know what this argument is
        else qDebug()<< "Skiping the argument "<< argument << " that is not a file and not a directory.";
    }
    std::string str;
    std::getline(std::cin, str);
#endif
    return 0;
}
