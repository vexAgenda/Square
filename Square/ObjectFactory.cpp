#include "ObjectFactory.h"
#include <fstream>

ObjectCapsule ObjectFactory::CreateObjectCapsule(const std::string& objectName, const std::string& fileName, Vector2F originPos, Vector2F velocity, Vector2F _targetPos, MoveType mType)
{
    ObjectCapsule buffer; 
    buffer._objectName = objectName;
    buffer._fileName = fileName;
    buffer._originPos = originPos;
    buffer._velocity = velocity;
    buffer._targetPos = _targetPos;
    buffer._mType = mType;
    return buffer;
}

void ObjectFactory::ReadString(std::ifstream& in, std::string& string)
{
    size_t size;
    in.read(reinterpret_cast<char*>(&size), sizeof(size));
    string.resize(size);
    in.read(&string[0], size);
}

void ObjectFactory::ReadColor(std::ifstream& in, Color& color)
{
    in.read(reinterpret_cast<char*>(&color.r), sizeof(int));
    in.read(reinterpret_cast<char*>(&color.g), sizeof(int));
    in.read(reinterpret_cast<char*>(&color.b), sizeof(int));
    in.read(reinterpret_cast<char*>(&color.a), sizeof(int));
}

void ObjectFactory::ReadVector2(std::ifstream& in, Vector2& vector2)
{
    in.read(reinterpret_cast<char*>(&vector2.x), sizeof(int));
    in.read(reinterpret_cast<char*>(&vector2.y), sizeof(int));
}

void ObjectFactory::ReadVector2F(std::ifstream& in, Vector2F& vector2f)
{
    in.read(reinterpret_cast<char*>(&vector2f.x), sizeof(float));
    in.read(reinterpret_cast<char*>(&vector2f.y), sizeof(float));
}

ObjectIOCapsule ObjectFactory::ReadObjectFromFile(std::ifstream& in)
{
    ObjectIOCapsule buffer;
    ReadString(in, buffer._type);
    ReadString(in, buffer._createBuffer._objectName);
    ReadString(in, buffer._createBuffer._fileName);
    if (buffer._type.contains("Text"))
    {
        Text text;
        in.read(reinterpret_cast<char*>(&buffer._text._ptSize), sizeof(int));
        ReadColor(in, buffer._fontColor);
        Text fontColor = { buffer._fontColor.r,buffer._fontColor.g ,buffer._fontColor.b ,buffer._fontColor.a };
        ReadString(in, buffer._text._msg);
    }
    ReadVector2F(in, buffer._createBuffer._originPos);
    ReadVector2F(in, buffer._createBuffer._velocity);
    in.read(reinterpret_cast<char*>(&buffer.hasTarget), sizeof(bool));
    if (buffer.hasTarget)
        ReadVector2F(in, buffer._createBuffer._targetPos);
    in.read(reinterpret_cast<char*>(&buffer.isRect), sizeof(bool));
    if (buffer.isRect)
    {
        ReadVector2(in, buffer._widthHeight);
        ReadColor(in, buffer._rectColor);
    }
    std::string mType;
    ReadString(in, mType);
    if (mType == "default")
        buffer._createBuffer._mType = MoveType::DEFAULT;
    else if (mType == "square")
        buffer._createBuffer._mType = MoveType::SQUARE;
    return buffer;
}

bool ObjectFactory::CreateObjectsFromFile(const std::string& fileName)
{
    std::ifstream in{ fileName };
    if (in.is_open())
    {
        size_t size;
        in.read(reinterpret_cast<char*>(&size), sizeof(size));
        for (int i = 0; i < size; ++i)
        {
            ObjectFileReadBuffer buffer = ReadObjectFromFile(in);
            if (buffer._type.contains("Text"))
            {
                CreateTextObject()
            }
            else if (buffer.isRect)
            {
                if (buffer.hasTarget)

                else
            }
            else
            {
                if (buffer.hasTarget)
            }
        }
    }
    else
        return false;
    return true;
}
