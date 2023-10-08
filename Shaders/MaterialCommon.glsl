
struct Material {
    sampler2D diffuse;
    sampler2D specular;
	sampler2D emissive;
	sampler2D normal;
    float shininess;
};

uniform Material material;
