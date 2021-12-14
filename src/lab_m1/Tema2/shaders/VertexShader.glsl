#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_color;
layout(location = 3) in vec2 v_normal;
layout(location = 2) in vec3 tex_coord;


// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 color;

// Output
out vec3 frag_color;
out vec3 frag_normal;

void main()
{
    frag_color = color;
    frag_normal = vec3(v_normal, 1);
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
