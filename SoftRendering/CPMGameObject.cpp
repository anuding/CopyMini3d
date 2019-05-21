#include "CPMGameObject.h"
#include <regex>

CPMGameObject::CPMGameObject()
{
}

bool CPMGameObject::InitAsTriangle()
{
	return true;
}

bool CPMGameObject::InitAsCube()
{
	return false;
}

void LoadFloatData(std::vector<Vector3> &list, std::string data)
{
	std::string::size_type sz1, sz2;     // alias of size_t
	float x = std::stof(data, &sz1);
	float y = std::stof(data.substr(sz1), &sz2);
	float z = std::stof(data.substr(sz1 + sz2));

	Vector3 tmp = { x, y ,z };
	list.push_back(tmp);
}

void LoadIntData(Index& index, std::string data)
{

}

bool CPMGameObject::InitFromObj(std::string filename)
{
	std::cout << "Start loading model from " << filename << std::endl;
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
		std::string pos_line = it->str().substr(1);
		LoadFloatData(pos_list, pos_line);
	}
	
	for (std::sregex_iterator it(file.begin(), file.end(), r_tex), end;     
		it != end;
		++it)
	{
		std::string tex_line = it->str().substr(2);
		LoadFloatData(tex_list, tex_line);
	}

	for (std::sregex_iterator it(file.begin(), file.end(), r_normal), end;
		it != end;
		++it)
	{
		std::string normal_line = it->str().substr(2);
		LoadFloatData(normal_list, normal_line);
	}

	for (std::sregex_iterator it(file.begin(), file.end(), r_face), end;
		it != end;
		++it)
	{
		std::string face_line = it->str().substr(2);
		for (std::sregex_iterator p(face_line.begin(), face_line.end(), r_numbers), end;
			p != end;
			++p)
		{
			// 5/4/3
			Index tmp;
			std::string::size_type sz1, sz2;
			std::string data = p->str();

			unsigned int pos = std::stoi(data, &sz1);
			unsigned int tex = std::stoi(data.substr(sz1+1), &sz2);
			unsigned int normal = std::stoi(data.substr(sz1 + sz2+2));
			tmp = { pos-1,tex-1,normal-1 };
			index_list.push_back(tmp);
		}
	}
	Vector3 t;
	if (tex_list.size() == 0)
		tex_list.push_back(t);
	
	//getchar();
	std::cout << "Loading completed." << std::endl;
	return false;
}