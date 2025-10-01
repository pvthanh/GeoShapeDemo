#include <iostream>
#include <string>
#include "GeoConverter.h"
#include "ShapeFileHandler.h"
#include <filesystem>
#include <vector>

using json = nlohmann::json;


int main() {
    // Ensure UTF-8 output on Linux
    std::setlocale(LC_ALL, "en_US.UTF-8");
    // 1. Find all .shp files in the data/gadm41_VNM_shp directory
    std::string shpDir = "data/gadm41_VNM_shp";
    std::vector<std::string> shpFiles;
    for (const auto& entry : std::filesystem::directory_iterator(shpDir)) {
        if (entry.path().extension() == ".shp") {
            shpFiles.push_back(entry.path().string());
        }
    }

    // 2. Load all shapefiles
    ShapeFileHandler shapeHandler;
    if (!shapeHandler.openAll(shpFiles)) {
        std::cerr << "Failed to load one or more shapefiles." << std::endl;
        return 1;
    }

    // 3. Use GeoConverter for a Vietnam address
    GeoConverter converter;
    std::string address = "Hanoi, Vietnam";
    std::cout << "Forward geocoding: " << address << std::endl;
    std::string geoResult = converter.addressToGeo(address);
    std::cout << geoResult << std::endl;

    // For demo, use hardcoded coordinates for Hanoi
    double latitude = 21.0285;
    double longitude = 105.8542;
    std::cout << "\nReverse geocoding: Latitude: " << latitude << ", Longitude: " << longitude << std::endl;
    std::string addrResult = converter.geoToAddress(latitude, longitude);
    std::cout << addrResult << std::endl;

    // 4. Query additional info from Shapefiles
    auto features = shapeHandler.queryByCoordinate(latitude, longitude);
    std::cout << "\nAdditional info from Shapefile for coordinate (" << latitude << ", " << longitude << "):\n";
    for (const auto& feature : features) {
        std::cout << "Geometry: " << feature.geometryType << "\n";
        for (const auto& attr : feature.attributes) {
            std::cout << attr.first << ": " << attr.second << "\n";
        }
        std::cout << "---\n";
    }

    // Interactive address input loop
    std::string userInput;
    while (true) {
        std::cout << "\nEnter address (or type 'exit' to quit): ";
        std::getline(std::cin, userInput);
        if (userInput == "exit") break;
        std::string geoResult = converter.addressToGeo(userInput);
        std::cout << "Converted coordinates: " << geoResult << std::endl;

        // For demo, try to parse latitude and longitude from geoResult (assume format: "Latitude: xx, Longitude: yy")
        double lat = 0.0, lon = 0.0;
        size_t latPos = geoResult.find("Latitude:");
        size_t lonPos = geoResult.find(", Longitude:");
        if (latPos != std::string::npos && lonPos != std::string::npos) {
            try {
                lat = std::stod(geoResult.substr(latPos + 9, lonPos - (latPos + 9)));
                lon = std::stod(geoResult.substr(lonPos + 12));
            } catch (...) {
                std::cout << "Could not parse coordinates from result." << std::endl;
                continue;
            }
            auto features = shapeHandler.queryByCoordinate(lat, lon);
            std::cout << "Additional info from Shapefile for coordinate (" << lat << ", " << lon << "):\n";
            for (const auto& feature : features) {
                std::cout << "Geometry: " << feature.geometryType << "\n";
                for (const auto& attr : feature.attributes) {
                    std::cout << attr.first << ": " << attr.second << "\n";
                }
                std::cout << "---\n";
            }
        } else {
            std::cout << "Could not extract coordinates from geocoding result." << std::endl;
        }
    }
    return 0;
}