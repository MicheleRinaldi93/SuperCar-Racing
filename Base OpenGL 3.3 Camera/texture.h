///*
//---------------------------------------------------------------------------
//Open Asset Import Library (assimp)
//---------------------------------------------------------------------------
//
//Copyright (c) 2006-2016, assimp team
//
//All rights reserved.
//
//Redistribution and use of this software in source and binary forms,
//with or without modification, are permitted provided that the following
//conditions are met:
//
//* Redistributions of source code must retain the above
//  copyright notice, this list of conditions and the
//  following disclaimer.
//
//* Redistributions in binary form must reproduce the above
//  copyright notice, this list of conditions and the
//  following disclaimer in the documentation and/or other
//  materials provided with the distribution.
//
//* Neither the name of the assimp team, nor the names of its
//  contributors may be used to endorse or promote products
//  derived from this software without specific prior
//  written permission of the assimp team.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//---------------------------------------------------------------------------
//*/
//
///** @file texture.h
// *  @brief Defines texture helper structures for the library
// *
// * Used for file formats which embed their textures into the model file.
// * Supported are both normal textures, which are stored as uncompressed
// * pixels, and "compressed" textures, which are stored in a file format
// * such as PNG or TGA.
// */
//
//#ifndef AI_TEXTURE_H_INC
//#define AI_TEXTURE_H_INC
//
//#include "types.h"
//
//#ifdef __cplusplus
//extern "C" {
//#endif
//
//
//    // --------------------------------------------------------------------------------
//    /** @def AI_MAKE_EMBEDDED_TEXNAME
//     *  Used to build the reserved path name used by the material system to
//     *  reference textures that are embedded into their corresponding
//     *  model files. The parameter specifies the index of the texture
//     *  (zero-based, in the aiScene::mTextures array)
//     */
//#if (!defined AI_MAKE_EMBEDDED_TEXNAME)
//#   define AI_MAKE_EMBEDDED_TEXNAME(_n_) "*" # _n_
//#endif
//
//
//#include "./Compiler/pushpack1.h"
//
//     // --------------------------------------------------------------------------------
//     /** @brief Helper structure to represent a texel in a ARGB8888 format
//     *
//     *  Used by aiTexture.
//     */
//    struct aiTexel
//    {
//        unsigned char b, g, r, a;
//
//#ifdef __cplusplus
//        //! Comparison operator
//        bool operator== (const aiTexel& other) const
//        {
//            return b == other.b && r == other.r &&
//                g == other.g && a == other.a;
//        }
//
//        //! Inverse comparison operator
//        bool operator!= (const aiTexel& other) const
//        {
//            return b != other.b || r != other.r ||
//                g != other.g || a != other.a;
//        }
//
//        //! Conversion to a floating-point 4d color
//        operator aiColor4D() const
//        {
//            return aiColor4D(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
//        }
//#endif // __cplusplus
//
//    } PACK_STRUCT;
//
//#include "./Compiler/poppack1.h"
//
//    // --------------------------------------------------------------------------------
//    /** Helper structure to describe an embedded texture
//     *
//     * Normally textures are contained in external files but some file formats embed
//     * them directly in the model file. There are two types of embedded textures:
//     * 1. Uncompressed textures. The color data is given in an uncompressed format.
//     * 2. Compressed textures stored in a file format like png or jpg. The raw file
//     * bytes are given so the application must utilize an image decoder (e.g. DevIL) to
//     * get access to the actual color data.
//     *
//     * Embedded textures are referenced from materials using strings like "*0", "*1", etc.
//     * as the texture paths (a single asterisk character followed by the
//     * zero-based index of the texture in the aiScene::mTextures array).
//     */
//    struct aiTexture
//    {
//        /** Width of the texture, in pixels
//         *
//         * If mHeight is zero the texture is compressed in a format
//         * like JPEG. In this case mWidth specifies the size of the
//         * memory area pcData is pointing to, in bytes.
//         */
//        unsigned int mWidth;
//
//        /** Height of the texture, in pixels
//         *
//         * If this value is zero, pcData points to an compressed texture
//         * in any format (e.g. JPEG).
//         */
//        unsigned int mHeight;
//
//        /** A hint from the loader to make it easier for applications
//         *  to determine the type of embedded compressed textures.
//         *
//         * If mHeight != 0 this member is undefined. Otherwise it
//         * is set set to '\\0\\0\\0\\0' if the loader has no additional
//         * information about the texture file format used OR the
//         * file extension of the format without a trailing dot. If there
//         * are multiple file extensions for a format, the shortest
//         * extension is chosen (JPEG maps to 'jpg', not to 'jpeg').
//         * E.g. 'dds\\0', 'pcx\\0', 'jpg\\0'.  All characters are lower-case.
//         * The fourth character will always be '\\0'.
//         */
//        char achFormatHint[4];
//
//        /** Data of the texture.
//         *
//         * Points to an array of mWidth * mHeight aiTexel's.
//         * The format of the texture data is always ARGB8888 to
//         * make the implementation for user of the library as easy
//         * as possible. If mHeight = 0 this is a pointer to a memory
//         * buffer of size mWidth containing the compressed texture
//         * data. Good luck, have fun!
//         */
//        C_STRUCT aiTexel* pcData;
//
//#ifdef __cplusplus
//
//        //! For compressed textures (mHeight == 0): compare the
//        //! format hint against a given string.
//        //! @param s Input string. 3 characters are maximally processed.
//        //!        Example values: "jpg", "png"
//        //! @return true if the given string matches the format hint
//        bool CheckFormat(const char* s) const
//        {
//            return (0 == ::strncmp(achFormatHint, s, 3));
//        }
//
//        // Construction
//        aiTexture()
//            : mWidth(0)
//            , mHeight(0)
//            , pcData(NULL)
//        {
//            achFormatHint[0] = achFormatHint[1] = 0;
//            achFormatHint[2] = achFormatHint[3] = 0;
//        }
//
//        // Destruction
//        ~aiTexture()
//        {
//            delete[] pcData;
//        }
//#endif
//    };
//
//
//#ifdef __cplusplus
//}
//#endif
//
//#endif // AI_TEXTURE_H_INC
//
////#pragma once
////#define STB_IMAGE_IMPLEMENTATION
////#include <iostream>
////#include <cstring>
////#include <string>
////
////#include <glad/glad.h>
////#include <GLFW/glfw3.h>
////#include <glm/glm.hpp>
////#include <glm/gtc/matrix_transform.hpp>
////#define GLM_ENABLE_EXPERIMENTAL
////#include <glm/gtx/rotate_vector.hpp>
////#include <glm/gtc/type_ptr.hpp>
////
////
////class Texture {
////public:
////    // load and create a texture 
////    unsigned int loadTexture(std::string filename)
////    {
////        unsigned int texture;
////
////        glGenTextures(1, &texture);
////        glBindTexture(GL_TEXTURE_2D, texture);
////        // set the texture wrapping parameters
////        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
////        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
////        // set texture filtering parameters
////        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
////        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
////        // load image, create texture and generate mipmaps
////        int width, height, nrChannels;
////        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
////                                                                                        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
////        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
////        if (data)
////        {
////            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
////            glGenerateMipmap(GL_TEXTURE_2D);
////        }
////        else
////        {
////            std::cout << "Failed to load texture" << std::endl;
////            return -1;
////        }
////        std::cout << "Texture loaded " << filename << std::endl;
////        stbi_image_free(data);
////
////        return texture;
////    }
////
////    unsigned int loadTextureAlpha(char const* path)
////    {
////        unsigned int textureID;
////        glGenTextures(1, &textureID);
////
////        int width, height, nrComponents;
////        unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
////        if (data)
////        {
////            GLenum format;
////            if (nrComponents == 1)
////                format = GL_RED;
////            else if (nrComponents == 3)
////                format = GL_RGB;
////            else if (nrComponents == 4)
////                format = GL_RGBA;
////
////            glBindTexture(GL_TEXTURE_2D, textureID);
////            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
////            glGenerateMipmap(GL_TEXTURE_2D);
////
////            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
////            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
////            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
////            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
////
////            std::cout << "Texture loaded " << path << std::endl;
////            stbi_image_free(data);
////        }
////        else
////        {
////            std::cout << "Texture failed to load at path: " << path << std::endl;
////            stbi_image_free(data);
////        }
////
////        return textureID;
////    }
////};