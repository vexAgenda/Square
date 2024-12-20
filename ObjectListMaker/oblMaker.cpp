#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <variant>
#include <unordered_map>
#include <optional>

void WriteString(std::ofstream& out, std::string& string);
void WriteMap(std::ofstream& out, std::unordered_map<std::string,bool>& map);

void ReadString(std::ifstream& in, std::string& string);
void ReadMap(std::ifstream& in, std::unordered_map<std::string,bool>& map);

struct Vector2
{
	int x;
	int y;
	friend std::ostream& operator<<(std::ostream& os, Vector2& vec)
	{
		return os << vec.x << " " << vec.y << '\n';
	}
	friend std::istream& operator>>(std::istream& is, Vector2& vec)
	{
		return is >> vec.x >> vec.y;
	}

	void Write(std::ofstream& out)
	{
		out.write(reinterpret_cast<char*>(&x), sizeof(int));
		out.write(reinterpret_cast<char*>(&y), sizeof(int));
	}
	void Read(std::ifstream& in)
	{
		in.read(reinterpret_cast<char*>(&x), sizeof(int));
		in.read(reinterpret_cast<char*>(&y), sizeof(int));
	}
	void Print()
	{
		std::print(std::cout,"{}, {}", x, y);
	}
};

struct Vector2F
{
	float x;
	float y;

	friend std::ostream& operator<<(std::ostream& os, Vector2F& vec)
	{
		return os << vec.x << " " << vec.y << '\n';
	}
	friend std::istream& operator>>(std::istream& is, Vector2F& vec)
	{
		return is >> vec.x >> vec.y;
	}

	void Write(std::ofstream& out)
	{
		out.write(reinterpret_cast<char*>(&x), sizeof(float));
		out.write(reinterpret_cast<char*>(&y), sizeof(float));
	}
	void Read(std::ifstream& in)
	{
		in.read(reinterpret_cast<char*>(&x), sizeof(float));
		in.read(reinterpret_cast<char*>(&y), sizeof(float));
	}
	void Print()
	{
		std::print(std::cout, "{}, {}", x, y);
	}
};

struct Color
{
	int r{};
	int g{};
	int b{};
	int a = 255;

	friend std::ostream& operator<<(std::ostream& os, Color& color)
	{
		return os << color.r << " " << color.g << " " << color.b << " " << color.a << '\n';
	}
	friend std::istream& operator>>(std::istream& is, Color& color)
	{
		return is >> color.r >> color.g >> color.b >> color.a;
	}
	void Write(std::ofstream& out)
	{
		out.write(reinterpret_cast<char*>(&r), sizeof(int));
		out.write(reinterpret_cast<char*>(&g), sizeof(int));
		out.write(reinterpret_cast<char*>(&b), sizeof(int));
		out.write(reinterpret_cast<char*>(&a), sizeof(int));
	}
	void Read(std::ifstream& in)
	{
		in.read(reinterpret_cast<char*>(&r), sizeof(int));
		in.read(reinterpret_cast<char*>(&g), sizeof(int));
		in.read(reinterpret_cast<char*>(&b), sizeof(int));
		in.read(reinterpret_cast<char*>(&a), sizeof(int));
	}
	void Print()
	{
		std::print(std::cout, "{}, {}, {}, {}", r,g,b,a);
	}
};
struct Text
{
	int _ptSize;
	Color _fontColor;
	std::string _msg;
	friend std::ostream& operator<<(std::ostream& os, Text& text)
	{
		return os << text._ptSize << " " << text._fontColor << " " << text._msg << '\n';
	}
	friend std::istream& operator>>(std::istream& is, Text& text)
	{
		return is >> text._ptSize >> text._fontColor >> text._msg;
	}
};


class ComponentCapsule
{
public:
	virtual ~ComponentCapsule() {}
	virtual void Write(std::ofstream& out) = 0;
	virtual void Read(std::ifstream& in) = 0;
	virtual void SetValue() = 0;
	virtual void Print() = 0;
};
class MoveComponentCapsule : public ComponentCapsule
{
public:
	Vector2F _velocity{};
	std::optional<Vector2F> _targetPos{};
	std::string _mType{};
	void Write(std::ofstream& out) override
	{
		_velocity.Write(out);
		bool hasValue = _targetPos.has_value();
		out.write(reinterpret_cast<char*>(&hasValue), sizeof(bool));
		if (_targetPos.has_value())
			_targetPos.value().Write(out);
		WriteString(out, _mType);
	}
	void Read(std::ifstream& in) override
	{
		_velocity.Read(in);
		bool hasValue;
		in.read(reinterpret_cast<char*>(&hasValue), sizeof(bool));
		if (hasValue)
		{
			Vector2F v2;
			v2.Read(in);
			_targetPos = v2;
		}
		else
			_targetPos = std::nullopt;
		ReadString(in, _mType);
	}
	void SetValue() override
	{
		std::cout << "Enter Velocity" << '\n';
		std::cin >> _velocity;
		CreateTarget();
		std::cout << "Enter Move Type" << '\n';
		std::cin >> _mType;
	}
	void Print()
	{
		std::print(std::cout, "[Move Component]\n Velocity : ");
		_velocity.Print();
		if (_targetPos.has_value())
		{
			std::print(std::cout, " Target Position : ");
			_targetPos.value().Print();
		}
		std::print(std::cout, "\n Move Type : {}\n", _mType);
	}
private:
	void CreateTarget()
	{
		std::print(std::cout, "Does this object have target");
		while (true)
		{
			std::string answer;
			std::cin >> answer;
			if (answer == "y")
			{
				std::cout << "Enter Target's position" << '\n';
				Vector2F target;
				std::cin >> target;
				_targetPos = target;
				break;
			}
			else if (answer == "n")
			{
				_targetPos = std::nullopt;
				break;
			}
		}
	}
};
class TextComponentCapsule : public ComponentCapsule
{
public:
	Text _text;
	void Write(std::ofstream& out) override
	{
		out.write(reinterpret_cast<char*>(&_text._ptSize), sizeof(int));
		_text._fontColor.Write(out);
		WriteString(out, _text._msg);
	}
	void Read(std::ifstream& in) override
	{
		in.read(reinterpret_cast<char*>(&_text._ptSize), sizeof(int));
		_text._fontColor.Read(in);
		ReadString(in, _text._msg);
	}
	void SetValue() override
	{
		std::cout << "Enter Text Point Size" << '\n';
		std::cin >> _text._ptSize;
		std::cout << "Enter Text Color r g b a" << '\n';
		std::cin >> _text._fontColor;
		std::cout << "Enter Text Message" << '\n';
		std::cin >> _text._msg;
	}
	void Print()
	{
		std::print(std::cout,"[Text Component]\n Text point size : {}\n Text color : ", _text._ptSize);
		_text._fontColor.Print();
		std::print(std::cout, "\n Text Msg : \"{}\"\n", _text._msg);
	}
};

class SpriteComponentCapsule : public ComponentCapsule
{
public:
	std::string _fileName{};
	std::optional<Vector2> _widthHeight{};
	std::optional<Color> _rectColor{};
	void Write(std::ofstream& out) override
	{
		bool hasValue = _widthHeight.has_value();
		out.write(reinterpret_cast<char*>(&hasValue), sizeof(bool));
		if (_widthHeight.has_value())
			_widthHeight.value().Write(out);
		hasValue = _rectColor.has_value();
		out.write(reinterpret_cast<char*>(&hasValue), sizeof(bool));
		if (_rectColor.has_value())
			_rectColor.value().Write(out);
	}
	void Read(std::ifstream& in) override
	{
		bool hasValue;
		in.read(reinterpret_cast<char*>(&hasValue), sizeof(bool));
		if (hasValue)
		{
			Vector2 wH;
			wH.Read(in);
			_widthHeight = wH;
		}
		else
			_widthHeight = std::nullopt;
		in.read(reinterpret_cast<char*>(&hasValue), sizeof(bool));
		if (hasValue)
		{
			Color c;
			c.Read(in);
			_rectColor = c;
		}
		else
			_rectColor = std::nullopt;
	}
	void SetValue() override
	{
		std::cout << "Enter File Name" << '\n';
		std::cin >> _fileName;
		CreateRect();
	}
	void Print()
	{
		std::print(std::cout, "[Sprite Component]\n");
		std::print(std::cout, " Texture file name : {}\n", _fileName);
		if (_widthHeight.has_value() && _rectColor.has_value())
		{
			std::print(std::cout, " Backgorund size : ");
			_widthHeight.value().Print();
			std::print(std::cout, "\n Background color : ");
			_rectColor.value().Print();
			std::cout << '\n';
		}
	}
private:
	void CreateRect()
	{
		std::print(std::cout, "Does this object have background?");
		while (true)
		{
			std::string answer;
			std::cin >> answer;
			if (answer == "y")
			{
				std::cout << "Enter Rect's width and height" << '\n';
				std::cin >> _widthHeight.value();
				std::cout << "Enter Rect Color r g b a" << '\n';
				std::cin >> _rectColor.value();

				break;
			}
			else if (answer == "n")
			{
				_widthHeight = std::nullopt;
				_rectColor = std::nullopt;
				break;
			}
		}
	}
};

class ButtonComponentCapsule: public ComponentCapsule
{
public:
	void Read(std::ifstream& in) override
	{
	}
	void Write(std::ofstream& out) override
	{
	}
	void SetValue() override
	{}
	void Print() override
	{
		std::print(std::cout,"[Button Component]\n");
	}
};

using Variant = std::variant<MoveComponentCapsule, SpriteComponentCapsule,
	TextComponentCapsule, ButtonComponentCapsule>;

class ObjectCapsule
{
public:
	std::string _objectName{};
	Vector2F _originPos{};
	std::unordered_map<std::string,bool> _hasComponent;

	template <typename T>
	void AddComponent()
	{
		_components.push_back(std::make_shared<T>());
		std::string objectName = typeid(T).name();
		_hasComponent[objectName] = true;
	}
	template<typename T>
	std::shared_ptr<T> GetComponent()
	{
		for (const auto& component : _components)
		{
			if (auto Component = std::dynamic_pointer_cast<T>(component))
				return Component;
		}
		return nullptr;
	}

	void Print(int i)
	{
		std::print(std::cout, "{}. {}\n", i, _objectName);
		std::print(std::cout, "Origin Position : ");
		_originPos.Print();
		std::cout << '\n';
		printAllComponents<Variant>();

	}
private:
	std::vector<std::shared_ptr<ComponentCapsule>> _components;
	template <typename Variant, size_t... Indices>
	void printAllComponentsImpl(std::index_sequence<Indices...>)
	{
		(([this]() {
			auto component = GetComponent<std::variant_alternative_t<Indices, Variant>>();
			if (component)
				component->Print();
			})(), ...);
	}
	template <typename Variant>
	void printAllComponents()
	{
		constexpr size_t variantSize = std::variant_size_v<Variant>;
		printAllComponentsImpl<Variant>(std::make_index_sequence<variantSize>{});
	}

};

template<typename Variant>
void hasComponent(ObjectCapsule& object)
{
	std::print(std::cout,"does this object have {}?\n", typeid(Variant).name());
	while (true)
	{
		std::string answer;
		std::cin >> answer;
		if (answer == "y")
		{
			object.AddComponent<Variant>();
			object.GetComponent<Variant>()->SetValue();
			break;
		}
		else if (answer == "n")
			break;
	}
}

void EditObject(ObjectCapsule& object);
void WriteToFile(std::ofstream& out, ObjectCapsule& buffer);
template<typename Variant, size_t... Indices>
void WriteComponentImpl(std::ofstream& out, ObjectCapsule& object, std::index_sequence<Indices...>);
template<typename Variant>
void WriteComponent(std::ofstream& out, ObjectCapsule& object);
template<typename Variant, size_t... Indices>
void ReadComponentImpl(std::ifstream& in, ObjectCapsule& object, std::index_sequence<Indices...>);
template<typename Variant>
void ReadComponent(std::ifstream& in, ObjectCapsule& object);

ObjectCapsule AddObject();
ObjectCapsule ReadFile(std::ifstream& in);

int main(void)
{
	std::vector<ObjectCapsule> objects;
	while (true)
	{
		std::string command;
		std::cout << "Object List Maker" << '\n';
		std::cout << "Add Delete Edit Save Load" << '\n';
		std::cout << "======================" << '\n';

		for (int i = 0; i < objects.size(); ++i)
		{
			objects[i].Print(i);
		}
		std::cout << "======================" << '\n';
		std::cout << "> ";
		std::getline(std::cin, command);
		if (command == "Add")
		{
			objects.push_back(AddObject());
		}
		else if (command == "Delete")
		{
			int where;
			std::cout << "Delete which? (0 ~ " << objects.size() - 1 << ")\n";
			std::cin >> where;
			if (where > objects.size() - 1 || where < 0)
				std::cout << "invalid size\n";
			else
			{
				objects.erase(objects.begin() + where);
			}
		}
		else if (command == "Edit")
		{
			int where;
			std::cout << "Edit which? (0 ~ " << objects.size() - 1 << ")\n";
			std::cin >> where;
			if (where > objects.size() - 1 || where < 0)
				std::cout << "invalid index\n";
			else
				EditObject(objects[where]);
		}
		else if (command == "Save")
		{
			std::cout << "Save Name?" << '\n';
			std::string saveLoc;
			std::cin >> saveLoc;
			saveLoc += ".obl";
			std::ofstream out{ saveLoc,std::ios_base::binary };
			size_t size = objects.size();
			out.write(reinterpret_cast<char*>(&size), sizeof(size));
			for (auto& object : objects)
			{
				WriteToFile(out, object);
			}
		}
		else if (command == "Load")
		{
			objects.clear();
			std::cout << "File Name?" << '\n';
			std::string fileLoc;
			std::cin >> fileLoc;
			fileLoc += ".obl";
			std::ifstream in{ fileLoc,std::ios_base::binary };
			if (in)
			{
				size_t size;
				in.read(reinterpret_cast<char*>(&size), sizeof(size));
				objects.resize(size);
				for (int i = 0; i < objects.size(); ++i)
				{
					objects[i] = ReadFile(in);
				}
			}
			else
			{
				std::cout << "file not exists" << '\n';
			}
		}
		system("timeout -1");
		system("cls");
	}
}


template <typename Variant, size_t... Indices>
void processAllComponentsImpl(ObjectCapsule& object, std::index_sequence<Indices...>)
{
	(hasComponent<std::variant_alternative_t<Indices, Variant>>(object), ...);
}	

template <typename Variant>
void processAllComponents(ObjectCapsule& object)
{
	constexpr size_t variantSize = std::variant_size_v<Variant>;
	processAllComponentsImpl<Variant>(object, std::make_index_sequence<variantSize>{});
}

ObjectCapsule AddObject()
{
	ObjectCapsule object;
	std::cout << "Enter Object Name" << '\n';
	std::cin >> object._objectName;
	std::cout << "Enter Origin Position" << '\n';
	std::cin >> object._originPos;
	processAllComponents<Variant>(object);
	return object;
}

void EditObject(ObjectCapsule& object)
{
	std::cout << "Enter Object Name" << '\n';
	std::cin >> object._objectName;
	std::cout << "Enter Origin Position" << '\n';
	std::cin >> object._originPos;
	processAllComponents<Variant>(object);
}

void WriteToFile(std::ofstream& out, ObjectCapsule& buffer)
{
	WriteString(out, buffer._objectName);
	buffer._originPos.Write(out);
	WriteMap(out, buffer._hasComponent);
	WriteComponent<Variant>(out, buffer); 
}

void WriteString(std::ofstream& out, std::string& string)
{
	size_t size = string.size();
	out.write(reinterpret_cast<char*>(&size), sizeof(size));
	out.write(string.c_str(), size);
}

void WriteMap(std::ofstream& out, std::unordered_map<std::string, bool>& map)
{
	size_t size = map.size();
	out.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (const auto [_objectName, _exists] : map)
	{
		std::string objectName = _objectName;
		WriteString(out, objectName);
		bool exists = _exists;
		out.write(reinterpret_cast<char*>(&exists), sizeof(bool));
	}
}


ObjectCapsule ReadFile(std::ifstream& in)
{
	ObjectCapsule object;
	ReadString(in, object._objectName);
	object._originPos.Read(in);
	ReadMap(in, object._hasComponent);
	ReadComponent<Variant>(in, object);
	return object;
}

void ReadString(std::ifstream& in, std::string& string)
{
	size_t size;
	in.read(reinterpret_cast<char*>(&size), sizeof(size));
	string.resize(size);
	in.read(const_cast<char*>(string.c_str()), size);
}

void ReadMap(std::ifstream& in, std::unordered_map<std::string, bool>& map)
{
	size_t size = map.size();
	in.read(reinterpret_cast<char*>(&size), sizeof(size));
	for (int i = 0; i < size; ++i)
	{
		std::string objectName;
		ReadString(in, objectName);
		bool exists;
		in.read(reinterpret_cast<char*>(&exists), sizeof(bool));
		map[objectName] = exists;
	}
}


template<typename Varaint, size_t Index>
void WriteComponentHelper(std::ofstream& out,ObjectCapsule& object)
{
	using ComponentType = std::variant_alternative_t<Index, Variant>;
	std::string componentName = typeid(ComponentType).name();
	if (object._hasComponent[componentName])
	{
		auto component = object.GetComponent<ComponentType>();
		if (component)
			component->Write(out);
	}
}

template<typename Variant, size_t ...Indices>
void WriteComponentImpl(std::ofstream& out, ObjectCapsule& object, std::index_sequence<Indices...>)
{
	(WriteComponentHelper<Variant, Indices>(out, object), ...);
}

template<typename Variant>
void WriteComponent(std::ofstream& out, ObjectCapsule& object)
{
	constexpr size_t variantSize = std::variant_size_v<Variant>;
	WriteComponentImpl<Variant>(out, object, std::make_index_sequence<variantSize>{});
}

template <typename Variant, size_t Index>
void ReadComponentHelper(std::ifstream& in, ObjectCapsule& object)
{
	using ComponentType = std::variant_alternative_t<Index, Variant>;
	std::string objectName = typeid(ComponentType).name();
	if (object._hasComponent[objectName])
	{
		object.AddComponent<ComponentType>();
		auto component = object.GetComponent<ComponentType>();
		component->Read(in);

	}
}
template<typename Variant, size_t ...Indices>
void ReadComponentImpl(std::ifstream& in, ObjectCapsule& object, std::index_sequence<Indices...>)
{
	(ReadComponentHelper<Variant,Indices>(in,object), ...);
}

template<typename Variant>
void ReadComponent(std::ifstream& in, ObjectCapsule& object)
{
	constexpr size_t variantSize = std::variant_size_v<Variant>;
	ReadComponentImpl<Variant>(in, object, std::make_index_sequence<variantSize>{});
}
