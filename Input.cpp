#include "Input.h"

IDirectInput8* Kujaku::Input::directInput_ = nullptr;
IDirectInputDevice8* Kujaku::Input::keyboard_ = nullptr;
IDirectInputDevice8* Kujaku::Input::mouse_ = nullptr;

BYTE Kujaku::Input::key_[256] = {};
BYTE Kujaku::Input::preKey_[256] = {};

DIMOUSESTATE2 Kujaku::Input::mouseState_ = {};
DIMOUSESTATE2 Kujaku::Input::preMouseState_ = {};

HWND Kujaku::Input::hwnd_;

void Kujaku::Input::Init(HRESULT hr, WNDCLASS wc, HWND hwnd) {
	hwnd_ = hwnd;

	// DirectInputの初期化
	hr = DirectInput8Create(wc.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, nullptr);
	assert(SUCCEEDED(hr));

	// キーボードデバイスの生成
	hr = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(hr));

	// 入力データ形式のセット
	hr = keyboard_->SetDataFormat(&c_dfDIKeyboard); // 標準形式
	assert(SUCCEEDED(hr));

	// 排他制御レベルのセット
	hr = keyboard_->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));

	// マウスデバイスの生成
	hr = directInput_->CreateDevice(GUID_SysMouse, &mouse_, NULL);
	assert(SUCCEEDED(hr));

	// 入力データ形式セット
	hr = mouse_->SetDataFormat(&c_dfDIMouse2);

	// 排他制御レベルのセット
	mouse_->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
}

void Kujaku::Input::Update() {
	// キーボード情報の取得開始
	keyboard_->Acquire();

	// 全キーの入力状態を取得する
	memcpy(preKey_, key_, 256);
	keyboard_->GetDeviceState(sizeof(key_), key_);

	// マウス情報の取得開始
	mouse_->Acquire();

	// 全マウスの入力状態を取得する
	preMouseState_ = mouseState_;
	mouse_->GetDeviceState(sizeof(mouseState_), &mouseState_);
}

Vector2 Kujaku::Input::GetMousePos() {
	POINT mousePoint;
	// マウスカーソルのスクリーン座標を取得
	GetCursorPos(&mousePoint);

	// スクリーン座標を指定のウィンドウのクライアント領域での座標に変換
	ScreenToClient(hwnd_, &mousePoint);

	return Vector2{static_cast<float>(mousePoint.x), static_cast<float>(mousePoint.y)};
}
