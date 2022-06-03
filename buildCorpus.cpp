#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>

using namespace std;

int main() {
    fstream reader("code_description_corpus.txt", ios::in);
    fstream docListEditor("corpus/docList.txt", ios::out);
    int id = 0;
    fstream writer("corpus/" + to_string(id) + ".txt", ios::out);
    docListEditor << to_string(id) + ".txt\n";
    string line;
    while(getline(reader, line)) {
        stringstream ss(line);
        string term;
        ss >> term;
        if(term == "<doc") {
            writer.close();
            writer.open("corpus/" + to_string(++id) + ".txt", ios::out);
            docListEditor << to_string(id) + ".txt\n";
            continue;
        }
        writer << line;
    }
    reader.close();
    docListEditor.close();
    writer.close();
    return 0;
}