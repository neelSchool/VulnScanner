#include "main.cpp"

//test for SQL Injection
bool testSQLInjection(const std::string& url) {
    std::string payload = "' OR '1'='1";
    std::string testUrl = url + "?id=" + payload;
    std::string response = sendRequest(testUrl);
    
    // Check for SQL error in response (this is a simplified example)
    return response.find("error") != std::string::npos;
}

//test for XSS vulnerabilities
bool testXSS(const std::string& url) {
    std::string payload = "<script>alert('XSS')</script>";
    std::string testUrl = url + "?input=" + payload;
    std::string response = sendRequest(testUrl);
    
    // Check if payload appears in response
    return response.find(payload) != std::string::npos;
}
