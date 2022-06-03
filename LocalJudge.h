#pragma once
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

class LocalJudge {
	public:
		LocalJudge();
		LocalJudge(std::string codeFileName);
		~LocalJudge();
		bool SetCodeFile(std::string codeFileName);
		bool ExecuteCodeFile(std::string inputFileName, std::string outputFileName);
        std::vector<bool> ClearSuspicion(std::string codeFileName);
        std::int32_t totalLineNum;
	private:
		std::string codeFileName;
        std::ifstream cppTextFile;
		std::fstream modCppTextFile;
        std::fstream routeFile;
        std::string elimSpace(std::string str);
        void BuildModFile();
        bool cmpFiles(std::string filenameA, std::string fileNameB);
};

LocalJudge::LocalJudge() {
	codeFileName = "";
    totalLineNum = 0;
    routeFile.open("route.txt", std::fstream::out); // generate "route.txt"
    routeFile.close();
}

LocalJudge::LocalJudge(std::string filename) {
    codeFileName = "";
    totalLineNum = 0;
    routeFile.open("route.txt", std::fstream::out); // generate "route.txt"
    routeFile.close();
    try {
        if (!SetCodeFile(filename)) {
            throw "No Such Directory or File.";
        }
    }
    catch (const char* error_message)
    {
        std::cerr << error_message << '\n';
    }
}

LocalJudge::~LocalJudge() {
    std::string str = "Mod_" + codeFileName;
	const char* fileName = str.c_str();
	int result = remove(fileName);
	try
	{
		if (result == 1) {
			throw "Mod_" + codeFileName + " deletion unsuccessful.";
		}
	}
	catch (const char* error_message)
	{
		std::cerr << error_message << '\n';
	}

	result = remove("route.txt");
	try
	{
		if (result == 1) {
			throw "\"route.txt\" deletion unsuccessful.";
		}
	}
	catch (const char* error_message)
	{
		std::cerr << error_message << '\n';
	}

    result = remove("output.txt");
	try
	{
		if (result == 1) {
			throw "\"output.txt\" deletion unsuccessful.";
		}
	}
	catch (const char* error_message)
	{
		std::cerr << error_message << '\n';
	}
}

bool LocalJudge::SetCodeFile(std::string filename) {
	if (codeFileName != "") {
		return false;
	}

    codeFileName = filename; // source code file

    cppTextFile.open(codeFileName);
    if(cppTextFile.fail()) return false;
    cppTextFile.close();

    modCppTextFile.open("Mod_" + codeFileName, std::fstream::out); // generate modified code file
    modCppTextFile.close();
    
    BuildModFile(); // build modified code file
    
    return true;
}

bool LocalJudge::ExecuteCodeFile(std::string inputFileName, std::string answerFileName) {
    std::string s = "g++ Mod_" + codeFileName; // compilation command
    const char* compCommand = s.c_str();
    system(compCommand); // compile

    std::fstream outputFile("output.txt", std::fstream::out); // generate "output.txt"
    outputFile.close();

    s = "./a.out <" + inputFileName + " >output.txt"; // execute with input file and ouptut file
    const char* exeCommand = s.c_str();
    system(exeCommand);// execute

    routeFile.open("route.txt", std::fstream::out | std::fstream::app); // add an endline
    routeFile << "\n";
    routeFile.close();

    return cmpFiles(answerFileName, "output.txt"); // compare answerFile & outputFile
}

std::string LocalJudge::elimSpace(std::string r) {
    // from the front
    for (int i = 0; i < r.length(); i++) {
        if (r[i] != ' ') {
            r = r.substr(i);
            break;
        }
    }
    

    // from the back
    for (int i = r.length(); i > 0; i--) {
        if (r[i-1] != ' ') {
            r = r.substr(0, i);
            break;
        }
    }

    return r;
}

std::vector<bool> LocalJudge::ClearSuspicion(std::string codeFileName) {
    cppTextFile.open(codeFileName);
    std::vector<bool> susCleared(1);
    std::string codeLineReader;
    while(getline(cppTextFile, codeLineReader)) {
        codeLineReader = elimSpace(codeLineReader);
        if(codeLineReader == "}" || codeLineReader == "" || codeLineReader == "{") {
            susCleared.push_back(true);
        } else {
            susCleared.push_back(false);
        }
    }
    return susCleared;
}

void LocalJudge::BuildModFile() {

    cppTextFile.open(codeFileName);
    std::string filename = "Mod_" + codeFileName;
    std::ofstream modCppTextFileWriter(filename);

    std::string reader;
    std::int32_t bracket = 0;
    bool notAllowed = false;
    while (getline(cppTextFile, reader)) {
        std::string originalReader = elimSpace(reader); // make a copy of the source code

        // include file stream header file
        if (totalLineNum == 0) {
            reader = "#include <fstream>\n" + reader;
        }

        // route.txt
        if (originalReader == "using namespace std;") {
            reader += " fstream routeFile(\"route.txt\", fstream::out|fstream::trunc);";
        }

        // close file or add record lineNum code
        std::stringstream ss(originalReader);
        std::string word;
        ss >> word;
        if (word == "return") {
            ss >> word;
            if(word == "0") {
                modCppTextFileWriter << "routeFile.close(); " << originalReader << '\n';
                totalLineNum++;
                continue;
            } else {
                reader = " routeFile <<  to_string(__LINE__) << \" \"; " + originalReader;
            }
        }

        // count bracket
        for (int i = 0; i < reader.length(); i++) {
            if (reader[i] == '{') {
                bracket++;
            }
            else if (reader[i] == '}') {
                bracket--;
                if(notAllowed) notAllowed = false;
            }
        }

        // conditions regarding adding __LINE__ or not
        if (originalReader.length() == 0 || originalReader == "{" || originalReader == "}" || word == "struct" || notAllowed) {
            totalLineNum++;
            modCppTextFileWriter << reader << '\n';
            if(word == "struct") {
                notAllowed = true;
            }
            continue;
        } else if (bracket != 0 && originalReader[originalReader.length() - 1] != '}') {
            reader += " routeFile <<  to_string(__LINE__) << \" \";";
        }

        // add reader to modCppTextFile
        reader += '\n';
        totalLineNum++;
        modCppTextFileWriter << reader;
    }
    totalLineNum++;

    cppTextFile.close();
    modCppTextFileWriter.close();
}

bool LocalJudge::cmpFiles(std::string fileNameA, std::string fileNameB) {
    std::ifstream aFile(fileNameA);
    std::ifstream bFile(fileNameB);
    std::vector<std::string> aWordList;
    std::vector<std::string> bWordList;
    std::string line;
    while(getline(aFile, line)) {
        std::stringstream ss(line);
        std::string word;
        while(ss >> word) aWordList.push_back(word);
    }
    aFile.close();

    while(getline(bFile, line)) {
        std::stringstream ss(line);
        std::string word;
        while(ss >> word) bWordList.push_back(word);
    }
    bFile.close();
    return (aWordList == bWordList);
}
