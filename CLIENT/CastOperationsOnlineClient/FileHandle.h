// Cast Operations Online Client
// file handler class 
// encapulates useful tools for file and string processings

#ifndef FILEHANDLE_H
#define FILEHANDLE_H

#include <fstream>
#include <iostream>
#include <string>

#include "constants.h"
class FileHandle
{
public:
	// constructor
	// filePath : the path to the file to handle
	FileHandle(std::string filePath) {
		currentLine = "";
		infile.open(filePath);
	}

	~FileHandle() {
		infile.close();
	}

	void branch(std::string filePath) {
		currentLine = "";
		infile.close();
		infile.open(filePath);
	} 
	// read next line in a file
	std::string readLine() {
		getline(infile, currentLine);
		return currentLine;
	}


private:
	std::ifstream infile;
	std::string currentLine; // a temporary string 
};
#endif