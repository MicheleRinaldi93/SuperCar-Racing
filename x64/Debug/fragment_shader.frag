#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D myTexture;

//uniform vec3 colorcube;

void main()
{
    FragColor=texture(myTexture,TexCoord);
    //FragColor = mix(texture(myTexture1, TexCoord), texture(myTexture2, TexCoord), 0.6) * vec4(colorcube, 1.0);
}