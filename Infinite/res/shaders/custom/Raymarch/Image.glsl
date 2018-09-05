float Sphere(vec3 centre, float radius, vec3 point)
{
	return length(centre - point) - radius;
}

float Scene(vec3 pos)
{
	float f = Sphere(vec3(-1.0, -1, 0), 1.0, pos);
    f = min(f, Sphere(vec3(1.0, -1, 0), 1.0, pos));
    f = min(f, Sphere(vec3(0, 1, 0), 1.0, pos));
    return f;
}

void Rotate(inout vec3 vector, vec2 angle)
{
    vector.yz = cos(angle.y) * vector.yz + sin(angle.y) * vec2(-1, 1) * vector.zy;
    vector.xz = cos(angle.x) * vector.xz + sin(angle.x) * vec2(-1, 1) * vector.zx;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 uv = (fragCoord - iResolution.xy * 0.5) / iResolution.x;
    
    vec3 ray = normalize(vec3(uv, 1.0));
    vec3 pos = vec3(0, 0, -8);
    
    vec2 angle = vec2(0.0, 0.0);
    
    if (iMouse.z >= 0.0) 
        angle = (iMouse.xy / iResolution.xy - 0.5) * 3.14;
        
    Rotate(pos, angle);
    Rotate(ray, angle);
    
	fragColor = vec4(1);
    
    for (int i = 0; i < 100; ++i)
    {
    	float h = Scene(pos);
        if (h < 0.01)
        {
        	fragColor = vec4(0);
            break;
        }
        pos += ray * h;
    }
    
    if (Scene(pos) < 0.01)
    {
    	vec3 normal;
        normal.x = Scene(pos + vec3(0.01, 0, 0)) - Scene(pos - vec3(0.01, 0, 0));
        normal.y = Scene(pos + vec3(0, 0.01, 0)) - Scene(pos - vec3(0, 0.01, 0));
        normal.z = Scene(pos + vec3(0, 0, 0.01)) - Scene(pos - vec3(0, 0, 0.01));
        normal = normalize(normal);
        
        vec3 lightSource = vec3(4, 4, -4);
   	 	float light = max(0.0, dot(normal, normalize(lightSource - pos)));
        vec3 ambient = vec3(0.05, 0.1, 0.15);
        vec3 lightCol = vec3(1.0, 0.9, 0.8);
        
    	fragColor.rgb = light * lightCol + ambient;
    }
    
    fragColor.rgb = pow(fragColor.rgb, vec3(1.0 / 2.2));
}
