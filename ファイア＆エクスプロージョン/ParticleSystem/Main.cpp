#include <DxLib.h>
#include <math.h>

const int windowWidth = 600;
const int windowHeight = 400;

const char TITLE[] = "Fire & Explosion";

const int arrayNum = 500;//パーティクルの数

const float PI = 3.141592f;

struct Vector2 {
	float x;
	float y;
};

struct Particle {
	Vector2 position;
	Vector2 velocity;
	float speed;
	float textureScale;
	int life;
	int texureColor;
};

char keys[256], prev_keys[256];

Particle particle[arrayNum];

Vector2 particle_pos = { windowWidth / 2, windowHeight / 2 };
Vector2 mouse_pos;

int init_tex;//初期化する色
int w_tex;
int r_tex;
int g_tex;
int b_tex;

int time = 0;
bool isFired = true;
bool isCentralized = false;

int currentClick, previousClick;

//画像読み込み関数
int LoadTexture(TCHAR* name)
{
	int tex = LoadGraph(name);

	if (tex == -1) {
		MessageBox(NULL, "画像の読み込みに失敗しました", ERROR, MB_OK);
		return 0;
	}
}

void InitParticle(Particle& particle) {

	if (!isFired) {
		particle.position = particle_pos;
		particle.velocity.x = cos((double)(rand() % 360) / 180 * PI);
		particle.velocity.y = sin((double)(rand() % 360) / 180 * PI);
		particle.speed = rand() % 8 + 5;
		particle.life = rand() % 100;
		particle.textureScale = (float)(rand() % 100) / 100;
		if (rand() % 20 == 0)
		{
			particle.textureScale = (float)(rand() % 80) / 100;
			particle.texureColor = w_tex;
		}
		else particle.texureColor = init_tex;
	}
	else {
		particle.position.x = particle_pos.x + rand() % 40 - 20;
		particle.position.y = particle_pos.y + rand() % 40 - 20;
		particle.velocity.x = 0.0f;
		particle.velocity.y = rand() % 6 * -1;
		particle.speed = (float)(rand() % 5) / 10;
		particle.life = rand() % 80;
		particle.textureScale = (float)(rand() % 100 + 25) / 100;
		if (rand() % 30 == 0)
		{
			particle.textureScale = (float)(rand() % 50) / 100;
			particle.texureColor = w_tex;
		}
		else particle.texureColor = init_tex;
	}
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLin,
	_In_ int nCmdShow
)
{
	//ウィンドウモードに設定
	ChangeWindowMode(TRUE);

	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	SetMainWindowText(TITLE);
	
	SetGraphMode(windowWidth, windowHeight, 32);

	SetWindowSizeExtendRate(1, 0);

	SetBackgroundColor(0, 0, 0);

	if (DxLib_Init() == -1)
	{
		return -1;
	}

	SetDrawBlendMode(DX_BLENDMODE_PMA_ADD, 150);

	for (int i = 0; i < arrayNum; i++)
	{
		InitParticle(particle[i]);
	}

	w_tex = LoadTexture((TCHAR*)"../Texture/particle.jpg");
	r_tex = LoadTexture((TCHAR*)"../Texture/particle_red.bmp");
	g_tex = LoadTexture((TCHAR*)"../Texture/particle_green.bmp");
	b_tex = LoadTexture((TCHAR*)"../Texture/particle_blue.bmp");

	init_tex = r_tex;

	while (1)
	{
		if (!isCentralized)
		{
			//パーティクル更新
			for (int i = 0; i < arrayNum; i++)
			{
				particle[i].position.x += particle[i].velocity.x * particle[i].speed;
				particle[i].position.y += particle[i].velocity.y * particle[i].speed;
				particle[i].life--;

				//初期化
				if (particle[i].life < 0) {
					InitParticle(particle[i]);
				}
			}
		}
		if (isCentralized) {
			for (int i = 0; i < arrayNum; i++)
			{
				particle[i].position.x -= particle[i].velocity.x * particle[i].speed;
				particle[i].position.y -= particle[i].velocity.y * particle[i].speed;
				particle[i].life--;

				//初期化
				if (particle[i].life < 0) {
					InitParticle(particle[i]);
				}
			}
		}

		memcpy(prev_keys, keys, sizeof(keys));

		GetHitKeyStateAll(keys);

		//previousClick = currentClick; //One Frame Click
		//currentClick = GetMouseInput();
		//if (currentClick && MOUSE_INPUT_LEFT && !previousClick && MOUSE_INPUT_LEFT)
		//{
		//	マウス処理
		//	int mouse_x = (int)mouse_pos.x;
		//	int mouse_y = (int)mouse_pos.y;
		//	GetMousePoint(&mouse_x, &mouse_y);
		//	mouse_pos.x = mouse_x;
		//	mouse_pos.y = mouse_y;
		//	particle_pos.x = mouse_pos.x;
		//	particle_pos.y = mouse_pos.y;
		//	isFired = !isFired;
		//}

		//描画位置を変更
		if ((GetMouseInput() && MOUSE_INPUT_LEFT) != 0)
		{
			//マウス処理
			int mouse_x = (int)mouse_pos.x;
			int mouse_y = (int)mouse_pos.y;
			GetMousePoint(&mouse_x, &mouse_y);
			mouse_pos.x = mouse_x;
			mouse_pos.y = mouse_y;

			particle_pos.x = mouse_pos.x;
			particle_pos.y = mouse_pos.y;
		}

		if (!prev_keys[KEY_INPUT_SPACE] && keys[KEY_INPUT_SPACE])
		{
			isFired = !isFired;
		}
		if (!prev_keys[KEY_INPUT_SPACE] && keys[KEY_INPUT_SPACE] && !isFired)
		{
			isCentralized = true;
		}
		if (!prev_keys[KEY_INPUT_SPACE] && keys[KEY_INPUT_SPACE] && isFired)
		{
			isCentralized = false;
		}
		

		//ブレンド設定
		if (!prev_keys[KEY_INPUT_1] && keys[KEY_INPUT_1])
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);//αブレンド
		}
		if (!prev_keys[KEY_INPUT_2] && keys[KEY_INPUT_2])
		{
			SetDrawBlendMode(DX_BLENDMODE_INVSRC, 150);//反転ブレンド
		}
		if (!prev_keys[KEY_INPUT_3] && keys[KEY_INPUT_3])
		{
			SetDrawBlendMode(DX_BLENDMODE_PMA_ADD, 150);//加算ブレンド
		}
		if (!prev_keys[KEY_INPUT_4] && keys[KEY_INPUT_4])
		{
			SetDrawBlendMode(DX_BLENDMODE_SUB, 150);//減算ブレンド
		}
		if (!prev_keys[KEY_INPUT_0] && keys[KEY_INPUT_0])
		{
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 150);//ブレンド無
		}

		if (!prev_keys[KEY_INPUT_W] && keys[KEY_INPUT_W])
		{
			init_tex = w_tex;//白
		}
		if (!prev_keys[KEY_INPUT_R] && keys[KEY_INPUT_R])
		{
			init_tex = r_tex;//赤
		}
		if (!prev_keys[KEY_INPUT_G] && keys[KEY_INPUT_G])
		{
			init_tex = g_tex;//緑
		}
		if (!prev_keys[KEY_INPUT_B] && keys[KEY_INPUT_B])
		{
			init_tex = b_tex;//青
		}

		ClearDrawScreen();

		DrawLine(0, windowHeight / 2, windowWidth, windowHeight / 2, GetColor(255, 255, 255));

		DrawLine(windowWidth / 2, 0, windowWidth / 2, windowHeight, GetColor(255, 255, 255));

		//操作説明
		DrawFormatString(0, 0, GetColor(255, 255, 255), "1 : αブレンド　2 : 反転　3 : 加算　4 ： 減算　0 ： ブレンド無");
		
		DrawFormatString(0, 16, GetColor(255, 255, 255), "Blend_Mode");
		
		DrawFormatString(0, 32, GetColor(255, 255, 255), "COLOR : R G B W");

		if (!isFired)
		{
			DrawFormatString(0, 48, GetColor(255, 255, 255), "SPACE : EXPLOSION");
		}
		else
		{
			DrawFormatString(0, 48, GetColor(255, 255, 255), "SPACE : CENTRALIZE");
		}

		//パーティクル描画
		for (int i = 0; i < arrayNum; i++)
		{
			DrawRotaGraph2(
				particle[i].position.x,
				particle[i].position.y,
				40,
				40,
				particle[i].textureScale,
				0.0f,
				particle[i].texureColor,
				FALSE,
				FALSE);
		}

		WaitTimer(20);

		if (ProcessMessage() == -1) break;

		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;
	}

	//Dxライブラリ終了処理
	DxLib_End();

	return 0;
}
