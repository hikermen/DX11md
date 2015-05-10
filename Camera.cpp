#include "Camera.h"

Camera::Camera(int width, int height) :
m_fFovAngle(DirectX::XM_PI / 5.0f),
m_fAspect((float)width / (float)height),
m_fNearZ(0.01f),
m_fFarZ(500.0f)
{
	// �J�����r���[�s��
	m_matView = DirectX::XMMatrixIdentity();

	// �ˉe�s��
	m_matProj = DirectX::XMMatrixPerspectiveFovLH(m_fFovAngle, m_fAspect, m_fNearZ, m_fFarZ);

	// ���ˉe�s��
	m_matOrtho = DirectX::XMMatrixOrthographicLH((float)300, (float)300, 1.0f, 800.0f);

	// ���s�����̈ʒu�x�N�g��
	m_vecLightPos = DirectX::XMFLOAT4(20.0f, 200.0f, 300.0f, 0);
}

Camera::~Camera()
{

}



//�f�o�b�O�p�J����
DebugCamera::~DebugCamera()
{

}

void DebugCamera::Update()
{
	// �r���[�s��
	if (GetKeyState(VK_UP) & 0x8000)
		m_matView *= DirectX::XMMatrixTranslation(0, 0, -0.1f);
	if (GetKeyState(VK_DOWN) & 0x8000)
		m_matView *= DirectX::XMMatrixTranslation(0, 0, 0.1f);
	if (GetKeyState(VK_RIGHT) & 0x8000)
		m_matView *= DirectX::XMMatrixRotationY(-0.002f);
	if (GetKeyState(VK_LEFT) & 0x8000)
		m_matView *= DirectX::XMMatrixRotationY(0.002f);
	if (GetKeyState('Q') & 0x8000)
		m_vecLightPos = DirectX::XMFLOAT4(20.0f, 200.0f, 300.0f, 0);
	if (GetKeyState('A') & 0x8000)
		m_vecLightPos = DirectX::XMFLOAT4(20.0f, 200.0f, -300.0f, 0);

}