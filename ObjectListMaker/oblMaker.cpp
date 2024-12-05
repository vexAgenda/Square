#include <fstream>
#include <iostream>
#include <string>
#include <vector>

enum ObjectType
{
	OBJ_GAMEOBJECT,
	OBJ_BUTTON,
	OBJ_TEXTBUTTON
};

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

struct ObjectBuffer
{
	std::string _type{};
	std::string _objectName{};
	std::string _fileName{};
	bool hasText{};
	Text _text{};
	Vector2F _originPos{} ;
	Vector2F _velocity{};
	bool hasTarget{ false };
	Vector2F _targetPos{};
	bool isRect{ false };
	Vector2 _widthHeight{};
	Color _rectColor{};
	std::string _mType{};
};
void CreateTarget(ObjectBuffer& object)
{
	while (true)
	{
		std::string answer;
		std::cin >> answer;
		if (answer == "y")
		{
			object.hasTarget = true;
			std::cout << "Enter Target's position" << '\n';
			std::cin >> object._targetPos;

			break;
		}
		else if (answer == "n")
		{
			object.hasTarget = false;
			object._targetPos = { 0,0 };
			break;
		}
	}
}
void CreateRect(ObjectBuffer& object)
{
	while (true)
	{
		std::string answer;
		std::cin >> answer;
		if (answer == "y")
		{
			object.isRect = true;
			std::cout << "Enter Rect's width and height" << '\n';
			std::cin >> object._widthHeight;
			std::cout << "Enter Rect Color r g b a" << '\n';
			std::cin >> object._rectColor;

			break;
		}
		else if (answer == "n")
		{
			object.isRect = false;
			object._widthHeight = { 0,0 };
			object._rectColor = { 0,0,0,255 };
			break;
		}
	}
}

void WriteToFile(std::ofstream& out,ObjectBuffer& buffer);
ObjectBuffer ReadFile(std::ifstream& in);
bool ReadBoolean(std::ifstream& in);
int main(void)
{
	std::vector<ObjectBuffer> objects;
	while (true)
	{
		std::string command;
		std::cout << "Object List Maker" << '\n';
		std::cout << "Add Save Load" <<'\n';
		std::cout << "======================" << '\n';

		for (int i = 0; i < objects.size(); ++i)
		{
			std::print(std::cout,"{}. {} | {} | File Location: {}\n", i, objects[i]._objectName, objects[i]._type,objects[i]._fileName);
			if (objects[i]._type.contains("Text"))
			{
				std::print(std::cout, "Point Size: {}px, Font Color [R: {}, G: {}, B: {}, A: {}], Message: \"{}\"\n", objects[i]._text._ptSize,
					objects[i]._text._fontColor.r, objects[i]._text._fontColor.g, objects[i]._text._fontColor.b, objects[i]._text._fontColor.a, objects[i]._text._msg);
			}
			std::print(std::cout, "Origin Pos: [{}, {}] Velocity : [{}, {}]\n", objects[i]._originPos.x, objects[i]._originPos.y,
				objects[i]._velocity.x, objects[i]._velocity.y);
			std::print(std::cout, "has Target: {}\n", objects[i].hasTarget);
			if (objects[i].hasTarget)
				std::print(std::cout, "Target Pos: [{}, {}]\n", objects[i]._targetPos.x, objects[i]._targetPos.y);
			std::print(std::cout, "is Rect: {}\n", objects[i].isRect);
			if (objects[i].isRect)
			{
				std::print(std::cout,"Width: {} Height: {}, Rect Color[R: {}, G: {}, B: {}, A: {}]\n",objects[i]._widthHeight.x,objects[i]._widthHeight.y,
					objects[i]._rectColor.r, objects[i]._rectColor.g, objects[i]._rectColor.b,objects[i]._rectColor.a);
			}
			std::print(std::cout, "Move Type: {}\n",objects[i]._mType);

		}
		std::cout << "======================" << '\n';
		std::cout << "> ";
		std::getline(std::cin, command);
		if (command == "Add")
		{
			ObjectBuffer object;
			std::cout << "Enter ObjectType" << '\n';
			std::cin >> object._type;
			std::cout << "Enter Object name" << '\n';
			std::cin >> object._objectName;
			std::string fileName;
			std::cout << "Enter FileName (only name, no location)" << '\n';
			std::cin >> fileName;
			object._fileName = "Data/" + fileName + ".png";
			if (object._type.contains("Text"))
			{
				object._fileName = "Data/Fonts/" + fileName + ".ttf";
				std::cout << "Enter Text ptSize" << '\n';
				std::cin >> object._text._ptSize;
				std::cout << "Enter Text Color r g b a" << '\n';
				std::cin >> object._text._fontColor;
				std::cout << "Enter Text message" << '\n';
				std::cin >> object._text._msg;
			}
			std::cout << "Enter Origin position" << '\n';
			std::cin >> object._originPos;
			std::cout << "Enter Origin Velocity" << '\n';
			std::cin >> object._velocity;
			std::cout << "Does this object have target? (y/n)" << '\n';
			CreateTarget(object); 
			std::cout << "Is this object just rect? (y/n)" << '\n';
			CreateRect(object);
			std::cout << "choose movetype : default | square" << '\n';
			std::cin >> object._mType;
			objects.push_back(object);
			
		}
		else if (command == "Save")
		{
			std::cout << "Save Name?" << '\n';
			std::string saveLoc;
			std::cin >> saveLoc;
			saveLoc += ".obl";
			std::ofstream out{ saveLoc,std::ios_base::binary };
			out << objects.size();
			for (auto& object : objects)
			{
				WriteToFile(out,object);
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
				in >> size;
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