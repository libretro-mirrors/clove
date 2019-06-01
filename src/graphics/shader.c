/*
#   clove
#
#   Copyright (C) 2016-2019 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../3rdparty/slre/slre.h"

#include "../include/shader.h"
#include "../include/gl.h"

static struct {
    graphics_Shader *activeShader;
    graphics_Shader defaultShader;
    int maxTextureUnits;
} moduleData;

bool graphics_Shader_compileAndAttachShaderRaw(graphics_Shader *program, GLenum shaderType, char const* code) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, (GLchar const **)&code, 0);
    glCompileShader(shader);

    glAttachShader(program->program, shader);

    GLint compileStatus;

    glGetShaderiv(shader,GL_COMPILE_STATUS,&compileStatus);
    if(compileStatus != GL_TRUE)
    {
        GLint info;
        glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&info);
        GLchar buffer[1024];

        int bufferSize;
        glGetShaderInfoLog(shader, info, &bufferSize, buffer);

        clove_error("%s %s \n","vertex shader compile error " , buffer);
        return false;
    }

    glGetShaderiv(shader,GL_COMPILE_STATUS,&compileStatus);
    if(compileStatus != GL_TRUE)
    {
        GLint info;
        glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&info);
        GLchar buffer[1024];

        int bufferSize;
        glGetShaderInfoLog(shader, info, &bufferSize, buffer);

        clove_error("%s %s \n","fragment shader compile error " , buffer);
        return false;
    }

    int state;
    int infolen;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &state);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infolen);

    char *info = malloc(infolen);
    glGetShaderInfoLog(shader, infolen, 0, info);
    info[infolen] = 0;
    switch(shaderType) {
    case GL_VERTEX_SHADER:
        free(program->warnings.vertex);
        program->warnings.vertex = info;
        break;

    case GL_FRAGMENT_SHADER:
        free(program->warnings.fragment);
        program->warnings.fragment = info;
        break;
    }

    glDeleteShader(shader);
    return state;
}

bool graphics_Shader_compileAndAttachShader(graphics_Shader *shader, GLenum shaderType, char const* code) {
    GLchar const* header;
    GLchar const* footer;
    int headerlen = 0;
    int footerlen = 0;
    switch(shaderType) {
    case GL_VERTEX_SHADER:
        header = vertexHeader;
        headerlen = sizeof(vertexHeader) - 1;
        footer = vertexFooter;
        footerlen = sizeof(vertexFooter) - 1;
        break;
    case GL_FRAGMENT_SHADER:
        header = fragmentHeader;
        headerlen = sizeof(fragmentHeader) - 1;
        footer = fragmentFooter;
        footerlen = sizeof(fragmentFooter) - 1;
        break;
    default:
        return false;
    }
    int codelen = strlen(code);
    GLchar *combinedCode = malloc(headerlen + footerlen + codelen + 1);
    memcpy(combinedCode, header, headerlen);
    memcpy(combinedCode + headerlen, (GLchar const*)code, codelen);
    memcpy(combinedCode + headerlen + codelen, footer, footerlen+1); // include zero terminator

    bool state = graphics_Shader_compileAndAttachShaderRaw(shader, shaderType, combinedCode);

    free(combinedCode);

    return state;
}

static int compareUniformInfo(graphics_ShaderUniformInfo const* a, graphics_ShaderUniformInfo const* b) {
    return strcmp(a->name, b->name);
}

int graphics_shader_toLoveComponents(GLenum type) {
    switch(type) {
    case GL_BOOL:
    case GL_INT:
    case GL_FLOAT:
        return 1;

    case GL_BOOL_VEC2:
    case GL_INT_VEC2:
    case GL_FLOAT_VEC2:
    case GL_FLOAT_MAT2:
        return 2;

    case GL_BOOL_VEC3:
    case GL_INT_VEC3:
    case GL_FLOAT_VEC3:
    case GL_FLOAT_MAT3:
        return 3;


    case GL_BOOL_VEC4:
    case GL_INT_VEC4:
    case GL_FLOAT_VEC4:
    case GL_FLOAT_MAT4:
        return 4;

    default:
        return 0;
    };
}

graphics_ShaderUniformType graphics_shader_toLoveType(GLenum type) {
    switch(type) {
    case GL_BOOL:
    case GL_BOOL_VEC2:
    case GL_BOOL_VEC3:
    case GL_BOOL_VEC4:
        return graphics_ShaderUniformType_bool;

    case GL_INT:
    case GL_INT_VEC2:
    case GL_INT_VEC3:
    case GL_INT_VEC4:
        return graphics_ShaderUniformType_int;

    case GL_FLOAT:
    case GL_FLOAT_VEC2:
    case GL_FLOAT_VEC3:
    case GL_FLOAT_VEC4:
    case GL_FLOAT_MAT2:
    case GL_FLOAT_MAT3:
    case GL_FLOAT_MAT4:
        return graphics_ShaderUniformType_float;

    case GL_SAMPLER_2D:
        return graphics_ShaderUniformType_sampler;

    default:
        return graphics_ShaderUniformType_none;
    };
}

static void readShaderUniforms(graphics_Shader *shader) {
    shader->uniformLocations.projection  = glGetUniformLocation(shader->program, "projection");
    shader->uniformLocations.view        = glGetUniformLocation(shader->program, "view");
    shader->uniformLocations.model       = glGetUniformLocation(shader->program, "model");
    shader->uniformLocations.textureRect = glGetUniformLocation(shader->program, "textureRect");
    shader->uniformLocations.tex         = glGetUniformLocation(shader->program, DEFAULT_SAMPLER);
    shader->uniformLocations.color       = glGetUniformLocation(shader->program, "color");
    shader->uniformLocations.size        = glGetUniformLocation(shader->program, "size");

    int maxLength;
    glGetProgramiv(shader->program, GL_ACTIVE_UNIFORMS, &shader->uniformCount);
    glGetProgramiv(shader->program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);

    // This wastes some memory, but it's also relatively fast
    shader->uniforms = (graphics_ShaderUniformInfo*)malloc(sizeof(graphics_ShaderUniformInfo) * shader->uniformCount);
    for(int i = 0; i < shader->uniformCount; ++i) {
        graphics_ShaderUniformInfo* info = shader->uniforms+i;
        info->name = (char*)malloc(maxLength);
        glGetActiveUniform(shader->program, i, maxLength, NULL, &info->elements, &info->type, info->name);

        info->location = glGetUniformLocation(shader->program, info->name);
        info->extra = 0;

        char *suffix = strstr(info->name, "[0]");
        if(suffix) {
            *suffix = 0;
        }
    }

    qsort(shader->uniforms, shader->uniformCount, sizeof(graphics_ShaderUniformInfo), (int(*)(void const*,void const*))compareUniformInfo);

}


static void allocateTextureUnits(graphics_Shader *shader) {

    shader->textureUnitCount = 0;
    for(int i = 0; i < shader->uniformCount; ++i) {
        if(shader->uniforms[i].type == GL_SAMPLER_2D) {
            if(strcmp(shader->uniforms[i].name, DEFAULT_SAMPLER)) {
                ++shader->textureUnitCount;
            }
        }
    }

    shader->textureUnits = malloc(sizeof(graphics_ShaderTextureUnitInfo) * shader->textureUnitCount);

    int currentUnit = 0;
    glUseProgram(shader->program);
    for(int i = 0; i < shader->uniformCount; ++i) {
        if(shader->uniforms[i].type == GL_SAMPLER_2D) {
            if(strcmp(shader->uniforms[i].name, DEFAULT_SAMPLER)) {
                glUniform1i(shader->uniforms[i].location, currentUnit+1);
                shader->uniforms[i].extra = shader->textureUnits + currentUnit;
                shader->textureUnits[currentUnit].unit = currentUnit + 1;
                shader->textureUnits[currentUnit].boundTexture = 0;
                ++currentUnit;
            }
        }
    }

}

graphics_ShaderCompileStatus graphics_Shader_new(graphics_Shader *shader, char const* vertexCode, char const* fragmentCode) {

    memset(shader, 0, sizeof(*shader));
    shader->warnings.vertex = malloc(1);
    shader->warnings.fragment = malloc(1);
    shader->warnings.program = malloc(1);
    *shader->warnings.vertex = *shader->warnings.fragment = *shader->warnings.program = 0;

    if(!vertexCode) {
        vertexCode = defaultVertexSource;
    }

    if(!fragmentCode) {
        fragmentCode = defaultFragmentSource;
    }

    shader->program = glCreateProgram();

    if(!graphics_Shader_compileAndAttachShader(shader, GL_VERTEX_SHADER, vertexCode)) {
        return graphics_ShaderCompileStatus_vertexError;
    }

    if(!graphics_Shader_compileAndAttachShader(shader, GL_FRAGMENT_SHADER, fragmentCode)) {
        return graphics_ShaderCompileStatus_fragmentError;
    }

    glBindAttribLocation(shader->program, 0, "vPos");
    glBindAttribLocation(shader->program, 1, "vUV");
    glBindAttribLocation(shader->program, 2, "vColor");
    glLinkProgram(shader->program);

    int linkState;
    glGetProgramiv(shader->program, GL_LINK_STATUS, &linkState);
    if (linkState != GL_TRUE) {
        clove_error("ERROR OpenGL : unable to compile shader\n");
        char shader_link_error[4096];
        glGetShaderInfoLog(shader->program, sizeof(shader_link_error), NULL, shader_link_error);
        clove_error("%s", shader_link_error);
        return graphics_ShaderCompileStatus_linkError;
    }

    readShaderUniforms(shader);

    allocateTextureUnits(shader);

    shader->customShader = false;

    return graphics_ShaderCompileStatus_okay;
}

void graphics_Shader_free(graphics_Shader* shader) {
    for(int i = 0; i < shader->uniformCount; ++i) {
        free(shader->uniforms[i].name);
    }
    free(shader->textureUnits);
    free(shader->uniforms);
    glDeleteProgram(shader->program);
}
void graphics_Shader_activate(mat4x4 const* projection, mat4x4 const* view, mat4x4 const* model, graphics_Quad const* textureRect, float const* useColor, float ws, float hs) {

    glUseProgram(moduleData.activeShader->program);

    float s[3] = { ws, hs};

    glUniform1i(moduleData.activeShader->uniformLocations.tex,               0);
    glUniformMatrix4fv(moduleData.activeShader->uniformLocations.projection,  1, 0, (GLfloat const*)projection);
    glUniformMatrix4fv(moduleData.activeShader->uniformLocations.view,  1, 0, (GLfloat const*)view);
    glUniformMatrix4fv(moduleData.activeShader->uniformLocations.model,  1, 0, (GLfloat const*)model);
    glUniformMatrix2fv(moduleData.activeShader->uniformLocations.textureRect, 1, 0, (GLfloat const*)textureRect);
    glUniform4fv(moduleData.activeShader->uniformLocations.color,1,useColor);
    glUniform2fv(moduleData.activeShader->uniformLocations.size,1,s);

    for(int i = 0; i < moduleData.activeShader->textureUnitCount; ++i) {
        glActiveTexture(GL_TEXTURE0 + moduleData.activeShader->textureUnits[i].unit);
        glBindTexture(GL_TEXTURE_2D, moduleData.activeShader->textureUnits[i].boundTexture);
        //printf("Count:%d Texture id:%d Unit:%d\n", moduleData.activeShader->textureUnitCount, moduleData.activeShader->textureUnits[i].boundTexture, moduleData.activeShader->textureUnits[i].unit);
    }
}

bool graphics_Shader_hasCustomShader(graphics_Shader *shader) {
    return shader->customShader;
}

void graphics_setDefaultShader(void) {
    moduleData.activeShader = &moduleData.defaultShader;
    moduleData.activeShader->customShader = false;
}

void graphics_setShader(graphics_Shader* shader) {
    moduleData.activeShader = shader;
    moduleData.activeShader->customShader = true;
}

graphics_Shader* graphics_getShader(void) {
    return moduleData.activeShader;
}

void graphics_shader_init(void) {
    graphics_Shader_new(&moduleData.defaultShader, NULL, NULL);
    moduleData.activeShader = &moduleData.defaultShader;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &moduleData.maxTextureUnits);
}

int graphics_getMaxTextureUnits() {
    return moduleData.maxTextureUnits;
}

#define mkScalarSendFunc(name, type, glfunc) \
    void graphics_Shader_ ## name(graphics_Shader *shader, graphics_ShaderUniformInfo const* info, int count, type const* numbers) {  \
    glUseProgram(shader->program); \
    glfunc(info->location, count, numbers); \
    }

mkScalarSendFunc(sendIntegers, GLint,   glUniform1iv)
mkScalarSendFunc(sendBooleans, GLint,   glUniform1iv)
mkScalarSendFunc(sendFloats,   GLfloat, glUniform1fv)

#undef mkScalarSendFunc

#define mkVectorSendFunc(name, valuetype, abbr) \
    void graphics_Shader_ ## name(graphics_Shader *shader, graphics_ShaderUniformInfo const* info, int count, valuetype const* numbers) {  \
    glUseProgram(shader->program);                                \
    switch(graphics_shader_toLoveComponents(info->type)) {       \
    case 2:                                                       \
    glUniform2 ## abbr ## v(info->location, count, numbers);    \
    break;                                                      \
    case 3:                                                       \
    glUniform3 ## abbr ## v(info->location, count, numbers);    \
    break;                                                      \
    case 4:                                                       \
    glUniform4 ## abbr ## v(info->location, count, numbers);    \
    break;                                                      \
    }                                                             \
    }

mkVectorSendFunc(sendIntegerVectors, GLint,   i)
mkVectorSendFunc(sendBooleanVectors, GLint,   i)
mkVectorSendFunc(sendFloatVectors,   GLfloat, f)

#undef mkVectorSendFunc

void graphics_Shader_sendFloatMatrices(graphics_Shader *shader, graphics_ShaderUniformInfo const* info, int count, float const* numbers) {
    glUseProgram(shader->program);

    switch(graphics_shader_toLoveComponents(info->type)) {
    case 2:
        glUniformMatrix2fv(info->location, count, false, numbers);
        break;

    case 3:
        glUniformMatrix3fv(info->location, count, false, numbers);
        break;

    case 4:
        glUniformMatrix4fv(info->location, count, false, numbers);
        break;
    }
}


void graphics_Shader_sendTexture(graphics_Shader *shader, graphics_ShaderUniformInfo const* info, GLuint texture) {
    graphics_ShaderTextureUnitInfo *unit = (graphics_ShaderTextureUnitInfo*)info->extra;
    unit->boundTexture = texture;
}


graphics_ShaderUniformInfo const* graphics_Shader_getUniform(graphics_Shader const* shader, char const* name) {
    // Dirty trick to avoid duplicate code: Name will be treated as graphics_ShaderUniformInfo.
    return bsearch(&name, shader->uniforms, shader->uniformCount, sizeof(graphics_ShaderUniformInfo), (int(*)(void const*, void const*))compareUniformInfo);
}
