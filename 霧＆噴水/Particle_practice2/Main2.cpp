#include<DxLib.h>
#include<math.h>

const int WindowsWidth = 600;
const int WindowsHeight = 400;

const int arrayNum = 800;

const float PI = 3.141592f;

const char TITLE[] = "Fountain";

struct Vector2 {
	float x;
	float y;
};

struct Particle {
	Vector2 position;
	Vector2 velocity;
	float speed;
	int life;
	int scale;
	int textureColor;
	int drawColor;
	int Red, Blue, Green;
};

Particle particle[arrayNum];
Vector2 particlePosition{ WindowsWidth / 2, WindowsHeight };
Vector2 mouse_pos;

int defaultTexture;
int time = 0;
bool isBumped = true;
bool R = false;
bool G = false;
bool B = true;

int LoadTexture(TCHAR* name)
{
	int tex = LoadGraph(name);

	if (tex == -1) {
		MessageBox(NULL, "画像の読み込みに失敗しました", ERROR, MB_OK);
		return 0;
	}
}

void InitializeParticle(Particle& particle, bool R, bool G, bool B)
{
	if (isBumped)
	{
		int RedColor = 40;
		int BlueColor = 40;
		int GreenColor = 40;

		particle.position.x = particlePosition.x;
		particle.position.y = particlePosition.y;
		if (time % 2 != 0)
		{
			particle.velocity.x = rand() % 12 * 1;
		}
		if (time % 2 == 0)
		{
			particle.velocity.x = rand() % 12 * -1;
		}
		particle.velocity.y = -15;

		particle.speed = (float)(rand() % 3) / 4;
		particle.life = rand() % 200;
		particle.scale = (float)(rand() % 100 + 25) / 100;
		particle.textureColor = defaultTexture;
		
		if (R) {
			RedColor = 255;
		}
		if (B) {
			BlueColor = 255;
		}
		if (G) {
			GreenColor = 255;
		}

		particle.Red = RedColor;
		particle.Blue = BlueColor;
		particle.Green = GreenColor;
	}
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
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

	if (DxLib_Init() == -1)
	{
		return -1;
	}

	SetDrawBlendMode(DX_BLENDMODE_PMA_ADD, 200);

	for (int i = 0; i < arrayNum; i++)
	{
		InitializeParticle(particle[i], R, G, B);
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
		
		DrawFormatString(0, 16, GetColor(255, 255, 255), "COLOR : R G B");

		SetDrawBright(0x0A, 0xE0, 0xBA);		

		time++;

		if (isBumped)
		{
			
			for (int i = 0; i < arrayNum; i++)
			{
				particle[i].position.x += particle[i].velocity.x * particle[i].speed;
				particle[i].position.y += particle[i].velocity.y * particle[i].speed;
				particle[i].life--;

				if (particle[i].position.y < WindowsHeight / 2)
				{
					particle[i].velocity.y++;
				}

				//初期化
				if (particle[i].life < 0) {
					InitializeParticle(particle[i], R, G, B);
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

		if (!prev[KEY_INPUT_R] && keys[KEY_INPUT_R])
		{
			R = !R;
		}
		if (!prev[KEY_INPUT_G] && keys[KEY_INPUT_G])
		{
			G = !G;			
		}
		if (!prev[KEY_INPUT_B] && keys[KEY_INPUT_B])
		{
			B = !B;
		}
		//if (!prev[KEY_INPUT_W] && keys[KEY_INPUT_W])
		//{
		//	changeColor = 0;
		//}
		//if (!prev[KEY_INPUT_T] && keys[KEY_INPUT_T])
		//{
		//	changeColor = 5;
		//}		

		switch (blendMode)
		{
		case 1:
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);//αブレンド
			break;
		case 2:
			SetDrawBlendMode(DX_BLENDMODE_INVSRC, 150);//反転ブレンド
			break;
		case 3:
			SetDrawBlendMode(DX_BLENDMODE_PMA_ADD, 150);//加算ブレンド
			break;
		case 4:
			SetDrawBlendMode(DX_BLENDMODE_SUB, 150);//減算ブレンド
			break;
		case 0:
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 150);//ブレンド無
			break;
		default:
			break;
		}

		for (int i = 0; i < arrayNum; i++)
		{

			SetDrawBright(particle[i].Red, particle[i].Green, particle[i].Blue);

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