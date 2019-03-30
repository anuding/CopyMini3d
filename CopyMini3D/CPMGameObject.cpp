#include "CPMGameObject.h"
#include <regex>
using namespace std;


CPMGameObject::CPMGameObject()
{
}

bool CPMGameObject::InitAsTriangle()
{
	return true;
}

bool CPMGameObject::InitAsCube()
{
	//Vertex mesh[8] = {
	//	{ { -1, -1,  1, 1 }, { 0, 0 }, { 1.0f, 0.2f, 0.2f }, 1 },
	//	{ {  1, -1,  1, 1 }, { 0, 1 }, { 0.2f, 1.0f, 0.2f }, 1 },
	//	{ {  1,  1,  1, 1 }, { 1, 1 }, { 0.2f, 0.2f, 1.0f }, 1 },
	//	{ { -1,  1,  1, 1 }, { 1, 0 }, { 1.0f, 0.2f, 1.0f }, 1 },
	//	{ { -1, -1, -1, 1 }, { 0, 0 }, { 1.0f, 1.0f, 0.2f }, 1 },
	//	{ {  1, -1, -1, 1 }, { 0, 1 }, { 0.2f, 1.0f, 1.0f }, 1 },
	//	{ {  1,  1, -1, 1 }, { 1, 1 }, { 1.0f, 0.3f, 0.3f }, 1 },
	//	{ { -1,  1, -1, 1 }, { 1, 0 }, { 0.2f, 1.0f, 0.3f }, 1 },
	//};
	//for (int i = 0; i < 8; i++)
	//{
	//	vertex_list.push_back(mesh[i]);
	//}


	//index_list = { 0,3,1, 1,3,2,
	//			   1,2,5, 5,2,6,
	//4,5,7, 7,6,5,
	//0,3,4, 3,7,4,
	//
	//3,2,6, 6,7,3,
	//0,1,4, 4,5,1};
	return false;
}

void LoadFloatData(std::vector<Vector3> &list, string data)
{
	std::string::size_type sz1, sz2;     // alias of size_t
	float x = std::stof(data, &sz1);
	float y = std::stof(data.substr(sz1), &sz2);
	float z = std::stof(data.substr(sz1 + sz2));

	Vector3 tmp = { x, y ,z };
	list.push_back(tmp);
}

void LoadIntData(Index& index, string data)
{

}

bool CPMGameObject::InitFromObj(std::string filename)
{
	cout << "Start loading model from " << filename << endl;
	std::string tmp, file;
	freopen(filename.c_str(), "r", stdin);
	while (getline(std::cin, tmp))
	{
		tmp += '\n';
		file += tmp;
	}
	freopen("CON", "r", stdin);
	std::string pattern_pos("v [^\n]+");
	std::string pattern_normal("vn [^\n]+");
	std::string pattern_tex("vt [^\n]+");
	std::string pattern_face("f [^\n]+");
	std::string pattern_numbers("[0-9]+/[0-9]+/[0-9]+");

	std::regex r_pos(pattern_pos);
	std::regex r_tex(pattern_tex);
	std::regex r_normal(pattern_normal);
	std::regex r_face(pattern_face);
	std::regex r_numbers(pattern_numbers);



	for (std::sregex_iterator it(file.begin(), file.end(), r_pos), end;     //end是尾后迭代器，regex_iterator是regex_iterator的string类型的版本
		it != end;
		++it)
	{
		string pos_line = it->str().substr(1);
		LoadFloatData(pos_list, pos_line);
	}
	
	for (std::sregex_iterator it(file.begin(), file.end(), r_tex), end;     
		it != end;
		++it)
	{
		string tex_line = it->str().substr(2);
		LoadFloatData(tex_list, tex_line);
	}

	for (std::sregex_iterator it(file.begin(), file.end(), r_normal), end;
		it != end;
		++it)
	{
		string normal_line = it->str().substr(2);
		LoadFloatData(normal_list, normal_line);
	}

	for (std::sregex_iterator it(file.begin(), file.end(), r_face), end;
		it != end;
		++it)
	{
		string face_line = it->str().substr(2);
		for (std::sregex_iterator p(face_line.begin(), face_line.end(), r_numbers), end;
			p != end;
			++p)
		{
			// 5/4/3
			Index tmp;
			std::string::size_type sz1, sz2;
			string data = p->str();

			unsigned int pos = std::stoi(data, &sz1);
			unsigned int tex = std::stoi(data.substr(sz1+1), &sz2);
			unsigned int normal = std::stoi(data.substr(sz1 + sz2+2));
			tmp = { pos-1,tex-1,normal-1 };
			index_list.push_back(tmp);
		}
	}

	
	//getchar();
	cout << "Loading completed." << endl;
	return false;
}