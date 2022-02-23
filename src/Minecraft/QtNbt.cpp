#include "QtNbt.h"

#include <QByteArray>
#include <QFile>
#include <QDir>
#include <QFileInfo>

#include <QDebug>


QDataStream::ByteOrder NbtDocument::m_byteOrder = QDataStream::LittleEndian;

void NbtBase::Debug(const int &spacing)
{
    Q_UNUSED(spacing);
}

NbtType NbtBase::Type()
{
    return m_Type;
}

bool NbtBase::isByte()
{
    return m_Type == NbtType::TAG_Byte;
}

bool NbtBase::isShort()
{
    return m_Type == NbtType::TAG_Short;
}

bool NbtBase::isInt()
{
    return m_Type == NbtType::TAG_Int;
}

bool NbtBase::isLong()
{
    return m_Type == NbtType::TAG_Long;
}

bool NbtBase::isFloat()
{
    return m_Type == NbtType::TAG_Float;
}

bool NbtBase::isDouble()
{
    return m_Type == NbtType::TAG_Double;
}

bool NbtBase::isByte_Array()
{
    return m_Type == NbtType::TAG_Byte_Array;
}

bool NbtBase::isString()
{
    return m_Type == NbtType::TAG_String;
}

bool NbtBase::isList()
{
    return m_Type == NbtType::TAG_List;
}

bool NbtBase::isCompound()
{
    return m_Type == NbtType::TAG_Compound;
}

bool NbtBase::isInt_Array()
{
    return m_Type == NbtType::TAG_Int_Array;
}

bool NbtBase::isLong_Array()
{
    return m_Type == NbtType::TAG_Long_Array;
}

NbtByte *NbtBase::toByte()
{
    return dynamic_cast<NbtByte*>(this);
}

NbtShort *NbtBase::toShort()
{
    return dynamic_cast<NbtShort*>(this);
}

NbtInt *NbtBase::toInt()
{
    return dynamic_cast<NbtInt*>(this);
}

NbtLong *NbtBase::toLong()
{
    return dynamic_cast<NbtLong*>(this);
}

NbtFloat *NbtBase::toFloat()
{
    return dynamic_cast<NbtFloat*>(this);
}

NbtDouble *NbtBase::toDouble()
{
    return dynamic_cast<NbtDouble*>(this);

}

NbtByteArray *NbtBase::toByteArray()
{
    return dynamic_cast<NbtByteArray*>(this);
}

NbtString *NbtBase::toString()
{
    return dynamic_cast<NbtString*>(this);
}

NbtList *NbtBase::toList()
{
    return dynamic_cast<NbtList*>(this);
}

NbtCompound *NbtBase::toCoumpound()
{
    return dynamic_cast<NbtCompound*>(this);
}

NbtIntArray *NbtBase::toIntArray()
{
    return dynamic_cast<NbtIntArray*>(this);
}

NbtLongArray *NbtBase::toLongArray()
{
    return dynamic_cast<NbtLongArray*>(this);
}


void NbtByte::FromArray(QByteArray &array)
{
    Value = array.left(1)[0];
    array.remove(0,1);
}

void NbtByte::ToArray(QByteArray &array)
{
    array.push_back(Value);
}

void NbtByte::Debug(const int &spacing)
{
    qDebug()<< QString(spacing, ' ') + Value;
}

void NbtShort::FromArray(QByteArray &array)
{
    QDataStream stream(array.left(2));
    stream.setByteOrder(NbtDocument::byteOrder());
    stream >> Value;
    array.remove(0,2);
}

void NbtShort::ToArray(QByteArray &array)
{
    QDataStream stream(&array, QIODevice::Append);
    stream.setByteOrder(NbtDocument::byteOrder());
    stream << Value;
}

void NbtShort::Debug(const int &spacing)
{
    qDebug()<< QString(spacing, ' ') + QString::number(Value);
}

void NbtInt::FromArray(QByteArray &array)
{
    QDataStream stream(array.left(4));
    stream.setByteOrder(NbtDocument::byteOrder());
    stream >> Value;
    array.remove(0,4);
}

void NbtInt::ToArray(QByteArray &array)
{
    QDataStream stream(&array, QIODevice::Append);
    stream.setByteOrder(NbtDocument::byteOrder());
    stream << Value;
}

void NbtInt::Debug(const int &spacing)
{
    qDebug()<< QString(spacing, ' ') + QString::number(Value);
}

void NbtLong::FromArray(QByteArray &array)
{
    QDataStream stream(array.left(8));
    stream.setByteOrder(NbtDocument::byteOrder());
    stream >> Value;
    array.remove(0,8);
}

void NbtLong::ToArray(QByteArray &array)
{
    QDataStream stream(&array, QIODevice::Append);
    stream.setByteOrder(NbtDocument::byteOrder());
    stream << Value;
}

void NbtLong::Debug(const int &spacing)
{
    qDebug()<< QString(spacing, ' ') + QString::number(Value);
}

void NbtFloat::FromArray(QByteArray &array)
{
    QDataStream stream(array.left(4));
    stream.setByteOrder(NbtDocument::byteOrder());
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    stream >> Value;
    array.remove(0,4);
}

void NbtFloat::ToArray(QByteArray &array)
{
    QDataStream stream(&array, QIODevice::Append);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    stream.setByteOrder(NbtDocument::byteOrder());
    stream << Value;
}

void NbtFloat::Debug(const int &spacing)
{
    qDebug()<< QString(spacing, ' ') + QString::number(Value);
}

void NbtDouble::FromArray(QByteArray &array)
{
    QDataStream stream(array.left(8));
    stream.setByteOrder(NbtDocument::byteOrder());
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    stream >> Value;
    array.remove(0,8);
}

void NbtDouble::ToArray(QByteArray &array)
{
    QDataStream stream(&array, QIODevice::Append);
    stream.setByteOrder(NbtDocument::byteOrder());
    stream << Value;
}

void NbtDouble::Debug(const int &spacing)
{
    qDebug()<< QString(spacing, ' ') + QString::number(Value);
}

void NbtByteArray::FromArray(QByteArray &array)
{
    int payloadSize = 0;
    {
        QDataStream stream(array.left(8));
        stream.setByteOrder(NbtDocument::byteOrder());
        stream >> payloadSize;
        array.remove(0,8);
    }

    Value.clear();
    for(int i = 0 ; i < payloadSize ; i++) {
        Value.push_back(array[0]);
        array.remove(0,1);
    }
}

void NbtByteArray::ToArray(QByteArray &array)
{
    // Add the payload size
    QDataStream stream(&array, QIODevice::Append);
    stream.setByteOrder(NbtDocument::byteOrder());
    stream << Value.size();

    for(int i = 0 ; i < Value.size() ; i++) {
        stream << Value[i];
    }
}

int NbtByteArray::count()
{
    return Value.count();
}

int NbtByteArray::length()
{
    return Value.length();
}

bool NbtByteArray::isEmpty()
{
    return Value.isEmpty();
}

int NbtByteArray::size()
{
    return Value.size();
}

void NbtByteArray::pushBack(char value)
{
    Value.push_back(value);
}

void NbtByteArray::pushFront(char value)
{
    Value.push_front(value);
}

void NbtByteArray::removeAt(int i)
{
    Value.removeAt(i);
}

void NbtByteArray::removeFirst()
{
    Value.removeFirst();
}

void NbtByteArray::removeLast()
{
    Value.removeFirst();
}

char NbtByteArray::at(int i)
{
    return Value.at(i);
}

void NbtByteArray::insert(int i, char value)
{
    Value.insert(i, value);
}

void NbtByteArray::Debug(const int &spacing)
{
    QString array;
    for(int i = 0 ; i < Value.size() ; i++) {
        array += Value.at(i);
        if(i < Value.size()-1) {
            array += ", ";
        }
    }
    qDebug()<< QString(spacing, ' ') + "[Size : " +QString::number(Value.size())+ "] { " + array + " }";
}

void NbtString::FromArray(QByteArray &array)
{
    unsigned short payloadSize = 0;
    {
        QDataStream stream(array.left(2));
        stream.setByteOrder(NbtDocument::byteOrder());
        stream >> payloadSize;
        array.remove(0,2);
    }

    Value = "";
    for(unsigned short i = 0 ; i < payloadSize ; i++) {
        Value.push_back(QChar(array[0]));
        array.remove(0,1);
    }
}

void NbtString::ToArray(QByteArray &array)
{
    unsigned short size = (unsigned short)Value.size();
    {
        // Add the payload size
        QDataStream stream(&array, QIODevice::Append);
        stream.setByteOrder(NbtDocument::byteOrder());
        // Add the size into the array
        stream << size;
    }
    array.append(Value.toUtf8());
}

void NbtString::Debug(const int &spacing)
{
    qDebug()<< QString(spacing, ' ') + Value;
}

NbtType NbtList::getValuesType() const
{
    return ValuesType;
}

void NbtList::setValuesType(NbtType newValuesType)
{
    ValuesType = newValuesType;
}

void NbtList::FromArray(QByteArray &array)
{
    char type = array[0];
    ValuesType = CharToType(type);
    array.remove(0,1);

    Value.clear();

    int size = 0;
    {
        QDataStream stream(array.left(4));
        stream.setByteOrder(NbtDocument::byteOrder());
        stream >> size;
        array.remove(0,4);
    }

    for(int i = 0 ; i < size ; i++) {
        NbtBase* base = TypeToBase(type);
        base->FromArray(array);
        Value.push_back(base);

    }
}

void NbtList::ToArray(QByteArray &array)
{
    array.push_back(TypeToChar(ValuesType));
    // Add the payload size
    {
        QDataStream stream(&array, QIODevice::Append);
        stream.setByteOrder(NbtDocument::byteOrder());
        stream << Value.size();
    }


    for(int i = 0 ; i < Value.size() ; i ++) {
        Value.at(i)->ToArray(array);
    }
}

void NbtList::Debug(const int &spacing)
{
    qDebug()<< QString(spacing, ' ') + "List : [Size : " +QString::number(Value.size())+ "] Contained type : " + TypeToString(ValuesType);
    for(int i = 0 ; i < Value.size() ; i++) {
        Value.at(i)->Debug(spacing + DebugSpaceIncrement);
    }
}

int NbtList::count()
{
    return Value.count();
}

int NbtList::length()
{
    return Value.length();
}

bool NbtList::isEmpty()
{
    return Value.isEmpty();
}

int NbtList::size()
{
    return Value.size();
}

void NbtList::pushBack(NbtBase *value)
{
    if(value->Type() == ValuesType) {
        Value.push_back(value);
    }
}

void NbtList::pushFront(NbtBase *value)
{
    if(value->Type() == ValuesType) {
        Value.push_front(value);
    }
}

void NbtList::removeAt(int i)
{
    Value.removeAt(i);
}

void NbtList::removeFirst()
{
    Value.removeFirst();
}

void NbtList::removeLast()
{
    Value.removeLast();
}

NbtBase *NbtList::at(int i)
{
    return Value.at(i);
}

void NbtList::insert(int i, NbtBase *value)
{
    if(value->Type() == ValuesType) {
        return Value.insert(i, value);

    }
}

void NbtCompound::FromArray(QByteArray &array)
{
    Value.clear();

    bool isFinished = false;
    while (!isFinished) {
        // Get the id
        char type = array[0];
        array.remove(0,1);
        // If the type is an end
        if(type == 0) {
            return;
        }


        NbtString key;
        key.FromArray(array);
        QString StrKey = key.Value;

        NbtBase* base = TypeToBase(type);
        base->FromArray(array);
        Value.insert(StrKey , base);
    }
}


void NbtCompound::ToArray(QByteArray &array)
{
    QStringList keys = Value.keys();
    for(int i = 0 ; i < keys.size() ; i++) {
        QString key = keys.at(i);
        NbtBase* base = Value[key];

        QDataStream stream(&array, QIODevice::Append);
        stream.setByteOrder(NbtDocument::byteOrder());
        // Add the type
        array.append(base->Type());
        // Add the key
        NbtString string(key);
        string.ToArray(array);
        // Add the base
        base->ToArray(array);
    }

    // Add the TAG_END value
    array.push_back(char(0x00));
}

void NbtCompound::Debug(const int &spacing)
{
    qDebug()<< QString(spacing, ' ') + "Compound : [Size : " +QString::number(Value.size())+ "]";
    QStringList keys = Value.keys();
    for(int i = 0 ; i < keys.size() ; i++) {
        QString key = keys[i];
        qDebug()<< QString(spacing+ DebugSpaceIncrement, ' ') + "<Key : " + key +"> <Type : " + TypeToString(Value[key]->Type()) + "> Value :";
        Value[key]->Debug(spacing+ DebugSpaceIncrement + DebugSpaceIncrement);
    }
}

void NbtCompound::remove(const QString &key)
{
    if(Value.contains(key)) Value.remove(key);
}

void NbtCompound::clear()
{
    Value.clear();
}

QStringList NbtCompound::keys()
{
    return Value.keys();
}

bool NbtCompound::isEmpty()
{
    return Value.isEmpty();
}

int NbtCompound::size()
{
    return Value.size();
}

int NbtCompound::count()
{
    return Value.count();
}

NbtBase *NbtCompound::at(const QString &key)
{
    if(Value.contains(key)) return Value[key];
    return nullptr;
}

void NbtCompound::insert(const QString &key, NbtBase *base)
{
    Value.insert(key, base);
}

bool NbtCompound::contains(const QString &key)
{
    return  Value.contains(key);
}

void NbtIntArray::FromArray(QByteArray &array)
{
    Value.clear();
    int size = 0;
    {
        QDataStream stream(array.left(4));
        stream.setByteOrder(NbtDocument::byteOrder());
        stream >> size;
        array.remove(0,4);
    }

    for(int i = 0 ; i < size ; i ++ ) {
        QDataStream stream(array.left(4));
        stream.setByteOrder(NbtDocument::byteOrder());
        int value = 0;
        stream >> value;
        Value.push_back(value);
        array.remove(0,4);
    }
}

void NbtIntArray::ToArray(QByteArray &array)
{
    // Add the size
    QDataStream stream(&array, QIODevice::Append);
    stream.setByteOrder(NbtDocument::byteOrder());
    // Add the size of the array
    stream << Value.size();

    for (int i = 0 ; i < Value.size() ; i++) {
        stream << Value.at(i);
    }
}

void NbtIntArray::Debug(const int &spacing)
{
    qDebug()<< QString(spacing, ' ') + "[Size : " +QString::number(Value.size())+ "]";
    for(int i = 0 ; i < Value.size() ; i++) {
        qDebug()<< QString(spacing + DebugSpaceIncrement, ' ') + QString::number(Value[i]);
    }
}

int NbtIntArray::count()
{
    return Value.count();
}

int NbtIntArray::length()
{
    return Value.length();
}

bool NbtIntArray::isEmpty()
{
    return Value.isEmpty();
}

int NbtIntArray::size()
{
    return Value.size();
}

void NbtIntArray::pushBack(int value)
{
    Value.push_back(value);
}

void NbtIntArray::pushFront(int value)
{
    Value.push_front(value);
}

void NbtIntArray::removeAt(int i)
{
    Value.removeAt(i);
}

void NbtIntArray::removeFirst()
{
    Value.removeFirst();
}

void NbtIntArray::removeLast()
{
    Value.removeLast();
}

int NbtIntArray::at(int i)
{
    return Value.at(i);
}

void NbtIntArray::insert(int i, int value)
{
    Value.insert(i , value);
}



void NbtLongArray::FromArray(QByteArray &array)
{
    Value.clear();
    int size = 0;
    {
        QDataStream stream(array.left(4));
        stream.setByteOrder(NbtDocument::byteOrder());
        stream >> size;
        array.remove(0,4);
    }

    for(int i = 0 ; i < Value.size() ; i++) {
        QDataStream stream(array.left(8));
        stream.setByteOrder(NbtDocument::byteOrder());
        stream >> Value;
        array.remove(0,8);
    }
}

void NbtLongArray::ToArray(QByteArray &array)
{
    // Add the size
    QDataStream stream(&array, QIODevice::Append);
    stream.setByteOrder(NbtDocument::byteOrder());
    // Add the size of the array
    stream << Value.size();

    for (int i = 0 ; i < Value.size() ; i++) {
        stream << Value.at(i);
    }
}

void NbtLongArray::Debug(const int &spacing)
{
    qDebug()<< QString(spacing, ' ') + "[Size : " +QString::number(Value.size())+ "]";
    for(int i = 0 ; i < Value.size() ; i++) {
        qDebug()<< QString(spacing + DebugSpaceIncrement, ' ') + QString::number(Value[i]);
    }
}

int NbtLongArray::count()
{
    return Value.count();

}

int NbtLongArray::length()
{
    return Value.length();
}

bool NbtLongArray::isEmpty()
{
    return Value.isEmpty();
}

int NbtLongArray::size()
{
    return Value.size();
}

void NbtLongArray::pushBack(long long value)
{
    Value.push_back(value);
}

void NbtLongArray::pushFront(long long value)
{
    Value.push_front(value);
}

void NbtLongArray::removeAt(int i)
{
    Value.removeAt(i);
}

void NbtLongArray::removeFirst()
{
    Value.removeFirst();
}

void NbtLongArray::removeLast()
{
    Value.removeLast();
}

long long NbtLongArray::at(int i)
{
    return Value.at(i);
}

void NbtLongArray::insert(int i, long long value)
{
    Value.insert(i, value);
}

NbtBase *TypeToBase(char c)
{
    switch (c) {
    case 1 : return new NbtByte();
    case 2 : return new NbtShort();
    case 3 : return new NbtInt();
    case 4 : return new NbtLong();
    case 5 : return new NbtFloat();
    case 6 : return new NbtDouble();
    case 7 : return new NbtByteArray();
    case 8 : return new NbtString();
    case 9 : return new NbtList();
    case 10 : return new NbtCompound();
    case 11 : return new NbtIntArray();
    case 12 : return new NbtLongArray();
    default: return nullptr;
    }
}

char TypeToChar(NbtType type) {
    switch (type) {
        case 1 : return 1;
        case 2 : return 2;
        case 3 : return 3;
        case 4 : return 4;
        case 5 : return 5;
        case 6 : return 6;
        case 7 : return 7;
        case 8 : return 8;
        case 9 : return 9;
        case 10 : return 10;
        case 11 : return 11;
        case 12 : return 12;
    default: return 0;
    }
}

NbtType CharToType(char c) {
    switch (c) {
        case 1 : return TAG_Byte;
        case 2 : return TAG_Short;
        case 3 : return TAG_Int;
        case 4 : return TAG_Long;
        case 5 : return TAG_Float;
        case 6 : return TAG_Double;
        case 7 : return TAG_Byte_Array;
        case 8 : return TAG_String;
        case 9 : return TAG_List;
        case 10 : return TAG_Compound;
        case 11 : return TAG_Int_Array;
        case 12 : return TAG_Long_Array;
        default: return TAG_End;
    }
}


QDataStream::ByteOrder NbtDocument::byteOrder()
{
    return m_byteOrder;
}

void NbtDocument::setByteOrder(QDataStream::ByteOrder newByteOrder)
{
    m_byteOrder = newByteOrder;
}

NbtDocument::NbtDocument(const QString &path)
{
    Load(path);
}

NbtDocument::NbtDocument(NbtCompound *compound)
{
    m_Compound = compound;
}

void NbtDocument::SetCompound(NbtCompound *compound)
{
    m_Compound = compound;
}

NbtCompound *NbtDocument::Compound() const
{
    return m_Compound;
}



void NbtDocument::ToArray(QByteArray &array)
{
    array.append(char(0x0a));
    array.append(char(0x00));
    array.append(char(0x00));

    if(m_Compound != nullptr) {
        m_Compound->ToArray(array);
    }
}

void NbtDocument::SaveAs(const QString &path)
{
    // Check if the path exists
    QFileInfo info(path);
    QString DirPath = info.dir().path();
    if(!QDir().exists(DirPath)) QDir().mkpath(DirPath);


    // Create the array that contains all the bytes
    QByteArray array;
    // Push the data into this array
    ToArray(array);

    QFile file(path);
    if(file.open(QFile::WriteOnly)) {
        file.write(array);
        file.flush();
        file.close();
    }
    else {
        qWarning()<< "<NbtDocument::SaveAs> Warning : Impossible to open the file as QFile::WriteOnly. Given path is : "<< path;
    }

}

void NbtDocument::Load(const QString &path)
{
    QFile file(path);
    if(file.exists() && file.open(QFile::ReadOnly)) {
        QByteArray array;
        array = file.readAll();

        file.flush();
        file.close();

        // If we got some data from the file
        if(!array.isEmpty()) {
            // Remove the compound header
            array.remove(0,3);

            m_Compound = new NbtCompound();
            m_Compound->FromArray(array);
        }
    }
    else {
        qWarning()<< "<NbtDocument::Load> Warning : Impossible to open the file as QFile::ReadOnly. Given path is : "<< path;
    }
}

void NbtDocument::Debug()
{
    if(m_Compound != nullptr) {
        m_Compound->Debug(0);
    }
    else {
        qDebug()<< "<NbtDocument::Debug> Compound was not set.";
    }
}



QString TypeToString(const NbtType &type)
{
    switch (type) {
        case TAG_Byte : return "TAG_Byte";
        case TAG_Short : return "TAG_Short";
        case TAG_Int : return "TAG_Int";
        case TAG_Long : return "TAG_Long";
        case TAG_Float : return "TAG_Float";
        case TAG_Double : return "TAG_Double";
        case TAG_Byte_Array : return "TAG_Byte_Array";
        case TAG_String : return "TAG_String";
        case TAG_List : return "TAG_List";
        case TAG_Compound : return "TAG_Compound";
        case TAG_Int_Array : return "TAG_Int_Array";
        case TAG_Long_Array : return "TAG_Long_Array";
        default: return "TAG_End";
    }
}
