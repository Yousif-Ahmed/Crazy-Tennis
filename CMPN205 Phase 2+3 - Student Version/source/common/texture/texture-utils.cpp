#include "texture-utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

our::Texture2D* our::texture_utils::empty(GLenum format, glm::ivec2 size){
    our::Texture2D* texture = new our::Texture2D();
    //DONE: (Req 10) Finish this function to create an empty texture with the given size and format
    
    // NOTE : don't know which format is passed intenal or what ????
    //Bind the texture 
    texture->bind();

    glTexImage2D(GL_TEXTURE_2D, 0 ,GL_RGBA8 , size.x , size.y , 0 , format ,GL_UNSIGNED_BYTE , (void*) nullptr );
    
    return texture;
}

our::Texture2D* our::texture_utils::loadImage(const std::string& filename, bool generate_mipmap) {
    glm::ivec2 size;
    int channels;
    //Since OpenGL puts the texture origin at the bottom left while images typically has the origin at the top left,
    //We need to till stb to flip images vertically after loading them
    stbi_set_flip_vertically_on_load(true);
    //Load image data and retrieve width, height and number of channels in the image
    //The last argument is the number of channels we want and it can have the following values:
    //- 0: Keep number of channels the same as in the image file
    //- 1: Grayscale only
    //- 2: Grayscale and Alpha
    //- 3: RGB
    //- 4: RGB and Alpha (RGBA)
    //Note: channels (the 4th argument) always returns the original number of channels in the file
    unsigned char* pixels = stbi_load(filename.c_str(), &size.x, &size.y, &channels, 4);
    if(pixels == nullptr){
        std::cerr << "Failed to load image: " << filename << std::endl;
        return nullptr;
    }
    // Create a texture
    our::Texture2D* texture = new our::Texture2D();
    //Bind the texture such that we upload the image data to its storage
    texture->bind() ;
    //Done: (Req 4) Finish this function to fill the texture with the data found in "pixels" and generate the mipmaps if requested
        
    // NOTE :we can set unpack alighnment ti 4-byte but this is not necessary since Alignment is 4 by default 

    // filling the texture with all data found un pixels variable 
    // target : which is specifies the target to which the texture is bound in our case we 
    // used GL_TEXTURE_2D bound 
    // level : specifies the level-of-detail number as Level 0 is the base image level
    // internal format : specifies the number of color components in the texture and we use RGBA
    // width: the width of texture image 
    // height : the width of texture image 
    // border : must be 0
    // format : Specifies the format of the pixel data in our case RGBA 
    // type : Specifies the data type of the pixel data in our case GL_UNSIGNED_BYTE
    // data : specifies a pointe to the image data in memory.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE,(void*) pixels);
    
    if (generate_mipmap)  glGenerateMipmap(GL_TEXTURE_2D); 


    stbi_image_free(pixels); //Free image data after uploading to GPU
    return texture;
}