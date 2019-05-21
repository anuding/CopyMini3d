#pragma once
class Singleton
{
public:
	Singleton();
	Singleton(const Singleton& instance) = delete;
	Singleton(const Singleton&& instance) = delete;
	virtual ~Singleton();
};

