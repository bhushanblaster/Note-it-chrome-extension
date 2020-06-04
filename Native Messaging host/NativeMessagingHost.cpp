
#include<iostream>
#include <fcntl.h>
#include <io.h>
#include<string>
#include "RequestHandler.h"


int main(int argc, char* argv[])
{

	_setmode(_fileno(stdin), _O_BINARY);

	// Sum the first 4 chars from stdin (the length of the message passed).
	char bInLen[4];
	std::cin.read(bInLen, 4);
	unsigned int inLen = *reinterpret_cast<unsigned int *>(bInLen);

	char *inMsg = new char[inLen];
	std::cin.read(inMsg, inLen);
	std::string inStr(inMsg); // if you have managed types, use them!
	delete[] inMsg;

	std::string response = processRequest(const_cast<char*>(inStr.c_str())); //get the response

	unsigned int outLen = response.length();
	char *bOutLen = reinterpret_cast<char *>(&outLen);
	std::cout.write(bOutLen, 4);
	std::cout << response << std::flush; //send the response
	return 0;
}

