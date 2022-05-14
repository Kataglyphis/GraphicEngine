#include "ShaderIncludes.h"
#include "DebugApp.h"
#include "File.h"

#include <stdio.h>
#include <string.h>
#include <array>
#include <sstream>
#include <cassert>

#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>

#define glNamedStringARB2 pfnNamedStringARB
PFNGLNAMEDSTRINGARBPROC pfnNamedStringARB;
// this method is setting all files we want to use in a shader per #include
// you have to specify the name(how file appears in shader)
// and its actual file location relatively
// https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_shading_language_include.txt
ShaderIncludes::ShaderIncludes()
{

    assert(includeNames.size() == file_locations_relative.size());

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

        if (glfwExtensionSupported("GL_ARB_shading_language_include")) {

            pfnNamedStringARB = (PFNGLNAMEDSTRINGARBPROC) glfwGetProcAddress("glNamedStringARB");

            glNamedStringARB2(GL_SHADER_INCLUDE_ARB, strlen(tmpstr), tmpstr,
                strlen(file_content.c_str()), file_content.c_str());
        }

        DebugApp_ins.areErrorPrintAll("From glNamedStringARB.");

    }

}

ShaderIncludes::~ShaderIncludes()
{
}
