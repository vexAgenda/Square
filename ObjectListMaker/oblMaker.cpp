#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

enum MoveType
{
	MT_DEFAULT,
	MT_SQUARE,
	MT_END
};

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

void WriteString(std::ofstream& out, std::string& string);
void WriteColor(std::ofstream& out, Color& color);
void WriteVector2(std::ofstream& out, Vector2& vector2);
void WriteVector2F(std::ofstream& out, Vector2F& vector2f);
void ReadString(std::ifstream& in, std::string& string);
void ReadColor(std::ifstream& in, Color& color);
void ReadVector2(std::ifstream& in, Vector2& vector2);
void ReadVector2F(std::ifstream& in, Vector2F& vector2f);


class ComponentCapsule
{
public:
	virtual ~ComponentCapsule() {}
	virtual void Write(std::ifstream& in) = 0;
	virtual void Read(std::ostream& out) = 0;
};
class MoveComponentCapsule : public ComponentCapsule
{
public:
	Vector2F _velocity{};
	std::optional<Vector2F> _targetPos{};
	std::string _mType{};
};
class TextComponentCapsule
{
public:
	Text _text;
};

class ObjectCapsule
{
public:
	std::string _objectName{};
	std::string _fileName{};
	std::unordered_map<std::string, bool> _hasComponent;
	Vector2F _originPos{};
	std::optional<Vector2> _widthHeight{};
	std::optional<Color> _rectColor{};

	template<typename T>
	T GetComponent()
	{
		for (const auto& component : _components)
		{
			using ComponentType = decltype(component);
			if constexpr (std::is_same_v<T, ComponentType>)
				return component;
		}
		return ;
	}
private:
	std::vector<ComponentCapsule> _components;
};

bool hasComponent(const std::string& componentName)
{
	std::print(std::cout,"does this object have {}?", componentName);
	while (true)
	{
		std::string answer;
		std::cin >> answer;
		if (answer == "y")
			return true;
		else if (answer == "n")
			return false;
	}
}
void CreateTarget(ObjectCapsule& object)
{
	std::print(std::cout, "Does this object have target");
	while (true)
	{
		std::string answer;
		std::cin >> answer;
		if (answer == "y")
		{
			std::cout << "Enter Target's position" << '\n';
			std::cin >> object.GetComponent<MoveComponentCapsule>()._targetPos.value();
			break;
		}
		else if (answer == "n")
		{
			object.GetComponent<MoveComponentCapsule>()._targetPos = std::nullopt;
			break;
		}
	}
}
void CreateRect(ObjectCapsule& object)
{
	std::print(std::cout,"Does this object have background?");
	while (true)
	{
		std::string answer;
		std::cin >> answer;
		if (answer == "y")
		{
			std::cout << "Enter Rect's width and height" << '\n';
			std::cin >> object._widthHeight.value();
			std::cout << "Enter Rect Color r g b a" << '\n';
			std::cin >> object._rectColor.value();

			break;
		}
		else if (answer == "n")
		{
			object._widthHeight = std::nullopt;
			object._rectColor = std::nullopt;
			break;
		}
	}
}

void PrintObject(ObjectCapsule& object);
void EditObject(ObjectCapsule& object);
void WriteToFile(std::ofstream& out, ObjectCapsule& buffer);
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
			PrintObject(objects[i]);
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

void PrintObject(ObjectCapsule& object)
{

}

ObjectCapsule AddObject()
{
	ObjectCapsule object;
	return object;
}

void EditObject(ObjectCapsule& object)
{

}

void WriteToFile(std::ofstream& out, ObjectCapsule& buffer)
{

}

void WriteString(std::ofstream& out, std::string& string)
{
	size_t size = string.size();
	out.write(reinterpret_cast<char*>(&size), sizeof(size));
	out.write(string.c_str(), size);
}

void WriteColor(std::ofstream& out, Color& color)
{
	out.write(reinterpret_cast<char*>(&color.r), sizeof(int));
	out.write(reinterpret_cast<char*>(&color.g), sizeof(int));
	out.write(reinterpret_cast<char*>(&color.b), sizeof(int));
	out.write(reinterpret_cast<char*>(&color.a), sizeof(int));
}

void WriteVector2(std::ofstream& out, Vector2& vector2)
{
	out.write(reinterpret_cast<char*>(&vector2.x), sizeof(int));
	out.write(reinterpret_cast<char*>(&vector2.y), sizeof(int));
}

void WriteVector2F(std::ofstream& out, Vector2F& vector2f)
{
	out.write(reinterpret_cast<char*>(&vector2f.x), sizeof(float));
	out.write(reinterpret_cast<char*>(&vector2f.y), sizeof(float));
}

ObjectCapsule ReadFile(std::ifstream& in)
{
	
}

void ReadString(std::ifstream& in, std::string& string)
{
	size_t size;
	in.read(reinterpret_cast<char*>(&size), sizeof(size));
	string.resize(size);
	in.read(const_cast<char*>(string.c_str()), size);
}

void ReadColor(std::ifstream& in, Color& color)
{
	in.read(reinterpret_cast<char*>(&color.r), sizeof(int));
	in.read(reinterpret_cast<char*>(&color.g), sizeof(int));
	in.read(reinterpret_cast<char*>(&color.b), sizeof(int));
	in.read(reinterpret_cast<char*>(&color.a), sizeof(int));
}
void ReadVector2(std::ifstream& in, Vector2& vector2)
{
	in.read(reinterpret_cast<char*>(&vector2.x), sizeof(int));
	in.read(reinterpret_cast<char*>(&vector2.y), sizeof(int));
}

void ReadVector2F(std::ifstream& in, Vector2F& vector2f)
{
	in.read(reinterpret_cast<char*>(&vector2f.x), sizeof(float));
	in.read(reinterpret_cast<char*>(&vector2f.y), sizeof(float));
}