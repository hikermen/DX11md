#ifndef COMMON_H
#define COMMON_F

//���ʂ̃w�b�_

#pragma warning (disable: 4238)

#define _XM_NO_INTRINSICS_

#include <windows.h>
#include <tchar.h>
#include <memory>

//timeGetTime�p
#include <mmsystem.h>
#pragma comment (lib, "winmm.lib")

//���������[�N���̏o�͗p
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#define NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define NEW new
#endif

//����������p�}�N��
#define SAFE_RELEASE(x) if(x != nullptr){x->Release(); x = nullptr;}
#define SAFE_DELETE(x)	if(x != nullptr){delete x; x = nullptr;}
#define SAFE_DELETE_ARRAY(x) if(x != nullptr){delete[] x; x = nullptr;}
#define SAFE_FREE(x) if(x != nullptr){free(x); x = nullptr;}


#endif
