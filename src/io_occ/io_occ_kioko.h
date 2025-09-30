/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#pragma once

#include "io_occ_common.h"
#include "../base/io_reader.h"
#include "../base/io_writer.h"
#include "../base/tkernel_utils.h"
#include <NCollection_Vector.hxx>

#include <type_traits>

namespace Mayo::IO {

// OpenCascade-based reader for Kioko Lang CAD file format
class OccKiokoReader : public Reader {
public:
    OccKiokoReader();
    OccKiokoReader(const OccKiokoReader&) = delete; // Not copyable
    OccKiokoReader& operator=(const OccKiokoReader&) = delete; // Not copyable
    ~OccKiokoReader();

    bool readFile(const FilePath& filepath, TaskProgress* progress) override;
    TDF_LabelSequence transfer(DocumentPtr doc, TaskProgress* progress) override;

    // Parameters for Kioko Lang CAD format reading
    enum class Precision {
        Low = 0,      // Fast reading, lower precision
        Medium = 1,   // Balanced precision and performance
        High = 2      // Maximum precision, slower reading
    };

    enum class GeometryMode {
        BRep = 0,     // Read as BRep geometry
        Mesh = 1,     // Read as mesh geometry
        Auto = 2      // Automatically detect geometry type
    };

    struct Parameters {
        Precision precision = Precision::Medium;
        GeometryMode geometryMode = GeometryMode::Auto;
        bool readColors = true;
        bool readMaterials = true;
        bool readNames = true;
        bool readLayers = true;
        std::string encoding = "UTF-8";
    };
    Parameters& parameters() { return m_params; }
    const Parameters& constParameters() const { return m_params; }

    static std::unique_ptr<PropertyGroup> createProperties(PropertyGroup* parentGroup);
    void applyProperties(const PropertyGroup* params) override;

private:
    class Properties;
    Parameters m_params;
    
    // Internal data storage for parsed Kioko Lang CAD content
    struct KiokoData {
        std::string content;
        bool isValid = false;
        std::string errorMessage;
    };
    KiokoData m_kiokoData;
};

// OpenCascade-based writer for Kioko Lang CAD file format
class OccKiokoWriter : public Writer {
public:
    OccKiokoWriter();
    OccKiokoWriter(const OccKiokoWriter&) = delete; // Not copyable
    OccKiokoWriter& operator=(const OccKiokoWriter&) = delete; // Not copyable
    ~OccKiokoWriter();

    bool transfer(Span<const ApplicationItem> appItems, TaskProgress* progress) override;
    bool writeFile(const FilePath& filepath, TaskProgress* progress) override;

    // Parameters for Kioko Lang CAD format writing
    enum class OutputPrecision {
        Low = 0,      // Fast writing, lower precision
        Medium = 1,   // Balanced precision and performance
        High = 2      // Maximum precision, slower writing
    };

    enum class GeometryOutput {
        BRep = 0,     // Write as BRep geometry
        Mesh = 1,     // Write as mesh geometry
        Both = 2      // Write both BRep and mesh
    };

    struct Parameters {
        OutputPrecision precision = OutputPrecision::Medium;
        GeometryOutput geometryOutput = GeometryOutput::BRep;
        bool writeColors = true;
        bool writeMaterials = true;
        bool writeNames = true;
        bool writeLayers = true;
        bool writeMetadata = true;
        std::string encoding = "UTF-8";
        std::string version = "1.0";
        std::string author;
        std::string description;
    };
    Parameters& parameters() { return m_params; }
    const Parameters& constParameters() const { return m_params; }

    static std::unique_ptr<PropertyGroup> createProperties(PropertyGroup* parentGroup);
    void applyProperties(const PropertyGroup* params) override;

private:
    class Properties;
    Parameters m_params;
    
    // Internal data storage for generated Kioko Lang CAD content
    struct GeneratedData {
        std::string content;
        bool isValid = false;
        std::string errorMessage;
    };
    GeneratedData m_generatedData;
};

} // namespace Mayo::IO
