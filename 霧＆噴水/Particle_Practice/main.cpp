#include<DxLib.h>
#include<math.h>
const int WindowsWidth = 600;
const int WindowsHeight = 400;

const int arrayNum = 600;

const float PI = 3.141592f;

const char TITLE[] = "Mist";

struct Vector2 {
	float x;
	float y;
};

struct Particle {
	Vector2 position;
	Vector2 velocity;
	float speed;
	int life;
	float scale;
	int textureColor;
	int drawColor;
};

Particle particle[arrayNum];
Vector2 particlePosition{WindowsWidth / 2, WindowsHeight / 2};
Vector2 mouse_pos;

int defaultTexture;
int time = 0;
bool isBumped = true;

int LoadTexture(TCHAR* name)
{
	int tex = LoadGraph(name);

	if (tex == -1) {
		MessageBox(NULL, "画像の読み込みに失敗しました", ERROR, MB_OK);
		return 0;
	}
}

void InitializeParticle(Particle& particle, int changeColor)
{
	if (isBumped)
	{
		particle.position.x = particlePosition.x + rand() % 30 - 20;
		particle.position.y = particlePosition.y + rand() % 30 - 20;
		if (time % 3 == 0)
		{
			particle.velocity.x = rand() % 15 * 1;
		}
		if (time % 2 == 0)
		{
			particle.velocity.x = rand() % 15 * -1;
		}
		if(time % 2 == 0)
		{ 
			particle.velocity.y = rand() % 15 * 1;
		}
		if (time % 4 == 0)
		{
			particle.velocity.y = rand() % 15 * -1;
		}
		
		particle.speed = (float)(rand() % 3) / 20;
		particle.life = rand() % 80;
		particle.scale = 0.5f;
		particle.textureColor = defaultTexture;
		particle.drawColor = changeColor;
	}
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
	ChangeWindowMode(TRUE);

	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	SetGraphMode(WindowsWidth, WindowsHeight, 32);

	SetMainWindowText(TITLE);

	SetWindowSizeExtendRate(0, 1);

	SetBackgroundColor(0x00, 0x00, 0x00);

	char keys[256];

	char prev[256];

	int changeColor = 5;

	int blendMode = 3;

	int alphaValue = 150;

	if (DxLib_Init() == -1)
	{
		return -1;
	}

	SetDrawBlendMode(DX_BLENDMODE_PMA_ADD, 150);

	for (int i = 0; i < arrayNum; i++)
	{
		InitializeParticle(particle[i], changeColor);
	}

	defaultTexture = LoadTexture((TCHAR*)"../Texture/particle.jpg");

	while (1)
	{
		ClearDrawScreen();

		memcpy(prev, keys, sizeof(keys));

		GetHitKeyStateAll(keys);

		SetDrawScreen(DX_SCREEN_BACK);		

		SetDrawBright(255, 255, 255);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 150);

		DrawLine(0, WindowsHeight / 2, WindowsWidth, WindowsHeight / 2, GetColor(255, 255, 255));

		DrawLine(WindowsWidth / 2, 0, WindowsWidth / 2, WindowsHeight, GetColor(255, 255, 255));

		DrawFormatString(0, 0, GetColor(255, 255, 255), "1 : αブレンド　2 : 反転　3 : 加算　4 ： 減算　0 ： ブレンド無");

		DrawFormatString(0, 16, GetColor(255, 255, 255), "COLOR : R G B W T");

		SetDrawBright(0x0A, 0xE0, 0xBA);

		time++;

		

		if (isBumped)
		{
			//パーティクル更新
			for (int i = 0; i < arrayNum; i++)
			{
				particle[i].position.x += particle[i].velocity.x * particle[i].speed;
				particle[i].position.y += particle[i].velocity.y * particle[i].speed;
				particle[i].life--;
				particle[i].scale+= 0.01f;

				//初期化
				if (particle[i].life < 0) {
					InitializeParticle(particle[i], changeColor);				
				}
			}
		}

		if ((GetMouseInput() && MOUSE_INPUT_LEFT) != 0)
		{
			//マウス処理
			int mouse_x = (int)mouse_pos.x;
			int mouse_y = (int)mouse_pos.y;
			GetMousePoint(&mouse_x, &mouse_y);
			mouse_pos.x = mouse_x;
			mouse_pos.y = mouse_y;

			particlePosition.x = mouse_pos.x;
			particlePosition.y = mouse_pos.y;
		}

		if (!prev[KEY_INPUT_1] && keys[KEY_INPUT_1])
		{
			blendMode = 1;
		}
		if (!prev[KEY_INPUT_2] && keys[KEY_INPUT_2])
		{
			blendMode = 2;
		}
		if (!prev[KEY_INPUT_3] && keys[KEY_INPUT_3])
		{
			blendMode = 3;
		}
		if (!prev[KEY_INPUT_4] && keys[KEY_INPUT_4])
		{
			blendMode = 4;
		}
		if (!prev[KEY_INPUT_0] && keys[KEY_INPUT_0])
		{
			blendMode = 0;
		}

		for (int i = 0; i < arrayNum; i++)
		{
			if (!prev[KEY_INPUT_R] && keys[KEY_INPUT_R])
			{
				changeColor = 1;
			}
			if (!prev[KEY_INPUT_G] && keys[KEY_INPUT_G])
			{
				changeColor = 2;

			}
			if (!prev[KEY_INPUT_B] && keys[KEY_INPUT_B])
			{
				changeColor = 3;
			}
			if (!prev[KEY_INPUT_W] && keys[KEY_INPUT_W])
			{
				changeColor = 0;
			}
			if (!prev[KEY_INPUT_T] && keys[KEY_INPUT_T])
			{
				changeColor = 5;
			}
		}

		switch (blendMode)
		{
		case 1:
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaValue);//αブレンド
			break;
		case 2:
			SetDrawBlendMode(DX_BLENDMODE_INVSRC, alphaValue);//反転ブレンド
			break;
		case 3:
			SetDrawBlendMode(DX_BLENDMODE_PMA_ADD, alphaValue);//加算ブレンド
			break;
		case 4:
			SetDrawBlendMode(DX_BLENDMODE_SUB, alphaValue);//減算ブレンド
			break;
		case 0:
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, alphaValue);//ブレンド無
			break;
		default:
			break;
		}

		for (int i = 0; i < arrayNum; i++)
		{
			switch (particle[i].drawColor)
			{
			case 1:
				SetDrawBright(255, 0, 0);
				break;
			case 2:
				SetDrawBright(0, 255, 0);
				break;
			case 3:
				SetDrawBright(0, 0, 255);
				break;
			case 0:
				SetDrawBright(255, 255, 255);
				break;
			case 5:
				SetDrawBright(0x0A, 0xE0, 0xBA);
				break;
			default:
				break;
			}

			DrawRotaGraph2(
				particle[i].position.x,
				particle[i].position.y,
				40,
				40,
				particle[i].scale,
				0.0f,
				particle[i].textureColor,
				FALSE,
				FALSE);
		}

		ScreenFlip();

		WaitTimer(20);

		if (ProcessMessage() == -1) break;

		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;
	}

	DxLib_End();
	return 0;
}