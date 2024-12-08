#include "ObjectFactory.h"
#include "GameObject.h"
#include "TextButton.h"

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

TextObjectCapsule ObjectFactory::CreateTextObjectCapsule(ObjectCapsule& objectCapsule, const Text& text)
{
    TextObjectCapsule textObject;
    textObject._objectCapsule = objectCapsule;
    textObject._text = text;
    return textObject;
}

RectObjectCapsule ObjectFactory::CreateRectObjectCapsule(ObjectCapsule& objectCapsule, Vector2 widthHeight, Color color)
{
    RectObjectCapsule rectObject;
    rectObject._objectCapsule = objectCapsule;
    rectObject._widthHeight = widthHeight;
    SDL_Color _color;
    _color.r = static_cast<Uint8>(color.r);
    _color.g = static_cast<Uint8>(color.g);
    _color.b = static_cast<Uint8>(color.b);
    _color.a = static_cast<Uint8>(color.a);
    rectObject._rectColor = _color;
    return rectObject;
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
    ReadString(in, buffer._objectCapsule._objectName);
    ReadString(in, buffer._objectCapsule._fileName);
    if (buffer._type.contains("Text"))
    {
        Text text;
        in.read(reinterpret_cast<char*>(&buffer._text._ptSize), sizeof(int));
        ReadColor(in, buffer._fontColor);
        Text fontColor = { buffer._fontColor.r,buffer._fontColor.g ,buffer._fontColor.b ,buffer._fontColor.a };
        ReadString(in, buffer._text._msg);
    }
    ReadVector2F(in, buffer._objectCapsule._originPos);
    ReadVector2F(in, buffer._objectCapsule._velocity);
    in.read(reinterpret_cast<char*>(&buffer.hasTarget), sizeof(bool));
    if (buffer.hasTarget)
        ReadVector2F(in, buffer._objectCapsule._targetPos);
    in.read(reinterpret_cast<char*>(&buffer.isRect), sizeof(bool));
    if (buffer.isRect)
    {
        ReadVector2(in, buffer._widthHeight);
        ReadColor(in, buffer._rectColor);
    }
    std::string mType;
    ReadString(in, mType);
    if (mType == "default")
        buffer._objectCapsule._mType = MoveType::DEFAULT;
    else if (mType == "square")
        buffer._objectCapsule._mType = MoveType::SQUARE;
    return buffer;
}

bool ObjectFactory::CreateObjectsFromFile(std::unique_ptr<ObjectManager>const&objectManager, SDL_Renderer* renderer, const std::string& fileName)
{
    std::ifstream in{ fileName };
    if (in.is_open())
    {
        size_t size;
        in.read(reinterpret_cast<char*>(&size), sizeof(size));
        for (int i = 0; i < size; ++i)
        {
            ObjectIOCapsule buffer = ReadObjectFromFile(in);
            if (buffer._type == "GameObject")
            {
                if (buffer.isRect)
                {
                    RectObjectCapsule rectObject = CreateRectObjectCapsule(
                        buffer._objectCapsule, buffer._widthHeight, buffer._rectColor);
                    if (buffer.hasTarget)
                        CreateMoveTargetRect<GameObject>(objectManager, renderer, rectObject);
                    else
                        CreateRect<GameObject>(objectManager, renderer, rectObject);
                }
                else
                {
                    if (buffer.hasTarget)
                        CreateMoveTargetObject<GameObject>(objectManager, renderer, buffer._objectCapsule);
                    else
                        CreateObject<GameObject>(objectManager, renderer, buffer._objectCapsule);
                }

            }
            else if (buffer._type == "Button")
            {
                if (buffer.isRect)
                {
                    RectObjectCapsule rectObject = CreateRectObjectCapsule(
                        buffer._objectCapsule, buffer._widthHeight, buffer._rectColor);
                    if (buffer.hasTarget)
                        CreateMoveTargetRect<Button>(objectManager, renderer, rectObject);
                    else
                        CreateRect<Button>(objectManager, renderer, rectObject);
                }
                else
                {
                    if (buffer.hasTarget)
                        CreateMoveTargetObject<Button>(objectManager, renderer, buffer._objectCapsule);
                    else
                        CreateObject<Button>(objectManager, renderer, buffer._objectCapsule);
                }
            }
            else if (buffer._type == "TextButton")
            {
                TextObjectCapsule textObject = CreateTextObjectCapsule(buffer._objectCapsule, buffer._text);
                CreateTextObject<TextButton>(objectManager, renderer, textObject);
            }
        }
    }
    else
    {
        SDL_Log("%s does not exist!", fileName.c_str());
        return false;
    }
    return true;
}
