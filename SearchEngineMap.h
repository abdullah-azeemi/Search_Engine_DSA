#pragma once
#include"utility.h"

class searchEngineII {
private:
    map<string,map<string, wordIndocument>> engine;

public:

    void crawl(string folderPath) {
        vector<string> files;
        getFilesInFolder(folderPath, files);

        for (const auto& file : files) {
            ifstream inputFile(file);
            if (!inputFile.is_open()) {
                cerr << "Error opening file.....";
                continue;
            }

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
                    auto& wordInDoc = wordMap[documentName];

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
    void save(string fileName) {
        ofstream outputFile(fileName, std::ios::binary);
        if (!outputFile.is_open()) {
            cerr << "Error opening file for saving.";
            return;
        }

        for (const auto& wordEntry : engine) {
            for (const auto& documentEntry : wordEntry.second) {
                const wordIndocument& wordInDoc = documentEntry.second;
                outputFile.write(reinterpret_cast<const char*>(&wordInDoc), sizeof(wordIndocument));
            }
        }

        outputFile.close();
    }
    void load(const string& fileName) {
        ifstream ifs(fileName);
        if (!ifs.is_open()) {
            cerr << "Error opening file: " << fileName << endl;
            return;
        }

        string word;
        while (ifs >> word) {
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            if (word == "word:") {
                ifs >> word;
                transform(word.begin(), word.end(), word.begin(), ::tolower);

                while (ifs >> word && word != "end") {
                    string documentName = word;
                    int position;
                    ifs >> position;
                    auto& wordMap = engine[word];
                    auto it = wordMap.find(documentName);
                    if (it == wordMap.end()) {
                        it = wordMap.emplace(documentName, wordIndocument(documentName, position)).first;
                    }
                    else {
                        it->second.addPosition(position);
                    }
                }
            }
        }

        ifs.close();
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
                if (token != "+" && token != "-") {
                    arguments.push_back(token);
                }
            }

            if (auto add = std::ranges::find(input, '+') != input.end()) {
                int occurrences = std::count(input.begin(), input.end(), '+');
                if (occurrences > 1) { // multiple
                    print(searchMultipleWordsMap(arguments), arguments);
                }
                else {
                    print(searchWordsMap(arguments[0], arguments[1]), arguments);
                }
            }

            else if (auto sub = std::ranges::find(input, '-') != input.end()) {
                int occurrences = std::count(input.begin(), input.end(), '+');

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
                        printSentence(subtractMap(before, arguments), before);
                    }
                }
                else {
                    vector<string> highlightWords; highlightWords.push_back(arguments[0]);
                    print(searchSubtractionMap(arguments[0], arguments[1]), highlightWords);
                }
            }
            // means a sentence
            else if (auto sub = std::ranges::find(input, ' ') != input.end()) {
                printSentence(searchSentenceMap(input), input);
            }
            // single word
            else {
                //searchWord(input);
                vector<string> words;
                words.push_back(input);
                print(searchWordMap(input), words);
            }
            _getch();
            system("cls");
            displayPromt();

        }
    }

    vector<pair<string, int>> searchWordMap(const string& word) const {
        vector<pair<string, int>> found;

        string lowercaseWord = word;
        transform(lowercaseWord.begin(), lowercaseWord.end(), lowercaseWord.begin(), ::tolower);

        auto itr = engine.find(lowercaseWord);
        if (itr != engine.end()) {
            for (const auto& ind : itr->second) {
                const auto& wordIndoc = ind.second;
                found.emplace_back(wordIndoc.getDocname(), wordIndoc.getPos().size());
            }
        }

        sort(found.begin(), found.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
            });

        return found;
    }
    vector<pair<string, int>> searchWordsMap(const string& word1, const string& word2) const {
        vector<pair<string, int>> found;

        string lowercaseWord1 = word1;
        string lowercaseWord2 = word2;
        transform(lowercaseWord1.begin(), lowercaseWord1.end(), lowercaseWord1.begin(), ::tolower);
        transform(lowercaseWord2.begin(), lowercaseWord2.end(), lowercaseWord2.begin(), ::tolower);

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
    vector<pair<string, int>> searchMultipleWordsMap(const vector<string>& words) const {
        vector<pair<string, int>> found;

        if (words.size() < 1) {
            cerr << "At least one word is required for this query." << endl;
            return found;
        }

        for (const auto& word : words) {
            auto wordResults = searchWordMap(word);

            // If any word doesn't have results, return an empty vector
            if (wordResults.empty()) {
                cerr << "No results found for word: " << word << endl;
                return found;
            }

            // Initialize the result vector with the results of the first word
            if (found.empty()) {
                found = wordResults;
            }
            else {
                // Intersect the current result with the results of the next word
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

        // Sort the final result
        sort(found.begin(), found.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
            });

        return found;
    }
    vector<pair<string, int>> searchSentenceMap(const string& sentence) const {
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

        if (sentenceWords.size() < 3) {
            std::cerr << "Invalid sentence format. Please provide a valid sentence enclosed in double quotes." << std::endl;
            return found;
        }

        sentenceWords.erase(sentenceWords.begin());
        sentenceWords.pop_back();

        // Map to store combined occurrences for each document
        map<string, int> combinedOccurrences;

        // Search for the sentence in the engine
        for (const auto& word : sentenceWords) {
            auto itr = engine.find(word);
            if (itr != engine.end()) {
                for (const auto& ind : itr->second) {
                    const auto& wordIndoc = ind.second;
                    // Update combined occurrences for the document
                    combinedOccurrences[wordIndoc.getDocname()] += wordIndoc.getPos().size();
                }
            }
        }

        // Convert the map to the result vector
        for (const auto& entry : combinedOccurrences) {
            found.emplace_back(entry.first, entry.second);
        }

        // Sort the results by combined occurrences
        sort(found.begin(), found.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
            });

        return found;
    }
    vector<pair<string, int>> searchSubtractionMap(const string& word1, const string& word2) const {
        vector<pair<string, int>> found;

        // Transform input words to lowercase
        string lowerWord1 = word1;
        string lowerWord2 = word2;
        std::transform(lowerWord1.begin(), lowerWord1.end(), lowerWord1.begin(), ::tolower);
        std::transform(lowerWord2.begin(), lowerWord2.end(), lowerWord2.begin(), ::tolower);

        // Search for documents containing word1
        auto itrWord1 = engine.find(lowerWord1);
        if (itrWord1 != engine.end()) {
            for (const auto& indWord1 : itrWord1->second) {
                const auto& wordIndocWord1 = indWord1.second;

                // Search for documents containing word2
                auto itrWord2 = engine.find(lowerWord2);
                if (itrWord2 != engine.end()) {
                    // Check if the same document also contains word2
                    if (itrWord2->second.find(wordIndocWord1.getDocname()) == itrWord2->second.end()) {
                        found.emplace_back(wordIndocWord1.getDocname(), wordIndocWord1.getPos().size());
                    }
                }
                else {
                    // If word2 is not found, include the document
                    found.emplace_back(wordIndocWord1.getDocname(), wordIndocWord1.getPos().size());
                }
            }
        }

        // Sort the results by occurrences
        sort(found.begin(), found.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
            });

        return found;
    }
    vector<pair<string, int>> subtractMap(const string& sentence, vector<string>& minusTheseWords) const {
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