// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;

// Texture sampler
uniform sampler2D onetex;

// Tiling uniforms
uniform vec2 tiling_factor;
uniform vec2 uv_offset;

// Darkening
uniform float darkness;


void main()
{
    // scrolling background
    vec2 uv = fract(uv_interp * tiling_factor + uv_offset);

    // Sample texture
    vec4 color = texture2D(onetex, uv);

    // Check for transparency
    if(color.a < 1.0)
    {
        discard;
    }

    // Assign color to fragment
    gl_FragColor = vec4(color.rgb * (1.0 - darkness), color.a);
    
}