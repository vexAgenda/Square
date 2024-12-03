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
	friend std::istream& operator>>(std::istream& in, Vector2F& vec)
	{
		return in >> vec.x >> vec.y;
	}
	friend std::ostream& operator<< (std::ostream& out, Vector2F vec)
	{
		return out << "{" << vec.x << "," << vec.y << "}";
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

	friend std::istream& operator>>(std::istream& in, ObjectBuffer& buffer)
	{
		return in >> buffer._type >> buffer._objectName >> buffer._fileName >> buffer._text
			>> buffer._originPos >> buffer._velocity >> buffer.hasTarget >>buffer._targetPos >> buffer._mType;
	}

	friend std::ostream& operator<<(std::ostream& out, ObjectBuffer& buffer)
	{
		return out << " " << buffer._type << " " << buffer._objectName << " " << buffer._fileName << " " << buffer._text
			<< " " << buffer._originPos << " " << buffer._velocity << " "<< buffer.hasTarget << " " << buffer._targetPos<< " " << buffer._mType;
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
			std::cout << "[" << i << "]. " << objects[i]._type << " " << objects[i]._objectName << " " << objects[i]._fileName
				<< objects[i]._text << " " << objects[i]._originPos << " " << objects[i]._velocity<< " " << objects[i]._targetPos
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

			if (in)
			{
				ObjectBuffer object;
				while (!in.eof())
				{
					in >> object;
					objects.push_back(object);
				}
			}
		}
		system("timeout -1");
		system("cls");
	}
}