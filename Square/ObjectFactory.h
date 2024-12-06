#pragma once

#include <memory>
#include <string>

#include "ObjectEssentials.h"
#include "ObjectManager.h"

struct SDL_Color;
struct SDL_Rect;

struct ObjectCapsule
{
	std::string _objectName;
	std::string _fileName;
	Vector2F _originPos;
	Vector2F _velocity;
	Vector2F _targetPos = { 0,0 };
	MoveType _mType;
};
struct TextObjectCapsule
{
	ObjectCapsule _createBuffer;
	Text _text;
};
struct ObjectIOCapsule
{
	ObjectCapsule _createBuffer;
	std::string _type;
	Text _text;
	Color _fontColor;
	bool hasTarget{ false };
	bool isRect{ false };
	Vector2 _widthHeight;
	Color _rectColor;
};

class ObjectFactory
{
public:
	ObjectFactory() {};
	~ObjectFactory() {};

	//File I/O function
	bool CreateObjectsFromFile(const std::string& fileName);
	// object-related
	ObjectCapsule CreateObjectCapsule(const std::string& objectName,
		const std::string& fileName, Vector2F originPos, Vector2F velocity,
		Vector2F _targetPos, MoveType mType);
	TextObjectCapsule CreateTextObjectCapsule(const std::string& objectName,
		const std::string& fileName, Vector2F originPos, Vector2F velocity,
		Vector2F _targetPos = Vector2F{ 0,0 }, MoveType mType = MoveType::DEFAULT,const Text& text);

	// CreateObject Functions
	template <typename T>
	std::shared_ptr<T> CreateObject(std::unique_ptr<ObjectManager>& objectManager,SDL_Renderer*
		renderer,const ObjectCreateBuffer& buffer)
	{
		auto object = std::make_shared<T>(buffer._objectName);
		objectManager->AddObject(object);
		object->LoadImage(renderer, buffer._fileName);
		object->InitMove(buffer._originPos,buffer._velocity, buffer._mType);
		return object;
	}
	template <typename T>
	std::shared_ptr<T> CreateMoveTargetObject(std::unique_ptr<ObjectManager>& objectManager, SDL_Renderer* renderer,
		const ObjectCreateBuffer& buffer)
	{
		auto object = CreateObject<T>(objectManager,renderer,buffer);
		object->PushTarget(buffer._targetPos);
		return object;
	}
	template <typename T>
	std::shared_ptr<T> CreateTextObject(std::unique_ptr<ObjectManager>& objectManager, SDL_Renderer* renderer,
		const ObjectCreateTextObjectBuffer& buffer)
	{

		auto object = std::make_shared<T>(objectName);
		object->PreloadInit(buffer._text);
		objectManager->AddObject(object);
		object->LoadImage(renderer, fileName);
		object->InitMove(pos[0], pos[1], mType);
		return object;
	}

	template <typename T>
	std::shared_ptr<T> CreateRect(std::unique_ptr<ObjectManager>& objectManager,SDL_Renderer*renderer, const std::string& objectName,
		const SDL_Rect& rect, Vector2F* pos, SDL_Color color, MoveType mType)
	{
		auto object = std::make_shared<T>(objectName);
		objectManager->AddObject(object);
		object->MakeRect(renderer, rect, color);
		object->InitMove(pos[0], pos[1], mType);
		return object;
	}
	template <typename T>
	std::shared_ptr<T> CreateMoveTargetRect(std::unique_ptr<ObjectManager>& objectManager, SDL_Renderer* renderer,const std::string& objectName,
		const SDL_Rect& rect, Vector2F* pos, SDL_Color color, MoveType mType)
	{
		auto object = CreateRect<T>(objectManager,renderer,objectName, rect, pos, color, mType);
		Vector2F targetPos = pos[2];
		object->PushTarget(targetPos);
		return object;
	}

private:
	void ReadString(std::ifstream& in, std::string& string);
	void ReadColor(std::ifstream& in, Color& color);
	void ReadVector2(std::ifstream& in, Vector2& vector2);
	void ReadVector2F(std::ifstream& in, Vector2F& vector2f);
	ObjectIOCapsule ReadObjectFromFile(std::ifstream& in);
};

