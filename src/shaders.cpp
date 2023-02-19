#include "shaders.h"

#include <string>
#include <optional>
#include <filesystem>
#include <fstream>

Shader defaultShader;
Shader indicatorShader;

int defaultShaderPColorUniform;
int defaultShaderSColorUniform;
int defaultShaderTColorUniform;

int indicatorShaderPColorUniform;
int indicatorShaderSColorUniform;
int indicatorShaderTColorUniform;

void preprocess(std::string& processedContents, char const * filename) {
    auto file = std::fstream{filename, std::ios::in};
    
    for (std::string line{}; std::getline(file, line);) {
        if (line.find("#include") == std::string::npos) {
            processedContents.append(line);
            processedContents.push_back('\n');
        } else {
            auto filenameStart = line.find('"') + 1;
            auto filenameEnd = line.rfind('"');
            
            preprocess(processedContents, (std::filesystem::path{filename}.parent_path() / line.substr(filenameStart, filenameEnd - filenameStart)).c_str());
        }
    }
}

Shader loadShaderPreprocess(char const * vertexShaderFile, char const * fragmentShaderFile) {
    std::optional<std::string> vshContents{};
    std::optional<std::string> fshContents{};
    
    if (vertexShaderFile) {
        std::string pContents{};
        
        preprocess(pContents, vertexShaderFile);
        
        vshContents = pContents;
    }
    
    if (fragmentShaderFile) {
        std::string pContents{};
        
        preprocess(pContents, fragmentShaderFile);
        
        fshContents = pContents;
    }
    
    return LoadShaderFromMemory(
        vshContents.has_value()? vshContents->c_str() : nullptr,
        fshContents.has_value()? fshContents->c_str() : nullptr
    );
}
