// Source code of vertex shader for particle system
#version 130

// Vertex buffer
in vec2 vertex; // Vertex coordinates
in vec2 dir; // Velocity
in float t; // Phase
in vec2 uv; // Texture coordinates

// Uniform (global) buffer
uniform mat4 transformation_matrix;
uniform mat4 view_matrix;
uniform float time; // Timer

// Attributes forwarded to the fragment shader
out vec4 color_interp;
out vec2 uv_interp;

uniform float birth_time;

void main()
{
    vec4 pos;
    float duration = 0.4;
    float speed = 8.0;
    float gravity = 10.0;

    float age = (time - birth_time) - t * 0.3;

    if (age < 0.0) {
        pos = vec4(vertex.x, vertex.y, 0.0, 1.0);
    } else if (age > duration) {
        pos = vec4(0.0, 0.0, 0.0, 1.0);
    } else  {
        pos = vec4(vertex.x + dir.x * age * speed, vertex.y + dir.y * age * speed - 0.5 * gravity * age * age, 0.0, 1.0);
    }

    float fade = 1.0 - clamp(age / duration, 0.0, 1.0);
    color_interp = vec4(fade, fade, fade, 1.0);

    gl_Position = view_matrix * transformation_matrix * pos;

    uv_interp = uv;
}
