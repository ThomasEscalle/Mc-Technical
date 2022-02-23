#include "BlockCoordinates.h"

#include "Project.h"

QString BlockCoordinates::ToString(bool worldCoordinates, Project *project)
{
    if(worldCoordinates == false ) {
        return QString::number(x) + " " + QString::number(y) + " " + QString::number(z);
    }
    else if(project != nullptr) {
        return  QString::number(x + project->StructureBlockCoordinate(0)) + " " +
                QString::number(y + project->StructureBlockCoordinate(1)) + " " +
                QString::number(z + project->StructureBlockCoordinate(2));
    }
    return QString::number(x) + " " + QString::number(y) + " " + QString::number(z);
}

QString BlockCoordinates::ToStringInvertedXZ(bool worldCoordinates, Project *project)
{
    if(worldCoordinates == false ) {
        return QString::number(z) + " " + QString::number(y) + " " + QString::number(x);
    }
    else if(project != nullptr) {
        return  QString::number(z + project->StructureBlockCoordinate(0)) + " " +
                QString::number(y + project->StructureBlockCoordinate(1)) + " " +
                QString::number(x + project->StructureBlockCoordinate(2));
    }
    return QString::number(z) + " " + QString::number(y) + " " + QString::number(x);
}
