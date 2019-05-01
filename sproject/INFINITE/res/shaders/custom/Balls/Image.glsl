// Raytracing in one weekend, chapter 12: Where next? Created by Reinder Nijhoff 2018
// @reindernijhoff
//
// https://www.shadertoy.com/view/XlycWh
//
// These shaders are my implementation of the raytracer described in the (excellent) 
// book "Raytracing in one weekend" [1] by Peter Shirley (@Peter_shirley). I have tried 
// to follow the code from his book as much as possible, but I had to make some changes 
// to get it running in a fragment shader:
//
// - There are no classes (and methods) in glsl so I use structs and functions instead. 
//   Inheritance is implemented by adding a type variable to the struct and adding ugly 
//   if/else statements to the (not so overloaded) functions.
// - The scene description is procedurally implemented in the world_hit function to save
//   memory.
// - The color function is implemented using a loop because it is not possible to have a 
//   recursive function call in glsl.
// - Only one sample per pixel per frame is calculated. Samples of all frames are added 
//   in Buffer A and averaged in the Image tab.
//
// You can find the raytracer in Buffer A.
//
// Chapter  7: Diffuse       https://www.shadertoy.com/view/llVcDz
// Chapter  9: Dielectrics   https://www.shadertoy.com/view/MlVcDz
// Chapter 11: Defocus Blur  https://www.shadertoy.com/view/XlGcWh
// Chapter 12: Where next?   https://www.shadertoy.com/view/XlycWh
//
// This particular shader can be optimized (a lot) by using an acceleration structure,
// as done in my shader "More spheres": https://www.shadertoy.com/view/lsX3DH
//
// [1] http://in1weekend.blogspot.com/2016/01/ray-tracing-in-one-weekend.html
//

void mainImage( out vec4 fragColor, in vec2 fragCoord ) {
    vec4 data = texelFetch(iChannel0, ivec2(fragCoord),0);
    fragColor = vec4(sqrt(data.rgb/data.w),1.0);
}