/*
* Created by Chris Lanham.
* DDS code by opengl-tutorials.org: http://www.opengl-tutorial.org
* Huge thanks to stb for the stb_image librarys
*/
#ifndef SIMPTEX_H	
#define SIMPTEX_H	

#include "GLEW\Include\glew.h"
#include "GLM\glm.hpp"
#include "GLM\ext.hpp"
#include "stb_image.h"
using namespace std;

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

static const GLchar* SUPPORTED_FILETYPES = "SimpTex currently supports .BMP / .PNG / .JPG / .TGA / .DDS \n";

static enum RECOGNISED_FILETYPES{
	FILETYPE_NOT_SUPPORTED,
	DDS,
	BMP,
	PNG,
	JPG,
	TGA,
};
static enum {
	FLAG_ANISOTROPIC_OFF	= 1,
	FLAG_PRE_MULTI_ALPHA	= 2,
	FLAG_TEXTURE_WRAP		= 4,
	FLAG_NEAR_FILTER		= 8,
	FLAG_DONT_FLIP_IMAGE	= 16,
	FLAG_DONT_MAKE_MIPMAPS	= 32,
};

static enum IMAGE_CHANNELS{
	LOAD_LUM = 1,
	LOAD_LUM_A = 2,
	LOAD_RGB = 3,
	LOAD_RGB_A = 4
};

// function declaration(s) 
static GLuint simpTex_loadTexture(const GLchar* imagePath);
static GLuint simpTex_loadTexture(const GLchar* imagePath, GLuint flags);
static GLubyte* simpTex_loadImageDataOnly(const GLchar* imagePath, GLint *width, GLint *height, GLint *channels, GLuint load_channels);
static GLvoid simpTex_deleteTexture(GLuint textureID);
// internal
static GLuint simpTex_Internal_loadTextureBasedOnType(const GLchar * imagePath, GLuint flags);
static GLuint simpTex_Internal_load_BMP_PNG_JPG_TGA(const char* imagePath, GLuint flags);
static GLuint simpTex_Internal_loadDDS(const GLchar* imagePath, GLuint flags);
static enum RECOGNISED_FILETYPES simpTex_Internal_checkFileType(const GLchar* imagePath);


// functions definition(s)
static GLuint simpTex_loadTexture(const GLchar * imagePath){
	return simpTex_Internal_loadTextureBasedOnType(imagePath, 0);
}

static GLuint simpTex_loadTexture(const GLchar * imagePath, GLuint flags){
	return simpTex_Internal_loadTextureBasedOnType(imagePath, flags);
}

static GLubyte* simpTex_loadImageDataOnly(const GLchar* imagePath, GLint *width, GLint *height, GLint *channels, GLuint load_channels){
	GLubyte* imageData = NULL;

	imageData = stbi_load(
		imagePath,		//file location
		width,			//image width in pixels
		height,			//image height in pixels
		channels,		//data type (1=B&W, 2=B&W+alpha, 3=RGB, 4=RGB+alpha)
		load_channels	//if non-zero, # of image components requested in result
		);
	if (!imageData || imageData == NULL) {
		fprintf(stderr, "Simptex Error: Could not load. Correct file? %s\n", imagePath);
		return false;
	}
	if (width < 0 || height < 0 || channels < 0){
		fprintf(stderr, "Simptex Error: Image size too small.  %s\n", imagePath);
		return false;
	}
	return imageData;
}

static GLvoid simpTex_deleteTexture(GLuint textureID){
	glDeleteTextures(1, &textureID);
}

static GLuint simpTex_Internal_loadTextureBasedOnType(const GLchar * imagePath, GLuint flags){

	/*
	* Check to see what type of file we need to load
	*/
	GLuint filetype = simpTex_Internal_checkFileType(imagePath);

	/*
	* use the correct function for that type or fail if type not supported
	*/
	switch (filetype)
	{
	case FILETYPE_NOT_SUPPORTED:
		fprintf(stderr, "Simptex Error: Image file type not supported or could not be identified.\nCheck image path extension: %s\n%s", imagePath, SUPPORTED_FILETYPES);
		return 0;
		break;
	case DDS:
		/*
		* use a seperate loader for DDS files
		*/
		return simpTex_Internal_loadDDS(imagePath, flags);
		break;
	case BMP:
	case JPG:
	case PNG:
	case TGA:
		/*
		* we can use the same loader for BMP/PNG/JPG thanks to stb_image libaray
		*/
		return simpTex_Internal_load_BMP_PNG_JPG_TGA(imagePath, flags);
		break;
	default:
		fprintf(stderr, "Simptex Error: Unknown. Image file: %s\n", imagePath);
		return 0;
		break;
	}
}

static GLuint simpTex_Internal_load_BMP_PNG_JPG_TGA(const char* imagePath, GLuint flags){
	GLuint
		textureID,
		imageFormat,
		textureFormat = NULL;
	GLint
		imageWidth,
		imageHeight,
		imageComponents = NULL;
	GLubyte* imageData = NULL;

	/*
	* Use std_image to load our image data
	*/
	imageData = stbi_load(
		imagePath,			//file location
		&imageWidth,		//image width in pixels
		&imageHeight,		//image height in pixels
		&imageComponents,	//data type (1=B&W, 2=B&W+alpha, 3=RGB, 4=RGB+alpha)
		0					//if non-zero, # of image components requested in result
		);

	/*
	* general checks. is it a suitable texture. is it a power of two is size (e.g. 64x64 or 128x128)
	*/
	if (!imageData || imageData == NULL) {
		fprintf(stderr, "Simptex Error: Could not load. Correct file? %s\n", imagePath);
		return 0;
	}
	//// check power of two using bitwise 
	if ((imageWidth != 0) && (imageWidth  & (imageWidth - 1)) != 0 ||
		(imageHeight != 0) && (imageHeight & (imageHeight - 1)) != 0)
	{
		fprintf(stderr, "Simptex Error: Texture %s is not power-of-2 dimensions\n", imagePath);
		return 0;
	}

	/*
	* set our image format
	*/
	switch (imageComponents)
	{
	case 1:
		textureFormat = GL_LUMINANCE;
		imageFormat = GL_DEPTH_COMPONENT;
		break;
	case 2:
		textureFormat = GL_LUMINANCE_ALPHA;
		imageFormat = GL_DEPTH_STENCIL;
		break;
	case 3:
		textureFormat = GL_RGB;
		imageFormat = GL_RGB;
		break;
	case 4:
		textureFormat = GL_RGBA;
		imageFormat = GL_RGBA;
		break;
	}

	/*
	* alter our image based on requested flags
	*/
	if (flags & FLAG_DONT_FLIP_IMAGE){
		/*
		* OpenGL stores images upside down, normally we'll want to flip BMP/PNG/JPG.
		* If the user requests we don't flip then we do nothing.
		*/
	}
	else {
		int i, j;
		for (j = 0; j * 2 < imageHeight; ++j)
		{
			int index1 = j * imageWidth * imageComponents;
			int index2 = (imageHeight - 1 - j) * imageWidth * imageComponents;
			for (i = imageWidth * imageComponents; i > 0; --i)
			{
				unsigned char temp = imageData[index1];
				imageData[index1] = imageData[index2];
				imageData[index2] = temp;
				++index1;
				++index2;
			}
		}
	}

	if (flags & FLAG_PRE_MULTI_ALPHA){
		int i;
		switch (imageComponents)
		{
		case 2:
			for (i = 0; i < 2 * imageWidth*imageHeight; i += 2)
			{
				imageData[i] = (imageData[i] * imageData[i + 1] + 128) >> 8;
			}
			break;
		case 4:
			for (i = 0; i < 4 * imageWidth*imageHeight; i += 4)
			{
				imageData[i + 0] = (imageData[i + 0] * imageData[i + 3] + 128) >> 8;
				imageData[i + 1] = (imageData[i + 1] * imageData[i + 3] + 128) >> 8;
				imageData[i + 2] = (imageData[i + 2] * imageData[i + 3] + 128) >> 8;
			}
			break;
		default:
			/*	no other number of channels contains alpha data	*/
			break;
		}
	}

	/*
	* give the data to OpenGL
	*/
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(
		GL_TEXTURE_2D,		// texture type
		0,					// detail level (minimap)
		imageFormat,		// how should openGL store this data
		imageWidth,			// width
		imageHeight,		// height
		0,					// border (must be 0)
		textureFormat,		// format of data given
		GL_UNSIGNED_BYTE,	// type of data given
		imageData			// our image data
		);

	// image near filtering ON/OFF
	if (flags & FLAG_NEAR_FILTER){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	else{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	// texture wraping ON/OFF
	if (flags & FLAG_TEXTURE_WRAP){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}
	// auto minimaps ON/OFF
	if (flags & FLAG_DONT_MAKE_MIPMAPS){
		/*
		* If user requests no Minimaps. Do nothing.
		*/
	}
	else{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	// anisotropic filtering ON/OFF
	if ((flags & FLAG_ANISOTROPIC_OFF) == false){
		GLfloat anisotropicFilterLevel = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropicFilterLevel);
		// set the maximum!
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropicFilterLevel);
	}

	/*
	* openGL now has the texture, delete local copy and return textureID
	*/
	delete[] imageData;
	return textureID;
}

static GLuint simpTex_Internal_loadDDS(const char* imagePath, GLuint flags){

	unsigned char header[124];

	FILE *fp;
	errno_t fileReadError;
	GLuint ErrorMessageMaxSize = 100;
	GLchar fileErrorMessage[100];

	///* try to open the file */
	if (fileReadError = fopen_s(&fp, imagePath, "rb")){
		fprintf(stderr, "Simptex Error: Cannot open file '%s': %s\n",
			imagePath,
			strerror_s(fileErrorMessage, ErrorMessageMaxSize, fileReadError)
			);
		return 0;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return 0;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);

	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	if ((flags & FLAG_DONT_MAKE_MIPMAPS) == false){
		for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
		{
			unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
			glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
				0, size, buffer + offset);

			offset += size;
			width /= 2;
			height /= 2;

			// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
			if (width < 1) width = 1;
			if (height < 1) height = 1;
		}
	}
	/* load the mipmaps */


	free(buffer);
	return textureID;
}

static enum RECOGNISED_FILETYPES simpTex_Internal_checkFileType(const GLchar* imagePath){
	GLuint filePathLength = strlen(imagePath);

	if ((imagePath[filePathLength - 3] == 'd' || imagePath[filePathLength - 3] == 'D') &&
		(imagePath[filePathLength - 2] == 'd' || imagePath[filePathLength - 2] == 'D') &&
		(imagePath[filePathLength - 1] == 's' || imagePath[filePathLength - 1] == 'S')){
		return DDS;
	}
	else if ((imagePath[filePathLength - 3] == 'b' || imagePath[filePathLength - 3] == 'B') &&
		(imagePath[filePathLength - 2] == 'm' || imagePath[filePathLength - 2] == 'M') &&
		(imagePath[filePathLength - 1] == 'p' || imagePath[filePathLength - 1] == 'P')){
		return BMP;
	}
	else if ((imagePath[filePathLength - 3] == 'p' || imagePath[filePathLength - 3] == 'P') &&
		(imagePath[filePathLength - 2] == 'n' || imagePath[filePathLength - 2] == 'N') &&
		(imagePath[filePathLength - 1] == 'g' || imagePath[filePathLength - 1] == 'G')){
		return PNG;
	}
	else if ((imagePath[filePathLength - 3] == 'j' || imagePath[filePathLength - 3] == 'J') &&
		(imagePath[filePathLength - 2] == 'p' || imagePath[filePathLength - 2] == 'P') &&
		(imagePath[filePathLength - 1] == 'g' || imagePath[filePathLength - 1] == 'G')){
		return JPG;
	}
	else if ((imagePath[filePathLength - 3] == 't' || imagePath[filePathLength - 3] == 'T') &&
		(imagePath[filePathLength - 2] == 'g' || imagePath[filePathLength - 2] == 'G') &&
		(imagePath[filePathLength - 1] == 'a' || imagePath[filePathLength - 1] == 'A')){
		return TGA;
	}
	else {
		return FILETYPE_NOT_SUPPORTED;
	}
}

////////////////////////////////////////////////////////////////////////////////
#endif