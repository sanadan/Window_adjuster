///
///	@fn		WA_Hook.h
///	@brief	�t�b�NDLL�G���g��
///
//	2007/04/19	�^�c	�V�K�쐬
//

#pragma once

// �ȉ��� ifdef �u���b�N�� DLL ����̃G�N�X�|�[�g��e�Ղɂ���}�N�����쐬���邽�߂� 
// ��ʓI�ȕ��@�ł��B���� DLL ���̂��ׂẴt�@�C���́A�R�}���h ���C���Œ�`���ꂽ WA_HOOK_EXPORTS
// �V���{���ŃR���p�C������܂��B���̃V���{���́A���� DLL ���g���v���W�F�N�g�Œ�`���邱�Ƃ͂ł��܂���B
// �\�[�X�t�@�C�������̃t�@�C�����܂�ł��鑼�̃v���W�F�N�g�́A 
// WA_HOOK_API �֐��� DLL ����C���|�[�g���ꂽ�ƌ��Ȃ��̂ɑ΂��A���� DLL �́A���̃}�N���Œ�`���ꂽ
// �V���{�����G�N�X�|�[�g���ꂽ�ƌ��Ȃ��܂��B
#ifdef WA_HOOK_EXPORTS
#define WA_HOOK_API __declspec(dllexport)
#else
#define WA_HOOK_API __declspec(dllimport)
#endif

WA_HOOK_API HRESULT WA_Enable_hook() ;
WA_HOOK_API HRESULT WA_Disable_hook() ;

// [[[[ End of this header ]]]]]