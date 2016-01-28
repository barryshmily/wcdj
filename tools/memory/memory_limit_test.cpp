#include <iostream>
#include <cstdlib>   
#include <new>        

void no_memory () {
	std::cout << "Failed to allocate memory!\n";
	std::exit (1);
}

unsigned long size = 2*1024*1024*1024ul + 1024*1024*700;

int main () {
	std::set_new_handler(no_memory);
	std::cout << "attempting to allocate " << size << "... ";
	char* p = new char [size];
	memset(p, 0x01, size);
	std::cout << "Ok\n";
	sleep(20);
	delete[] p;
	return 0;
}
