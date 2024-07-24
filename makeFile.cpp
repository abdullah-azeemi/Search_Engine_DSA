#include "utility.h"

struct Review {
    std::string Id;
    std::string ProductId;
    std::string UserId;
    std::string ProfileName;
    int HelpfulnessNumerator;
    int HelpfulnessDenominator;
    int Score;
    int Time;
    std::string Summary;
    std::string Text;
};

void createReviewTextFile(const Review& review, const std::string& outputFolder) {
    std::string fileName = outputFolder + "/review_" + review.Id + ".txt";
    std::ofstream file(fileName, std::ios::out);

    if (file.is_open()) {
       
       file<< review.Text << "\n";
       string str = review.Text;
       std::istringstream iss(str);
       std::vector<std::string> lines;

       while (std::getline(iss, str, '.')) {
           lines.push_back(str);
       }

       for (const auto& line : lines) {
           file << line << endl;
       }

        file.close();
    }
    else {
        std::cerr << "Error opening file: " << fileName << std::endl;
    }
}

int q() {
    std::string reviewsFilePath = "C:\\Users\\HP\\source\\repos\\DSA_Final_Project\\DSA_Final_Project\\reviews.csv";
    std::string outputFolder = "C:\\Users\\HP\\source\\repos\\DSA_Final_Project\\DSA_Final_Project\\Files";

    // Open the CSV file
    std::ifstream inputFile(reviewsFilePath);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening CSV file: " << reviewsFilePath << std::endl;
        return 1;
    }

    std::string line;
    std::getline(inputFile, line); // Skip the header line

    // Create 10,000 reviews and corresponding text files
    for (int i = 1; i <= 100; ++i) {
        // Read a line from the CSV file
        std::getline(inputFile, line);
        std::istringstream iss(line);

        // Parse the CSV values
        Review review;
        std::getline(iss, review.Id, ',');
        std::getline(iss, review.ProductId, ',');
        std::getline(iss, review.UserId, ',');
        std::getline(iss, review.ProfileName, ',');
        
        std::string a;
        std::getline(iss, a,',');
        review.HelpfulnessNumerator = a[0];
        a.clear();
        std::getline(iss, a,',');
        review.HelpfulnessDenominator = a[0];
        a.clear();
        std::getline(iss, a, ',');
        review.Score = a[0];
        a.clear();
        std::getline(iss, a, ',');
        review.Time = a[0];
        std::getline(iss, review.Summary, ',');
        std::getline(iss, review.Text, ',');

        // Create the review text file
        createReviewTextFile(review, outputFolder);
    }

    // Close the CSV file
    inputFile.close();

    return 0;
}
