#define CURL_STATICLIB
#include <curl/curl.h>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
using namespace std;
using json = nlohmann::json;


// Function stolen for CURL_WriteData so we can parse the response
size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {

	// Curl Initilization
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	struct curl_slist *headers=NULL;

	
	string token_start = "Authorization: Token ";
	string token = ""; // Insert token here
	string token_header;
	token_header.append(token_start).append(token);
	
	// String to Character Array from GeeksForGeeks (I want to turn this into a function but am not certain on how to)
	
	char head[token_header.length()];   
    int i; 
	
    for (i = 0; i < sizeof(head); i++) { 
        head[i] = token_header[i]; 
	}
	
	// ADD AUTHENTICATION HEADER
	headers = curl_slist_append(headers, head);
	

	string base_url = "https://steamladder.com/api/v1/profile/";
	string steamid64;
	string url_string;
	cout << "Enter SteamID64:";
	cin >> steamid64;
	url_string.append(base_url).append(steamid64).append("/");
	// cout << url_string;
	
	// Another String to Character Array from GeeksForGeeks because I couldn't figure out how to create a function with character arrays (I had to add +1 for null terminator thing)
	
	char url_char[url_string.length()]; 
	
    for (i = 0; i < sizeof(url_char)+1; i++) { 
        url_char[i] = url_string[i]; 
	}
	// cout << "\n" << url_char;
	
	string readBuffer;
	
	// SET GET PARAMATERS AND PERFORM
	curl_easy_setopt(curl, CURLOPT_URL, url_char);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	cout << "|RAW| \n\n" << readBuffer;
	cout << "\n\n|PARSED|\n\n";
	json jsonn = json::parse(readBuffer);
	cout << "User Name: " << jsonn["steam_user"]["steam_name"] << "\n";
	cout << "Steam Level: " << jsonn["steam_stats"]["level"] << "\n";
	cout << "Playtime (minutes): " << jsonn["steam_stats"]["games"]["total_playtime_min"] << "\n";
	int minutes = jsonn["steam_stats"]["games"]["total_playtime_min"];
	int hours = minutes / 60;
	cout << "Playtime (hours): " << hours << "\n";
	
	
	
	cin.get();
	return 0;
	
}
