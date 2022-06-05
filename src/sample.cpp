#include "sample.h"
#include "mat4.h"
#include "quat.h"
#include "uniform.h"
#include "Draw.h"

void Sample::Initialize() {
	mRotation = 0.0f;
	mShader = new Shader("Shaders/static.vert", "Shaders/lit.frag");
	mDisplayTexture = new Texture("Assets/uv.png");

	mVertexPositions = new Attribute<vec3>();
	mVertexNormals = new Attribute<vec3>();
	mVertexTexCoords = new Attribute<vec2>();
	mIndexBuffer = new IndexBuffer();

	std::vector<vec3> positions;
	positions.push_back(vec3(-1, -1, 0));
	positions.push_back(vec3(-1, 1, 0));
	positions.push_back(vec3(1, -1, 0));
	positions.push_back(vec3(1, 1, 0));
	mVertexPositions->Set(positions);

	std::vector<vec3> normals;
	normals.resize(4, vec3(0, 0, 1));
	mVertexNormals->Set(normals);

	std::vector<vec2> uvs;
	uvs.push_back(vec2(0, 0));
	uvs.push_back(vec2(0, 1));
	uvs.push_back(vec2(1, 0));
	uvs.push_back(vec2(1, 1));
	mVertexTexCoords->Set(uvs);

	std::vector<unsigned int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);
	mIndexBuffer->Set(indices);
}

void Sample::Update(float inDeltaTime) {
	mRotation += inDeltaTime * 45.0f;
	while (mRotation > 360.0f) {
		mRotation -= 360.0f;
	}
}

void Sample::Render(float inAspectRatio) {
	mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
	mat4 view = lookAt(vec3(0, 0, -5), vec3(0, 0, 0), vec3(0, 1, 0));
	mat4 model = quatToMat4(angle_axis(mRotation * DEG2RAD, vec3(0, 0, 1)));

	mShader->bind();

	mVertexPositions->BindTo(mShader->get_attribute("position"));
	mVertexNormals->BindTo(mShader->get_attribute("normal"));
	mVertexTexCoords->BindTo(mShader->get_attribute("texCoord"));

	Uniform<mat4>::Set(mShader->get_uniform("model"), model);
	Uniform<mat4>::Set(mShader->get_uniform("view"), view);
	Uniform<mat4>::Set(mShader->get_uniform("projection"), projection);

	Uniform<vec3>::Set(mShader->get_uniform("light"), vec3(0, 0, 1));

	mDisplayTexture->Set(mShader->get_uniform("tex0"), 0);

	Draw(*mIndexBuffer, DrawMode::Triangles);

	mDisplayTexture->UnSet(0);

	mVertexPositions->UnBindFrom(mShader->get_attribute("position"));
	mVertexNormals->UnBindFrom(mShader->get_attribute("normal"));
	mVertexTexCoords->UnBindFrom(mShader->get_attribute("texCoord"));

	mShader->unbind();
}

void Sample::Shutdown() {
	delete mShader;
	delete mDisplayTexture;
	delete mVertexPositions;
	delete mVertexNormals;
	delete mVertexTexCoords;
	delete mIndexBuffer;
}