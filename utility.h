#pragma once
#include<iostream>
#include<algorithm>
#include<filesystem>
#include<map>
#include<windows.h>
#include<conio.h>
#include<ranges>
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS 1
#include<unordered_map>
#include<vector>
#include<fstream>
#include<sstream>
#include<string>

using namespace std;
namespace fs = std::filesystem;

static class wordIndocument {
private:

    string documentName;
    vector <int> position;

public:
    wordIndocument() {
        documentName.clear();
        position.clear();
    }
    string getDocname() const { return documentName; }
    vector<int> getPos() const { return position; }

    wordIndocument(string _docName, int _pos) {
        documentName = _docName;
        position.push_back(_pos);
    }
    void addPosition(int _pos) {
        position.push_back(_pos);
    }
};

inline bool caseInsensitiveCompare(char c1, char c2) {
    return std::toupper(c1) == std::toupper(c2);
}
inline void getFilesInFolder(const string& folderPath, vector<string>& files) {
    try {
        for (const auto& entry : fs::directory_iterator(folderPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                files.push_back(entry.path().string());
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        cout << "Error accessing folder: " << e.what() << endl;
    }
}
static void getRowColbyLeftClick(int& rpos, int& cpos)
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD Events;
    INPUT_RECORD InputRecord;
    SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
    do
    {
        ReadConsoleInput(hInput, &InputRecord, 1, &Events);
        if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
            rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
            break;
        }
    } while (true);
}
inline void gotoRowCol(int rpos, int cpos)
{
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = cpos;
    scrn.Y = rpos;
    SetConsoleCursorPosition(hOuput, scrn);
}
static void SetClr(int clr)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), clr);
}
static void SetClr(int textColor, int bgColor)
{
    int color = static_cast<int>(textColor) + static_cast<int>(bgColor) * 16;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
static void displayPromt() {
    gotoRowCol(1, 50);
    cout << "POODLE" << endl;
    gotoRowCol(5, 25);
    cout << "Enter Search : ";
    gotoRowCol(6, 40);
    cout << "_______________________________________________________________";
    gotoRowCol(5, 41);
}
inline string subtractStrings(string s1) {
    string ans;
    copy(s1.begin() + 67, s1.end(), std::back_inserter(ans));
    return ans;
}

inline void printWordfind2(const std::string& line, const std::vector<std::string>& requiredWords) {
    istringstream iss(line);
    string word, prevWord1, prevWord2, nextWord1, nextWord2;

    while (iss >> word) {
        prevWord2 = prevWord1;
        prevWord1 = word;
        
        auto wordExists = std::find_if(
            requiredWords.begin(),
            requiredWords.end(),
            [&](const std::string& requiredWord) {
                return word.size() == requiredWord.size() &&
                    std::equal(word.begin(), word.end(), requiredWord.begin(), &caseInsensitiveCompare);
            }
        );

        if (wordExists != requiredWords.end()) {
            SetClr(15);
            cout << prevWord2 << " "  << " ";
            SetClr(4);
            cout << *wordExists << " ";
            SetClr(15);
            //std::cout << word << " ";
            iss >> nextWord1 >> nextWord2;
            cout << nextWord1 << " " << nextWord2 << "............." <<endl;
        }
    }
}
inline void printWordfind(string fileName, vector<string> requiredWords) {

    SetClr(15);
    string address = "\Files\\";
    address += fileName;
    ifstream rdr(address);
    cout << endl;
    string word,word1,word2;
    while (rdr) {

        bool CH = false;
        rdr >> word;
        rdr >> word1;

        auto word1Exsists = std::find_if(
            requiredWords.begin(),
            requiredWords.end(),
            [&](const std::string& str) {
                return str.size() == word.size() && std::equal(str.begin(), str.end(), word.begin(), &caseInsensitiveCompare);
            }
        );
        auto word2Exsists = std::find_if(
            requiredWords.begin(),
            requiredWords.end(),
            [&](const std::string& str) {
                return str.size() == word1.size() && std::equal(str.begin(), str.end(), word1.begin(), &caseInsensitiveCompare);
            }
        );
        

        if (word1Exsists != requiredWords.end()) {
            CH = true;
            SetClr(4);
            cout << word << " ";
            if (word2Exsists == requiredWords.end()) {
                SetClr(15);
            }
            cout << word1 << " ";
            SetClr(15);
            rdr >> word >> word1;

            auto word1Exsists = std::ranges::find(requiredWords, word);
            auto word2Exsists = std::ranges::find(requiredWords, word1);
            if (word1Exsists == requiredWords.end()) {
                SetClr(15);
            }
            cout << word << " ";
            SetClr(4);
            if (word2Exsists == requiredWords.end()) {
                SetClr(15);
            }
            cout << word1 << " ";
            SetClr(4);
        }
        else if (word2Exsists != requiredWords.end()) {
            SetClr(4);
            CH = true;
            if (word1Exsists == requiredWords.end()) {
                SetClr(15);
            }
            cout << word << " ";
            SetClr(4);
            cout << word1 << " ";
           
            rdr >> word >> word1;

            auto word1Exsists = std::ranges::find(requiredWords, word);
            auto word2Exsists = std::ranges::find(requiredWords, word1);
            if (word1Exsists == requiredWords.end()) {
                SetClr(15);
            }
            cout << word << " ";
            SetClr(4);
            if (word2Exsists == requiredWords.end()) {
                SetClr(15);
            }
            cout << word1 << " ";
            SetClr(4);
           
        }

        rdr >> word2;
        auto word3Exsists = std::ranges::find(requiredWords, word2);
        if (word3Exsists != requiredWords.end()) {
            CH = true;
            if (word1Exsists == requiredWords.end()) {
                SetClr(15);
            }
            cout << word << " ";
            SetClr(4);
            if (word2Exsists == requiredWords.end()) {
                SetClr(15);
            }
            cout << word1 << " ";
            SetClr(4);
            cout << word2 << " ";
            SetClr(15);
            if (rdr) {
                rdr >> word;
                cout << word << " ";
            }
            if (rdr) {
                rdr >> word;
                cout << word << " ";
            }
            cout << " .........    ";
        }

        if (CH) {
            SetClr(15);
            cout << "..........";
        }
    }
   /* while (getline(rdr, word)) {
        printWordfind2(word, requiredWords);
    }*/
}

inline void printWordfindHost(string & ans, string fileName, vector<string> requiredWords) {

    SetClr(15);
    string address = "\Files\\";
    address += fileName;
    ifstream rdr(address);
    cout << endl;
    string word, word1, word2;
    while (rdr) {

        bool CH = false;
        rdr >> word;
        rdr >> word1;
        auto word1Exsists = std::ranges::find(requiredWords, word);
        auto word2Exsists = std::ranges::find(requiredWords, word1);

        if (word1Exsists != requiredWords.end()) {
            CH = true;
            ans = ans  + "<div style='background:yellow;'>" + word +"&nbsp;</div>" + " ";
            if (word2Exsists == requiredWords.end()) {
                ans += word1 += " ";
            }
            else {
                ans = ans + "<div style='background:yellow;'>" + word1 + "&nbsp;</div>" + " ";
            }
            rdr >> word >> word1;
            auto word1Exsists = std::ranges::find(requiredWords, word);
            auto word2Exsists = std::ranges::find(requiredWords, word1);
            if (word1Exsists == requiredWords.end()) {
                ans = ans + word + " ";
            }
            else {
                ans = ans + "<div style='background:yellow;'>" + word + "&nbsp;</div>" + " ";
            }
            
            if (word2Exsists == requiredWords.end()) {
                ans = ans + word1 + " ";
            }
            else {
                ans = ans + "<div style='background:yellow;'>" + word1 + "&nbsp;</div>" + " ";
            }
            ans = ans + word1 + " ";
        }
        else if (word2Exsists != requiredWords.end()) {
            CH = true;
            if (word1Exsists == requiredWords.end()) {
                ans = ans + word + " ";
            }
            else {
                ans = ans + "<div style='background:yellow;'>" + word + "&nbsp;</div>" + " ";
            }
            ans = ans + word1 + " ";
            rdr >> word >> word1;

            auto word1Exsists = std::ranges::find(requiredWords, word);
            auto word2Exsists = std::ranges::find(requiredWords, word1);
            if (word1Exsists == requiredWords.end()) {
                ans = ans + word + " ";
            }
            else {
                ans = ans + "<div style='background:yellow;'>" + word + "&nbsp;</div>" + " ";
            }
            if (word2Exsists == requiredWords.end()) {
                ans = ans + word1 + " ";
            }
            else {
                ans = ans + "<div style='background:yellow;'>" + word1 + "&nbsp;</div>" + " ";
            }
        }

        rdr >> word2;
        auto word3Exsists = std::ranges::find(requiredWords, word2);
        if (word3Exsists != requiredWords.end()) {
            CH = true;
            if (word1Exsists == requiredWords.end()) {
                ans = ans + word + " ";
            }
            else {
                ans = ans + "<div style='background:yellow;'>" + word + "&nbsp;</div>" + " ";
            }
            if (word2Exsists == requiredWords.end()) {
                ans = ans + word + " ";
            }
            else {
                ans = ans + "<div style='background:yellow;'>" + word1 + "&nbsp;</div>" + " ";
            }
            ans = ans + word2 + " ";
            if (rdr) {
                rdr >> word;
                ans = ans + word + " ";
            }
            if (rdr) {
                rdr >> word;
                ans = ans + word + " ";
            }
            ans += "......";
        }
        if (CH) {
            ans = ans + "........." + "<br>";
        }
    }
}
inline void printForSenetence(string fileName, string sentence) {

    string address = "\Files\\";
    address += fileName;
    ifstream rdr(address);
    while (rdr) {
        string line;
        size_t index = -1;
        getline(rdr, line);
        auto isPresent = std::search(
            line.begin(), line.end(), sentence.begin(), sentence.end(),[](char c1, char c2) { return std::toupper(c1) == std::toupper(c2); });
        if (isPresent != line.end()) {
            index = distance(line.begin(), isPresent);
        }
        if (index != -1) {
            cout << fileName << " : ";
            SetClr(15);
            for (int ri = 0; ri < line.size(); ri++) {
                if (ri == index) {
                    SetClr(4);
                }
                else if (ri == index + sentence.size()) {
                    SetClr(15);
                }
                cout << line[ri];
                
            }
            SetClr(15);
            cout << "................\n";
        }
    }
}
inline void print(vector<pair<string, int>> isPresent, vector<string> searchWord) {
    if (!isPresent.empty()) {
        for (const auto& entry : isPresent) {
            cout << entry.first << " : ";
            printWordfind(entry.first, searchWord);
            cout << "\n";
        }
    }
    else {
        std::cout << "No results found for word '" << searchWord[0] << "'." << std::endl;
    }
}
inline void printHost(string& ans,vector<pair<string, int>> isPresent, vector<string> searchWord) {
    if (!isPresent.empty()) {
        for (const auto& entry : isPresent) {
            //cout << entry.first << " : ";
            ans += entry.first;
            ans += ":";
            ans += "<div style= 'display:flex'>";
            //ans += "<br/>";
            printWordfindHost(ans,entry.first, searchWord);
            //cout << "\n";
            ans += "\n";
            ans += "</div>";
            ans += "<br>";
        }
    }
    else {
        ans += "No results found for word '";
    }
}
inline void printSentence(vector<pair<string, int>> isPresent, string sentence) {
    if (!isPresent.empty()) {
        for (const auto& entry : isPresent) {
            printForSenetence(entry.first, sentence);
        }
    }
}
inline void printSingleFile(const string& fileName, const std::string& targetWord) {
    
    string address = "\Files\\";
    address += fileName;
    std::ifstream fileReader(address);

    if (!fileReader.is_open()) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return;
    }

    std::string line;
    while (std::getline(fileReader, line)) {
        std::istringstream iss(line);
        std::vector<std::string> words(std::istream_iterator<std::string>{iss},
            std::istream_iterator<std::string>());

        for (size_t i = 0; i < words.size(); ++i) {
            std::string currentWord = words[i];
            std::transform(currentWord.begin(), currentWord.end(), currentWord.begin(), ::tolower);

            if (currentWord == targetWord) {
                // Print the previous two words
                size_t startIndex = (i >= 2) ? i - 2 : 0;
                for (size_t j = startIndex; j < i; ++j) {
                    std::cout << words[j] << " ";
                }

                // Print the current word
                std::cout << "[" << words[i] << "] ";

                // Print the next two words
                size_t endIndex = min(i + 2, words.size());
                for (size_t j = i + 1; j < endIndex; ++j) {
                    std::cout << words[j] << " ";
                }

                std::cout << std::endl;
            }
        }
    }

    fileReader.close();
}

inline void highlightWords(string& document, vector<string>& Mwords) {
    //const std::string front = "<div style='background:yellow;'>";
    const string front = "<span class = 'highlighted-text'> ";
   // const std::string back = "</div>";
    const string back = "</span> ";

    for (const auto& word : Mwords) {
        istringstream iss(document);
        string token;
        int count = 0;
        while (iss >> token) {
            if (token == word) {
                count++;
            }
        }
        int itr = 0;
        size_t pos = document.find(word);
        while (pos != std::string::npos && itr != count-1) {
            document.insert(pos, front);
            pos += front.length();  
            pos = pos + word.length();
            document.insert(pos, back);
            pos += back.length();  
            pos = document.find(word, pos);
            //itr++;
        }
    }
}
inline void returnQueries(string& ans, string fileName, vector<string> requiredWords) {

    SetClr(15);
    string address = "\Files\\";
    address += fileName;
    ifstream rdr(address);
    cout << endl;
    string word, word1, word2;
   /* while (!rdr) {

        bool CH = false;
        rdr >> word;
        rdr >> word1;
        auto word1Exsists = std::ranges::find(requiredWords, word);
        auto word2Exsists = std::ranges::find(requiredWords, word1);

        if (word1Exsists != requiredWords.end()) {
            CH = true;
            ans = ans +word + " "+ word1 + " ";
            rdr >> word >> word1;
            auto word1Exsists = std::ranges::find(requiredWords, word);
            auto word2Exsists = std::ranges::find(requiredWords, word1);            
            ans = ans + word + " " + word1 + " ";
        }
        else if (word2Exsists != requiredWords.end()) {
            CH = true;
            ans = ans + word + " " +  word1 + " ";
        }

        rdr >> word2;
        auto word3Exsists = std::ranges::find(requiredWords, word2);
        if (word3Exsists != requiredWords.end()) {
            CH = true;
            ans = ans + word + " " + word1 + " " + word2 + " ";
            ans += "  ...................";
        }
        if (CH) {
            ans = ans + "  ................";
        }
    }*/
    while (getline(rdr, word)) {

        auto word1Exsists = std::ranges::find(requiredWords, word);
        if (word1Exsists == requiredWords.end()) {
            ans += word;
            ans += "  ..............  ";
        }
    }
    highlightWords(ans, requiredWords);
}


inline void printQueries(string& ans, vector<pair<string, int>> isPresent, vector<string> searchWord) {
    if (!isPresent.empty()) {    
        for (const auto& entry : isPresent) {
            ans += entry.first;
            ans += ":";
            ans += "<div style= 'display:flex'>";
            returnQueries(ans, entry.first, searchWord);
            ans += "</div>";
            ans += "<br>";
            ans += "\n";
        }
    }
    else {
        ans += "No results found for word '";
    }
} 