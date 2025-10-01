# GeoShapeDemo

A C++ demo project for geocoding and querying additional information from Vietnam Shapefiles using GDAL and nlohmann/json.

## Features
- Convert address to coordinates and vice versa (GeoConverter)
- Load and query multiple Shapefiles for Vietnam region (ShapeFileHandler)
- Print geocoding results and additional info from Shapefiles

## Prerequisites
- C++17 compiler
- CMake >= 3.16
- GDAL development libraries (`libgdal-dev`)
- nlohmann/json (auto-fetched by CMake)

## Setup & Build
1. **Install dependencies:**
   ```bash
   sudo apt-get update
   sudo apt-get install -y build-essential cmake libgdal-dev
   ```
2. **Clone the repository:**
   ```bash
   git clone https://github.com/pvthanh/GeoShapeDemo.git
   cd GeoShapeDemo
   ```
3. **Prepare Shapefiles:**
   - Place Vietnam Shapefiles in `data/gadm41_VNM_shp/` (already included if you cloned the full repo).

4. **Build the project:**
   ```bash
   cmake -S . -B build
   cmake --build build
   ```

## Run
```bash
./build/geocoder_demo
```

## How It Works
1. Loads all `.shp` files in `data/gadm41_VNM_shp/`.
2. Converts a sample Vietnam address to coordinates and vice versa.
3. Queries the loaded Shapefiles for additional info about the location.
4. Prints results with proper Unicode support.

## Diagram
```mermaid
flowchart TD
    A[User Input: Address or Coordinate] --> B[GeoConverter]
    B -->|Address to Coordinate| C[Coordinate]
    B -->|Coordinate to Address| D[Address]
    C --> E[ShapeFileHandler]
    E -->|Query by Coordinate| F[Shapefile Data]
    F --> G[Print Additional Info]
    D --> G
```

## License
MIT
