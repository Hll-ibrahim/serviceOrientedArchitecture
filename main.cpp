#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

// For convenience
using json = nlohmann::json;

// Write callback function for cURL to store the response data in a string
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    ((std::string*)userp)->append((char*)contents, totalSize);
    return totalSize;
}

int main() {
    CURL* curl;
    CURLcode res;
    std::string url = "https://jsonplaceholder.typicode.com/users";
    std::string response;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << "cURL failed: " << curl_easy_strerror(res) << std::endl;
            return 1;
        }

        // Cleanup
        curl_easy_cleanup(curl);

        // Parse JSON data
        auto parsed_json = json::parse(response);

        // Get user input for user id
        int input_user_id;
        std::cout << "Enter user ID: ";
        std::cin >> input_user_id;

        // Iterate through the array to find the matching user
        for(const auto& user : parsed_json) {
            if(user["id"] == input_user_id) {
                std::cout << "Name: " << user["name"] << std::endl;
                std::cout << "Username: " << user["username"] << std::endl;
                break;
            }
        }
    }
    return 0;
}
