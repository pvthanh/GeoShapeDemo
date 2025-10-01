#pragma once
#include <string>
#include <vector>
#include <gdal.h>
#include <ogrsf_frmts.h>

struct ShapeFeature {
    std::string geometryType;
    std::string wkt;
    std::vector<std::pair<std::string, std::string>> attributes;
};


#include <memory>

class ShapeFileHandler {
public:
    ShapeFileHandler();
    ~ShapeFileHandler();
    // Load multiple shapefiles
    bool openAll(const std::vector<std::string>& filenames);
    std::vector<ShapeFeature> readAllFeatures();
    std::vector<ShapeFeature> queryByCoordinate(double latitude, double longitude);
    void close();
private:
    std::vector<GDALDataset*> datasets;
};
