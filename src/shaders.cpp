#include "shaders.h"

#include <string>
#include <sstream>
#include <optional>
#include <filesystem>
#include <fstream>

#include "rlAdditions.h"

Shader defaultShader;
Shader indicatorShader;
Shader shadowShader;

Material shadowMaterial;
RenderTexture2D shadowMap;

int defaultShaderPColorUniform;
int defaultShaderSColorUniform;
int defaultShaderTColorUniform;

int defaultShaderLightViewUniform;

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
    
    #ifndef NDEBUG
    if (vshContents.has_value()) {
        printf("--- %s ---\n", vertexShaderFile);
        auto vscStream = std::istringstream{*vshContents};
        int lineNum = 1;
        for (std::string line; std::getline(vscStream, line); ) {
            printf("%4d: %s\n", lineNum, line.c_str());
            lineNum++;
        }

        printf("\n\n");
    }

    if (fshContents.has_value()) {
        printf("--- %s ---\n", fragmentShaderFile);
        auto fscStream = std::istringstream{*fshContents};
        int lineNum = 1;
        for (std::string line; std::getline(fscStream, line); ) {
            printf("%4d: %s\n", lineNum, line.c_str());
            lineNum++;
        }
    }
    #endif
    
    return LoadShaderFromMemory(
        vshContents.has_value()? vshContents->c_str() : nullptr,
        fshContents.has_value()? fshContents->c_str() : nullptr
    );
}

Shader loadShadowShader() {
    auto shader = loadShaderPreprocess("resources/shaders/shadows.vs", "resources/shaders/shadows.fs");

    shadowMaterial = LoadMaterialDefault();
    shadowMaterial.shader = shader;

    shadowMap = LoadShadowMapTexture(2048, 2048);
}