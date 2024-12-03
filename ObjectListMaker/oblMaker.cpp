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

struct ObjectBuffer
{
	std::string _type{};
	std::string _objectName{"test"};
	std::string _fileName{"Data/null.png"};
	std::string _text{ "test" };
	Vector2F _originPos{0,1} ;
	Vector2F _velocity{ 0,0 };
	bool hasTarget{ false };
	Vector2F _targetPos{ 0,1 };
	std::string _mType{};
	friend std::ostream& operator<<(std::ostream& os, ObjectBuffer& buffer)
	{
		return  os << buffer._type << '\n' << buffer._objectName << '\n'
			<< buffer._fileName << '\n' << buffer._text << '\n' <<
			buffer._originPos << buffer._velocity << buffer.hasTarget << '\n'
			<< buffer._targetPos << buffer._mType << '\n';
	}
};

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
			std::cout << "[" << i << "]. " << objects[i]._type << " " << objects[i]._objectName << " " << objects[i]._fileName << " "
				<< objects[i]._text << " {" << objects[i]._originPos.x << ", " << objects[i]._originPos.y << "}"
				<< " {" << objects[i]._velocity.x << ", " << objects[i]._velocity.y << "}"
				<< " " << objects[i].hasTarget
				<< " {" << objects[i]._targetPos.x << ", " << objects[i]._targetPos.y << "} "
				<<" " << objects[i]._mType <<'\n';
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
				std::cout << "Enter Text label" << '\n';
				std::cin >> object._text;
			}
			std::cout << "Enter Origin position" << '\n';
			std::cin >> object._originPos;
			std::cout << "Enter Origin Velocity" << '\n';
			std::cin >> object._velocity;
			std::cout << "Does this object have target? (y/n)" << '\n';
			while (true)
			{
				std::string answer;
				std::cin >> answer;
				if (answer == "y")
				{
					object.hasTarget = true;
					std::cout << "Enter Target Position" << '\n';
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
				out << object;
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
			size_t size;
			in >> size;
			objects.resize(size);
			for (int i = 0; i < objects.size(); ++i)
			{
				std::getline(in, objects[i]._type);
				std::getline(in, objects[i]._objectName);
				std::getline(in, objects[i]._fileName);
				std::getline(in, objects[i]._text);
				in >> objects[i]._originPos.x >> objects[i]._originPos.y;
				in >> objects[i]._velocity.x >> objects[i]._velocity.y;
				in >> objects[i].hasTarget;
				in >> objects[i]._targetPos.x >> objects[i]._targetPos.y;
				in.ignore();
				std::getline(in, objects[i]._mType);

			}

		}
		system("timeout -1");
		system("cls");
	}
}