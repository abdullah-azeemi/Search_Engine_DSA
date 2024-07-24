#include "httplib.h"
#include"searchEnginehash.h"
#include"searchEngineMap.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>


int main1() {
    searchEngineII poodle;
    poodle.crawl("C:\\Users\\HP\\source\\repos\\DSA_Final_Project\\DSA_Final_Project\\Files");
    //poodle.loadEngine("Save.txt");
    poodle.broadcast();
    poodle.save("saveMap.txt");

    return 0;
}
int main2() {
    searchEngine poodle;
    poodle.crawl("C:\\Users\\HP\\source\\repos\\DSA_Final_Project\\DSA_Final_Project\\Files");
    //poodle.loadEngine("Save.txt");
    poodle.broadcast();

    poodle.saveEngine("Save.txt");
    return 0;
}
int main() {
    
    searchEngine poodle;
    poodle.crawl("C:\\Users\\HP\\source\\repos\\DSA_Final_Project\\DSA_Final_Project\\Files");
    httplib::Server svr;
    svr.Get("/api/search", [&poodle](const httplib::Request& req, httplib::Response& res) {
        std::string query = req.get_param_value("query");
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        
        std::string searchResults = poodle.callSearchEngine(query);
        res.set_content(searchResults, "application/json");
        
        });
    svr.listen("localhost", 5500);
    return 0;
}
 