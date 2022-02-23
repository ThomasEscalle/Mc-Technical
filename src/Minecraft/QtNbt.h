#ifndef QNBT_H
#define QNBT_H


#include <QObject>
#include <QList>
#include <QMap>
#include <QDataStream>

const int DebugSpaceIncrement = 2;




enum NbtType {
    TAG_End = 0,
    TAG_Byte = 1,
    TAG_Short = 2,
    TAG_Int = 3,
    TAG_Long = 4,
    TAG_Float = 5,
    TAG_Double = 6,
    TAG_Byte_Array = 7,
    TAG_String = 8,
    TAG_List = 9,
    TAG_Compound = 10,
    TAG_Int_Array = 11,
    TAG_Long_Array = 12
};

class NbtByte;
class NbtShort;
class NbtInt;
class NbtLong;
class NbtFloat;
class NbtDouble;
class NbtByteArray;
class NbtString;
class NbtList;
class NbtCompound;
class NbtIntArray;
class NbtLongArray;



class NbtDocument
{
    // The root compound
    NbtCompound* m_Compound = nullptr;

    // The order of the bytes
    static QDataStream::ByteOrder m_byteOrder;

public:
    // Constructors
    NbtDocument(const QString& path = "");
    NbtDocument(NbtCompound* compound);

public:

    // Set the root nbtCompound
    void SetCompound(NbtCompound* compound);

    // Getter to the root compound
    NbtCompound* Compound() const;

public:
    // convert to array
    void ToArray(QByteArray& array);

    // Save the bits into a file with the corresponding path
    void SaveAs(const QString& path);
    // Load the bits from a file
    void Load(const QString& path);

    // Debug the document in the console
    void Debug();


public:
    static QDataStream::ByteOrder byteOrder();
    static void setByteOrder(QDataStream::ByteOrder newByteOrder);
};


class NbtBase
{
protected:
    NbtType m_Type;

public:
    virtual void ToArray(QByteArray& array) = 0;
    virtual void FromArray(QByteArray& array) = 0;
    virtual void Debug(const int& spacing = 0);

public:
    NbtType Type();

    bool isByte();
    bool isShort();
    bool isInt();
    bool isLong();
    bool isFloat();
    bool isDouble();
    bool isByte_Array();
    bool isString();
    bool isList();
    bool isCompound();
    bool isInt_Array();
    bool isLong_Array();

    NbtByte* toByte();
    NbtShort* toShort();
    NbtInt* toInt();
    NbtLong* toLong();
    NbtFloat* toFloat();
    NbtDouble* toDouble();
    NbtByteArray* toByteArray();
    NbtString* toString();
    NbtList* toList();
    NbtCompound* toCoumpound();
    NbtIntArray* toIntArray();
    NbtLongArray* toLongArray();
};



class NbtByte : public NbtBase
{
public:
    char Value = 0;
    NbtByte(char defaultValue = 0) {
        Value = defaultValue;
        m_Type = NbtType::TAG_Byte;
    }

    void FromArray(QByteArray &array) override;
    void ToArray(QByteArray &array) override;
    void Debug(const int &spacing = 0) override;
};

class NbtShort : public NbtBase
{
public:
    short Value = 0;
    NbtShort(short defaultValue = 0) {
        Value = defaultValue;
        m_Type = NbtType::TAG_Short;
    }

    void FromArray(QByteArray &array) override;
    void ToArray(QByteArray &array) override;
    void Debug(const int &spacing = 0) override;
};

class NbtInt : public NbtBase
{
public:
    int Value = 0;
    NbtInt(int defaultValue = 0) {
        Value = defaultValue;
        m_Type = NbtType::TAG_Int;
    }

    void FromArray(QByteArray &array) override;
    void ToArray(QByteArray &array) override;
    void Debug(const int &spacing = 0) override;
};

class NbtLong : public NbtBase
{
public:
    qint64 Value = 0;
    NbtLong(qint64 defaultValue = 0) {
        Value = defaultValue;
        m_Type = NbtType::TAG_Long;
    }

    void FromArray(QByteArray &array) override;
    void ToArray(QByteArray &array) override;
    void Debug(const int &spacing = 0) override;
};

class NbtFloat : public NbtBase
{
public:
    float Value = 0.0f;
    NbtFloat(float defaultValue = 0.0f) {
        Value = defaultValue;
        m_Type = NbtType::TAG_Float;
    }

    void FromArray(QByteArray &array) override;
    void ToArray(QByteArray &array) override;
    void Debug(const int &spacing = 0) override;
};

class NbtDouble : public NbtBase
{
public:
    double Value = 0;
    NbtDouble(double defaultValue = 0.0) {
        Value = defaultValue;
        m_Type = NbtType::TAG_Double;
    }

    void FromArray(QByteArray &array) override;
    void ToArray(QByteArray &array) override;
    void Debug(const int &spacing = 0) override;
};

class NbtByteArray : public NbtBase
{
    QList<char> Value;
public:
    NbtByteArray() {
        m_Type = NbtType::TAG_Byte_Array;
    }

    void FromArray(QByteArray &array) override;
    void ToArray(QByteArray &array) override;
    void Debug(const int &spacing = 0) override;


    int count();
    int length();
    bool isEmpty();
    int size();
    void pushBack(char value);
    void pushFront(char value);
    void removeAt(int i);
    void removeFirst();
    void removeLast();
    char at(int i);
    void insert(int i , char value);

};

class NbtString : public NbtBase
{
public:
    QString Value = "";
    NbtString(QString defaultValue = "") {
        Value = defaultValue;
        m_Type = NbtType::TAG_String;
    }

    void FromArray(QByteArray &array) override;
    void ToArray(QByteArray &array) override;
    void Debug(const int &spacing = 0) override;


};

class NbtList : public NbtBase
{
    QList<NbtBase*> Value;
    NbtType ValuesType = NbtType::TAG_Byte;
public:
    NbtList(NbtType type = NbtType::TAG_Byte) {
        ValuesType = type;
        m_Type = NbtType::TAG_List;
    }

    void FromArray(QByteArray &array) override;
    void ToArray(QByteArray &array) override;
    void Debug(const int &spacing = 0) override;

    int count();
    int length();
    bool isEmpty();
    int size();
    void pushBack(NbtBase*);
    void pushFront(NbtBase*);
    void removeAt(int i);
    void removeFirst();
    void removeLast();
    NbtBase* at(int i);
    void insert(int i , NbtBase*);
    NbtType getValuesType() const;
    void setValuesType(NbtType newValuesType);
};

class NbtCompound : public NbtBase
{
    QMap<QString, NbtBase*> Value;
public:
    NbtCompound() {
        m_Type = NbtType::TAG_Compound;
    }

    virtual void FromArray(QByteArray &array) override;
    virtual void ToArray(QByteArray &array) override;
    void Debug(const int &spacing = 0) override;


    void insert(const QString& key, NbtBase* base);
    bool contains(const QString& key);
    void remove(const QString& key);
    void clear();
    QStringList keys();
    bool isEmpty();
    int size();
    int count();
    NbtBase* at(const QString& key);
};

class NbtIntArray : public NbtBase
{
    QList<int> Value;
public:
    NbtIntArray() {
        m_Type = NbtType::TAG_Int_Array;
    }

    void FromArray(QByteArray &array) override;
    void ToArray(QByteArray &array) override;
    void Debug(const int &spacing = 0) override;

    int count();
    int length();
    bool isEmpty();
    int size();
    void pushBack(int value);
    void pushFront(int value);
    void removeAt(int i);
    void removeFirst();
    void removeLast();
    int at(int i);
    void insert(int i , int value);
};

class NbtLongArray : public NbtBase
{
    QList<__int64> Value;
public:
    NbtLongArray() {
        m_Type = NbtType::TAG_Long_Array;
    }

    void FromArray(QByteArray &array) override;
    void ToArray(QByteArray &array) override;
    void Debug(const int &spacing = 0) override;

    int count();
    int length();
    bool isEmpty();
    int size();
    void pushBack(__int64 value);
    void pushFront(__int64 value);
    void removeAt(int i);
    void removeFirst();
    void removeLast();
    __int64 at(int i);
    void insert(int i , __int64 value);
};







NbtBase *TypeToBase(char c);
char TypeToChar(NbtType type);
NbtType CharToType(char c);
QString TypeToString(const NbtType& type);

#endif // QNBT_H
