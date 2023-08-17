#include <Novice.h>
#include"mt.h"
#include "ImGuiManager.h"
const char kWindowTitle[] = "学籍番号";
const int kWindowWidth = 1280;
const int kWindowHeight = 720;


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };

	Sphere sphere1
	{
		{0.0f, 0.0f, 0.0f},
		1.0f
	};

	Sphere sphere2
	{
		{0.0f, 0.0f, 0.0f},
		0.5f
	};

	uint32_t colorS1 = WHITE;
	uint32_t colorS2 = WHITE;
	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		Matrix4x4 cameraMatrix =MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix =MakeViewPortMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		if (IsCollision(sphere1, sphere2))
		{
			colorS1 = RED;
		}
		else
		{
			colorS1 = WHITE;
		}
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawShere(sphere1, worldViewProjectionMatrix, viewportMatrix, colorS1);
		DrawShere(sphere2, worldViewProjectionMatrix, viewportMatrix, colorS2);
		DrawGrid(worldViewProjectionMatrix, viewportMatrix);

		ImGui::Begin("Debug");
		ImGui::DragFloat3("sphere1", &sphere1.center.x, 0.1f, -1.0f, 1.0f);
		ImGui::DragFloat("sphere1", &sphere1.radius, 0.1f, 0.0f, 1.0f);
		ImGui::DragFloat3("sphere2", &sphere2.center.x, 0.1f, -1.0f, 1.0f);
		ImGui::DragFloat("sphere2", &sphere2.radius, 0.1f, 0.0f, 1.0f);

		ImGui::End();
		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
