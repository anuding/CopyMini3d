#include <thread>
#include <iostream>
#include <vector>

void hello()
{
	std::cout << "Hello from thread " << std::endl;
}

int main()
{
	std::thread t1;

	//t1.join();
	std::cout << "Main Thread" << std::endl;
	//std::vector<std::thread> threads;
	//for (int i = 0; i < 5; ++i) {
	//	threads.push_back(hello);
	//}
	getchar();
	return 0;
}