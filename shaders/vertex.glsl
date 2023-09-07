#version 430
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 color;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLight;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightView;
uniform mat4 lightProjection;
uniform mat3 modelNormal;
uniform bool isPlatform;


void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = modelNormal * aNormal;
    FragPosLight = lightProjection * lightView * vec4(FragPos, 1.0);
    if(isPlatform)
        color = vec3(0.75, 0.75, 0.75);
    else if(aPos.y < 0)
        color = vec3(0.3, 0, 0.1);
    else
        color = vec3(0.1, 0, 0.8);
}