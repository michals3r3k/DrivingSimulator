#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform mat4 M1;

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 normal; //wektor normalny w przestrzeni modelu
in vec2 texCoord0;


in vec4 specular; 
in vec4 diffuse; 
in vec4 ambient; 

//Zmienne interpolowane
out vec4 ic;
out vec4 l;
out vec4 n;
out vec4 v;
out vec2 iTexCoord0;

out vec4 iSpecular; 
out vec4 iDiffuse; 
out vec4 iAmbient; 


void main(void) {


    iSpecular = specular;
    iDiffuse = diffuse;
    iAmbient = ambient;




    gl_Position=P*V*M*vertex;
    vec4 lp = vec4(0, 20, 0, 1); //przestrzeń świata
    l = normalize(V * lp - V*M*vertex); //wektor do światła w przestrzeni oka
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex); //wektor do obserwatora w przestrzeni oka
    n = normalize(V * M * normal); //wektor normalny w przestrzeni oka
    iTexCoord0 = texCoord0;
    
    //gl_Position=P*V*M*vertex;
}
