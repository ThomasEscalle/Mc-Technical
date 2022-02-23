#ifndef JSONHELPER_H
#define JSONHELPER_H

#include <QObject>

class JsonHelper
{
public:
    ///
    /// \brief ParseJsonObject Parse a string to a json object.
    /// \param value The string that you want to parse.
    /// \return The Json Object or an empty one if you could not parse it.
    ///
    static QJsonObject ParseJsonObject(const QString& value);

    ///
    /// \brief ParseJsonArray Parse a string to a json array.
    /// \param value The string that you want to parse.
    /// \return The Json array or an empty one if you could not parse it.
    ///
    static QJsonArray ParseJsonArray(const QString& value);



    ///
    /// \brief GetCommandsList Get a list of commands (list of strings) from a json object
    /// at a give key
    /// \param sourceObject The object where to search the array
    /// \param key The key of the array into the object
    /// \return THe list of commands that is returned (empty if there is nothing found)
    ///
    static QStringList GetCommandsList(const QJsonObject& sourceObject, const QString& key);

};

#endif // JSONHELPER_H
