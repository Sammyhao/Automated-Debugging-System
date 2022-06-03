#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "LocalJudge.h"
#include "DocVec.h"
using namespace std;

const double k = -1.38629436112;

struct codeBlock
{
	int fstLine, lstLine; // first line & last line of code block
	double sus; // suspiciousness value of code block
};

bool cmp(codeBlock a, codeBlock b) { // sort code blocks in descending order based on suspiciousness value
	return a.sus > b.sus;
}

vector<string> getFileNameList(string path) { // get a vector of all filenames within a given path of folder
	struct dirent *entry = NULL;
    DIR *dir = NULL;
	vector<string> fileNameList;
	dir = opendir(path.c_str());
	if (dir != NULL) {
        while ((entry = readdir(dir)))
			// if(entry->d_name == "." || entry->d_name == "..") continue;
			fileNameList.push_back(entry->d_name);
    }
	closedir(dir);
	return fileNameList;
}

void display_cb(codeBlock& cb) { // display the code blocks in the given form
	printf("Line %d ~ %d : %f%%\n", cb.fstLine, cb.lstLine, (cb.sus) * 100);
}

int main() {
	LocalJudge judge;

	string codeFileName = "";

	printf("Automated Debugging System ------------------------------ \n");
	while(!judge.SetCodeFile(codeFileName)) { // generate modified source code file and return successful/ failed
		printf("Enter code file name: ");
		cin >> codeFileName; // source code filename
	}
	printf("\n--------------------------------------------------------- \n");
	
	int succCount = 0, failCount = 0; // count of successful test cases, count of failed test cases
	int totalLineNum = judge.totalLineNum; // total line number of source code

	vector<int> succ(totalLineNum+1); // how many successful test cases passed in the line
	vector<int> fail(totalLineNum+1); // how many failed test cases passed in the line
	vector<double> lineRankOfSus(totalLineNum+1); // suspiciousness ranking

	string inputFileNames, outputFileNames; // input filename list, output filename list
	printf("Enter all input filenames: ");
	getline(cin,inputFileNames); // for the first blank line
	getline(cin,inputFileNames); // all input filenames
	printf("Enter all output filenames: ");
	getline(cin,outputFileNames); // all output filenames
	stringstream input(inputFileNames), output(outputFileNames);
	string inputFileName, outputFileName; // single input filename, single output filename

	printf("\n--------------------------------------------------------- \n");
	printf("Bug Localization Analysis for %s: \n", codeFileName.c_str());

	int count = 1; // current number of execution
	
	while(input >> inputFileName && output >> outputFileName) { // every pair of input and output file
		bool result = judge.ExecuteCodeFile(inputFileName, outputFileName); // execute the source code by input and output -> get result
		ifstream routeFile("route.txt", ifstream::in); // execution path recorded
		string route;
		vector<bool> hasRecorded(totalLineNum+1);

		getline(routeFile, route);
		routeFile.close();
		printf("Execution Path for Test Case %d : %s- %s\n", count, route.c_str(), result ? "Pass" : "Failed"); // output every execution: execution number, execution route, execution result

		stringstream ss(route); // read route 
		string lineNumStr;
		if(result) { // if current execution succeeds
			succCount++; // add one to succCount
			while(ss >> lineNumStr) { // seperate into every line number
				int lineNum = atoi(lineNumStr.c_str()); // transform to int
				lineNum--; // modified version line number -> original version line number
				if(!hasRecorded[lineNum]) { // every line should only be added once for this execution
					succ[lineNum]++; // add one to the given line in the successful test cases vector
					hasRecorded[lineNum] = true; // current line already added once
				}
			}
		} else { // if current execution fails
			failCount++; // add one to failCount
			while(ss >> lineNumStr) {  // seperate into every line number
				int lineNum = atoi(lineNumStr.c_str()); // transform to int
				lineNum--; // modified version line number -> original version line number
				if(!hasRecorded[lineNum]) {  // every line should only be added once for this execution
					fail[lineNum]++; // add one to the given line in the failed test cases vector
					hasRecorded[lineNum] = true; // current line already added once
				}
			}
		}
		count++; // add one to the current number of execution
	}
	
	set<int> useDeltaMethod;
	vector<double> falseEnterSus(totalLineNum + 1);
	int useDeltaMethodCnt = 0;
	for(int i = 1; i <= totalLineNum; i++) {
		double succRatio = (succCount == 0) ? 0 : (double)succ[i] / succCount; // ratio of the number of times every line of code being passed by in an successful test case to the number of successful test cases
		double failRatio = (failCount == 0) ? 0 : (double)fail[i] / failCount; // ratio of the number of times every line of code being passed by in an failed test case to the number of failed test cases
		lineRankOfSus[i] = (succRatio + failRatio == 0) ? 0 : (double) max(failRatio / (succRatio + failRatio), (succRatio - failRatio) / (succRatio + failRatio)); // the ratio of failed ratio to the summation of successful ratio and failed ratio
		if(succRatio == 0 && failRatio != 0) {
			lineRankOfSus[i] = 0.5;
			continue;
		}
		falseEnterSus[i] = failRatio / (succRatio + failRatio);
		if(lineRankOfSus[i] == (succRatio - failRatio) / (succRatio + failRatio)) {
			useDeltaMethod.insert(i);
			if(lineRankOfSus[i] != lineRankOfSus[i-1]) {
				useDeltaMethodCnt++;
			}
		}
	}

	for(set<int>::iterator it = useDeltaMethod.begin(); it != useDeltaMethod.end(); it++) {
		lineRankOfSus[*it] = max(falseEnterSus[*it], lineRankOfSus[*it] / (useDeltaMethodCnt+1));
	}

	// transforming all previous information into codeBlocks
	vector<codeBlock> codeBlocks; // code block list: stores information of all code blocks of the source code
	vector<bool> susClearedLines = judge.ClearSuspicion(codeFileName); // get the suspicion-cleared line list

    int lb = 1, rb = 1; // left bound of code block, right bound of code block
    while(lb <= totalLineNum && rb <= totalLineNum) { // while the left and right bound are within the line number range of source code
        if(lineRankOfSus[lb] == lineRankOfSus[rb]) { // when the current left bound has the same suspiciousness value as the right bound
            rb++; // push the right bound rightward
        } else { // when the current left bound has different suspiciousness values as the right bound
            codeBlock cb; // generate a code block
            cb.fstLine = lb; // left bound: current left bound
            cb.lstLine = rb - 1; // right bound: current right bound - 1
            cb.sus = lineRankOfSus[lb]; // suspiciousness value: the suspiciousness value of the lines of this codeblock
			codeBlocks.push_back(cb); // add the code block into the code block list
            lb = rb; // update the left bound to become same as the right bound
        }
    }
	// the entail of the last code block to the code block list (since the current right bound will be out of the range)
	codeBlock cb;
    cb.fstLine = lb;
    cb.lstLine = rb - 2;
    cb.sus = lineRankOfSus[lb];
	codeBlocks.push_back(cb);
    lb = rb;

	sort(codeBlocks.begin(), codeBlocks.end(), cmp); // sort the code block list to a descending order based on their suspiciousness values
	vector<bool> susCleared(codeBlocks.size()); // code block that has no suspicion for sure
	for(int i = 0; i < susCleared.size(); i++) {
		int j;
		cb = codeBlocks[i];
		for(j = cb.fstLine; j <= cb.lstLine; j++) {
			if(!susClearedLines[j]) {
				break;
			}
		}
		if(j == cb.lstLine + 1) {
			susCleared[i] = true;
		} else {
			susCleared[i] = false;
		}
	}

	printf("\n--------------------------------------------------------- \n");
	// Final Output
	printf("Initial Suspiciousness Ranking: \n");
	// show the suspiciousness ranking
    for(int i = 0; i < codeBlocks.size(); i++) {
		display_cb(codeBlocks[i]);
	}

	DocVec dv;
	printf("\n--------------------------------------------------------- \n");
	cout << "Enter description for the source code: \n";
	string srcCodeDescStr;
	string line;
	while(getline(cin, line) && line != "") {
		srcCodeDescStr += line + " ";
	}
	printf("\n--------------------------------------------------------- \n");
	cout << "Enter description for every code blocks: \n";
	vector<string> codeBlocksDesc(codeBlocks.size()); // description of every codeBlocks
	string codeBlocksDescStr; // store the same content as codeBlocksDesc but in only one string
	for(int i = 0; i < codeBlocks.size(); i++) {
		cout << "Description for code block " << i + 1 << " (Line " << codeBlocks[i].fstLine << " to " << codeBlocks[i].lstLine << ") : ";
		getline(cin, codeBlocksDesc[i]);
		codeBlocksDescStr += codeBlocksDesc[i] + ' '; // update codeBlocksDescStr
	}

	// find the most relevant file and its corresponding folder
	map<string, double> codeBlocksMap = dv.getVec(srcCodeDescStr, "string"); // the tf-idf vector of current code blocks descriptions

	fstream docListReader("database/docList.txt", ios::in); // open the document list file
	string maxRelevantFilename; // filename of the most relevant file
	double maxRelevanceScore = 0; // maximum relevant score
	string docName; // name of the document in docList.txt
	int docNum = 0;
	while(getline(docListReader, docName)) { // iterate through all the files
		map<string, double> newCodeBlocksMap = dv.getVec("database/" + docName, "filename"); // the tf-idf vector of the new file
		map<string, double> tempNewCodeBlocksMap;

		// only record the terms within codeBlocksMap
		for(map<string, double>::iterator it = codeBlocksMap.begin(); it != codeBlocksMap.end(); it++) {
			tempNewCodeBlocksMap[it->first] = newCodeBlocksMap[it->first];
		}

		newCodeBlocksMap = tempNewCodeBlocksMap; // update newCodeBlocksMap

		double newRelevanceScore = dv.cosSim(codeBlocksMap, newCodeBlocksMap); // calculate the cosine similarity between current code blocks descriptions and new file
		if(newRelevanceScore > maxRelevanceScore) { // if new file is more relevant
			maxRelevanceScore = newRelevanceScore; // replace the maxRelevanceScore with the newRelevanceScore
			maxRelevantFilename = docName; // replace the maxRelevanceFilename with the new filename
		}
		docNum++;
	}

	docListReader.close();
	
	vector<double> finalSus(codeBlocks.size());  // records the final suspiciousness value of the codeblocks
	// initialization of finalSus
	for(int i = 0; i < finalSus.size(); i++) {
		finalSus[i] = codeBlocks[i].sus;
	}
	string newFilename; // new file name

	printf("\n--------------------------------------------------------- \n");
	printf("Most relevant file is %s, with its relevance score %f\n", maxRelevantFilename.c_str(), maxRelevanceScore);

	if(maxRelevanceScore > 0.8) {
		string cmpFoldername = maxRelevantFilename.substr(0, maxRelevantFilename.size() - 4); // get cmpFoldername from the most relevant file's name

		cout << "Iterating through " << cmpFoldername << "...\n";

		vector<string> fileList = getFileNameList("database/" + cmpFoldername);  // all the files within cmpFolder

		// iterate through every file
		vector<double> codeBlocksSus(codeBlocks.size()); // the code block suspiciousness sequence for further comparison with other files' suspiciousness sequence
		// initialization of codeBlocksSus
		for(int i = 0; i < codeBlocksSus.size(); i++) {
			codeBlocksSus[i] = codeBlocks[i].sus;
		}
		codeBlocksMap = dv.getVec(codeBlocksDescStr, "string"); // codeBlocksMap is the tf-idf vector for the code block descriptions of current source code file

		vector<double> updateWeight(codeBlocks.size()); // records the update weight of the code blocks
		vector<double> totalRelevScore(codeBlocks.size()); // records the total relevance score of the code blocks
		for(int fileIdx = 2; fileIdx < fileList.size(); fileIdx++) { // the first two values in fileList are not valid
			printf("\n--------------------------------------------------------- \n");
			cout << "File " << fileIdx-1 << ": " << fileList[fileIdx] << "\n\n";

			fstream fileReader("database/" + cmpFoldername + "/" + fileList[fileIdx], ios::in); // open every file in cmpFolder

			// In every file, match with the corresponding code blocks with descriptions
			vector<codeBlock> cmpCodeBlocks;
			vector<string> cmpCodeBlocksDesc;
			set<int> buggyCodeBlocks; // records the fixed buggy code blocks
			string cbLine, descLine; // every line of the document
			int codeBlockIdx = 0;

			while(getline(fileReader, cbLine) && getline(fileReader, descLine)) { // read two line at a time, line 1 = information (fstLine, lstLine, sus) of code block, line 2 = description of code block
				codeBlock cb;
				stringstream ss(cbLine);
				string dataStr;
				ss >> dataStr; // first line
				cb.fstLine = atoi(dataStr.c_str());
				ss >> dataStr; // last line
				cb.lstLine = atoi(dataStr.c_str());
				ss >> dataStr; // suspicious value
				cb.sus = atof(dataStr.c_str());
				ss >> dataStr;
				if(dataStr == "fixed") {
					buggyCodeBlocks.insert(codeBlockIdx);
				}

				cmpCodeBlocks.push_back(cb); // push back code block into cmpCodeBlocks
				cmpCodeBlocksDesc.push_back(descLine); // push back description line into cmpCodeBlocksDesc
				codeBlockIdx++;
			}

			fileReader.close();

			vector<int> cbMatch(codeBlocks.size()); // cbMatch[codeBlocks index] = corresponding cmpCodeBlocks index
			// vector<double> cbMatchRelev(codeBlocks.size()); // cbMatch[codeBlocks index] = corresponding cmpCodeBlocks textual relevance
			vector<double> cbMatchSus(codeBlocks.size()); // cbMatch[codeBlocks index] = corresponding cmpCodeBlocks suspiciousness value relevance
			string cmpCodeBlocksDescStr;
			for(int i = 0; i < codeBlocksDesc.size(); i++) { // iterate through every code block description of current source code 
				if(susCleared[i]) {
					cbMatch[i] = -1; // -1 indicates there's no need for ith code block to match any historical code block since it's suspicion-cleared
					continue;
				}
				map<string, double> cbDescVec = dv.getVec(codeBlocksDesc[i], "string"); // generate tf-idf vector for current code block description
				int maxDescRelevIdx = 0; // index of the most relevant description in cmpCodeBlocksDesc
				double maxDescRelevScore = 0; // relevance score of the most relevant description in cmpCodeBlocksDesc

				// find the most relevant codeblock in the compared codeblock vector
				for(int j = 0; j < cmpCodeBlocks.size(); j++) { // iterate through all codeblocks in cmpCodeBlocks
					map<string, double> cmpCbDescVec = dv.getVec(cmpCodeBlocksDesc[j], "string");
					double newDescRelevScore = dv.cosSim(cbDescVec, cmpCbDescVec);
					if(newDescRelevScore > maxDescRelevScore) {
						maxDescRelevScore = newDescRelevScore;
						maxDescRelevIdx = j;
					}
				}
				cbMatch[i] = maxDescRelevIdx; // match the current code block index of current source code to the most relevant code block in the compared codeblock vector
				double deltaVal = (max(codeBlocks[i].sus, cmpCodeBlocks[cbMatch[i]].sus) - min(codeBlocks[i].sus, cmpCodeBlocks[cbMatch[i]].sus));
				cbMatchSus[i] = exp(deltaVal * k);
			}

			cout << "Code Block Matching: \n";
			for(int i = 0; i < cbMatch.size(); i++) {
				cout << "code block " << i + 1 << " to code block " << cbMatch[i] + 1 << '\n';
				cmpCodeBlocksDescStr += (cbMatch[i] >= 0) ? cmpCodeBlocksDesc[cbMatch[i]] + ' ' : "";
			}
			cout << '\n';

			map<string, double> cmpCodeBlocksMap = dv.getVec(cmpCodeBlocksDescStr, "string"); // cmpCodeBlocksMap is the tf-idf vector for the code block descriptions of the historical source code file

			// build corresponding suspiciousness sequence based on the order of cbMatch
			vector<double> cmpCodeBlocksSus(codeBlocksSus.size());
			for(int i = 0; i < cmpCodeBlocksSus.size(); i++) {
				if(cbMatch[i] == -1) { // if suspicion cleared
					cmpCodeBlocksSus[i] = 0;
					continue;
				}
				cmpCodeBlocksSus[i] = cmpCodeBlocks[cbMatch[i]].sus;
			}

			// file relevance: cosine similarity between source code file description and compared source code file description
			double fileRelevanceScore = dv.cosSim(codeBlocksMap, cmpCodeBlocksMap);

			// bug relevance: cosine similarity between source code file suspiciousness sequence and corresponding compared source code file suspiciousness sequence
			double bugRelevanceScore = dv.cosSim(codeBlocksSus, cmpCodeBlocksSus);

			// cout << "The file relevance is " << fileRelevanceScore << " (fileRelevanceScore) x " << bugRelevanceScore << " (bugRelevanceScore)\n";
			cout << "buggyCodeBlocks of compared file: ";
			for(set<int>::iterator it = buggyCodeBlocks.begin(); it != buggyCodeBlocks.end(); it++) {
				cout << *it + 1 << " ";
			}
			cout << "\n\n";
			cout << "Updated suspiciousness values: " << '\n';
			
			// use the cosine similarity to update suspiciousness values of code blocks corresponding to fixed ones
			for(int i = 0; i < codeBlocks.size(); i++) {
				double relevanceScore = fileRelevanceScore * bugRelevanceScore * cbMatchSus[i]; // calculate the relevance score between code blocks
				if(buggyCodeBlocks.count(cbMatch[i])) {
					cout << "code block " << i + 1 << ": \n";
					cout << "Relevance: " <<  relevanceScore << "\n";
					updateWeight[i] += relevanceScore * relevanceScore;
					totalRelevScore[i] += relevanceScore;
				}
			} 
		}

		for(int i = 0; i < codeBlocks.size(); i++) {
			updateWeight[i] = (totalRelevScore[i]) ? updateWeight[i] / totalRelevScore[i] : 0;
			finalSus[i] += (1 - finalSus[i]) * updateWeight[i];
		}

		// add descriptions to the most relevant file
		fstream docReader("database/" + maxRelevantFilename, ios::out|ios::app);
		docReader << srcCodeDescStr << '\n';
		docReader.close();
		newFilename = cmpFoldername + "/" + codeFileName.substr(0, codeFileName.size() - 4) + ".txt"; // new file name
	}
	// the current code doesn't fit into any catagorized datasets. Create a new category (new code description file + new folder with new file of code blocks info & descriptions + description file name in docList.txt)
	else {
		// create new code description file
		string newCodeDescFilename = to_string(docNum + 1) + ".txt";
		fstream newCodeDescFileReader("database/" + newCodeDescFilename, fstream::out);
		newCodeDescFileReader << srcCodeDescStr << '\n';
		newCodeDescFileReader.close();
		// update docList.txt
		docListReader.open("database/docList.txt", ios::out|ios::app);
		docListReader << newCodeDescFilename << '\n';
		docListReader.close();
		// create a new folder
		string newFoldername = to_string(docNum + 1); // new folder name
		newFoldername = newFoldername;
		string dir = "database/" + newFoldername;
		mkdir(dir.c_str(), 0777); // create a new folder using new folder name
		newFilename = newFoldername + "/" + codeFileName.substr(0, codeFileName.size() - 4) + ".txt"; // new file name
		cout << newFilename << '\n';
	}

	vector<codeBlock> finalCodeBlocks = codeBlocks; // final version of the code block information list

	for(int i = 0; i < codeBlocks.size(); i++) {
		finalCodeBlocks[i].sus = finalSus[i];
	}

	sort(finalCodeBlocks.begin(), finalCodeBlocks.end(), cmp);

	// print the eventual result
	printf("\n--------------------------------------------------------- \n");
	// Final Output
	printf("Final Suspiciousness Ranking: \n");
	for(int i = 0; i < finalCodeBlocks.size(); i++) {
		printf("Line %d ~ %d : %f%%\n", finalCodeBlocks[i].fstLine, finalCodeBlocks[i].lstLine, (finalCodeBlocks[i].sus) * 100);
	}

	// ask which code block fixed at last
	printf("\n--------------------------------------------------------- \n");
	printf("Enter the eventual line fixed: \n");
	string fixedLinesStr, fixedLineStr;
	getline(cin, fixedLinesStr); // input all fixed line numbers
	stringstream fixedLinesStrReader(fixedLinesStr);
	string fixedLine;
	set<int> fixedLines; // records all lines being fixed
	while(fixedLinesStrReader >> fixedLine) { // seperate input into individual words (line number)
		fixedLines.insert(atoi(fixedLine.c_str())); // insert the line number into fixedLines
	}
	printf("\n--------------------------------------------------------- \n");
	
	// upload the file of current code block information + description to the file
	fstream newFile("database/" + newFilename, fstream::out); // create new file using new file name
	for(int i = 0; i < codeBlocks.size(); i++) {
		string cbInfo = to_string(codeBlocks[i].fstLine) + ' ' + to_string(codeBlocks[i].lstLine) + ' ' + to_string(codeBlocks[i].sus);
		for(int j = codeBlocks[i].fstLine; j <= codeBlocks[i].lstLine; j++) { // iterate through every line of the current code block to check if code block includes fixed lines
			if(fixedLines.count(j)) { // if there exists lines being fixed, add "fixed" to the current code block and break
				cbInfo += " fixed";
				break;
			}
		}
		cbInfo += '\n' + codeBlocksDesc[i] + '\n'; // add the description into the current code block info string
		newFile << cbInfo; // add current code block info string into the new file
	}
	newFile.close();
	return 0;
}
