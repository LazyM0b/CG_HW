#include "GameComponent.h"

GameComponent::GameComponent() {}

void GameComponent::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, MeshTypes type) {
	this->type = type;
	float phi = (1.0f + sqrt(5.0f)) * 0.5f; // golden ratio
	float coord1 = 1.0f;
	float coord2 = 1.0f / phi;
	int numSubdivide;

	switch (type)
	{
	case Triangle:
		pointsCnt = 3;

		points = {
		{Vector4(1.0f, 1.0f, 0.5f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f)},
		{Vector4(-1.0f, -1.0f, 0.5f, 1.0f),Vector4(1.0f, 1.0f, 1.0f, 1.0f)},
		{Vector4(-1.0f, 1.0f, 0.5f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f)},
		};
		indeces = { 0,1,2 };
		break;
	case Square:
		pointsCnt = 4;

		points = {
		{Vector4(1.0f, 1.0f, 0.5f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f)},
		{Vector4(-1.0f, -1.0f, 0.5f, 1.0f),Vector4(1.0f, 1.0f, 1.0f, 1.0f)},
		{Vector4(1.0f, -1.0f, 0.5f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f)},
		{Vector4(-1.0f, 1.0f, 0.5f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f)},
		};
		indeces = { 0,1,2, 1,0,3 };
		break;
	case Circle:
		pointsCnt = 100;

		points = { { Vector4(0.0f, 0.0f, 0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) } };
		for (int i = 1; i < pointsCnt; ++i) {
			points.push_back(Vertex());
			points[i].location = Vector4(cos((i - 1) / (pointsCnt - 2.0f) * 6.28f), sin((i - 1) / (pointsCnt - 2.0f) * 6.28f), 0.5f, 1.0f);
			points[i].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			indeces.push_back(0);
			indeces.push_back((i + 1) % pointsCnt);
			indeces.push_back(i);
		}
	case Cube:
		pointsCnt = 8;
		points = {
			{Vector4(-1.0f, 1.0f, -1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)},
			{Vector4(1.0f, 1.0f, -1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)},
			{Vector4(-1.0f, -1.0f, -1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)},
			{Vector4(1.0f, -1.0f, -1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)},
			{Vector4(-1.0f, 1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)},
			{Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)},
			{Vector4(-1.0f, -1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)},
			{Vector4(1.0f, -1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)}
		};
		indeces = {
			0,1,2, 1,3,2,
			0,5,1, 0,4,5,
			1,5,7, 1,7,3,
			0,6,4, 0,2,6,
			4,6,5, 5,6,7,
			2,3,6, 3,7,6
		};
	break; 
	case Sphere:

		pointsCnt = 12;
		numSubdivide = 3;

		points =
		{
			{Vector4(0.0f, coord2, -coord1, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)},
			{Vector4(coord2, coord1, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)},
			{Vector4(-coord2, coord1, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)},
			{Vector4(0.0f, coord2, coord1, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)},
			{Vector4(0.0f, -coord2, coord1, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)},
			{Vector4(-coord1, 0.0f, coord2, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)},
			{Vector4(0.0f, -coord2, -coord1, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)},
			{Vector4(coord1, 0.0f, -coord2, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)},
			{Vector4(coord1, 0.0f, coord2, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)},
			{Vector4(-coord1, 0.0f, -coord2, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)},
			{Vector4(coord2, -coord1, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)},
			{Vector4(-coord2, -coord1, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)}
		};		indeces =
		{
		2,1,0, 1,2,3, 5,4,3, 4,8,3, 7,6,0,
		6,9,0, 11,10,4, 10,11,6, 9,5,2, 5,9,11,
		8,7,1, 7,8,10, 2,5,3, 8,1,3, 9,2,0,
		1,7,0, 11,9,6, 7,10,6, 5,11,4, 10,8,4
		};		for (int j = 0; j < numSubdivide; ++j) {			SphereSubdivide(points, indeces);			pointsCnt = points.size();			for (int i = 0; i < pointsCnt; ++i)				PointNormalize(points[i]);		}
		break;
	default:
		break;
	}

	boxCollider = DirectX::BoundingBox(Vector3::Zero, Vector3::One);
	TriangleComponent::Initialize(device);
}

void GameComponent::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, MeshTypes type, std::vector<Vector4> colors, UINT detailsLVL) {
	this->type = type; 
	float phi = (1.0f + sqrt(5.0f)) * 0.5f; // golden ratio
	float coord1 = 1.0f;
	float coord2 = 1.0f / phi;
	int numSubdivide;

	switch (type)
	{
	case Triangle:
		pointsCnt = 3;

		points = {
		{Vector4(1.0f, 1.0f, 0.5f, 1.0f),	colors[0]},
		{Vector4(-1.0f, -1.0f, 0.5f, 1.0f),colors[1]},
		{Vector4(-1.0f, 1.0f, 0.5f, 1.0f),	colors[2]},
		};
		indeces = { 0,1,2 };
		break;
	case Square:
		pointsCnt = 4;

		points = {
		{Vector4(1.0f, 1.0f, 0.5f, 1.0f),	colors[0]},
		{Vector4(-1.0f, -1.0f, 0.5f, 1.0f),colors[1]},
		{Vector4(1.0f, -1.0f, 0.5f, 1.0f),	colors[2]},
		{Vector4(-1.0f, 1.0f, 0.5f, 1.0f),	colors[3]},
		};
		indeces = { 0,1,2, 1,0,3 };
		break;
	case Circle:
		pointsCnt = 100;

		points = { { Vector4(0.0f, 0.0f, 0.5f, 1.0f), colors[0] } };
		for (int i = 1; i < pointsCnt; ++i) {
			points.push_back(Vertex());
			points[i].location = Vector4(cos((i - 1) / (pointsCnt - 2.0f) * 6.28f), sin((i - 1) / (pointsCnt - 2.0f) * 6.28f), 0.5f, 1.0f);
			points[i].color = colors[i];
			indeces.push_back(0);
			indeces.push_back((i + 1) % pointsCnt);
			indeces.push_back(i);
		}
		break;
	case Cube:
		pointsCnt = 8;
		points = {
			{Vector4(-1.0f, 1.0f, -1.0f, 1.0f), colors[0]},
			{Vector4(1.0f, 1.0f, -1.0f, 1.0f), colors[1]},
			{Vector4(-1.0f, -1.0f, -1.0f, 1.0f), colors[2]},
			{Vector4(1.0f, -1.0f, -1.0f, 1.0f), colors[3]},
			{Vector4(-1.0f, 1.0f, 1.0f, 1.0f), colors[4]},
			{Vector4(1.0f, 1.0f, 1.0f, 1.0f), colors[5]},
			{Vector4(-1.0f, -1.0f, 1.0f, 1.0f), colors[6]},
			{Vector4(1.0f, -1.0f, 1.0f, 1.0f), colors[7]}
		};
		indeces = {
			0,1,2, 1,3,2,
			0,5,1, 0,4,5,
			1,5,7, 1,7,3,
			0,6,4, 0,2,6,
			4,6,5, 5,6,7,
			2,3,6, 3,7,6
		};
		break;
	case Sphere:

		pointsCnt = 12;
		numSubdivide = detailsLVL;

		points =
		{
			{Vector4(0.0f, coord2, -coord1, 1.0f), colors[0] },
			{Vector4(coord2, coord1, 0.0f, 1.0f), colors[1] },
			{Vector4(-coord2, coord1, 0.0f, 1.0f), colors[0] + colors[1]},
			{Vector4(0.0f, coord2, coord1, 1.0f), colors[1] + colors[0]},
			{Vector4(0.0f, -coord2, coord1, 1.0f), colors[0] },
			{Vector4(-coord1, 0.0f, coord2, 1.0f), colors[1]},
			{Vector4(0.0f, -coord2, -coord1, 1.0f), colors[0] - colors[1]},
			{Vector4(coord1, 0.0f, -coord2, 1.0f), colors[1] - colors[0]},
			{Vector4(coord1, 0.0f, coord2, 1.0f), colors[0]},
			{Vector4(-coord1, 0.0f, -coord2, 1.0f), colors[1] }, 
			{Vector4(coord2, -coord1, 0.0f, 1.0f), colors[0] + colors[1]},
			{Vector4(-coord2, -coord1, 0.0f, 1.0f), colors[1] - colors[0]}
		};		indeces =
		{
		2,1,0, 1,2,3, 5,4,3, 4,8,3, 7,6,0,
		6,9,0, 11,10,4, 10,11,6, 9,5,2, 5,9,11,
		8,7,1, 7,8,10, 2,5,3, 8,1,3, 9,2,0,
		1,7,0, 11,9,6, 7,10,6, 5,11,4, 10,8,4
		};		for (int j = 0; j < numSubdivide; ++j) {			SphereSubdivide(points, indeces);			pointsCnt = points.size();			for (int i = 0; i < pointsCnt; ++i)				PointNormalize(points[i]);		}
		break;
	default:
		break;
	}

	boxCollider = DirectX::BoundingBox(Vector3::Zero, Vector3::One);
	sphereCollider = DirectX::BoundingSphere(Vector3::Zero, 1.0f);
	TriangleComponent::Initialize(device);
}


void GameComponent::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, MeshTypes type, const std::string& filePath)
{
	this->type = type;

	/*if (!this->LoadModel(filePath))
		return;*/
	//this->points = vertices;
	//this->indeces = indeces;

	pointsCnt = points.size();

	sphereCollider = DirectX::BoundingSphere(Vector3::Zero, 1.0f);
	TriangleComponent::Initialize(device);
}

void GameComponent::Draw(ID3D11DeviceContext* context) {
	TriangleComponent::Draw(context);
}

void GameComponent::Reload() {
	scale = Vector3::One;
	rotation = rotation.CreateFromYawPitchRoll(Vector3(0.0f, 0.0f, 0.0f));
	translation = Vector3::Zero;
}

void GameComponent::SphereSubdivide(std::vector<Vertex>& points, std::vector<int>& indeces)
{
	std::vector<Vertex> pointsTmp;
	std::vector<int> indecesTmp;
	int startIndex = points.size();
	int index1 = -1, index2 = -1, index3 = -1, indTmp = -1;
	int curSize = points.size();

	for (auto point : points)
		pointsTmp.push_back(point);

	for (int i = 0; i < indeces.size() / 3; ++i) {

		Vertex point1 = { findCenter(points[indeces[i * 3]].location, points[indeces[i * 3 + 1]].location), points[indeces[i * 3 + 2]].color };
		//Vertex point1 = { findCenter(points[indeces[i * 3]].location, points[indeces[i * 3 + 1]].location), points[1].color };
		index1 = curSize;

		indTmp = CheckForUnique(points, point1, startIndex);

		if (indTmp == -1) {
			pointsTmp.push_back(point1);
			++curSize;
		}
		else
			index1 = indTmp;

		Vertex point2 = { findCenter(points[indeces[i * 3]].location, points[indeces[i * 3 + 2]].location), points[indeces[i * 3 + 1]].color };
		//Vertex point2 = { findCenter(points[indeces[i * 3]].location, points[indeces[i * 3 + 2]].location), points[1].color }; 
		index2 = curSize;

		indTmp = CheckForUnique(points, point2, startIndex);

		if (indTmp == -1) {
			pointsTmp.push_back(point2);
			++curSize;
		}
		else
			index2 = indTmp;

		Vertex point3 = { findCenter(points[indeces[i * 3 + 1]].location, points[indeces[i * 3 + 2]].location), points[indeces[i * 3]].color };
		//Vertex point3 = { findCenter(points[indeces[i * 3 + 1]].location, points[indeces[i * 3 + 2]].location), points[0].color };
		index3 = curSize;

		indTmp = CheckForUnique(points, point3, startIndex);

		if (indTmp == -1) {
			pointsTmp.push_back(point3);
			++curSize;
		}
		else
			index3 = indTmp;


		indecesTmp.push_back(indeces[i * 3]);
		indecesTmp.push_back(index1);
		indecesTmp.push_back(index2);

		indecesTmp.push_back(indeces[i * 3 + 1]);
		indecesTmp.push_back(index3);
		indecesTmp.push_back(index1);

		indecesTmp.push_back(indeces[i * 3 + 2]);
		indecesTmp.push_back(index2);
		indecesTmp.push_back(index3);

		indecesTmp.push_back(index1);
		indecesTmp.push_back(index3);
		indecesTmp.push_back(index2);
	}
	points.clear();
	indeces.clear();
	points = pointsTmp;
	indeces = indecesTmp;
}

Vector4 GameComponent::findCenter(const Vector4& point1, const Vector4& point2) {
	return Vector4((point1.x + point2.x) / 2.0f, (point1.y + point2.y) / 2.0f, (point1.z + point2.z) / 2.0f, 1.0f);
}

int GameComponent::CheckForUnique(const std::vector<Vertex>& points, Vertex pointNew, int startInd) {
	for (int i = startInd; i < points.size(); ++i) {
		if (points[i].location.x == pointNew.location.x && points[i].location.y == pointNew.location.y && points[i].location.z == pointNew.location.z)
			return i;
	}
	return -1;
}


//bool GameComponent::LoadModel(const std::string& filePath)
//{
//	Assimp::Importer importer;
//
//	const aiScene* pScene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
//
//	if (pScene == NULL)
//		return false;
//
//	this->ProcessNode(pScene->mRootNode, pScene);
//
//	return true;
//}
//
//
//void GameComponent::ProcessNode(aiNode* node, const aiScene* scene)
//{
//	for (int i = 0; i < node->mNumMeshes; ++i) {
//		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//		meshes.push_back(this->ProcessMesh(mesh, scene));
//	}
//
//	for (int i = 0; i < node->mNumChildren; ++i)
//		this->ProcessNode(node->mChildren[i], scene);
//}
//
//
//TriangleComponent* GameComponent::ProcessMesh(aiMesh* mesh, const aiScene* scene)
//{
//	TriangleComponent* object = new TriangleComponent();
//	for (int i = 0; i < mesh->mNumVertices; ++i) {
//		Vertex vertex;
//		vertex.location.x = mesh->mVertices[i].x;
//		vertex.location.y = mesh->mVertices[i].y;
//		vertex.location.z = mesh->mVertices[i].z;
//		vertex.location.w = 1.0f;
//
//		if (mesh->mTextureCoords[0]) {
//			vertex.texCoord.x = (float)mesh->mTextureCoords[0][i].x;
//			vertex.texCoord.y = (float)mesh->mTextureCoords[0][i].y;
//		}
//
//		object->points.push_back(vertex);
//	}
//
//	for (int i = 0; i < mesh->mNumFaces; ++i) {
//		aiFace face = mesh->mFaces[i];
//
//		for (int j = 0; j < face.mNumIndices; ++j)
//			object->indeces.push_back(face.mIndices[j]);
//	}
//
//	return object;
//}

void GameComponent::PointNormalize(Vertex& point)
{	float tmp = std::sqrt(std::pow(point.location.x, 2) + std::pow(point.location.y, 2) + std::pow(point.location.z, 2));	point.location.x /= tmp;	point.location.y /= tmp;	point.location.z /= tmp;
}
