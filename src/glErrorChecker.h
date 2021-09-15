#pragma once

#include <GL\glew.h>

#include <iostream>

class glErrorChecker {

public:

bool areErrorPrintAll(std::string AdditionalArrayMessage = "Empty") {
    GLenum err;
    bool isError = false;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::string errorCode;
        switch (err)
        {
        case GL_INVALID_ENUM:                  errorCode = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 errorCode = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             errorCode = "INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:                errorCode = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:               errorCode = "STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:                 errorCode = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: errorCode = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << "Error, " << errorCode << " | " << "\nAdditional Error Message: " << AdditionalArrayMessage << std::endl;
        isError = true;
    }

    return isError;

}


    // use this before executing gl functions to check if there uncaptured errors
bool arePreError(std::string AdditionalArrayMessage = "Empty") {
    GLenum err;
    bool isError = false;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::string errorCode;
        switch (err)
        {
        case GL_INVALID_ENUM:                  errorCode = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 errorCode = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             errorCode = "INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:                errorCode = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:               errorCode = "STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:                 errorCode = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: errorCode = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << errorCode << " Error appears befor executing the function, " << " | " << "\nAdditional Error Message: " << AdditionalArrayMessage << std::endl;
        isError = true;
    }

    return isError;
}

};