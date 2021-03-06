#include "PixelShader.h"
#include <Windows.h>
#include "PBR.h"
#include <thread>
#define RGBA(r,g,b,a)   ((COLORREF) ((uint8_t)(a)<<24)|\
									((uint8_t)(r)<<16|\
									((uint8_t)(g)<<8)|\
									((uint8_t)(b))))                                   
#define ABS(a) ((a) >= 0 ? (a) : -(a))
#define PI 3.1415926

#define RED RGBA(255,0,0,0)
#define GREEN RGBA(0,255,0,0)
#define BLUE RGBA(0,0,255,0)
#define YELLOW RGBA(255,255,0,0)
#define PURPLE RGBA(255,0,255,0)
#define LIGHTBLUE RGBA(0,255,255,0)
#define WHITE RGBA(255,255,255,0)
#define BLACK RGBA(0,0,0,0)
#define CR_BK RGBA(255,250,250,0)

float maxz = INT_MIN, minz = INT_MAX, sum = 0;

Vector3 lightPositions[] = {
	Vector3(-10.0f,  10.0f, 10.0f),
	Vector3(10.0f,  10.0f, 10.0f),
	Vector3(-10.0f, -10.0f, 10.0f),
	Vector3(10.0f, -10.0f, 10.0f),
};
Vector3 lightColors[] = {
	Vector3(300.0f, 300.0f, 300.0f),
	Vector3(300.0f, 300.0f, 300.0f),
	Vector3(300.0f, 300.0f, 300.0f),
	Vector3(300.0f, 300.0f, 300.0f)
};

PixelShader::PixelShader()
{
}
PixelShader::PixelShader(unsigned int ** _frame_buffer, float ** _zbuffer)
{
	frame_buffer = _frame_buffer;
	zbuffer = _zbuffer;
}

PixelShader::~PixelShader()
{
}
void PixelShader::decodeOneStep(const char* filename, std::vector<std::vector<unsigned int>>& tex)
{
	std::vector<unsigned char> image; //the raw pixels
	unsigned width, height;

	//decode
	unsigned error = lodepng::decode(image, width, height, filename);

	//if there's an error, display it
	if (error)
	{
		std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

	}
		
	tex.resize(height);
	for (auto& i : tex)
	{
		i.resize(width);
	}
	//the pixels are now in the std::vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
	int cnt = 0;
	for (int i = 0; i < width; i++)
	{
		//std::vector<IUINT32> tmp;
		//texture.push_back(tmp);
		for (int j = 0; j < height; j++)
		{
			tex[i][j] = RGB(image[cnt], image[cnt + 1], image[cnt + 2]);
			cnt += 4;
		}
	}
}

Vector3 ToRGB(texture& tex, int x, int y)
{
	IUINT32 color = tex[x][y];
	unsigned char r = color;
	unsigned char g = color >> 8;
	unsigned char b = color >> 16;
	unsigned char a = color >> 24;
	Vector3 ans(r / 255.0f, g / 255.0f, b / 255.0f);
	return ans;
}

Vector3 PixelShader::GetColor(texture& tex,float u, float v)
{
	if (u < 0 || v < 0 || isnan(u) || isnan(v))
	{
		std::cout << "u: " << u << ", v: " << v << std::endl;
		return Vector3(0,0,0);
	}

	int ulen = tex.size() - 1;
	int vlen = tex[0].size() - 1;

	u = u * ulen;
	v = v * ulen;

	int ul = u;
	int ur = (ul != ulen) ? ul + 1 : ul;
	int vl = v;
	int vr = (vl != vlen) ? vl + 1 : vl;

	Vector3 ulvr, urvr, ulvl, urvl, down, top,ans;
	ulvr = ToRGB(tex, ul, vr);
	urvr = ToRGB(tex, ur, vr);
	ulvl = ToRGB(tex, ul, vl);
	urvl = ToRGB(tex, ur, vl);
	down = ulvr * (1.0f - u + (float)ul) + urvr * (u - (float)ul);
	top = ulvl * (1.0f - u + (float)ul) + urvl * (u - (float)ul);
	ans= down * (1.0f - v + (float)vl) + top * (v - (float)vl);

	return ans;
}

void PixelShader::PBRShading(std::vector<Fragment> & fragment_buffer)
{
	for (auto& v : fragment_buffer)
	{
		if (v.screen_pos.x >= 800 || v.screen_pos.x <= 0 || v.screen_pos.y >= 600 || v.screen_pos.y <= 0)
			continue;
		int x = v.screen_pos.x, y = v.screen_pos.y;

		if (isnan(v.screen_pos.z))
			continue;

		if (v.screen_pos.z < zbuffer[y][x])//
		{
			Vector3 N = Normalize(v.normal);
			Vector3 V = Normalize(camPos - v.world_pos);

			Vector3 F0(0.04f, 0.04f, 0.04f);

			Vector3 albedo_value = GetColor(albedo, v.tex.x, v.tex.y);
			albedo_value.SetVector(pow(albedo_value.x, 2.2f), pow(albedo_value.y, 2.2f), pow(albedo_value.z, 2.2f));
			float roughness_value = GetColor(roughness, v.tex.x, v.tex.y).x;
			float metallic_value = GetColor(metallic, v.tex.x, v.tex.y).x;
			float ao_value = GetColor(ao, v.tex.x, v.tex.y).x;


			F0 = F0 + (albedo_value - F0)*metallic_value;

			Vector3 Lo;
			for (int i = 0; i < 4; ++i)
			{
				Vector3 L = Normalize(lightPositions[i] - v.world_pos);
				Vector3 H = Normalize(V + L);
				float distance = Length(lightPositions[i] - v.world_pos);
				float attenuation = 1.0 / (distance * distance);
				Vector3 radiance = lightColors[i] * attenuation;

				float NDF = DistributionGGX(N, H, roughness_value);
				float G = GeometrySmith(N, V, L, roughness_value);
				Vector3 F = fresnelSchlick(Clamp(Dot(H, V), 0.0, 1.0), F0);

				Vector3 nominator = NDF * G * F;
				float denominator = 4 * max(Dot(N, V), 0.0) * max(Dot(N, L), 0.0);
				Vector3 specular = nominator * (1 / max(denominator, 0.001)); 

				Vector3 kS = F;
			
				Vector3 kD = Vector3(1.0, 1.0, 1.0) - kS;

				kD = kD * (1.0 - metallic_value);


				float NdotL = max(Dot(N, L), 0.0);


				Lo = Lo + Modulate((Modulate(kD, albedo_value) * (1 / PI) + specular), radiance) * NdotL;
			}

			Vector3 ambient = Modulate(Vector3(0.03, 0.03, 0.03), albedo_value) * ao_value;

			Vector3 color = ambient + Lo;

			// HDR tonemapping
			color = Modulate(color, 1 / (color + Vector3(1.0, 1.0, 1.0)));
			// gamma correct
			//color = pow(color, Vector3(0.454f, 0.454f, 0.454f));
			color.SetVector(pow(color.x, 0.454f), pow(color.y, 0.454f), pow(color.z, 0.454f));
			//FragColor = std::vector4(color, 1.0);
			frame_buffer[y][x] = RGB(color.z * 255,
				color.y * 255,
				color.x * 255);
			zbuffer[y][x] = v.screen_pos.z;
		}
	}
}

void ConstantShading(
	const std::vector<Fragment> & fragment_buffer,
	int start,
	int end,
	PixelShader* p)
{
	for (int i=start;i<end;i++)
	{
		auto v = fragment_buffer[i];
		if (v.screen_pos.x >= 800 || v.screen_pos.x <= 0 || v.screen_pos.y >= 600 || v.screen_pos.y <= 0)
			continue;
		int x = v.screen_pos.x, y = v.screen_pos.y;

		if (isnan(v.screen_pos.z))
			continue;

		if (v.screen_pos.z < p->zbuffer[y][x])//
		{
			Vector3 tex_color = p->GetColor(p->albedo, v.tex.x, v.tex.y);
			Vector3 diffuse = tex_color;

			p->frame_buffer[y][x] = RGB(diffuse.z * 255,
				diffuse.y * 255,
				diffuse.x * 255);
			p->zbuffer[y][x] = v.screen_pos.z;
		}
	}
}

void PixelShader::LightShading(std::vector<Fragment> & fragment_buffer)
{
	for (auto& v : fragment_buffer)
	{
		if (v.screen_pos.x >= 800 || v.screen_pos.x <= 0 || v.screen_pos.y >= 600 || v.screen_pos.y <= 0)
			continue;
		int x = v.screen_pos.x, y = v.screen_pos.y;

		if (isnan(v.screen_pos.z))
			continue;

		if (v.screen_pos.z < zbuffer[y][x])//
		{
			Vector3 tex_color = GetColor(albedo, v.tex.x, v.tex.y);
			float theta = light.theta;
			Vector3 ld(cos(theta), 1, sin(theta));//light dir
			ld = Normalize(ld);
			Vector3 l(0.8, 0.8, 0.8);//light color
			Vector3 m(1.0, 1.0, 1.0);//material
			Vector3 n = Normalize(v.normal);

			Vector3 diffuse = Modulate(l, tex_color) * max(Dot(ld, n), 0);
			//diffuse.Setstd::vector(v.normal.x, v.normal.y, v.normal.z);
			//diffuse.Setstd::vector(1, 0, 0);
			frame_buffer[y][x] = RGB(diffuse.z * 255,
				diffuse.y * 255,
				diffuse.x * 255);
			zbuffer[y][x] = v.screen_pos.z;
		}
	}
}

void PixelShader::Shading(std::vector<Fragment> & fragment_buffer)
{
	int len = fragment_buffer.size();
	//ConstantShading(fragment_buffer,0,len, this);
	PBRShading(fragment_buffer);
	//LightShading(fragment_buffer);
	
}

void PixelShader::LoadTextures(std::vector<const char*> filenames)
{
	decodeOneStep(filenames[0], albedo);
	decodeOneStep(filenames[1], roughness);
	decodeOneStep(filenames[2], metallic);
	decodeOneStep(filenames[3], ao);
}
