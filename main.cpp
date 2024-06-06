#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    ((std::string*)userp)->append((char*)contents, totalSize);
    return totalSize;
}

int main() {

    // curl yapılandırılma
    CURL* curl;
    CURLcode res;
    string url = "https://jsonplaceholder.typicode.com/users";
    string response;
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // verilerin kontrolü
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            cerr << "Hata!  " << curl_easy_strerror(res) << endl;
            return 1;
        }

        // Gelen verilerin atanması
        curl_easy_cleanup(curl);
        auto parsed_json = json::parse(response);

        string input_user_id;
        bool kontrol = false;
        int input_user_id_int;

        while(1){
            //Kullanıcıdan input aldığımız yer
            cout << "\nEnter user ID: ";
            cin >> input_user_id;
            if(input_user_id == "exit"){
                break;
            }
            else {
                try {
                    input_user_id_int = stoi(input_user_id);
                } catch (invalid_argument& e) {
                    cout << "Lütfen Bir tam sayı girin" << endl;
                    continue;
                } catch (out_of_range& e) {
                    cout << "The input number is out of range." << endl;
                    continue;
                }
            }
            kontrol = false;

            if(input_user_id_int % 1 == 0){
                cout << "Lütfen Bir tam sayı girin" << endl;
            }
            else{
                // bütün kullanıcıları teker teker kontrol eden döngü
                for(const auto& user : parsed_json) {
                    if(user["id"] == input_user_id_int) {
                        kontrol = true;
                        cout << "Id: " << input_user_id_int << endl;
                        cout << "Name: " << user["name"] << endl;
                        cout << "Username: " << user["username"] << endl;
                        break;
                    }
                }

                // verilen id, api dan gelen json verilerinden birinde değilse
                if(!kontrol){
                    cout << "Kullanıcı bulunamadı";
                }
            }
        }
        return 1;
    }
    return 0;
}
