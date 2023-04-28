#version 430
in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingVertPos;
in vec3 varyingHalfVector;
in vec4 shadow_coord;
out vec4 fragColor;

// 与顶点着色器相同的统一变量
// 但并不直接在当前片段着色器使用

struct PositionalLight
{ 
	vec4 ambient; 
	vec4 diffuse; 
	vec4 specular; 
	vec3 position;
};

struct Material
{ 
	vec4 ambient; 
	vec4 diffuse; 
	vec4 specular; 
	float shininess;
};

uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

layout (binding=0) uniform sampler2DShadow shTex;

float lookup(float ox, float oy)
{ 
	//偏移量需要乘以1/windowsize，这里我们简单地假设窗口大小为1000像素×1000像素，将乘数硬编码为0.001
	float t = textureProj(shTex,  shadow_coord + vec4(ox * 0.001 * shadow_coord.w, oy * 0.001 * shadow_coord.w, -0.01, 0.0)); //第三个参数（-0.01）是用于消除阴影痤疮的偏移量
	return t;
}

void main(void)
{
	// 正规化光照向量、法向量、视觉向量
	vec3 L = normalize(varyingLightDir); 
	vec3 N = normalize(varyingNormal); 
	vec3 V = normalize(-varyingVertPos); 
	vec3 H = normalize(varyingHalfVector); 

	// -----正常渲染阴影，无柔和阴影
	/*
	// 计算光照与平面法向量间的角度
	float cosTheta = dot(L,N);
	// 计算法向量N与角平分线向量H之间的角度
	float cosPhi = dot(H,N); 

	// 计算ADS分量(按像素)，并合并以构建输出颜色
	// 角平分线向量H已经在顶点着色器中计算过，并在光栅器中进行过插值
	vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz; 
	vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(cosTheta,0.0); 
	vec3 specular = light.specular.xyz * material.specular.xyz * pow(max(cosPhi,0.0), material.shininess*3.0); // 最后乘以3.0作为改善镜面高光的微调

	float notlnShadow = textureProj(shTex, shadow_coord);//阴影纹理中检索正确的值
	if(notlnShadow == 1.0)
	{
		fragColor = vec4((ambient + diffuse + specular), 1.0);
	}
	else
	{
		fragColor = vec4((ambient), 1.0);
	}
	*/

	float shadowFactor = 0.0;

	// -----生成一个4采样抖动的柔和阴影——效果没64采用好，但性能高
	/*
    float swidth = 2.5;     //可调整的阴影扩散量，用于调整阴影边缘的“柔和”区域的大小。
    // 根据glFragCoord mod 2生成4采样模式中的一个
    vec2 offset = mod(floor(gl_FragCoord.xy), 2.0) * swidth;
    shadowFactor += lookup(-1.5*swidth + offset.x, 1.5*swidth - offset.y);
    shadowFactor += lookup(-1.5*swidth + offset.x, -0.5*swidth - offset.y);
    shadowFactor += lookup( 0.5*swidth + offset.x, 1.5*swidth - offset.y);
    shadowFactor += lookup( 0.5*swidth + offset.x, -0.5*swidth - offset.y);
    shadowFactor = shadowFactor / 4.0; // shadowFactor是4个采样点的平均值
	*/

    // -----生成64采样的高分辨率柔和阴影——性能没64采用好，但效果好
    float swidth = 2.5;     // 可调整的阴影扩散量，用于调整阴影边缘的“柔和”区域的大小。
    float endp = swidth*3.0 +swidth/2.0; 
    for (float m=-endp ; m<=endp ; m=m+swidth) //-8.75、-6.25、-3.75、-1.25、1.25、3.75、6.25、8.75 x轴八个采样点
    {   for (float n=-endp ; n<=endp ; n=n+swidth)  //-8.75、-6.25、-3.75、-1.25、1.25、3.75、6.25、8.75	y轴八个采样点
        {         
			shadowFactor += lookup(m,n); 
		}   
	} 
    shadowFactor = shadowFactor / 64.0; 

    vec4 shadowColor = globalAmbient * material.ambient + light.ambient * material.ambient; 
    vec4 lightedColor = light.diffuse * material.diffuse * max(dot(L,N),0.0) 
                      + light.specular * material.specular 
                      * pow(max(dot(H,N),0.0),material.shininess*3.0); 

    fragColor = vec4((shadowColor.xyz + shadowFactor*(lightedColor.xyz)),1.0);

}