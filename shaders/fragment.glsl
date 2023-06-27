#version 330 core

struct Material {
	sampler2D color; //diffuse and ambient colors (now a texture)
	sampler2D specular; //specular highlight color (usually the light color or something close to between the light and the object's colors) (now a texture)
	sampler2D emission;
	float shininess; //reflectivity, or "metalness". This controls the spec highlight's exponent property.

};

//same thing as above but defines scale values

//very basic point light. does not even have basic functions.
struct BaseLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

//these lights have no position. usually good for suns in games.
struct DirectionLight {
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


out vec4 FragColor;

in vec2 coords;
in vec3 desiredColor;
in vec3 lightColor;
in vec3 normal;
in vec3 fragPosition;
in vec3 lightPosit;
in vec3 camPosition;


uniform Material mat;
uniform DirectionLight light;

vec3 dirlightdirection(DirectionLight light) {
	return normalize(-light.direction);
}

vec3 baselightdirection(BaseLight light) {
	return normalize(light.position - fragPosition);
}

void main() {

	//this is all lighting calcs for phong shading
	//it consists of ambient lighting, diffuse mapping and specular mapping

	//ambient
	vec3 ambientLighting = light.ambient * vec3(texture(mat.color, coords));

	//diffuse

	vec3 lightdir = dirlightdirection(light); //this is pretty much agnostic (it's not specifically relative to it's section)


	float diff =  max(dot(normalize(normal), lightdir), 0.0);
	vec3 diffusionLighting = light.diffuse * (diff * vec3(texture(mat.color, coords)));

	//specular



	vec3 cameradir = normalize(camPosition - fragPosition); //this is also agnostic (it's not specifically relative to it's section)

	vec3 speclight = reflect(-lightdir, normal);

	float spec = pow(max(dot(cameradir, speclight), 0.0), mat.shininess);
	vec3 specularHighlight = light.specular *  (spec * vec3(texture(mat.specular, coords)));

	//merge into phong shading model

	FragColor = vec4((ambientLighting + diffusionLighting + specularHighlight + vec3(texture(mat.emission, coords))), 1.0);

	//FragColor = mix(texture(TexSampler, FTexCoords), texture(TexSampler2, FTexCoords), 0.3);

}

