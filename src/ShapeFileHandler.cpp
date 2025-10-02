
#include <vector>
#include "ShapeFileHandler.h"
#include <iostream>

std::vector<ShapeFeature> ShapeFileHandler::queryByCoordinate(double latitude, double longitude) {
    std::vector<ShapeFeature> results;
    OGRPoint queryPoint(longitude, latitude); // Note: OGR uses (lon, lat)
    for (auto* dataset : datasets) {
        if (!dataset) continue;
        for (int i = 0; i < dataset->GetLayerCount(); ++i) {
            OGRLayer* layer = dataset->GetLayer(i);
            layer->ResetReading();
            OGRFeature* feature;
            while ((feature = layer->GetNextFeature()) != nullptr) {
                OGRGeometry* geom = feature->GetGeometryRef();
                if (geom && geom->Contains(&queryPoint)) {
                    ShapeFeature sf;
                    char* wkt = nullptr;
                    geom->exportToWkt(&wkt);
                    sf.geometryType = geom->getGeometryName();
                    sf.wkt = wkt ? wkt : "";
                    CPLFree(wkt);
                    for (int j = 0; j < feature->GetFieldCount(); ++j) {
                        OGRFieldDefn* fieldDef = feature->GetFieldDefnRef(j);
                        std::string name = fieldDef->GetNameRef();
                        std::string value = feature->IsFieldSet(j) ? feature->GetFieldAsString(j) : "";
                        sf.attributes.emplace_back(name, value);
                    }
                    results.push_back(sf);
                }
                OGRFeature::DestroyFeature(feature);
            }
        }
    }
    return results;
}

ShapeFileHandler::ShapeFileHandler() {
    GDALAllRegister();
}

ShapeFileHandler::~ShapeFileHandler() {
    close();
}

bool ShapeFileHandler::openAll(const std::vector<std::string>& filenames) {
    close();
    bool success = true;
    for (const auto& filename : filenames) {
        GDALDataset* ds = (GDALDataset*) GDALOpenEx(filename.c_str(), GDAL_OF_VECTOR, nullptr, nullptr, nullptr);
        if (ds) {
            datasets.push_back(ds);
        } else {
            success = false;
        }
    }
    return success;
}



void ShapeFileHandler::close() {
    for (auto* ds : datasets) {
        if (ds) GDALClose(ds);
    }
    datasets.clear();
}
