#include "FileHelper.h"

#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


void FileHelper::SaveJsonToFile(const QString &filePath, const QJsonObject &jsonObject)
{
    // Get the directory path of the file
    QStringList splitedPath = QString(filePath).replace("\\", "/").split("/");
    splitedPath.removeLast();

    // Create the path if it does not exists
    QString dirPath = splitedPath.join("/");
    CreatePathIfDoesNotExist(dirPath);

    // Open the file as write only and save the json object as a byte array
    QFile file(filePath);
    if(file.open(QFile::WriteOnly)) {
        QJsonDocument doc(jsonObject);
        file.write(doc.toJson());

        file.flush();
        file.close();
    }
    else {
        qDebug()<< "<SaveJsonToFile(object)> Impossible to open the file for writting. path : "<< filePath;
    }
}

void FileHelper::SaveJsonToFile(const QString &filePath, const QJsonArray &jsonArray)
{
    // Get the directory path of the file
    QStringList splitedPath = QString(filePath).replace("\\", "/").split("/");
    splitedPath.removeLast();

    // Create the path if it does not exists
    QString dirPath = splitedPath.join("/");
    CreatePathIfDoesNotExist(dirPath);

    // Open the file as write only and save the json array as a byte array
    QFile file(filePath);
    if(file.open(QFile::WriteOnly)) {
        QJsonDocument doc(jsonArray);
        file.write(doc.toJson());

        file.flush();
        file.close();
    }
    else {
        qDebug()<< "<SaveJsonObject(array)> Impossible to open the file for writting. path : "<< filePath;
    }
}

void FileHelper::CreatePathIfDoesNotExist(const QString &path)
{
    if(!QDir().exists(path)) QDir().mkpath(path);
}

QJsonObject FileHelper::OpenJsonObject(const QString &path)
{
    QFile file(path);
    if(file.exists()&& file.open(QFile::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();

        if(doc.isObject() && !doc.isNull()) {
            return doc.object();
        }

    }
    return QJsonObject();
}

QJsonArray FileHelper::OpenJsonArray(const QString &path)
{
    QFile file(path);
    if(file.exists()&& file.open(QFile::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();

        if(doc.isArray() && !doc.isNull()) {
            return doc.array();
        }

    }
    return QJsonArray();
}

bool FileHelper::CopyFileContent(const QString &originPath, const QString &targetPath)
{
    // The content of the source file
    QByteArray FileContent;

    // Opent the source file as read only
    QFile sourceFile(originPath);
    if(sourceFile.open(QFile::ReadOnly)) {
        FileContent = sourceFile.readAll();
        sourceFile.flush();
        sourceFile.close();
    }
    // If there was an error
    else {
        qWarning()<< "<CopyFileContent> Impossible to open the file as ReadOnly. path " << sourceFile;
        return false;
    }

    // Open the target file for writting
    QFile targetFile(targetPath);
    if(targetFile.open(QFile::WriteOnly)) {
        targetFile.write(FileContent);
        targetFile.flush();
        targetFile.close();
    }

    // If there was an error
    else {
        qWarning()<< "<CopyFileContent> Impossible to open the file as WriteOnly. path " << targetFile;
        return false;
    }


    return true;
}





