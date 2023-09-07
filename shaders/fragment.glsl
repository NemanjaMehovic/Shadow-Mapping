#version 430
in vec3 color;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLight;

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 lightPos;

uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    
    float bias = max(0.025 * (1.0 - dot(Normal, lightDir)), 0.0005);

    float shadow = 0.0;
    int sampleRadius = 2;
	vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
	for(int y = -sampleRadius; y <= sampleRadius; y++)
	{
		for(int x = -sampleRadius; x <= sampleRadius; x++)
		{
		    float closestDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * pixelSize).r;
			if (currentDepth > closestDepth + bias)
				shadow += 1.0f;     
		}    
	}

	shadow /= pow((sampleRadius * 2 + 1), 2);
    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
} 

void main()
{
    vec3 lightColor = vec3(1, 1, 1);
    vec3 ambientLight = vec3(0.4, 0.4, 0.4);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor; 

    float shadow = ShadowCalculation(FragPosLight, lightDir);

    vec3 finalColor = (ambientLight + (1.0 - shadow) * (diffuse + specular)) * color;
    FragColor = vec4(finalColor, 1.0);
} 