
#include "StdAfx.h"
#include <fstream>
#include "Quitter.h"

Quitter::Quitter(string path){
	this->filePath = path + "\\quit.txt";
}

bool Quitter::read(){
	ifstream file( filePath.c_str() );
	if( file.is_open() ){
		string line;
		if( getline(file, line) ){
			cout << line << endl;
			if( line  == "--- release instrument ---" ){
				cout << "External instruction to Quit received." << endl;
				file.close();
				return true;
			}
		}
		file.close();
	}
	return false;
}