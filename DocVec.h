#pragma once
#include <iostream>
#include <cstring>
#include <fstream>
#include <map>
#include <cmath>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include "OleanderStemmingLibrary-master/include/olestem/stemming/english_stem.h"

class DocVec {
    public:
        DocVec();
        ~DocVec();
        std::map<std::string, double> getVec(std::string str, std::string mode);
        double cosSim(std::map<std::string, double>& queryDoc, std::map<std::string, double>& cmpDoc);
        double cosSim(std::vector<double>& queryVec, std::vector<double>& cmpVec);
        
    private:
        void elimPunc(std::string& text);
        void lowerCase(std::string& text);
        void stemming(std::vector<std::string>& vocabList);
        void vocabularize(std::string text, std::vector<std::string>& vocabList);
        void tfidf(std::vector<std::string>& vocabList, std::map<std::string, double>& tfidf_vec);
        std::map<std::string, double> tfidf_vec;
        std::fstream docReader;
};

DocVec::DocVec() {
}

DocVec::~DocVec() {
}

std::map<std::string, double> DocVec::getVec(std::string str, std::string mode) {
    std::vector<std::string> vocabList;
    std::map<std::string, double> tfidf_vec;
    std::string docStr;
    if(mode == "filename") {
        docReader.open(str, std::fstream::in);
        std::string reader;
        while(getline(docReader, reader)) {
            elimPunc(reader);
            lowerCase(reader);
            docStr += reader + ' ';
        }
    } else if(mode == "string") {
        docStr = str;
        elimPunc(docStr);
        lowerCase(docStr);
    }
    vocabularize(docStr, vocabList);
    stemming(vocabList);
    docReader.close();
    tfidf(vocabList, tfidf_vec);
    return tfidf_vec;
}

void DocVec::elimPunc(std::string& text) {
    int size = text.size();
    for(int i = 0; i < size; i++) {
        if((text[i] < '0' || text[i] > '9') && (text[i] < 'A' || text[i] > 'Z') && (text[i] < 'a' || text[i] > 'z')) {
            text[i] = ' ';
        }
    }
}

void DocVec::lowerCase(std::string& text) {
    int size = text.size();
    for(int i = 0; i < size; i++) {
        if(text[i] >= 'A' && text[i] <= 'Z') {
            text[i] += 32;
        }
    }
}

void DocVec::stemming(std::vector<std::string>& vocabList) {
    typedef std::basic_string<wchar_t> myString;

    for(int i = 0; i < vocabList.size(); i++) {
        myString word(L"");

        for(int j = 0; j < vocabList[i].length(); j++) {
            word.push_back(vocabList[i][j]);
        }

        stemming::english_stem<myString> StemEnglish;
        StemEnglish(word);

        vocabList[i] = "";
        for(int j = 0; j < word.size(); j++) {
            vocabList[i].push_back(word.at(j));
        }
    }
}

void DocVec::vocabularize(std::string text, std::vector<std::string>& vocabList) {
    std::stringstream ss(text);
    std::string word;
    while(ss >> word) {
        vocabList.push_back(word);
    }
}

double idfHelper(std::string term, std::vector<std::vector<std::string> >& allVocabList) {
    int count = 0;
    for(int i = 0; i < allVocabList.size(); i++) { // iterate over all documents
        if(std::binary_search(allVocabList[i].begin(), allVocabList[i].end(), term)) count++;
    }
    return 1 + std::log(double(allVocabList.size()+1) / double(count+1));
}

void DocVec::tfidf(std::vector<std::string>& vocabList, std::map<std::string, double>& tfidf_vec) {
    // Term Frequency (TF)
    std::map<std::string, double> tf_val;
    std::map<std::string, double> idf_val;
    for(int i = 0; i < vocabList.size(); i++) {
        tf_val[vocabList[i]]++;
    }
    for(std::map<std::string, double>::iterator it = tf_val.begin(); it != tf_val.end(); it++) {
        tf_val[it->first] /= vocabList.size();
    }

    // Inverse Document Frequency (IDF)
    std::fstream docListReader("corpus/docList.txt", std::ios::in);
    std::vector<std::string> docList;
    std::string docFileName;
    while(getline(docListReader, docFileName)) {
        docList.push_back(docFileName);
    }

    std::vector<std::vector<std::string> > allVocabList(docList.size());
    for(int i = 0; i < docList.size(); i++) {
        std::fstream docReader("corpus/" + docList[i], std::ios::in);
        std::string line;
        while(getline(docReader, line)) {
            std::stringstream ss(line);
            std::string word;
            while(ss >> word) {
                allVocabList[i].push_back(word);
            }
        }
        docReader.close();
    }

    // debug
    // std::cout << "Retrieve the files of corpus: \n";
    // for(int i = 0; i < allVocabList.size(); i++) {
    //     std::cout << "corpus/" << docList[i] << ": ";
    //     for(int j = 0; j < allVocabList[i].size(); j++) {
    //         std::cout << allVocabList[i][j] << " ";
    //     }
    //     std::cout << '\n';
    // }
    // debug

    for(std::map<std::string, double>::iterator it = tf_val.begin(); it != tf_val.end(); it++) {
        std::string term = it->first;
        idf_val[term] = idfHelper(term, allVocabList);
    }
    
    // Combination of TF and IDF
    for(std::map<std::string, double>::iterator it = tf_val.begin(); it != tf_val.end(); it++) {
        tfidf_vec[it->first] = tf_val[it->first] * idf_val[it->first];
    }

    docListReader.close();
}

double DocVec::cosSim(std::map<std::string, double>& queryDoc, std::map<std::string, double>& cmpDoc) {
    // create the union set
    std::set<std::string> unionWordList;
    for(std::map<std::string, double>::iterator it = queryDoc.begin(); it != queryDoc.end(); it++) {
        unionWordList.insert(it->first);
    }
    for(std::map<std::string, double>::iterator it = cmpDoc.begin(); it != cmpDoc.end(); it++) {
        unionWordList.insert(it->first);
    }

    double magnQD = 0, magnCD = 0;
    double relevance_score = 0;

    for(std::set<std::string>::iterator it = unionWordList.begin(); it != unionWordList.end(); it++) {
        double queryVal = queryDoc[*it], cmpVal = cmpDoc[*it];
        relevance_score += queryVal * cmpVal;
        magnQD += queryVal * queryVal;
        magnCD += cmpVal * cmpVal;
    }

    if(magnQD == 0 || magnCD == 0) {
        return 0;
    }
    
    relevance_score = relevance_score / (double)(sqrt(magnQD) * sqrt(magnCD));
    return relevance_score;
}

double DocVec::cosSim(std::vector<double>& queryVec, std::vector<double>& cmpVec) {
    double magnQD = 0, magnCD = 0;
    double relevance_score = 0;
    for(int i = 0; i < queryVec.size(); i++) {
        double queryVal = queryVec[i], cmpVal = cmpVec[i];
        relevance_score += queryVal * cmpVal;
        magnQD += queryVal * queryVal;
        magnCD += cmpVal * cmpVal;
    }

    if(magnQD == 0 || magnCD == 0) {
        return 0;
    }
    
    relevance_score = relevance_score / (double)(sqrt(magnQD) * sqrt(magnCD));
    return relevance_score;
}