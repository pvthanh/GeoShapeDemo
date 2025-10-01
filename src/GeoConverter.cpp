#include "GeoConverter.h"
#include <curl/curl.h>
#include <sstream>

using json = nlohmann::json;

namespace {
    size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
        size_t totalSize = size * nmemb;
        output->append((char*)contents, totalSize);
        return totalSize;
    }
}

std::string GeoConverter::addressToGeo(const std::string& address) {
    CURL* curl = curl_easy_init();
    std::string response;
    if (curl) {
        char* escaped = curl_easy_escape(curl, address.c_str(), 0);
        std::string url = std::string("https://nominatim.openstreetmap.org/search?q=") + escaped + "&format=json";
        curl_free(escaped);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "GeoDemo/1.0 (your@email.com)");
    curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    // Parse JSON and extract coordinates
    try {
        auto results = json::parse(response);
        if (!results.empty()) {
            double lat = std::stod(results[0]["lat"].get<std::string>());
            double lon = std::stod(results[0]["lon"].get<std::string>());
            std::ostringstream oss;
            oss << "Latitude: " << lat << ", Longitude: " << lon;
            return oss.str();
        }
    } catch (...) {}
    return "No result";
}

std::string GeoConverter::geoToAddress(double latitude, double longitude) {
    CURL* curl = curl_easy_init();
    std::string response;
    if (curl) {
        std::ostringstream oss;
        oss << "https://nominatim.openstreetmap.org/reverse?lat=" << latitude << "&lon=" << longitude << "&format=json";
        std::string url = oss.str();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "GeoDemo/1.0 (your@email.com)");
    curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    // Parse JSON and extract address
    try {
        auto result = json::parse(response);
        if (result.contains("display_name")) {
            return result["display_name"].get<std::string>();
        }
    } catch (...) {}
    return "No result";
}
