/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "myCube.h"
#include "myTeapot.h"
#include "MyCar.h"
#include "OBJ_Loader.h"
#include "OBJModel.h"

float aspectRatio=1;
ShaderProgram *sp;

MyCar * car;
MyCar* enemyCar;
MyCar* enemyCar1;
MyCar* enemyCar2;

objl::Loader loader;


//Odkomentuj, żeby rysować kostkę
//float* vertices = myCubeVertices;
//float* normals = myCubeNormals;
//float* texCoords = myCubeTexCoords;
//float* colors = myCubeColors;
//int vertexCount = myCubeVertexCount;


//Odkomentuj, żeby rysować czajnik
float* vertices = myTeapotVertices;
float* normals = myTeapotVertexNormals;
float* texCoords = myTeapotTexCoords;
float* colors = myTeapotColors;
int vertexCount = myTeapotVertexCount;

float base[] = {
    -50.0f, 0.0f,  50.0f,  1.0f,
	50.0f,  0.0f,  50.0f,  1.0f,
	-50.0f, 0.0f,  -50.0f, 1.0f, //-1 |-
	50.0f,  0.0f,   50.0f, 1.0f,
	-50.0f, 0.0f,  -50.0f, 1.0f,
	50.0f, 0.0f,  -50.0f,  1.0f, //-2 -- góra _|
	50.0f,  -4.0f,  50.0f,  1.0f,
	-50.0f, -4.0f,  50.0f,  1.0f,
	-50.0f, -4.0f,  -50.0f, 1.0f, //-3
	-50.0f, -4.0f,  -50.0f, 1.0f,
	50.0f, -4.0f,  -50.0f,  1.0f,
	50.0f,  -4.0f,   50.0f, 1.0f, //-4 -- dół
	50.0f,  -4.0f,  50.0f,  1.0f,
	50.0f,  0.0f,  50.0f,  1.0f,
	50.0f,  -4.0f,  -50.0f, 1.0f, //-5
	50.0f,  -4.0f,  -50.0f, 1.0f,
	50.0f,  0.0f,  50.0f,  1.0f,
	-50.0f, 0.0f,  -50.0f, 1.0f, //-6 -- prawa
	50.0f,  0.0f,  50.0f,  1.0f,
	50.0f,  -4.0f,  50.0f,  1.0f,
	-50.0f, -4.0f,  50.0f, 1.0f, //-7 --
	50.0f,  1.0f,  50.0f,  1.0f,
	-50.0f, -4.0f,  50.0f, 1.0f,
	-50.0f, 0.0f,   50.0f, 1.0f, //-8 -- przód
	-50.0f, 0.0f,   50.0f, 1.0f,
	-50.0f, -4.0f,  50.0f,  1.0f,
	-50.0f, 0.0f,  -50.0f, 1.0f, //-9
	-50.0f, 0.0f,  -50.0f, 1.0f,
	-50.0f, -4.0f,  -50.0f,  1.0f,
	-50.0f, -4.0f,  50.0f, 1.0f, //-10 lewa
	-50.0f,  0.0f,  -50.0f,  1.0f,
	50.0f, 0.0f,  -50.0f,  1.0f,
	-50.0f, -4.0f,  -50.0f, 1.0f, //-11
	-50.0f,  -4.0f,  -50.0f,  1.0f,
	50.0f, -4.0f,   -50.0f,  1.0f,
	50.0f, 0.0f,   -50.0f, 1.0f, //-12 tył
};

float baseNormals[] = {
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,//--
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,//--
	0.0f, -1.0f, 0.0f, 0.0f,
	0.0f, -1.0f, 0.0f, 0.0f,
	0.0f, -1.0f, 0.0f, 0.0f,//--
	0.0f, -1.0f, 0.0f, 0.0f,
	0.0f, -1.0f, 0.0f, 0.0f,
	0.0f, -1.0f, 0.0f, 0.0f,//--
	1.0f, 0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 0.0f,	
	1.0f, 0.0f, 0.0f, 0.0f,//--
	1.0f, 0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 0.0f,//--
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,//--
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,//--
	-1.0f, 0.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f, 0.0f,//--
	-1.0f, 0.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f, 0.0f,//--
	0.0f, 0.0f, -1.0f, 0.0f,
	0.0f, 0.0f, -1.0f, 0.0f,
	0.0f, 0.0f, -1.0f, 0.0f,//--
	0.0f, 0.0f, -1.0f, 0.0f,
	0.0f, 0.0f, -1.0f, 0.0f,
	0.0f, 0.0f, -1.0f, 0.0f,//--
};

float baseTexCoords[] {
	0.0f,1.0f, 
	1.0f,1.0f, 
	0.0f,0.0f, //-
	1.0f,1.0f, 
	0.0f,0.0f, 
	1.0f,0.0f, //-
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,//-
	0.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,//-
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,//-
	0.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,//-
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,//-
	0.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,//-
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,//-
	0.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,//-
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,//-
	0.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,//-
};

struct MeshToLoad
{
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec4> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<unsigned int> indices;
	objl::Material material;
	GLuint kaLoadedMap;
	GLuint kdLoadedMap;
};

GLuint tex0;
GLuint tex1;
GLuint tex2;

//objl::Loader mazdaLoader;
OBJModel objModel;

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}


void keyCallback(GLFWwindow* window,int key,int scancode,int action,int mods) {
    if (action==GLFW_PRESS) {
		if (key == GLFW_KEY_SPACE) car->startBreak();
		if (key == GLFW_KEY_UP) car->goForward();
		if (key == GLFW_KEY_DOWN) car->goBackWards();
        if (key == GLFW_KEY_LEFT) car->turnLeft();
        if (key == GLFW_KEY_RIGHT) car->turnRight();
    }
    if (action==GLFW_RELEASE) {
		if (key == GLFW_KEY_SPACE) car->stopBreaking();
		if (key == GLFW_KEY_UP) car->stopGoingForward();
		if (key == GLFW_KEY_DOWN) car->stopGoingBackward();
        if (key == GLFW_KEY_LEFT) car->stopTurning();
        if (key == GLFW_KEY_RIGHT) car->stopTurning();
    }
}

void windowResizeCallback(GLFWwindow* window,int width,int height) {
    if (height==0) return;
    aspectRatio=(float)width/(float)height;
    glViewport(0,0,width,height);
}

GLuint readTexture(const char* filename) {
    GLuint tex;
    glActiveTexture(GL_TEXTURE0);

    //Wczytanie do pamięci komputera
    std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
    unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
    //Wczytaj obrazek
    unsigned error = lodepng::decode(image, width, height, filename);

    //Import do pamięci karty graficznej
    glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
    glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
    //Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return tex;
}

std::vector<MeshToLoad> getCarMeshesToLoad()
{
	loader.LoadFile("Car-Model/3dcar/Torque Twister/Torque Twister.obj");
	std::vector<MeshToLoad> meshes;
	for (int i = 0; i < loader.LoadedMeshes.size(); i++)
	{
		MeshToLoad meshToLoad;
		std::vector<glm::vec4> vertices;
		std::vector<glm::vec4> normals;
		std::vector<glm::vec2> texCoords;

		objl::Mesh mesh = loader.LoadedMeshes[i];
		for (int j = 0; j < mesh.Vertices.size(); j++)
		{
			objl::Vertex vertex = mesh.Vertices[j];
			meshToLoad.vertices.push_back(glm::vec4(vertex.Position.X, vertex.Position.Y, vertex.Position.Z, 1.0f));
			meshToLoad.normals.push_back(glm::vec4(vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z, 0.0f));
			meshToLoad.texCoords.push_back(glm::vec2(vertex.TextureCoordinate.X, vertex.TextureCoordinate.Y));
		}
		meshToLoad.indices = mesh.Indices;
		meshToLoad.material = mesh.MeshMaterial;
		std::cout<<meshToLoad.material.name<<"\n";
		meshToLoad.kaLoadedMap = readTexture(meshToLoad.material.map_Ka.c_str());
		meshToLoad.kdLoadedMap = readTexture(meshToLoad.material.map_Kd.c_str());
		meshes.push_back(meshToLoad);
	}
	return meshes;
}

std::vector<MeshToLoad> carMeshes;

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window,windowResizeCallback);
	glfwSetKeyCallback(window,keyCallback);

	sp=new ShaderProgram("v_simplest.glsl",NULL,"f_simplest.glsl");
	car = new MyCar(40.0f, 60.0f, glm::vec3(-34,0,-28), false, 0.5f, 40.0f);
	enemyCar = new MyCar(20.0f, 1.0f, glm::vec3(-34, 0, -28), true, 0.5f, 20.0f);
	enemyCar1 = new MyCar(15.0f, 1.0f, glm::vec3(-34, 0, -28), true, 1.0f, 10.0f);
	enemyCar2 = new MyCar(5.0f, 1.0f, glm::vec3(-34, 0, -28), true, 1.6f, 5.0f);
	tex0 = readTexture("tor2.png");
	tex1 = readTexture("metal_spec.png");
	tex2 = readTexture("tiger.png");
	carMeshes = getCarMeshesToLoad();
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
    delete sp;
}

void drawCar(MyCar* car)
{
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(car->getModelMatrix()));
	glEnableVertexAttribArray(sp->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, myTeapotVertices); //Wskaż tablicę z danymi dla atrybutu vertex

	glEnableVertexAttribArray(sp->a("color"));  //Włącz przesyłanie danych do atrybutu color
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, myTeapotColors); //Wskaż tablicę z danymi dla atrybutu color

	glEnableVertexAttribArray(sp->a("normal"));  //Włącz przesyłanie danych do atrybutu normal
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, myTeapotVertexNormals); //Wskaż tablicę z danymi dla atrybutu normal

	glEnableVertexAttribArray(sp->a("texCoord0"));  //Włącz przesyłanie danych do atrybutu texCoord
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, myTeapotTexCoords); //Wskaż tablicę z danymi dla atrybutu texCoord

	glUniform1i(sp->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex2);

	glUniform1i(sp->u("textureMap1"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount); //Narysuj obiekt

	glDisableVertexAttribArray(sp->a("vertex"));  //Wyłącz przesyłanie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("color"));  //Wyłącz przesyłanie danych do atrybutu color
	glDisableVertexAttribArray(sp->a("normal"));  //Wyłącz przesyłanie danych do atrybutu normal
	glDisableVertexAttribArray(sp->a("texCoord0"));  //Wyłącz przesyłanie danych do atrybutu texCoord0
}

void drawCar1(MyCar* car)
{
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec4> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<unsigned int> indices;
/*		
	for (int i = 0; i < loader.LoadedVertices.size(); i++)
	{
		objl::Vertex vertex = loader.LoadedVertices[i];
		vertices.push_back(glm::vec4(vertex.Position.X, vertex.Position.Y, vertex.Position.Z, 1.0f));
		normals.push_back(glm::vec4(vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z, 0.0f));
		texCoords.push_back(glm::vec2(vertex.TextureCoordinate.X, vertex.TextureCoordinate.Y));
	}
	for (int i = 0; i < loader.LoadedMaterials.size(); i++)
	{
		objl::Material material = loader.LoadedMaterials[i];
	}
*/

	glm::mat4 M = glm::mat4(1.0f);
	M = glm::scale(M, glm::vec3(20.0f));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	for (int i = 0; i < carMeshes.size(); i++)
	{
		MeshToLoad mesh = carMeshes[i];
		objl::Material material = mesh.material;

		float specular[] = {material.Ks.X, material.Ks.Y, material.Ks.Z, 0.0f};
		float diffuse[] = {material.Kd.X, material.Kd.Y, material.Kd.Z, 0.0f};
		float ambient[] = {material.Ka.X, material.Ka.Y, material.Ka.Z, 0.0f};

		//diffuse - jasne przy źródle
		//specular - osbicia od obiektu
		//ambient - minimalna jasnośc obiektu

		glEnableVertexAttribArray(sp->u("specular"));
		glVertexAttribPointer(sp->u("specular"), 4, GL_FLOAT, false, 0, specular);

		glEnableVertexAttribArray(sp->u("diffuse"));
		glVertexAttribPointer(sp->u("diffuse"), 4, GL_FLOAT, false, 0, diffuse);

		glEnableVertexAttribArray(sp->u("ambient"));
		glVertexAttribPointer(sp->u("ambient"), 4, GL_FLOAT, false, 0, ambient);

		glEnableVertexAttribArray(sp->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
		glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, mesh.vertices.data()); //Wskaż tablicę z danymi dla atrybutu vertex

		glEnableVertexAttribArray(sp->a("normal"));  //Włącz przesyłanie danych do atrybutu normal
		glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, mesh.normals.data()); //Wskaż tablicę z danymi dla atrybutu normal

		glEnableVertexAttribArray(sp->a("texCoord0"));  //Włącz przesyłanie danych do atrybutu texCoord
		glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, mesh.texCoords.data()); //Wskaż tablicę z danymi dla atrybutu texCoord

		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, mesh.indices.data());

		glUniform1i(sp->u("textureMap0"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh.kaLoadedMap);

		glUniform1i(sp->u("textureMap1"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mesh.kdLoadedMap);

		glDisableVertexAttribArray(sp->a("vertex"));  //Wyłącz przesyłanie danych do atrybutu vertex
		glDisableVertexAttribArray(sp->a("normal"));  //Wyłącz przesyłanie danych do atrybutu normal
		glDisableVertexAttribArray(sp->a("texCoord0"));  //Wyłącz przesyłanie danych do atrybutu texCoord0
		glDisableVertexAttribArray(sp->a("specular"));  //Wyłącz przesyłanie danych do atrybutu vertex
		glDisableVertexAttribArray(sp->a("diffuse"));  //Wyłącz przesyłanie danych do atrybutu normal
		glDisableVertexAttribArray(sp->a("ambient"));  //Wyłącz przesyłanie danych do atrybutu texCoord0
	}

//	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(car->getModelMatrix()));
	//glm::mat4 M = glm::mat4(1.0f);
	//M = glm::scale(M, glm::vec3(20.0f));
	//glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	//glEnableVertexAttribArray(sp->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
	//glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, vertices.data()); //Wskaż tablicę z danymi dla atrybutu vertex

	//glEnableVertexAttribArray(sp->a("normal"));  //Włącz przesyłanie danych do atrybutu normal
	//glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals.data()); //Wskaż tablicę z danymi dla atrybutu normal

	//glEnableVertexAttribArray(sp->a("texCoord0"));  //Włącz przesyłanie danych do atrybutu texCoord
	//glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoords.data()); //Wskaż tablicę z danymi dla atrybutu texCoord

	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

	//glDisableVertexAttribArray(sp->a("vertex"));  //Wyłącz przesyłanie danych do atrybutu vertex
	//glDisableVertexAttribArray(sp->a("normal"));  //Wyłącz przesyłanie danych do atrybutu normal
	//glDisableVertexAttribArray(sp->a("texCoord0"));  //Wyłącz przesyłanie danych do atrybutu texCoord0
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window){//,float actual_angle_x, float actual_x, float actual_z, glm::vec3 position) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 M = glm::mat4(1.0f);
	glm::mat4 V = glm::mat4(1.0f);
	V = glm::lookAt(
		glm::vec3(0, 100, -5),
		glm::vec3(0, 0, 0), 
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 P = glm::mat4(1.0f);
	P = glm::perspective(
			100 * PI / 180.0f, // ustawiamy FOV
			aspectRatio,
			0.01f,
			300.0f); //Wylicz macierz rzutowania

    sp->use();//Aktywacja programu cieniującego
    //Przeslij parametry programu cieniującego do karty graficznej

	// -- rysuj plansze
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(car->getCameraMatrix()));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(sp->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, base); //Wskaż tablicę z danymi dla atrybutu vertex

	glEnableVertexAttribArray(sp->a("normal"));  //Włącz przesyłanie danych do atrybutu normal
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, baseNormals); //Wskaż tablicę z danymi dla atrybutu normal

	glEnableVertexAttribArray(sp->a("texCoord0"));  //Włącz przesyłanie danych do atrybutu texCoord
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, baseTexCoords); //Wskaż tablicę z danymi dla atrybutu texCoord

	glUniform1i(sp->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex0);

	glUniform1i(sp->u("textureMap1"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex1);

	glDrawArrays(GL_TRIANGLES, 0, 12*3); //Narysuj obiekt
	glDisableVertexAttribArray(sp->a("vertex"));  //Wyłącz przesyłanie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("normal"));  //Wyłącz przesyłanie danych do atrybutu normal
	glDisableVertexAttribArray(sp->a("texCoord0"));  //Wyłącz przesyłanie danych do atrybutu texCoord0
	// -- rysuj plansze

	drawCar1(car);
	drawCar(enemyCar);
	drawCar(enemyCar1);
	drawCar(enemyCar2);
    glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	//Główna pętla
	glm::vec3 position = glm::vec3(1.0f, 0.5f, 1.0f);
	glfwSetTime(0); //Zeruj timer
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		car->updatePosition(glfwGetTime());
		enemyCar->goAutonomous(glfwGetTime());
		enemyCar1->goAutonomous(glfwGetTime());
		enemyCar2->goAutonomous(glfwGetTime());

		/*car->hasColision(enemyCar);
		car->hasColision(enemyCar1);
		car->hasColision(enemyCar2);
		enemyCar2->hasColision(car);
		enemyCar1->hasColision(car);
		enemyCar2->hasColision(enemyCar1);
		enemyCar1->hasColision(enemyCar2);*/

        glfwSetTime(0); //Zeruj timer
		drawScene(window); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
