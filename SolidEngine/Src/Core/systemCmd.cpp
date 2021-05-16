//
// Created by ryan1 on 16/05/2021.
//
#include <string>
#include <iostream>
#include <cstdio>
#include "Core/systemCmd.hpp"

#ifdef _WIN32


int wincmd(std::string cmd, std::string & output, unsigned int maxOutput)
{
	FILE* out = _popen((cmd+ " 2>&1").c_str(), "rt");
	output.clear();
	output.resize(maxOutput);
	std::fread(output.data(), sizeof(char), maxOutput,out);
	output = "\n" + output;
	int errorCOde = _pclose(out);
	return errorCOde;

}
#endif
