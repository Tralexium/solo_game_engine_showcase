#version 430 core
out vec4 FragColour;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

in vec3 Normal;  
in vec3 FragPos;
in vec2 TexCoords;
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColour;
uniform vec3 objectColour;

const float ambientStrength = 0.5;
const float specularStrength = 0.8; 
const float shininess = 32.0;

void main()
{
	vec3 L = normalize(lightPos - viewPos);   
    vec3 E = normalize(-viewPos);
    vec3 H = normalize(L + E);

    // ambient
    vec3 ambient = ambientStrength * lightColour;
  	
    // diffuse 
	vec3 norm = texture(texture_normal1, TexCoords).rgb; // normals texture
    norm = normalize(norm * 2.0 - 1.0);  // normalize
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = clamp(diff * lightColour, 0.0, 1.0);
    
    // specular  
    float spec = pow(max(dot(norm, H), 0.0), shininess);
    vec3 specular = clamp(specularStrength * spec * lightColour, 0.0, 1.0);  	
		
    vec3 result = (ambient + diffuse + specular) * objectColour;
	FragColour = texture(texture_diffuse1, TexCoords) * vec4(result, 1.0);
} 
