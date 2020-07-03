#include <sstream>
#include <fstream>

#include "program_builder.h"

namespace sid {
    ProgramBuilder& ProgramBuilder::NewProgram() {
        m_CurrentProgram = std::make_shared<Program>();
        return *this;
    }

    ProgramBuilder& ProgramBuilder::AddShader(std::string file_name, ShaderType shader_type) {
        std::stringstream source_code;

        std::string line;
        std::ifstream file(file_name);

        while(std::getline(file,line)) {
            source_code << line << "\n";
        }
        file.close();

        auto shader = std::make_unique<Shader>(source_code.str(), shader_type);
        m_CurrentProgram->AddShader(std::move(shader));
        return *this;
    }

    ProgramPtr ProgramBuilder::Build() {
        auto result = m_CurrentProgram;
        m_CurrentProgram->Compile();
        m_CurrentProgram.reset();
        return result;
    }
}