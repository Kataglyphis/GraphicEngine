#include "ShaderIncludes.h"
#include "DebugApp.h"
#include "File.h"

#include <array>
#include <sstream>
#include <cassert>

ShaderIncludes::ShaderIncludes()
{
}

// https://cpp.hotexamples.com/examples/-/-/glNamedStringARB/cpp-glnamedstringarb-function-examples.html
void ShaderIncludes::set(   std::vector<const char*> includeNames,
                            std::vector<const char*> file_locations_relative)
{

    assert(includeNames.size() == file_locations_relative.size());

    // https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_shading_language_include.txt
    DebugApp DebugApp_ins;

    std::vector<std::string> file_locations_abs;
    for (int i = 0; i < includeNames.size(); i++) {

        std::stringstream aux;
        aux << CMAKELISTS_DIR;
        aux << file_locations_relative[i];
        file_locations_abs.push_back(aux.str());
    }

    for (int i = 0; i < includeNames.size(); i++) {

        File file(file_locations_abs[i]);
        std::string file_content = file.read();
        char tmpstr[200];
        sprintf(tmpstr, "/%s", includeNames[i]);
        glNamedStringARB(GL_SHADER_INCLUDE_ARB, strlen(tmpstr), tmpstr, strlen(file_content.c_str()), file_content.c_str());
        DebugApp_ins.areErrorPrintAll("From glNamedStringARB.");

    }

}

ShaderIncludes::~ShaderIncludes()
{
}
