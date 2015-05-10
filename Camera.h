#ifndef CAMERA_H
#define CAMERA_H

#include "Common.h"
#include <DirectXMath.h>

//カメラクラス
//光源をこいつが持つべきか要検討
class Camera
{
protected:

	FLOAT m_fFovAngle;
	FLOAT m_fAspect;
	FLOAT m_fNearZ;
	FLOAT m_fFarZ;

	DirectX::XMMATRIX m_matView;
	DirectX::XMMATRIX m_matProj;
	DirectX::XMMATRIX m_matOrtho;

	DirectX::XMFLOAT4 m_vecLightPos;

public:

	Camera(int width, int height);
	virtual ~Camera();

	inline DirectX::XMMATRIX* GetView(){ return &m_matView; }
	inline DirectX::XMMATRIX* GetProjection(){ return &m_matProj; }
	inline DirectX::XMMATRIX* GetOrtho(){ return &m_matOrtho; }

	inline DirectX::XMFLOAT4* GetVecLightPos(){ return &m_vecLightPos; }

	inline void SetView(DirectX::XMMATRIX* matView){ m_matView = *matView; }
	inline void SetProjection(DirectX::XMMATRIX* matProj){ m_matProj = *matProj; }
	inline void SetOrtho(DirectX::XMMATRIX* matOrtho){ m_matOrtho = *matOrtho; }

	inline void SetVecLightPos(DirectX::XMFLOAT4* vecLightPos){ m_vecLightPos = *vecLightPos; }

};


//デバッグ用カメラ
class DebugCamera : public Camera
{
public:
	DebugCamera(int width, int height) : Camera(width, height){}
	virtual ~DebugCamera();

	void Update();

};


#endif