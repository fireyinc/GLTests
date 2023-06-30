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

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float distance;
};

struct SpotLight {
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float outerconeangle;
	float innerconeangle;

	float distance;
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
uniform PointLight light;
// uniform PointLight light2;


vec3 calcbaselight(vec3 cameradir, BaseLight light) {
	vec3 lightdir = normalize(light.position - fragPosition);

	//ambient

	vec3 ambientLighting = light.ambient * vec3(texture(mat.color, coords));

	//diffusion

	float diff =  max(dot(normalize(normal), lightdir), 0.0);
	vec3 diffusionLighting = light.diffuse * (diff * vec3(texture(mat.color, coords)));

	//specular


	vec3 speclight = reflect(-lightdir, normal);

	float spec = pow(max(dot(cameradir, speclight), 0.0), mat.shininess);
	vec3 specularHighlight = light.specular *  (spec * vec3(texture(mat.specular, coords)));

	return (ambientLighting + diffusionLighting + specularHighlight);

}

vec3 calcdirlight(DirectionLight light, vec3 cameradir) {
	vec3 lightdir = normalize(-light.direction);

	//ambient

	vec3 ambientLighting = light.ambient * vec3(texture(mat.color, coords));

	//diffusion

	float diff =  max(dot(normalize(normal), lightdir), 0.0);
	vec3 diffusionLighting = light.diffuse * (diff * vec3(texture(mat.color, coords)));

	//specular


	vec3 speclight = reflect(-lightdir, normal);

	float spec = pow(max(dot(cameradir, speclight), 0.0), mat.shininess);
	vec3 specularHighlight = light.specular *  (spec * vec3(texture(mat.specular, coords)));

	return (ambientLighting + diffusionLighting + specularHighlight);
}


vec3 calcpointlight(PointLight light, vec3 cameradir) {

	vec3 lightdir = normalize(light.position - fragPosition);


	float constant = 1.0;
	float linear = 4.5/light.distance;
	float quadratic = 75.0/(light.distance*light.distance);
	float distance = length(light.position - fragPosition);



	float attenuation = 1.0/(constant + linear * distance + quadratic * (distance*distance));
	
	//ambient

	vec3 ambientLighting = light.ambient * vec3(texture(mat.color, coords));

	//diffusion

	float diff =  max(dot(normalize(normal), lightdir), 0.0);
	vec3 diffusionLighting = light.diffuse * (diff * vec3(texture(mat.color, coords)));

	//specular


	vec3 speclight = reflect(-lightdir, normal);

	float spec = pow(max(dot(cameradir, speclight), 0.0), mat.shininess);
	vec3 specularHighlight = light.specular *  (spec * vec3(texture(mat.specular, coords)));

	ambientLighting *= attenuation;
	diffusionLighting *= attenuation;
	specularHighlight *= attenuation;

	return (ambientLighting + diffusionLighting + specularHighlight);
}

vec3 calcspotlight(SpotLight light, vec3 cameradir) {
	vec3 fragDir = normalize(light.position - fragPosition);
	float fragAngle = dot(fragDir, normalize(-light.direction));

	float intensity = clamp(((fragAngle-light.outerconeangle)/(light.innerconeangle-light.outerconeangle)), 0.0, 1.0);
	vec3 lightdir = normalize(light.position - fragPosition);

	float constant = 1.0;
	float linear = 4.5/light.distance;
	float quadratic = 75.0/(light.distance*light.distance);
	float distance = length(light.position - fragPosition);

	float attenuation = 1.0/(constant + linear*distance + quadratic*(distance*distance));

	if (fragAngle > light.outerconeangle) {
		//ambient

		vec3 ambientLighting = light.ambient * vec3(texture(mat.color, coords));

		//diffusion

		float diff =  max(dot(normalize(normal), lightdir), 0.0);
		vec3 diffusionLighting = light.diffuse * (diff * vec3(texture(mat.color, coords)));

		//specular


		vec3 speclight = reflect(-lightdir, normal);

		float spec = pow(max(dot(cameradir, speclight), 0.0), mat.shininess);
		vec3 specularHighlight = light.specular *  (spec * vec3(texture(mat.specular, coords)));

		diffusionLighting *= intensity;
		specularHighlight *= intensity;

		//ambientLighting *= attenuation;
		diffusionLighting *= attenuation;
		specularHighlight *= attenuation;

		return (ambientLighting + diffusionLighting + specularHighlight);
	}

	else {
		vec3 ambientLighting = light.ambient * vec3(texture(mat.color, coords));
		return (ambientLighting * attenuation);
	}

}


void main() {
	//future optimizations to note:
	//calculate ambient only once here and use it in all the functions
	//calculating the light positions for both types of lights once here might look inefficient but it really isnt if using multiple types of lights many times




	vec3 cameradir = normalize(camPosition - fragPosition); 

	vec3 lighteffects;

	lighteffects += calcspotlight(light, cameradir);


	//merge into phong shading model

	FragColor = vec4((lighteffects + vec3(texture(mat.emission, coords))), 1.0);

	//FragColor = mix(texture(TexSampler, FTexCoords), texture(TexSampler2, FTexCoords), 0.3);

}

