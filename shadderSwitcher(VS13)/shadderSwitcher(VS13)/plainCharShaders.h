/*
* Created by Chris Lanham.
*/

/*
*	Vertex Shaders
*/
static const GLchar* VertexShader =
{
	"#version 400\n"

	"layout(location = 0) in vec3 vertexPosition;\n"
	"layout(location = 1) in vec3 vertexNormal;\n"
	"layout(location = 2) in vec2 vertexTextureUV;\n"
	"layout(location = 3) in vec4 vertexColour;\n"

	"out vec2 textureUV;\n"
	"out vec4 fragColour;\n"

	"void main(void)\n"
	"{\n"
	"	gl_Position = vec4(vertexPosition,1);\n"
	"	textureUV = vertexTextureUV;\n"
	"	fragColour = vertexColour;\n"
	"}\n"
};

/*
*	Fragment Shaders
*/
static const GLchar* FragmentShader_red =
{
	"#version 400\n"

	"in vec4 fragColour;\n"

	"out vec4 out_colour;\n"

	"uniform sampler2D texture;\n"

	"void main(void)\n"
	"{\n"
	"	out_colour = vec4(1.0, 0.0, 0.0, 1.0);\n"
	"}\n"
};
static const GLchar* FragmentShader_green =
{
	"#version 400\n"

	"in vec4 fragColour;\n"

	"out vec4 out_colour;\n"

	"uniform sampler2D texture;\n"

	"void main(void)\n"
	"{\n"
	"	out_colour = vec4(0.0, 1.0, 0.0, 1.0);\n"
	"}\n"
};
static const GLchar* FragmentShader_blue =
{
	"#version 400\n"

	"in vec2 textureUV;\n"
	"in vec4 fragColour;\n"

	"out vec4 out_colour;\n"

	"uniform sampler2D texture;\n"

	"void main(void)\n"
	"{\n"
	"	out_colour = vec4(0.0, 0.0, 1.0, 1.0);\n"
	"}\n"
};
static const  GLchar* FragmentShader_white = {
	"#version 400\n"

	"out vec4 out_colour;\n"

	"void main(void){\n"
	"	out_colour = vec4(1.0, 1.0, 1.0, 1.0);\n"
	"}\n"
};
static const  GLchar* FragmentShader_colour = {
	"#version 400\n"

	"in vec4 fragColour;\n"

	"out vec4 out_colour;\n"

	"void main(void){\n"
	"	out_colour = fragColour;\n"
	"}\n"
};
static const  GLchar* FragmentShader_texture = {
	"#version 400\n"

	"in vec2 textureUV;\n"

	"out vec4 out_colour;\n"

	"uniform sampler2D texture;\n"

	"void main(void){\n"
	"	vec4 colour = vec4(1,1,1,1);\n"
	"	out_colour = colour * texture2D( texture, textureUV );\n"
	"}\n"
};
static const  GLchar* FragmentShader_textureAndColour = {
	"#version 400\n"

	"in vec4 fragColour;\n"

	"out vec4 out_colour;\n"

	"uniform sampler2D texture;\n"

	"void main(void){\n"
	"out_colour = fragColour * texture2D(texture, textureUV);\n"
	"}\n"
};
static const  GLchar* FragmentShader_textureAndColourMix = {
	"#version 400\n"

	"in vec2 textureUV;\n"
	"in vec4 fragColour;\n"

	"out vec4 out_colour;\n"

	"uniform sampler2D texture;\n"

	"void main(void){\n"
	"	out_colour = mix(\n"
	"		fragColour,\n"
	"		texture2D(texture, textureUV), \n"
	"		0.75f\n"
	"	);\n"
	"}\n"
};