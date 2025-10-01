#pragma once
#include <string>
#include <nlohmann/json.hpp>

class GeoConverter {
public:
    // Convert address to geo coordinates (forward geocoding)
    std::string addressToGeo(const std::string& address);

    // Convert geo coordinates to address (reverse geocoding)
    std::string geoToAddress(double latitude, double longitude);
};
