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
	ObjectCapsule _objectCapsule;
	Text _text;
};
struct RectObjectCapsule
{
	ObjectCapsule _objectCapsule;
	Vector2 _widthHeight;
	SDL_Color _rectColor;
};
struct ObjectIOCapsule
{
	ObjectCapsule _objectCapsule;
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
	bool CreateObjectsFromFile(std::unique_ptr<ObjectManager>const&,SDL_Renderer* renderer,const std::string& fileName);
	// object-related
	ObjectCapsule CreateObjectCapsule(const std::string& objectName,
		const std::string& fileName, Vector2F originPos, Vector2F velocity,
		Vector2F _targetPos, MoveType mType);
	TextObjectCapsule CreateTextObjectCapsule(ObjectCapsule& objectCapsule,
		const Text& text);
	RectObjectCapsule CreateRectObjectCapsule(ObjectCapsule& objectCapsule,
		Vector2 widthHeight, Color color);

	// CreateObject Functions
	template <typename T>
	std::shared_ptr<T> CreateObject(std::unique_ptr<ObjectManager>const& objectManager,SDL_Renderer*
		renderer,const ObjectCapsule& buffer)
	{
		auto object = std::make_shared<T>(buffer._objectName);
		objectManager->AddObject(object);
		object->LoadImage(renderer, buffer._fileName);
		object->InitMove(buffer._originPos,buffer._velocity, buffer._mType);
		return object;
	}
	template <typename T>
	std::shared_ptr<T> CreateMoveTargetObject(std::unique_ptr<ObjectManager>const& objectManager, SDL_Renderer* renderer,
		const ObjectCapsule& buffer)
	{
		auto object = CreateObject<T>(objectManager,renderer,buffer);
		object->PushTarget(buffer._targetPos);
		return object;
	}
	template <typename T>
	std::shared_ptr<T> CreateTextObject(std::unique_ptr<ObjectManager>const& objectManager, SDL_Renderer* renderer,
		const TextObjectCapsule& buffer)
	{

		auto object = std::make_shared<T>(buffer._objectCapsule._objectName);
		object->PreloadInit(buffer._text);
		objectManager->AddObject(object);
		object->LoadImage(renderer, buffer._objectCapsule._fileName);
		object->InitMove(buffer._objectCapsule._originPos,buffer._objectCapsule._velocity,
			buffer._objectCapsule._mType);
		return object;
	}

	template <typename T>
	std::shared_ptr<T> CreateRect(std::unique_ptr<ObjectManager>const& objectManager,SDL_Renderer*renderer,
		const RectObjectCapsule& rectObject)
	{
		auto object = std::make_shared<T>(rectObject._objectCapsule._objectName);
		objectManager->AddObject(object);
		SDL_Rect rect{0,0,rectObject._widthHeight.x,rectObject._widthHeight.y};
		object->MakeRect(renderer,rect, rectObject._rectColor);
		object->InitMove(rectObject._objectCapsule._originPos, rectObject._objectCapsule._velocity,
			rectObject._objectCapsule._mType);
		return object;
	}
	template <typename T>
	std::shared_ptr<T> CreateMoveTargetRect(std::unique_ptr<ObjectManager>const& objectManager, SDL_Renderer* renderer,
		const RectObjectCapsule& rectObject)
	{
		std::shared_ptr<T> object = CreateRect<T>(objectManager,renderer,rectObject);
		object->PushTarget(rectObject._objectCapsule._targetPos);
		return object;
	}

private:
	void ReadString(std::ifstream& in, std::string& string);
	void ReadColor(std::ifstream& in, Color& color);
	void ReadVector2(std::ifstream& in, Vector2& vector2);
	void ReadVector2F(std::ifstream& in, Vector2F& vector2f);
	ObjectIOCapsule ReadObjectFromFile(std::ifstream& in);
};

