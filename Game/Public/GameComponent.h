#pragma once

#include "TriangleComponent.h"
#include "DirectXCollision.h"
#include "SimpleMath.h"
#include "DisplayWin32.h"
#include "CameraManager.h"
#include <d3d11.h>
#include <wrl.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/material.h>	
#include <WICTextureLoader.h>

using namespace DirectX::SimpleMath;

class GameComponent: public TriangleComponent {
public:

	GameComponent();
	GameComponent(Microsoft::WRL::ComPtr<ID3D11Device> device, const GameComponent& other);
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, MeshTypes type);
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, MeshTypes type, std::vector<Vector4> colors, UINT detailsLVL);
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, MeshTypes type, const std::string& modelPath, const wchar_t* texturePath);
	void Draw(ID3D11DeviceContext* context, CameraManager* camManager);
	void Reload();
	void Update();
	void DestroyResources();
	void PointNormalize(Vertex& point);
	void SphereSubdivide(std::vector<Vertex>& points, std::vector<int>& indeces);
	Vector4 findCenter(const Vector4& point1, const Vector4& point2);
	int CheckForUnique(const std::vector<Vertex>& points, Vertex pointNew, int startInd);
	
	bool LoadModel(const std::string& filePath);
	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);

	bool LoadTexture(Microsoft::WRL::ComPtr<ID3D11Device> device, const wchar_t* texturePath);
	//ID3D11ShaderResourceView** GetSRV() { return &m_texture; };

	bool collisionEnabled = false;
	bool isMovable = false;
	bool isPlayerControllable = false;
	UINT pointsCnt;
	DirectX::BoundingBox boxCollider;
	DirectX::BoundingSphere sphereCollider;
	float speed = 0.0f;
	float speedMax = 0.0f;
	float period;
	float distanceToParent;
	Vector3 scale;
	Quaternion rotation;
	Vector3 translation;
	Vector3 impulse;
	Vector3 velocity;
	Matrix positionW;
	Matrix positionL;
	Microsoft::WRL::ComPtr <ID3D11Resource> resource;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	GameComponent* parent;
};