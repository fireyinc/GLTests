
#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec2 TexCoords;
layout (location = 2) in vec3 normalMap;

out vec3 desiredColor;
out vec3 lightColor;
out vec3 normal;

out vec3 fragPosition;
out vec3 lightPosit;
out vec3 camPosition;
out vec2 coords;

//projection and world space stuff
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

//colors
uniform vec3 cubecolor;
uniform vec3 lightcolor;
uniform vec3 cameraposition;

//positions
uniform vec3 lightposition;


void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);


    //shader data handoffs (all of these has an out)

    fragPosition = vec3(model * vec4(aPos, 1.0));


    lightPosit = lightposition;

    lightColor = lightcolor;
    desiredColor = cubecolor;
    coords = TexCoords;
    camPosition = cameraposition;

    //this is unoptimized for gpu coding, and this action runs better on the cpu than the gpu
    //once i can do this on a uniform i will do it on a uniform

    normal = mat3(transpose(inverse(model))) * normalMap;

    //end shader handoffs

}       