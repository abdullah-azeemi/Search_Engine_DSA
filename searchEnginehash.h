#pragma once
#include"utility.h"
class searchEngine {
private:
	    unordered_map <string, unordered_map<string, wordIndocument>>engine;
	    float size;
public:
	searchEngine() {
		//loadEngine();
   }
    void broadcast() {
        displayPromt();
        int x = 0;
        string input;
        while (true) {
            getline(cin, input);
            gotoRowCol(9, 0);
            vector<string> arguments;
            istringstream iss(input);
            string token;
            while (std::getline(iss, token, ' ') || std::getline(iss, token, '+') || std::getline(iss, token, '-')) {
                if (token !=  "+" && token != "-") {
                    arguments.push_back(token);
                }
            }
            if (auto add = std::ranges::find(input, '+') != input.end()) {
                int occurrences = count(input.begin(), input.end(),'+');
                if (occurrences > 1) { // multiple
                    print(searchMultipleWords(arguments), arguments);
                }
                else {
                    print(searchWords(arguments[0], arguments[1]), arguments);
                }
            } 
            else if (auto sub = std::ranges::find(input, '-') != input.end()) {
                int occurrences = count(input.begin(), input.end(), '+');
                if (occurrences > 1) { // multiple
                    
                    size_t pos = input.find('-');
                    if (pos != std::string::npos) {
                        string before = input.substr(0, pos);
                        string after = input.substr(pos, input.size() - 1);
                        char charToRemove = '-';
                        after.erase(std::remove_if(after.begin(), after.end(), [charToRemove](char c) {
                            return c == charToRemove;}), after.end());
                   
                        vector<string> arguments;
                        istringstream iss(after);
                        string segment;
                        while (getline(iss, segment, ' ')) {
                            if (!segment.empty()) {
                                arguments.push_back(segment);
                            }
                        }
                        printSentence(subtract(before, arguments), before);
                    }
                }
                else {
                    vector<string> highlightWords; highlightWords.push_back(arguments[0]);
                    print(searchSubtraction(arguments[0], arguments[1]), highlightWords);
                }
            }
            // means a sentence
            else if (auto sub = std::ranges::find(input, ' ') != input.end()) {
                printSentence(searchSentence(input), input);
            }
            // single word
            else {
                //searchWord(input);
                vector<string> words;
                words.push_back(input);
                print(searchWord(input), words);
            }
            _getch();
            system("cls");
            displayPromt();
        }
    }
    void crawl(const string& folderPath) {
        vector<string> files;
        getFilesInFolder(folderPath, files);
        for (const auto& file : files) {
            ifstream inputFile(file);
            string documentName = subtractStrings(file);
            string line;
            int position = 0;
            while (getline(inputFile, line)) {
                istringstream iss(line);
                string word;
                while (iss >> word) {
                    std::for_each(word.begin(), word.end(), [](char& ch) {
                        ch = std::tolower(ch);
                        });
                    auto& wordMap = engine[word];
                    auto it = wordMap.find(documentName);
                    if (it == wordMap.end()) {
                        it = wordMap.emplace(documentName, wordIndocument(documentName, position)).first;
                    }
                    auto& wordInDoc = it->second;
                    if (wordInDoc.getDocname().empty()) {
                        wordInDoc = wordIndocument(documentName, position);
                    }
                    else {
                        wordInDoc.addPosition(position);
                    }
                    position++;
                }
            }
            inputFile.close();
        }
    }
    void saveEngine(const string& fileName) {
        ofstream file(fileName);
        if (!file.is_open()) {
            cout << "Error opening file for saving: " << fileName;
            return;
        }
        for (const auto& wordEntry : engine) {
            const auto& word = wordEntry.first;
            const auto& docMap = wordEntry.second;
            file << word << " ";
            for (const auto& docEntry : docMap) {
                const auto& docName = docEntry.first;
                const auto& wordInDoc = docEntry.second;
                file << docName << " ";
                for (int pos : wordInDoc.getPos()) {
                    file << pos << " ";
                }
            }
            file << endl;
        }
        file.close();
    }
    void loadEngine(const string& fileName) {
        ifstream file(fileName);
        if (!file.is_open()) {
            cout << "Error opening file for loading: " << fileName << endl;
            return;
        }
        string line;
        while (std::getline(file, line)) {
            istringstream iss(line);
            string word;
            iss >> word;  // Read the main word
            string docName;
            int pos;
            while (iss >> docName >> pos) {
                std::for_each(docName.begin(), docName.end(), [](char& ch) {
                    ch = std::tolower(ch);
                    });
                if (!docName.empty()) {
                    auto& wordMap = engine[word];
                    auto it = wordMap.find(docName);
                    if (it == wordMap.end()) {
                        it = wordMap.emplace(docName, wordIndocument(docName, pos)).first;
                    }
                    else {
                        it->second.addPosition(pos);
                    }
                }
            }
        }
        file.close();
    }

    //void test() {
    //    std::vector<std::string> phrases = {
    //    "cat",
    //    "there is",
    //    "cat is",
    //    "dog - are",
    //    // Repeat the above phrases to fill up to 100 indices
    //    "cat",
    //    "there is",
    //    "cat is",
    //    "dog - are",
    //    // ... Repeat as needed to reach 100 indices
    //    };
    //}


    string callSearchEngine(string input) {

        string ans;
        vector<string> arguments;
        istringstream iss(input);
        string token;
        while (std::getline(iss, token, ' ') || std::getline(iss, token, '+') || std::getline(iss, token, '-')) {
            if (token != "+" && token != "-") {
                arguments.push_back(token);
            }
        }
        
        if (input == "load" || input == "save" || input == "test") {
            if (input == "load") {
                //loadEngine("C:\\Users\\HP\\source\\repos\\DSA_Final_Project\\DSA_Final_Project\\Files");
                ans.clear();
                ans = ans + "<h4>" + "<br>" + "Loaded Successfully" + "</h4>";
            }
            else if (input == "save") {
                saveEngine("Save.txt");
                ans.clear();
                ans = ans + "<h4>" + "<br>" + "Saved successfully" + "</h4>";
            }
        }
        else {
            auto add = std::ranges::find(input, '+');
            auto sub = std::ranges::find(input, '-');
            if (add == input.end() && sub != input.end()) {
                int occurrences = std::count(input.begin(), input.end(), '+');
                if (arguments.size() > 2) { // multiple
                    printHost(ans, searchMultipleWords(arguments), arguments);
                }
                else {
                    printQueries(ans, searchWords(arguments[0], arguments[1]), arguments);
                }
            }
            else if (auto sub = std::ranges::find(input, '-') != input.end()) {
                int occurrences = std::count(input.begin(), input.end(), '-');

                if (occurrences > 1) { // multiple

                    size_t pos = input.find('-');
                    if (pos != std::string::npos) {
                        string before = input.substr(0, pos);
                        string after = input.substr(pos, input.size() - 1);
                        char charToRemove = '-';
                        after.erase(std::remove_if(after.begin(), after.end(), [charToRemove](char c) {
                            return c == charToRemove; }), after.end());

                        vector<string> arguments;
                        istringstream iss(after);
                        string segment;
                        while (getline(iss, segment, ' ')) {
                            if (!segment.empty()) {
                                arguments.push_back(segment);
                            }
                        }
                        //printSentence(subtract(before, arguments), before);
                        vector<string> words;
                        istringstream ise(before);
                        do {
                            string word;
                            ise >> word;
                            words.push_back(word);
                        } while (ise);
                        words.pop_back();

                        printQueries(ans, subtract(before, arguments), words);
                    }
                }
                else {
                    vector<string> highlightWords; highlightWords.push_back(arguments[0]);
                    printQueries(ans, searchSubtraction(arguments[0], arguments[1]), highlightWords);
                }
            }
            // means a sentence
            else if (auto sub = std::ranges::find(input, ' ') != input.end()) {
                //printSentence(searchSentence(input), input);
                vector<string> words;
                istringstream iss(input);
                do {
                    string word;
                    iss >> word;
                    words.push_back(word);
                } while (iss);
                words.pop_back();
                printQueries(ans, searchSentence(input), words);
            }
            // single word
            else {
                //searchWord(input);
                vector<string> words;
                words.push_back(input);
                printQueries(ans, searchWord(input), words);
            }
        }
        return ans;
    }

    vector<pair<string, int>> searchWord(string word)const { // --- q1() --- //
        vector<pair<string, int>> found;
        std::for_each(word.begin(), word.end(), [](char& ch) { //Transforming the word to loweer case, (case insensitive)
            ch = std::tolower(ch); });
        auto itr = engine.find(word);
        if (itr != engine.end()) {
            for (auto ind : itr->second) {
                auto wordIndoc = ind.second;
                found.emplace_back(wordIndoc.getDocname(), wordIndoc.getPos().size());
            }
        }
        sort(found.begin(), found.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
            });

        return found;
    }
    vector<pair<string, int>> searchWords(string word1, string word2) const {
        vector<pair<string, int>> found;
        string lowercaseWord1 = word1,lowercaseWord2 = word2;
        std::transform(lowercaseWord1.begin(), lowercaseWord1.end(), lowercaseWord1.begin(), ::tolower);
        std::transform(lowercaseWord2.begin(), lowercaseWord2.end(), lowercaseWord2.begin(), ::tolower);
        auto itr1 = engine.find(lowercaseWord1);
        auto itr2 = engine.find(lowercaseWord2);
        if (itr1 != engine.end() && itr2 != engine.end()) {
            for (const auto& ind1 : itr1->second) {
                const auto& wordInDoc1 = ind1.second;
                const string& docName = wordInDoc1.getDocname();
                auto itr2InDoc = itr2->second.find(docName);
                if (itr2InDoc != itr2->second.end()) {
                    const auto& wordInDoc2 = itr2InDoc->second;
                    found.emplace_back(docName, wordInDoc1.getPos().size() + wordInDoc2.getPos().size());
                }
            }
        }
        sort(found.begin(), found.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
            });
        return found;
    }
    vector<pair<string, int>> searchMultipleWords(const vector<string>& words) const {
        vector<pair<string, int>> found;
        if (words.size() < 1) {
            cout << "At least one word is required for this query." << endl;
            return found;
        }
        for (const auto& word : words) {
            auto wordResults = searchWord(word);
            if (wordResults.empty()) {
                cout << "No results found for word: " << word << endl;
                return found;
            }
            if (found.empty()) {
                found = wordResults;
            }
            else {
                
                found.erase(
                    remove_if(found.begin(), found.end(), [&wordResults](const auto& result) {
                        return find_if(wordResults.begin(), wordResults.end(),
                        [&result](const auto& wordResult) {
                                return wordResult.first == result.first;
                            }) == wordResults.end();
                        }),
                    found.end());
            }
        }
        sort(found.begin(), found.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
            });
        return found;
    }
    vector<pair<string, int>> searchSentence(const string& sentence) const {
        vector<pair<string, int>> found;
        istringstream iss(sentence);
        vector<string> sentenceWords;
        string word;
        while (iss >> word) {
            std::for_each(word.begin(), word.end(), [](char& ch) {
                ch = std::tolower(ch);
                });
            sentenceWords.push_back(word);
        }
        /*if (sentenceWords.size() < 3) {
            cout << "Invalid sentence format. Please provide a valid sentence enclosed in double quotes." << endl;
            return found;
        }*/
        //sentenceWords.erase(sentenceWords.begin());
        //sentenceWords.pop_back();
        unordered_map<string, int> combinedOccurrences;
        for (const auto& word : sentenceWords) {
            auto itr = engine.find(word);
            if (itr != engine.end()) {
                for (const auto& ind : itr->second) {
                    const auto& wordIndoc = ind.second;
                    combinedOccurrences[wordIndoc.getDocname()] += wordIndoc.getPos().size();
                }
            }
        }
        for (const auto& entry : combinedOccurrences) {
            found.emplace_back(entry.first, entry.second);
        }
        sort(found.begin(), found.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
            });
        return found;
    }
    vector<pair<string, int>> searchSubtraction(const string& word1, const string& word2) const {
        vector<pair<string, int>> found;
        string lowerWord1 = word1,lowerWord2 = word2;
        std::transform(lowerWord1.begin(), lowerWord1.end(), lowerWord1.begin(), ::tolower);
        std::transform(lowerWord2.begin(), lowerWord2.end(), lowerWord2.begin(), ::tolower);
        auto itrWord1 = engine.find(lowerWord1);
        if (itrWord1 != engine.end()) {
            for (const auto& indWord1 : itrWord1->second) {
                const auto& wordIndocWord1 = indWord1.second;
                auto itrWord2 = engine.find(lowerWord2);
                if (itrWord2 != engine.end()) {
                    if (itrWord2->second.find(wordIndocWord1.getDocname()) == itrWord2->second.end()) {
                        found.emplace_back(wordIndocWord1.getDocname(), wordIndocWord1.getPos().size());
                    }
                }
                else {
                    found.emplace_back(wordIndocWord1.getDocname(), wordIndocWord1.getPos().size());
                }
            }
        }
        sort(found.begin(), found.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
            });
        return found;
    }
    vector<pair<string, int>> subtract(const string& sentence, vector<string>& minusTheseWords) const {
        istringstream iss(sentence);
        vector<string> sentenceWords(istream_iterator<string>{iss},
            istream_iterator<string>{});
        if (!sentenceWords.empty()) {
            sentenceWords.pop_back();
        }
        transform(sentenceWords.begin(), sentenceWords.end(), sentenceWords.begin(),
            [](string& word) {
                transform(word.begin(), word.end(), word.begin(), ::tolower);
                return word;
            });
        vector<pair<string, int>> result;

        for (const auto& word : sentenceWords) {
            auto itr = engine.find(word);
            if (itr != engine.end()) {
                for (const auto& ind : itr->second) {
                    const auto& wordIndoc = ind.second;
                    if (all_of(sentenceWords.begin(), sentenceWords.end(),
                        [&wordIndoc](const string& searchWord) {
                            return wordIndoc.getPos().size() > 0;
                        })) {
                        result.emplace_back(wordIndoc.getDocname(), wordIndoc.getPos().size());
                    }
                }
            }
        }
        return result;
    }
};
