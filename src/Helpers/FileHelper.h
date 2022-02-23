#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <QObject>

class FileHelper
{
public:
    ///
    /// \brief SaveJsonToFile Save a json object into a file.
    /// \param filePath The path of the file where to save the object.
    /// \param jsonObject The object to save
    ///
    static void SaveJsonToFile(const QString& filePath , const QJsonObject& jsonObject);

    ///
    /// \brief SaveJsonToFile Save a json array into a file
    /// \param filePath The path of the file where to save the array.
    /// \param jsonArray The array to save
    ///
    static void SaveJsonToFile(const QString& filePath, const QJsonArray& jsonArray);



    ///
    /// \brief CreatePathIfDoesNotExist Check if a directory path exists and create it if not
    /// \param path The path of the directory to check
    ///
    static void CreatePathIfDoesNotExist(const QString& path);


    ///
    /// \brief OpenJsonObject Open a file and convert it's content to json object.
    /// \param path The path of the file to open.
    /// \return The generated object or an empty one if we could not parse the data or open the file.
    ///
    static QJsonObject OpenJsonObject(const QString& path);

    ///
    /// \brief OpenJsonArray Open a file and convert it's content to json array.
    /// \param path The path of the file to open.
    /// \return The generated array or an empty one if we could not parse the data or open the file.
    ///
    static QJsonArray OpenJsonArray(const QString& path);


    ///
    /// \brief CopyFileContent Copy the content of a file to a target file.
    /// This is opening, reading, and creating a new file. It avoid protections problem when copying a file.
    /// \param originPath The path of the source file where you want to copy the content.
    /// \param targetPath The path of the target file where you want to paste the content.
    /// \return If the file was succesfully copied.
    ///
    static bool CopyFileContent(const QString& originPath, const QString& targetPath);
};

#endif // FILEHELPER_H
