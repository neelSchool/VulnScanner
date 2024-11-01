#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <chrono>
#include <iomanip>
#include <sstream>

// Function prototypes
std::string sendRequest(const std::string& url);
std::vector<std::string> extractLinks(const std::string& html);
std::string resolveUrl(const std::string& baseUrl, const std::string& relativeUrl);
void crawlApplication(const std::string& baseUrl);
void logVulnerability(const std::string& vulnerability, const std::string& url);
std::string getCurrentTime();

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <target_url>" << std::endl;
        return 1;
    }

    std::string baseUrl = argv[1]; // Get target URL from command line
    crawlApplication(baseUrl);

    return 0;
}

// Function to resolve relative URLs
std::string resolveUrl(const std::string& baseUrl, const std::string& relativeUrl) {
    if (relativeUrl.substr(0, 4) == "http") {
        return relativeUrl;
    }

    if (relativeUrl[0] == '/') {
        std::regex regex(R"((https?://[^/]+)(/.*)?)");
        std::smatch match;
        if (std::regex_search(baseUrl, match, regex)) {
            return match[1].str() + relativeUrl;
        }
    }

    return baseUrl + "/" + relativeUrl;
}

// Function to extract links from HTML response
std::vector<std::string> extractLinks(const std::string& html) {
    std::vector<std::string> links;
    std::regex linkRegex(R"(<a\s+(?:[^>]*?\s+)?href="([^"]*)")");
    std::smatch match;

    std::string::const_iterator searchStart(html.cbegin());
    while (std::regex_search(searchStart, html.cend(), match, linkRegex)) {
        links.push_back(match[1].str());
        searchStart = match.suffix().first;
    }

    return links;
}

// Function to log detected vulnerabilities
void logVulnerability(const std::string& vulnerability, const std::string& url) {
    std::ofstream logFile("vulnerabilities.log", std::ios::app);
    if (logFile.is_open()) {
        logFile << "[" << getCurrentTime() << "] " << vulnerability << " found at " << url << std::endl;
        logFile.close();
    }
}

// Function to get the current time
std::string getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %X");
    return oss.str();
}

// Function to crawl the application
void crawlApplication(const std::string& baseUrl) {
    std::string response = sendRequest(baseUrl);
    std::cout << "Crawled: " << baseUrl << std::endl;

    std::vector<std::string> links = extractLinks(response);

    for (const std::string& link : links) {
        std::string absoluteUrl = resolveUrl(baseUrl, link);
        std::cout << "Testing link: " << absoluteUrl << std::endl;

        // Call vulnerability tests
        if (testSQLInjection(absoluteUrl)) {
            logVulnerability("SQL Injection", absoluteUrl);
        }
        if (testXSS(absoluteUrl)) {
            logVulnerability("XSS", absoluteUrl);
        }
        if (testCSRF(absoluteUrl)) {
            logVulnerability("CSRF", absoluteUrl);
        }
        if (testSSRF(absoluteUrl)) {
            logVulnerability("SSRF", absoluteUrl);
        }
    }
}

// Mock function to simulate sending an HTTP request (to be replaced)
std::string sendRequest(const std::string& url) {
    // ... Implementation of HTTP request ...
    return "<html><body><a href='/nextpage'>Next</a></body></html>"; // Example HTML response
}
