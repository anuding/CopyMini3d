#include "VertexShader.h"
#include <iostream>


VertexShader::VertexShader()
{
	
}


VertexShader::~VertexShader()
{
}



void VertexShader::Shading(
	vector<Vector3>& pos_list,
	vector<Index>& index_list,
	vector<Vector3>& tex_list,
	vector<Vector3>& normal_list)
{
	fragment_buffer.reserve(800 * 600);
	pos_buffer.resize(pos_list.size());
	//World -> View -> Projection 
	for (int i = 0; i < pos_list.size(); i++)
	{
		//Vector4, Vector3 = screen_pos,world_pos
		auto res = Mul(Vector4(pos_list[i], 1.0f), world);
		Vector3 tmp(res.x, res.y, res.z);//Vector3(Mul(Vector4(pos_list[i],1.0f), world));
		pos_buffer[i].second = tmp;
		pos_buffer[i].first = Mul(Vector4(pos_list[i], 1.0f), world);
		pos_buffer[i].first = Mul(pos_buffer[i].first, view);
		pos_buffer[i].first = Mul(pos_buffer[i].first, proj);


		//perspective divsion
		pos_buffer[i].first.x = pos_buffer[i].first.x / pos_buffer[i].first.w;
		pos_buffer[i].first.y = pos_buffer[i].first.y / pos_buffer[i].first.w;
		pos_buffer[i].first.z = pos_buffer[i].first.z / pos_buffer[i].first.w;
		pos_buffer[i].first.w = pos_buffer[i].first.w / pos_buffer[i].first.w;

		//screen mapping
		pos_buffer[i].first.x =  pos_buffer[i].first.x * 100.0f + 800 / 2;
		pos_buffer[i].first.y = -pos_buffer[i].first.y * 100.0f + 600 / 2;
	}

	for (int i = 0; i < index_list.size(); i += 3)
	{
		
		Fragment a, b, c;
		a.world_pos = pos_buffer[index_list[i].pos].second;
		b.world_pos = pos_buffer[index_list[i + 1].pos].second;
		c.world_pos = pos_buffer[index_list[i + 2].pos].second;

		a.screen_pos = pos_buffer[index_list[i].pos].first;
		b.screen_pos = pos_buffer[index_list[i + 1].pos].first;
		c.screen_pos = pos_buffer[index_list[i + 2].pos].first;

		a.normal = normal_list[index_list[i].normal];
		b.normal = normal_list[index_list[i + 1].normal];
		c.normal = normal_list[index_list[i + 2].normal];

		a.tex = tex_list[index_list[i].tex];
		b.tex = tex_list[index_list[i + 1].tex];
		c.tex = tex_list[index_list[i + 2].tex];





		//判断绕序

		if (culling_mode == BACKFACE)
		{
			Vector4 e0(a.world_pos - b.world_pos, 0);
			Vector4 e1(c.world_pos - b.world_pos, 0);
			Vector4 n = Normalize(Cross(e0, e1));
			Vector4 eyesight(viewpoint.x, viewpoint.y, viewpoint.z, 0);
			if (Dot(eyesight, n) < 0)//同向,需要剔除
				continue;
		}
		else if (culling_mode == FRONTFACE)
		{
			Vector4 e0(a.world_pos - b.world_pos, 0);
			Vector4 e1(c.world_pos - b.world_pos, 0);
			Vector4 n = Normalize(Cross(e0, e1));
			Vector4 eyesight(viewpoint.x, viewpoint.y, viewpoint.z, 0);
			if (Dot(eyesight, n) > 0)//同向,需要剔除
				continue;
		}


		if (a.screen_pos.y < b.screen_pos.y)
			swap(a, b);
		if (a.screen_pos.y < c.screen_pos.y)
			swap(a, c);
		if (b.screen_pos.y < c.screen_pos.y)
			swap(b, c);

		Fragment d;
		float grad = (b.screen_pos.y - c.screen_pos.y) / (a.screen_pos.y - c.screen_pos.y);

		d = Interpolate(c, a, grad);
		if (b.screen_pos.x > d.screen_pos.x)
		{
			/*
			a
			| \
			d	b
			| /
			c
			*/
			DrawTriA(a, d, b);
			DrawTriV(c, d, b);
		}
		else
		{
			/*
				a
			  /	|
			b	d
			  \	|
				c
			*/
			DrawTriA(a, b, d);
			DrawTriV(c, b, d);
		}
	}
}

void VertexShader::Scanline(Fragment a, Fragment b)
{
	for (float x = a.screen_pos.x; x <= b.screen_pos.x; x++)
	{
		float grad = (x - a.screen_pos.x) / (b.screen_pos.x - a.screen_pos.x);
		if (isnan(grad) || grad < 0)
			continue;
		Fragment v = Interpolate(a, b, grad);
		//fragment_buffer.push_back(v);
		fragment_buffer.emplace_back(v);

	}
}

void VertexShader::DrawTriA(Fragment top, Fragment left, Fragment right)
{
	/*
	top
	|\
	| \
	|  \
	|___\
	left right
	*/
	//if (abs(left.screen_pos.y - top.screen_pos.y) < 1 || abs(right.screen_pos.y - top.screen_pos.y) < 1)
	//	return;
	for (int y = left.screen_pos.y; y <= top.screen_pos.y; y++)
	{
		float grad = (y - left.screen_pos.y) / (top.screen_pos.y - left.screen_pos.y);
		if (grad < 0)
			continue;
		Fragment lv = Interpolate(left, top, grad);
		Fragment rv = Interpolate(right, top, grad);

		Scanline(lv, rv);
	}
}
void VertexShader::DrawTriV(Fragment bottom, Fragment left, Fragment right)
{
	/*
	left  right
	_____
	|   /
	|  /
	| /
	|/
	bottom
	*/
	//if (abs(left.screen_pos.y - bottom.screen_pos.y) < 1 || abs(right.screen_pos.y - bottom.screen_pos.y) < 1)
	//	return;
	for (int y = bottom.screen_pos.y; y <= left.screen_pos.y; y++)
	{
		float grad = (y - bottom.screen_pos.y) / (left.screen_pos.y - bottom.screen_pos.y);
		if (grad < 0)
			continue;
		Fragment lv = Interpolate(bottom, left, grad);
		Fragment rv = Interpolate(bottom, right, grad);

		Scanline(lv, rv);
	}
}
