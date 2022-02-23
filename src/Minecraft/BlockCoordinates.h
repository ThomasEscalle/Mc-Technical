#ifndef BLOCKCOORDINATES_H
#define BLOCKCOORDINATES_H

#include <QString>

class Project;

struct BlockCoordinates{
public:
    int x = 0;
    int y = 0;
    int z = 0;

    QString ToString(bool worldCoordinates = false, Project* proj = nullptr);

    QString ToStringInvertedXZ(bool worldCoordinates = false , Project* proj = nullptr);
};


#endif // BLOCKCOORDINATES_H
