//ヘッダーファイル読み込み
#include "Dxlib.h"
#include "math.h"

//コンパイル時の警告表示抑制
#pragma warning( disable:4305 4244 4305 )

void Game(void); //ゲーム本体
void Menu(void); //メニュー
void Result(void); //リザルト
float Root(int,int,int,int);

//マクロ
#define PI 3.141592 //円周率
#define RIGHT CheckHitKey(KEY_INPUT_RIGHT) //右
#define LEFT CheckHitKey(KEY_INPUT_LEFT) //左
#define UP CheckHitKey(KEY_INPUT_UP) //上
#define DOWN CheckHitKey(KEY_INPUT_DOWN) //下
#define CHECK CheckHitKey(KEY_INPUT_RETURN) //決定
#define SHOOT CheckHitKey(KEY_INPUT_SPACE) //発砲
#define PAUSE CheckHitKey(KEY_INPUT_BACK) //ポーズ

//汎用変数宣言
int set = 0; //選択肢用変数
int keywait = 0; //入力待機(処理まで一定時間待つ用)
int cf; //クリアフラグ -1=NULL 0=FALSE 1=TRUE
int nextf = 0; //処理待機変数
int damhit; //被弾数
int nohit; //着弾数
int skincolor[10] = {0xff0000,0xffc0cb,0xffd700,0xadff2f,0x008000,0x0000ff,0x800080,0xffa500,0x808080,0xffffff}; //自機色(赤・橙・黄金・黄緑・緑・青・紫・桃・灰・白)
int bulletcolor[5] = {0xff00ff,0x00ff00,0x00ffff,0xffff00,0xffffff}; //弾色(マゼンタ・ライム・シアン・黄・白)
int cl = 0, bl = 0; //色番号
char es[3][50] = { {"Ｅ Ａ Ｓ Ｙ" }, {"Ｎ Ｏ Ｍ Ａ Ｌ"}, {"Ｈ Ａ Ｒ Ｄ"} }; //難易度
char em[3][50] = { {"Ｌ Ｏ Ｗ"},{"Ｎ Ｏ Ｍ Ａ Ｌ"},{"Ｈ Ｉ Ｇ Ｈ"} }; //敵機移動速度
int esl = 1, emove = 1; //敵機難易度・速度
int FONT;
int BGM1, BGM2, BGM3, BGM4;
int SE1, SE2, SE3, SE4, SE5, SE6, SE7, SE8, SE9;

//タイトル画面(メインループ)
int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nC)
{
	SetMainWindowText("Assalt BlitZ");
	SetWindowIconID(102);
	SetGraphMode(1120, 630, 32);		//画面サイズ設定(どれかひとつを選択)
	ChangeWindowMode(TRUE);				//ウィンドウモードで起動、コメントアウトでフルスクリーン
	if (DxLib_Init() == -1) return -1;	//DXライブラリ初期化
	SetDrawScreen(DX_SCREEN_BACK);		//裏画面に描画

	FONT = CreateFontToHandle("PixelMplus10-Regular.ttf", 24, 1);
	BGM1 = LoadSoundMem("BGM/Title.mp3");
	BGM2 = LoadSoundMem("BGM/Battle.mp3");
	BGM3 = LoadSoundMem("BGM/GameClear.mp3");
	BGM4 = LoadSoundMem("BGM/GameOver.mp3");
	SE1 = LoadSoundMem("SE/bomb.mp3");
	SE2 = LoadSoundMem("SE/fail.mp3");
	SE3 = LoadSoundMem("SE/hit.mp3");
	SE4 = LoadSoundMem("SE/poka.mp3");
	SE5 = LoadSoundMem("SE/powerup1.mp3");
	SE6 = LoadSoundMem("SE/powerup2.mp3");
	SE7 = LoadSoundMem("SE/select.mp3");
	SE8 = LoadSoundMem("SE/shoot.mp3");
	SE9 = LoadSoundMem("SE/pause.wav");

	int Font = CreateFontToHandle("PixelMplus10-Regular.ttf", 9, 4);

	PlaySoundMem(BGM1, DX_PLAYTYPE_LOOP);
	//メインループ・タイトル画面
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)	//OSから終了指示が来るか、ESCキーを押されるまで繰り返し
	{
		nextf++;
		keywait++;
		ClearDrawScreen();	//描画画面（裏画面）を黒でクリア

		//上下キーで選択
		if (UP == 1 && keywait >= 10) {
			PlaySoundMem(SE7, DX_PLAYTYPE_BACK);
			set--;
			keywait = 0;
			if (set < 1)set = 3;
		}
		if (DOWN == 1 && keywait >= 10) {
			PlaySoundMem(SE7, DX_PLAYTYPE_BACK);
			set++;
			keywait = 0;
			if (set > 3)set = 1;
		}

		DrawBox(190, 100, 890, 300, 0xffffff, TRUE);
		DrawBox(200, 110, 880, 290, 0x000000, TRUE);
		DrawBox(203, 113, 877, 287, 0xffffff, TRUE);
		DrawExtendFormatStringToHandle(202, 98, 10.0, 10.0, 0x880015, Font, "ＡＳＳＡＵＬＴ\n  ＢＬＩＴＺ");
		DrawExtendFormatStringToHandle(198, 102, 10.0, 10.0, 0xFFC90E, Font, "ＡＳＳＡＵＬＴ\n  ＢＬＩＴＺ");

		if (set == 1) DrawExtendFormatStringToHandle(400, 400, 2.0, 2.0, 0xffffff, Font, "─ Ｇ Ａ Ｍ Ｅ　Ｓ Ｔ Ａ Ｒ Ｔ─");
		else DrawExtendFormatStringToHandle(400, 400, 2.0, 2.0, 0xffffff, Font, "　 Ｇ Ａ Ｍ Ｅ　Ｓ Ｔ Ａ Ｒ Ｔ");
		if (set == 2) DrawExtendFormatStringToHandle(460, 450, 2.0, 2.0, 0xffffff, Font, "─ Ｏ Ｐ Ｔ Ｉ Ｏ Ｎ ─");
		else DrawExtendFormatStringToHandle(460, 450, 2.0, 2.0, 0xffffff, Font, "　 Ｏ Ｐ Ｔ Ｉ Ｏ Ｎ");
		if (set == 3) DrawExtendFormatStringToHandle(400, 500, 2.0, 2.0, 0xffffff, Font, "─ Ｇ Ａ Ｍ Ｅ　Ｃ Ｌ Ｏ Ｓ Ｅ ─");
		else DrawExtendFormatStringToHandle(400, 500, 2.0, 2.0, 0xffffff, Font, "　 Ｇ Ａ Ｍ Ｅ　Ｃ Ｌ Ｏ Ｓ Ｅ");
		//エンターキーを押して決定
		if (CheckHitKey(KEY_INPUT_RETURN) == 1) {
			PlaySoundMem(SE4, DX_PLAYTYPE_BACK);
			switch (set)
			{
			case 1:
				set = 0; //ゲーム開始
				StopSoundMem(BGM1);
				Game();
				break;
			case 2:
				set = 0; //メニュー
				Menu();
				break;
			case 3:
				DxLib_End(); //ゲーム終了
				break;
			}

		}

		if (nextf > 100) nextf = 0;
		if (nextf < 50) DrawExtendFormatStringToHandle(400, 600, 2.0, 2.0, 0xffffff, Font, "Ｐ Ｕ Ｓ Ｈ　Ｔ Ｏ　Ｅ Ｎ Ｔ Ｅ Ｒ");

		ScreenFlip();	//表と裏（描画画面）の入れ換え

	}


	//DXライブラリ終了処理
	DxLib_End();

	return 0;

}

//メニュー画面
void Menu(void)
{
	//変数宣言
	int Font1 = CreateFontToHandle("PixelMplus10-Regular.ttf", 12, 9);
	int Font2 = CreateFontToHandle("PixelMplus10-Regular.ttf", 16, 9);
	int Font3 = CreateFontToHandle("PixelMplus10-Regular.ttf", 9, 4);

	int y1 = -30, y2 = -30, y3 = -30; //アニメーション変数
	int selectf = 0; //操作フラグ
	int opset = 3;

	//色々初期化
	set = 1;
	keywait = 0;

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)	//OSから終了指示が来るか、ESCキーを押されるまで繰り返し
	{
		nextf++;
		keywait++;
		ClearDrawScreen();	//描画画面（裏画面）を黒でクリア

		//左右キーでページ移動
		if (LEFT == 1 && keywait >= 10 && opset == 3) {
			PlaySoundMem(SE7, DX_PLAYTYPE_BACK);
			set--;
			keywait = 0;
			if (set < 1)set = 3;
		}
		if (RIGHT == 1 && keywait >= 10 && opset == 3) {
			PlaySoundMem(SE7, DX_PLAYTYPE_BACK);
			set++;
			keywait = 0;
			if (set > 3)set = 1;
		}
		
		//操作方法説明
		if (set == 1) {
			//上下キーで設定箇所を選択
			if (UP == 1 && keywait >= 10) {
				PlaySoundMem(SE7, DX_PLAYTYPE_BACK);
				opset--;
				keywait = 0;
				if (opset < 3)opset = 4;
			}
			if (DOWN == 1 && keywait >= 10) {
				PlaySoundMem(SE7, DX_PLAYTYPE_BACK);
				opset++;
				keywait = 0;
				if (opset > 4)opset = 3;
			}
			DrawExtendFormatStringToHandle(300, 50, 2.0, 2.0, 0xffffff, Font1, "Ｇ Ａ Ｍ Ｅ　Ｏ Ｐ Ｅ Ｒ Ａ Ｔ Ｉ Ｏ Ｎ");

			DrawExtendFormatStringToHandle(150, 160, 2.0, 2.0, 0xffffff, Font2, "Ａ Ｃ Ｔ Ｉ Ｏ Ｎ ： ←／→ Ｋ Ｅ Ｙ");
			DrawExtendFormatStringToHandle(150, 240, 2.0, 2.0, 0xffffff, Font2, "Ｓ Ｈ Ｏ Ｏ Ｔ ： Ｓ Ｐ Ａ Ｓ Ｅ　Ｋ Ｅ Ｙ");
			DrawExtendFormatStringToHandle(150, 320, 2.0, 2.0, 0xffffff, Font2, "Ｐ Ａ Ｕ Ｓ Ｅ ： Ｂ Ａ Ｃ Ｋ　Ｋ Ｅ Ｙ");
		}
		//敵機設定
		else if (set == 2) {
			//上下キーで設定箇所を選択
			if (UP == 1 && keywait >= 10) {
				PlaySoundMem(SE7, DX_PLAYTYPE_BACK);
				opset--;
				keywait = 0;
				if (opset < 1)opset = 4;
			}
			if (DOWN == 1 && keywait >= 10) {
				PlaySoundMem(SE7, DX_PLAYTYPE_BACK);
				opset++;
				keywait = 0;
				if (opset > 4)opset = 1;
			}
			//左右キーで設定変更(敵機難易度)
			if (LEFT == 1 && keywait >= 10 && opset == 1) {
				PlaySoundMem(SE7, DX_PLAYTYPE_BACK);
				esl--;
				keywait = 0;
				if (esl < 0)esl = 2;
			}
			if (RIGHT == 1 && keywait >= 10 && opset == 1) {
				PlaySoundMem(SE7, DX_PLAYTYPE_BACK);
				esl++;
				keywait = 0;
				if (esl > 2)esl = 0;
			}
			//左右キーで設定変更(敵機移動速度)
			if (LEFT == 1 && keywait >= 10 && opset == 2) {
				PlaySoundMem(SE7, DX_PLAYTYPE_BACK);
				emove--;
				keywait = 0;
				if (emove < 0) emove = 2;
			}
			if (RIGHT == 1 && keywait >= 10 && opset == 2) {
				PlaySoundMem(SE7, DX_PLAYTYPE_BACK);
				emove++;
				keywait = 0;
				if (emove > 2) emove = 0;
			}

			//画面描画
			DrawExtendFormatStringToHandle(300, 50, 2.0, 2.0, 0xffffff, Font1, "Ｃ Ｏ Ｎ Ｆ Ｉ Ｇ Ｕ Ｒ Ａ Ｔ Ｉ Ｏ Ｎ");

			DrawExtendFormatStringToHandle(100, 120, 2.0, 2.0, 0xffffff, Font2, "Ｅ Ｎ Ｅ Ｍ Ｙ　Ｓ Ｅ Ｔ");
			if (opset == 1) DrawExtendFormatStringToHandle(140, 210, 2.0, 2.0, 0xffffff, Font2, "Ｓ Ｔ Ｒ Ｅ Ｎ Ｇ Ｔ Ｈ : ←%s→", es[esl]);
			else DrawExtendFormatStringToHandle(150, 230, 2.0, 2.0, 0xffffff, Font2, "Ｓ Ｔ Ｒ Ｅ Ｎ Ｇ Ｔ Ｈ : 　%s　", es[esl]);
			if (opset == 2) DrawExtendFormatStringToHandle(140, 300, 2.0, 2.0, 0xffffff, Font2, "Ｓ Ｐ Ｅ Ｅ Ｄ ： ←%s→", em[emove]);
			else DrawExtendFormatStringToHandle(150, 300, 2.0, 2.0, 0xffffff, Font2, "Ｓ Ｐ Ｅ Ｅ Ｄ ： 　%s　", em[emove]);


		}
		//自機設定
		else if (set == 3) {
			//上下キーで設定個所を選択
			if (UP == 1 && keywait >= 10) {
				PlaySoundMem(SE7, DX_PLAYTYPE_BACK);
				opset--;
				keywait = 0;
				if (opset < 1)opset = 4;
			}
			if (DOWN == 1 && keywait >= 10) {
				PlaySoundMem(SE7, DX_PLAYTYPE_BACK);
				opset++;
				keywait = 0;
				if (opset > 4)opset = 1;
			}
			//左右キーで設定変更(自機色)
			if (LEFT == 1 && keywait >= 10 && opset == 1) {
				PlaySoundMem(SE7, DX_PLAYTYPE_BACK);
				cl--;
				keywait = 0;
				if (cl < 0) cl = 9;
			}
			if (RIGHT == 1 && keywait >= 10 && opset == 1) {
				PlaySoundMem(SE7, DX_PLAYTYPE_BACK);
				cl++;
				keywait = 0;
				if (cl > 9) cl = 0;
			}
			//左右キーで設定変更(弾色)
			if (LEFT == 1 && keywait >= 10 && opset == 2) {
				PlaySoundMem(SE7, DX_PLAYTYPE_BACK);
				bl--;
				keywait = 0;
				if (bl < 0) bl = 4;
			}
			if (RIGHT == 1 && keywait >= 10 && opset == 2) {
				PlaySoundMem(SE7, DX_PLAYTYPE_BACK);
				bl++;
				keywait = 0;
				if (bl > 4) bl = 0;
			}

			//画面描画
			DrawExtendFormatStringToHandle(300, 50, 2.0, 2.0, 0xffffff, Font1, "Ｃ Ｏ Ｎ Ｆ Ｉ Ｇ Ｕ Ｒ Ａ Ｔ Ｉ Ｏ Ｎ");

			DrawExtendFormatStringToHandle(100, 120, 2.0, 2.0, 0xffffff, Font2, "Ｃ Ｏ Ｌ Ｏ Ｒ　Ｓ Ｅ Ｌ Ｅ Ｃ Ｔ");
			if(opset==1) DrawExtendFormatStringToHandle(140, 220, 2.0, 2.0, 0xffffff, Font2, "Ｓ Ｋ Ｉ Ｎ ： ←　　　　　→");
			else DrawExtendFormatStringToHandle(150, 230, 2.0, 2.0, 0xffffff, Font2, "Ｓ Ｋ Ｉ Ｎ ：");
			if(opset==2) DrawExtendFormatStringToHandle(140, 340, 2.0, 2.0, 0xffffff, Font2, "Ｂ Ｕ Ｌ Ｌ Ｅ Ｔ ： ←　　　　　→");
			else DrawExtendFormatStringToHandle(150, 350, 2.0, 2.0, 0xffffff, Font2, "Ｂ Ｕ Ｌ Ｌ Ｅ Ｔ ：");
			DrawTriangle(530, 230 - 40 / sqrt(2), 530 - 40, 230 + 40, 530 + 40, 230 + 40, skincolor[cl], TRUE);
			DrawCircle(630, 360, 10, bulletcolor[bl], TRUE);
		}

		//画面描画
		if (opset == 3) {
			DrawExtendFormatStringToHandle(480, 550, 2.0, 2.0, 0xffffff, Font3, "　Ｐ Ａ Ｇ Ｅ　\n←　 %d／ 3　→", set);
		}
		else {
			DrawExtendFormatStringToHandle(480, 550, 2.0, 2.0, 0xffffff, Font3, "　Ｐ Ａ Ｇ Ｅ　\n　　 %d／ 3　　", set);
		}

		if (opset == 4) {
			DrawExtendFormatStringToHandle(480, 600, 2.0, 2.0, 0xffffff, Font3, "< Ｂ Ａ Ｃ Ｋ >");
			if (CHECK == 1) {
				PlaySoundMem(SE4, DX_PLAYTYPE_BACK);
				set = 0;
				return;
			}
		}
		else {
			DrawExtendFormatStringToHandle(480, 600, 2.0, 2.0, 0xffffff, Font3, "  Ｂ Ａ Ｃ Ｋ  ");
		}
		

		ScreenFlip();	//表と裏（描画画面）の入れ換え
	}

}

//ゲーム処理
void Game(void) {

	cf = -1;//クリアフラグ初期化
	nohit = 0;
	damhit = 0;

	int Font1 = CreateFontToHandle("PixelMplus10-Regular.ttf", 24, 6); //72
	int Font2 = CreateFontToHandle("PixelMplus10-Regular.ttf", 16, 6); //64
	int Font3 = CreateFontToHandle("PixelMplus10-Regular.ttf", 14, 6); //58
	int Font4 = CreateFontToHandle("PixelMplus10-Regular.ttf", 14, 8); //42
	int Font5 = CreateFontToHandle("PixelMplus10-Regular.ttf", 10, 4); //20

	//変数宣言
	int i; //ループ処理用
	int f; //イベントフラグ
	int jx = 560, jy = 650, jl = 0; //自機用
	int mx = 0, my = 0, mf = 0; //ミサイル(通常)
	int rmf[100], rmw[100], rmx[100], rmy[100], rmwait = 0; //ミサイル(連射)2%
	int tmx = 0, tmy = 0, tmf = 0, tmwait = 0; //ミサイル(追尾)3%
	float tmr; //ミサイル(追尾)3%
	float smx[100], smy[100], smr[100]; //ミサイル(散弾)5%
	int smf[100], smwait = 0; //ミサイル(散弾)5%
	int kmx = 0, kmy = 0, kmf = 0; //ミサイル(高速)10%
	int remx = 0, remy = 0, remf = 0, ref = 0; //回復玉20%
	int a = 100; //自機ミサイルゲージ処理用
	int itemx = 0, itemy = 0, itemf = 0, item = 0;
	int ex = GetRand(1020) + 50, ey = -20, ed = 1, el = 0; //敵機用
	int emx = 0, emy = 0, emf = 0; //敵ミサイル
	int eef = 0;
	int b = 100; //敵機ミサイルゲージ処理用
	float d; //当たり判定用

	int bf = 0; //自機ミサイル変更フラグ

	int gh; //ポーズ時のグラフィック保存
	int pf = 0; //ポーズフラグ

	//カウントダウン用変数
	int now;
	int count;

	//ミサイル用変数初期化
	for (i = 0; i < 99; i++) {
		rmf[i] = 0;
		rmw[i] = 0;
		smf[i] = 0;
	}

	//空のグラフィックデータを作成(1120×630)
	gh = MakeGraph(1120, 630);

	//カウントダウン処理
	now = GetNowCount();
	while (GetNowCount() - now < 4000) {
		ClearDrawScreen();	//描画画面（裏画面）を黒でクリア

		count = GetNowCount() - now;

		

		if (jy > 480 && ey < 150) {
			jy -= 5;
			ey += 5;
		}
		if (jl < 500 && el < 500) {
			jl += 5;
			el += 5;
		}

		DrawTriangle(jx, jy - 40 / sqrt(2), jx - 40, jy + 40, jx + 40, jy + 40, skincolor[cl], TRUE);
		DrawTriangle(ex, ey + 40 / sqrt(2), ex - 40, ey - 40, ex + 40, ey - 40, 0x0000ff, TRUE);

		//自機体力・弾丸ゲージ
		DrawBox(30, 600, 1050, 620, 0xffffff, TRUE);
		DrawBox(30, 600, jl * 2 + 50, 620, 0x00ff00, TRUE);
		DrawTriangle(jl * 2 + 30, 600, jl * 2 + 50, 600, jl * 2 + 50, 620, 0xffffff, TRUE);
		DrawBox(10, 580, 330, 600, 0xffffff, TRUE);
		if (count > 3000)DrawBox(10, 580, a * 3 + 30, 600, bulletcolor[bl], TRUE);
		DrawTriangle(a * 3 + 10, 580, a * 3 + 30, 580, a * 3 + 30, 600, 0xffffff, TRUE);
		DrawTriangle(10, 620, 10, 580, 50, 620, 0x000000, TRUE);
		DrawTriangle(310, 580, 330, 580, 330, 600, 0x000000, TRUE);
		DrawTriangle(1030, 600, 1050, 600, 1050, 620, 0x000000, TRUE);
		//敵機体力・弾丸ゲージ
		DrawBox(1090, 10, 70, 30, 0xffffff, TRUE);
		DrawBox(1090, 10, 1070 - el * 2, 30, 0x00ff00, TRUE);
		DrawTriangle(1070 - el * 2, 10, 1070 - el * 2, 30, 1090 - el * 2, 30, 0xffffff, TRUE);
		DrawBox(1110, 30, 790, 50, 0xffffff, TRUE);
		if (count > 3000)DrawBox(1110, 30, 1090 - b * 3, 50, 0x00ffff, TRUE);
		DrawTriangle(1090 - b * 3, 30, 1090 - b * 3, 50, 1110 - b * 3, 50, 0xffffff, TRUE);
		DrawTriangle(1070, 10, 1110, 10, 1110, 50, 0x000000, TRUE);
		DrawTriangle(790, 30, 790, 50, 810, 50, 0x000000, TRUE);
		DrawTriangle(70, 10, 70, 30, 90, 30, 0x000000, TRUE);

		if (count < 1000) {
			DrawExtendFormatStringToHandle(530, 280, 3.0, 3.0, 0xffffff, Font4, "３");
		}
		else if (count < 2000) {
			DrawExtendFormatStringToHandle(520, 270, 4.0, 4.0, 0xffffff, Font3, "２");
		}
		else if (count < 3000) {
			DrawExtendFormatStringToHandle(515, 265, 4.0, 4.0, 0xffffff, Font2, "１");
		}
		else if (count < 4000) {
			DrawExtendFormatStringToHandle(370, 260, 3.0, 3.0, 0xffffff, Font1, "ＳＴＡＲＴ！");
		}

		ScreenFlip();
	}

	PlaySoundMem(BGM2, DX_PLAYTYPE_LOOP);

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)	//OSから終了指示が来るか、ESCキーを押されるまで繰り返し
	{

		rmwait++; //ミサイル発射待機(連射)
		smwait++; //ミサイル発射待機(散弾)
		eef++;


		ClearDrawScreen();	//描画画面（裏画面）を黒でクリア

		//ゲームオーバー処理
		if (cf == 0) {
			nextf++;
			//自機描画
			if (nextf % 2 == 0) {
				DrawTriangle(jx, jy - 40 / sqrt(2), jx - 40, jy + 40, jx + 40, jy + 40, skincolor[cl], TRUE);
			}
			jy++;

			//敵機描画
			DrawTriangle(ex, ey + 40 / sqrt(2), ex - 40, ey - 40, ex + 40, ey - 40, 0x0000ff, TRUE);
			ex = ex + 5 * ed;
			if (ex >= 1070) ed *= -1;
			if (ex <= 50) ed *= -1;

			//自機体力・弾丸ゲージ
			DrawBox(30, 600, 1050, 620, 0xffffff, TRUE);
			DrawBox(30, 600, jl + 50, 620, 0x00ff00, TRUE);
			DrawTriangle(jl + 30, 600, jl + 50, 600, jl + 50, 620, 0xffffff, TRUE);
			DrawBox(10, 580, 330, 600, 0xffffff, TRUE);
			DrawBox(10, 580, a * 3 + 30, 600, bulletcolor[bl], TRUE);
			DrawTriangle(a * 3 + 10, 580, a * 3 + 30, 580, a * 3 + 30, 600, 0xffffff, TRUE);
			DrawTriangle(10, 620, 10, 580, 50, 620, 0x000000, TRUE);
			DrawTriangle(310, 580, 330, 580, 330, 600, 0x000000, TRUE);
			DrawTriangle(1030, 600, 1050, 600, 1050, 620, 0x000000, TRUE);
			//敵機体力・弾丸ゲージ
			DrawBox(1090, 10, 70, 30, 0xffffff, TRUE);
			DrawBox(1090, 10, 1070 - el, 30, 0x00ff00, TRUE);
			DrawTriangle(1070 - el, 10, 1070 - el, 30, 1090 - el, 30, 0xffffff, TRUE);
			DrawBox(1110, 30, 790, 50, 0xffffff, TRUE);
			DrawBox(1110, 30, 1090 - b * 3, 50, 0x00ffff, TRUE);
			DrawTriangle(1090 - b * 3, 30, 1090 - b * 3, 50, 1110 - b * 3, 50, 0xffffff, TRUE);
			DrawTriangle(1070, 10, 1110, 10, 1110, 50, 0x000000, TRUE);
			DrawTriangle(790, 30, 790, 50, 810, 50, 0x000000, TRUE);
			DrawTriangle(70, 10, 70, 30, 90, 30, 0x000000, TRUE);

			DrawExtendFormatStringToHandle(350, 200, 3.0, 3.0, 0x0000ff, Font1, " ＧＡＭＥ \nＯＶＥＲ…");

			if (jy > 700) {
				Result();
				return;
			}
		}
		//ゲームクリア処理
		else if (cf == 1) {
			nextf++;

			//自機描画
			DrawTriangle(jx, jy - 40 / sqrt(2), jx - 40, jy + 40, jx + 40, jy + 40, skincolor[cl], TRUE);

			//敵機描画
			if (nextf % 2 == 0) {
				DrawTriangle(ex, ey + 40 / sqrt(2), ex - 40, ey - 40, ex + 40, ey - 40, 0x0000ff, TRUE);
			}
			ey--;

			//自機体力・弾丸ゲージ
			DrawBox(30, 600, 1050, 620, 0xffffff, TRUE);
			DrawBox(30, 600, jl + 50, 620, 0x00ff00, TRUE);
			DrawTriangle(jl + 30, 600, jl + 50, 600, jl + 50, 620, 0xffffff, TRUE);
			DrawBox(10, 580, 330, 600, 0xffffff, TRUE);
			DrawBox(10, 580, a * 3 + 30, 600, bulletcolor[bl], TRUE);
			DrawTriangle(a * 3 + 10, 580, a * 3 + 30, 580, a * 3 + 30, 600, 0xffffff, TRUE);
			DrawTriangle(10, 620, 10, 580, 50, 620, 0x000000, TRUE);
			DrawTriangle(310, 580, 330, 580, 330, 600, 0x000000, TRUE);
			DrawTriangle(1030, 600, 1050, 600, 1050, 620, 0x000000, TRUE);
			//敵機体力・弾丸ゲージ
			DrawBox(1090, 10, 70, 30, 0xffffff, TRUE);
			DrawBox(1090, 10, 1070 - el, 30, 0x00ff00, TRUE);
			DrawTriangle(1070 - el, 10, 1070 - el, 30, 1090 - el, 30, 0xffffff, TRUE);
			DrawBox(1110, 30, 790, 50, 0xffffff, TRUE);
			DrawBox(1110, 30, 1090 - b * 3, 50, 0x00ffff, TRUE);
			DrawTriangle(1090 - b * 3, 30, 1090 - b * 3, 50, 1110 - b * 3, 50, 0xffffff, TRUE);
			DrawTriangle(1070, 10, 1110, 10, 1110, 50, 0x000000, TRUE);
			DrawTriangle(790, 30, 790, 50, 810, 50, 0x000000, TRUE);
			DrawTriangle(70, 10, 70, 30, 90, 30, 0x000000, TRUE);

			DrawExtendFormatStringToHandle(350, 200, 3.0, 3.0, 0xff0000, Font1, "　ＧＡＭＥ　\nＣＬＥＡＲ！");

			if (ey < -20) {
				Result();
				return;
			}


		}
		//通常処理
		else {
			nextf++;
			SetFontSize(16);

			//自機描画
			//if (UP == 1 && jy >= 10 + 40 / sqrt(2)) jy -= 10;
			//if (DOWN == 1 && jy <= 580) jy += 10;
			if (RIGHT == 1 && jx <= 1070) jx += 10;
			if (LEFT == 1 && jx >= 50) jx -= 10;
			DrawTriangle(jx, jy - 40 / sqrt(2), jx - 40, jy + 40, jx + 40, jy + 40, skincolor[cl], TRUE);

			//自機ミサイル発射(通常)
			if (SHOOT == 1 && mf == 0 && bf == 0) {
				PlaySoundMem(SE8,DX_PLAYTYPE_BACK);
				mx = jx;
				my = jy - 40 / sqrt(2);
				mf = 1;
				a = 0;
			}
			if (mf == 1 && tmf == 0 && kmf == 0) {
				DrawCircle(mx, my, 10, bulletcolor[bl], TRUE);
				my -= 10;
				if (my < -10) {
					mf = 0;
					a = 100;
				}
				//敵機への当たり判定
				d = Root(mx, my, ex, ey);//sqrt((float)((ex - mx) * (ex - mx) + (ey - my) * (ey - my)));
				if (d <= 50.0) {
					DrawCircle(mx, my, 30, 0xffff00, TRUE);
					mf = 0;
					a = 100;
					nohit++;
					//敵ダメージ処理
					el--;
					//イベント処理
					f = GetRand(99); //0〜99でランダムにフラグを立てる
					if (f < 20) ref = 1; // フラフが20未満でkfを1にする(20%)
					else if (f < 40) item = 1; // フラフが20以上40未満でrfを1にする(1%)
				}
			}

			//回復イベント発射 確率20%
			if (remf == 0 && ref == 1) {
				remx = ex;
				remy = ey + 40 / sqrt(2);
				remf = 1;
			}
			if (remf == 1) {
				SetFontSize(24);
				DrawFormatString(remx - 12, remy - 12, 0x00ff00, "＋");
				//DrawCircle(remx, remy, 10, GetColor(0, GetRand(255), 0), TRUE);
				remy++;
				ref = 0;
				if (remy >= 640) remf = 0;
				//自機への当たり判定
				d = Root(remx, remy, jx, jy);//d = sqrt((float)((jx - remx) * (jx - remx) + (jy - remy) * (jy - remy)));
				if (d <= 50.0) {
					PlaySoundMem(SE5, DX_PLAYTYPE_BACK);
					remf = 0;
					if (jl < 500)jl += 5;
					DrawBox(0, 0, 1120, 630, 0x00ff00, TRUE);
				}
			}

			//特殊弾イベント発射 確率20%
			if (itemf == 0 && item == 1) {
				itemx = ex;
				itemy = ey + 40 / sqrt(2);
				itemf = 1;
			}
			if (itemf == 1) {
				SetFontSize(36);
				DrawFormatString(itemx - 18, itemy - 18, GetRand(0xffffff), "？");
				//DrawCircle(remx, remy, 10, GetColor(0, GetRand(255), 0), TRUE);
				itemy++;
				item = 0;
				if (itemy >= 640) itemf = 0;
				//自機への当たり判定
				d = Root(itemx, itemy, jx, jy);//d = sqrt((float)((jx - itemx) * (jx - itemx) + (jy - itemy) * (jy - itemy)));
				if (d <= 50.0) {
					PlaySoundMem(SE6, DX_PLAYTYPE_BACK);
					itemf = 0;
					f = GetRand(9); //0〜9でランダムにフラグを立てる
					if (f < 2)	bf = 1; // フラフが2未満でrfを1にする(1%)
					else if (f < 3) bf = 2; // フラフが3未満でtfを1にする(3%)
					else if (f < 5) bf = 3; // フラフが5未満でsfを1にする(5%)
					else if (f < 10) bf = 4; // フラフが10未満でsfを1にする(10%)
					DrawBox(0, 0, 1120, 630, 0xffffff, TRUE);
				}
			}

			//自機ミサイル発射(連射)確率2%
			if (rmwait >= 10) {
				for (i = 0; i < 99; i++) {
					if (SHOOT == 1 && rmf[i] == 0 && bf == 1) {
						PlaySoundMem(SE8, DX_PLAYTYPE_BACK);
						rmx[i] = jx;
						rmy[i] = jy - 40 / sqrt(2);
						rmf[i] = 1;
						rmwait = 0;
						a -= 2;
						break;
					}
				}
			}
			for (i = 0; i < 99; i++) {
				if (rmf[i] == 1) {
					DrawCircle(rmx[i], rmy[i], 10, bulletcolor[bl], TRUE);
					rmy[i] -= 10;
					if (rmy[i] < -10)	rmf[i] = 0;
					//敵機への当たり判定
					d = Root(rmx[i], rmy[i], ex, ey);//d = sqrt((float)((ex - rmx[i]) * (ex - rmx[i]) + (ey - rmy[i]) * (ey - rmy[i])));
					if (d <= 50.0) {
						PlaySoundMem(SE3, DX_PLAYTYPE_BACK);
						DrawCircle(rmx[i], rmy[i], 30, 0xffff00, TRUE);
						rmf[i] = 0;
						nohit++;
						//敵ダメージ処理
						el -= 15;
						//回復イベント処理
						f = GetRand(99); //0〜99からランダムにフラグを立てる
						if (f < 20) ref = 1; // フラフが20未満でkfを1にする(20%)
					}
					if (a <= 0 && rmf[i] == 0) {
						bf = 0;
					}
				}
			}
			//自機ミサイル発射(追尾)確率3%
			if (SHOOT == 1 && tmf == 0 && bf == 2)
			{
				PlaySoundMem(SE8, DX_PLAYTYPE_BACK);
				tmx = jx;
				tmy = jy - 40 / sqrt(2);
				tmr = 0.0;
				tmwait = 0;
				tmf = 1;
				a -= 20;
			}
			if (tmf == 1)
			{
				tmx = tmx - sin(tmr) * 5;
				tmy -= 10;
				//tmy = tmy - cos(tmr) * 10;
				DrawCircle(tmx, tmy, 10, bulletcolor[bl], TRUE);
				if (tmy >= 700 || tmy <= 0)tmf = 0;

				tmwait++;

				if (tmwait >= 10)
				{
					tmr = atan2((float)tmx - ex, tmy - ey);
					tmwait = 0;
				}
				d = Root(tmx, tmy, ex, ey); //d = sqrt((float)((ex - tmx) * (ex - tmx) + (ey - tmy) * (ey - tmy)));
				if (d <= 50.0) {
					PlaySoundMem(SE3, DX_PLAYTYPE_BACK);
					DrawCircle(tmx, tmy, 30, 0xffff00, TRUE);
					tmf = 0;
					nohit++;
					//敵ダメージ処理
					el -= 5;
					//回復イベント処理
					f = GetRand(99); //0〜99からランダムにフラグを立てる
					if (f < 20) ref = 1; // フラフが20未満でkfを1にする(20%)
				}
				if (a <= 0 && tmf == 0) {
					bf = 0;
				}
			}

			////ミサイル発射(散弾)確率5%
			if (SHOOT == 1 && smwait >= 10 && bf == 3)
			{
				for (i = 0; i <= 99; i++)
				{
					if (smf[i] == 0 && a > 0)
					{
						PlaySoundMem(SE8, DX_PLAYTYPE_BACK);
						smf[i] = 1;
						smx[i] = jx;
						smy[i] = jy - 40 / sqrt(2);
						smr[i] = ((float)(GetRand(3141592)) / 4.0) / 1000000.0 - 3.141592 * 1.1;
						smwait = 0;
						a--;
						break;
					}
				}
			}
			for (i = 0; i <= 99; i++)
			{
				if (smf[i] == 1)
				{
					smx[i] = smx[i] + sin(smr[i]) * 10.0;
					smy[i] = smy[i] + cos(smr[i]) * 10.0;
					DrawCircle(smx[i], smy[i], 10, bulletcolor[bl], TRUE);
					if (smy[i] <= -20) smf[i] = 0;
					d = Root(smx[i], smy[i], ex, ey); //d = sqrt((float)((ex - smx[i]) * (ex - smx[i]) + (ey - smy[i]) * (ey - smy[i])));
					if (d <= 50.0) {
						PlaySoundMem(SE3, DX_PLAYTYPE_BACK);
						DrawCircle(smx[i], smy[i], 30, 0xffff00, TRUE);
						smf[i] = 0;
						nohit++;
						//敵ダメージ処理
						el -= 3;
						//回復イベント処理
						f = GetRand(99); //0〜99からランダムにフラグを立てる
						if (f < 20) ref = 1; // フラフが20未満でkfを1にする(20%)
					}
					if (a <= 0 && smf[0] == 0) {
						bf = 0;
					}
				}
			}

			//ミサイル発射(散弾)確率5%
			//if (SHOOT == 1 && bf == 3)
			//{
			//	for (i = 0; i <= 999; i++)
			//	{
			//		if (smf[i] == 0 && a > 0)
			//		{
			//			smf[i] = 1;
			//			smx[i] = jx;
			//			smy[i] = jy - 40 / sqrt(2);
			//			smr[i] = ((float)(GetRand(3141592)) / 4.0) / 1000000.0 - 3.141592 * 1.1;
			//			a--;
			//			break;
			//		}
			//	}
			//}
			//for (i = 0; i <= 999; i++)
			//{
			//	if (smf[i] == 1)
			//	{
			//		smx[i] = smx[i] + sin(smr[i]) * 10.0;
			//		smy[i] = smy[i] + cos(smr[i]) * 10.0;
			//		DrawCircle(smx[i], smy[i], 10, bulletcolor[bl], TRUE);
			//		if (smy[i] <= -20) smf[i] = 0;
			//		d = sqrt((float)((ex - smx[i]) * (ex - smx[i]) + (ey - smy[i]) * (ey - smy[i])));
			//		if (d <= 50.0) {
			//			PlaySoundMem(SE3, DX_PLAYTYPE_BACK);
			//			DrawCircle(smx[i], smy[i], 30, 0xffff00, TRUE);
			//			smf[i] = 0;
			//			nohit++;
			//			//敵ダメージ処理
			//			el -= 3;
			//			//回復イベント処理
			//			f = GetRand(99); //0〜99からランダムにフラグを立てる
			//			if (f < 20) ref = 1; // フラフが20未満でkfを1にする(20%)
			//		}
			//		if (a <= 0 && smf[0] == 0) {
			//			bf = 0;
			//		}
			//	}
			//}

			//ミサイル発射(高速)確率10%
			if (SHOOT == 1 && kmf == 0 && bf == 4) {
				PlaySoundMem(SE8, DX_PLAYTYPE_BACK);
				kmx = jx;
				kmy = jy - 40 / sqrt(2);
				kmf = 1;
				a -= 10;
			}
			if (kmf == 1) {
				DrawCircle(kmx, kmy, 10, bulletcolor[bl], TRUE);
				kmy -= 20;
				if (kmy < -10) {
					kmf = 0;
				}
				//敵機への当たり判定
				d = Root(kmx, kmy, ex, ey); //d = sqrt((float)((ex - kmx) * (ex - kmx) + (ey - kmy) * (ey - kmy)));
				if (d <= 50.0) {
					PlaySoundMem(SE3, DX_PLAYTYPE_BACK);
					DrawCircle(kmx, kmy, 30, 0xffff00, TRUE);
					kmf = 0;
					nohit++;
					//敵ダメージ処理
					el -= 10;
					//回復イベント処理
					f = GetRand(99); //0〜99からランダムにフラグを立てる
					if (f < 20) ref = 1; // フラフが20未満でkfを1にする(20%)
				}
				if (a <= 0 && kmf == 0) {
					bf = 0;
				}
			}

			//敵機描画
			DrawTriangle(ex, ey + 40 / sqrt(2), ex - 40, ey - 40, ex + 40, ey - 40, 0x0000ff, TRUE);
			if (emove == 0) ex = ex + 2 * ed;
			else if (emove == 1) ex = ex + 5 * ed;
			else if (emove == 2) ex = ex + 10 * ed;

			if (jx < ex && eef > GetRand(3) * 10 + 80) {
				ed = -1;
				eef = 0;
			}
			else if (jx > ex&& eef > GetRand(3) * 10 + 80) {
				ed = 1;
				eef = 0;
			}
			if (ex >= 1070) ed = -1;
			if (ex <= 50) ed = 1;

			//敵機ミサイル発射
			if (emf == 0 && nextf == GetRand(10) + 5 && abs(jx - ex) < 100) {
				emx = ex;
				emy = ey + 40 / sqrt(2);
				emf = 1;
				b = 0;
			}
			if (emf == 1) {
				DrawCircle(emx, emy, 10, 0x00ffff, TRUE);
				emy += 10;
				if (emy >= 640) {
					emf = 0;
					b = 100;
				}
				//自機への当たり判定
				d = Root(emx, emy, jx, jy); //d = sqrt((float)((jx - emx) * (jx - emx) + (jy - emy) * (jy - emy)));
				if (d <= 50.0) {
					PlaySoundMem(SE1,DX_PLAYTYPE_BACK);
					emf = 0;
					DrawBox(0, 0, 1120, 630, 0xff0000, TRUE);
					b = 100;
					nextf = 0;
					damhit++;
					if (esl == 0) jl -= 5;
					if (esl == 1) jl -= 10;
					if (esl == 2) jl -= 50;
				}
			}

			if (jl <= 0) { 
				cf = 0;
				StopSoundMem(BGM2);
				PlaySoundMem(SE2, DX_PLAYTYPE_BACK);
			}
			if (el <= 0) {
				cf = 1;
				StopSoundMem(BGM2);

			}
			if (nextf > 15)nextf = 0;

			//特殊効果発動時出力
			if (bf == 0) DrawExtendFormatStringToHandle(330, 580, 2.0, 2.0, bulletcolor[bl], Font5, "NORMAL:%d",a/100);
			if (bf == 1) DrawExtendFormatStringToHandle(330, 580, 2.0, 2.0, bulletcolor[bl], Font5, "RAPID:%d",a/2);
			if (bf == 2) DrawExtendFormatStringToHandle(330, 580, 2.0, 2.0, bulletcolor[bl], Font5, "TRACK:%d",a/20);
			if (bf == 4) DrawExtendFormatStringToHandle(330, 580, 2.0, 2.0, bulletcolor[bl], Font5, "SPEED:%d",a/10);
			if (bf == 3) DrawExtendFormatStringToHandle(330, 580, 2.0, 2.0, bulletcolor[bl], Font5, "SCATTER:%d",a);

			//自機体力・弾丸ゲージ
			DrawBox(30, 600, 1050, 620, 0xffffff, TRUE);
			DrawBox(30, 600, jl*2 + 50, 620, 0x00ff00, TRUE);
			DrawTriangle(jl * 2 + 30, 600, jl * 2 + 50, 600, jl * 2 + 50, 620, 0xffffff, TRUE);
			DrawBox(10, 580, 330, 600, 0xffffff, TRUE);
			DrawBox(10, 580, a * 3 + 30, 600, bulletcolor[bl], TRUE);
			DrawTriangle(a * 3 + 10, 580, a * 3 + 30, 580, a * 3 + 30, 600, 0xffffff, TRUE);
			DrawTriangle(10, 620, 10, 580, 50, 620, 0x000000, TRUE);
			DrawTriangle(310, 580, 330, 580, 330, 600, 0x000000, TRUE);
			DrawTriangle(1030, 600, 1050, 600, 1050, 620, 0x000000, TRUE);
			//敵機体力・弾丸ゲージ
			DrawBox(1090, 10, 70, 30, 0xffffff, TRUE);
			DrawBox(1090, 10, 1070 - el * 2, 30, 0x00ff00, TRUE);
			DrawTriangle(1070 - el * 2, 10, 1070 - el * 2, 30, 1090 - el * 2, 30, 0xffffff, TRUE);
			DrawBox(1110, 30, 790, 50, 0xffffff, TRUE);
			DrawBox(1110, 30, 1090 - b * 3, 50, 0x00ffff, TRUE);
			DrawTriangle(1090 - b * 3, 30, 1090 - b * 3, 50, 1110 - b * 3, 50, 0xffffff, TRUE);
			DrawTriangle(1070, 10, 1110, 10, 1110, 50, 0x000000, TRUE);
			DrawTriangle(790, 30, 790, 50, 810, 50, 0x000000, TRUE);
			DrawTriangle(70, 10, 70, 30, 90, 30, 0x000000, TRUE);


			//ポーズ機能
			if (PAUSE == 1)	//■■■■■■■■■■■■■■■■■■■■■■■
			{
				StopSoundMem(BGM2);
				GetDrawScreenGraph(0, 0, 1120, 630, gh);
				pf = 0;
				PlaySoundMem(SE9,DX_PLAYTYPE_BACK);
				
				while (ProcessMessage() == 0 && pf != 1)	//OSから終了指示が来るか、Bキーを押されるまで繰り返し
				{
					SetFontSize(16);
					//ClearDrawScreen();	//描画画面（裏画面）を黒でクリア
					keywait++;

					DrawGraph(0, 0, gh, FALSE);

					for (int yy = 0; yy <= 630; yy += 2)
					{
						DrawLine(0, yy, 1120, yy, GetColor(0, 0, 0));
					}

					if (UP == 1 && keywait >= 10) {
						PlaySoundMem(SE7, DX_PLAYTYPE_BACK);
						set--;
						keywait = 0;
						if (set < 1)set = 2;
					}
					if (DOWN == 1 && keywait >= 10) {
						PlaySoundMem(SE7, DX_PLAYTYPE_BACK);
						set++;
						keywait = 0;
						if (set > 2)set = 1;
					}

					SetFontSize(64);
					DrawExtendFormatStringToHandle(100, 240, 4.0, 4.0, GetRand(0xffffff), Font2, "Ｐ　Ａ　Ｕ　Ｓ　Ｅ");

					SetFontSize(16);
					if (set == 1) DrawExtendFormatStringToHandle(750, 190, 2.0, 2.0, 0xffffff, Font5, "ＢＡＣＫ　ＴＩＴＬＥ");
					else DrawExtendFormatStringToHandle(800, 200, 2.0, 2.0, 0xffffff, Font5, "ＢＡＣＫ　ＴＩＴＬＥ");
					if (set == 2) DrawExtendFormatStringToHandle(750, 490, 2.0, 2.0, 0xffffff, Font5, "ＧＡＭＥ　ＲＥＳＴＡＲＴ");
					else DrawExtendFormatStringToHandle(800, 500, 2.0, 2.0, 0xffffff, Font5, "ＧＡＭＥ　ＲＥＳＴＡＲＴ");

					if (CHECK == 1) {
						PlaySoundMem(SE4, DX_PLAYTYPE_BACK);
						switch (set)
						{
						case 1:
							StopSoundMem(BGM2);
							PlaySoundMem(BGM1,DX_PLAYTYPE_LOOP);
							pf = 1;
							set = 0;
							return;
							break;
						case 2:
							PlaySoundMem(BGM2, DX_PLAYTYPE_LOOP,FALSE);
							pf = 1;
							set = 0;
							break;
						}
					}

					ScreenFlip();
				}
			}
		}

		ScreenFlip();	//表と裏（描画画面）の入れ換え

	}

}

//リザルト処理(動きめっちゃ付ける)
void Result(void)
{

	int Font1 = CreateFontToHandle("PixelMplus10-Regular.ttf", 16, 6);
	int Font2 = CreateFontToHandle("PixelMplus10-Regular.ttf", 12, 6);
	int Font3 = CreateFontToHandle("PixelMplus10-Regular.ttf", 10, 6);

	int x1 = 1130, x2 = 1130, x3 = 1130;

	if (cf == 1) PlaySoundMem(BGM3, DX_PLAYTYPE_LOOP);
	if (cf == 0) PlaySoundMem(BGM4, DX_PLAYTYPE_LOOP);

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)	//OSから終了指示が来るか、ESCキーを押されるまで繰り返し
	{
		ClearDrawScreen();	//描画画面（裏画面）を黒でクリア

		if (x1 >= 50) x1 -= 10;
		if (x1 <= 50 && x2 > 200) x2 -= 10;
		if (x2 <= 200 && x3 > 250) x3 -= 10;

		switch (cf)
		{
		case 0:
			SetFontSize(32);
			DrawExtendFormatStringToHandle(x1, 100, 2.0, 2.0, 0x0000ff, Font1, "Ｇ Ａ Ｍ Ｅ　Ｆ Ａ Ｉ Ｌ Ｅ Ｄ"); //DrawFormatString(x1, 100, 0x0000ff, "Ｇ Ａ Ｍ Ｅ　Ｆ Ａ Ｉ Ｌ Ｅ Ｄ");
			SetFontSize(24);
			DrawExtendFormatStringToHandle(x1 + 50, 150, 2.0, 2.0, 0x00ffff, Font2, "Ｌ Ｅ Ｖ Ｅ Ｌ ： %s − %s", es[esl], em[emove]); //DrawFormatString(x1 + 50, 150, 0x00ffff, "Ｌ Ｅ Ｖ Ｅ Ｌ ： %s − %s", es[esl], em[emove]);

			break;
		case 1:
			SetFontSize(32);
			DrawExtendFormatStringToHandle(x1, 100, 2.0, 2.0, skincolor[cl], Font1, "Ｇ Ａ Ｍ Ｅ　Ｓ Ｕ Ｃ Ｃ Ｅ Ｓ Ｓ Ｆ Ｕ Ｌ"); //DrawFormatString(x1, 100, skincolor[cl], "Ｇ Ａ Ｍ Ｅ　Ｓ Ｕ Ｃ Ｃ Ｅ Ｓ Ｓ Ｆ Ｕ Ｌ");
			SetFontSize(24);
			DrawExtendFormatStringToHandle(x1 + 50, 150, 2.0, 2.0, bulletcolor[bl], Font2, "Ｌ Ｅ Ｖ Ｅ Ｌ ： %s − %s", es[esl], em[emove]); //DrawFormatString(x1 + 50, 150, bulletcolor[bl], "Ｌ Ｅ Ｖ Ｅ Ｌ ： %s − %s", es[esl], em[emove]);

			break;
		}

		SetFontSize(24);
		DrawExtendFormatStringToHandle(x2, 300, 2.0, 2.0, 0xff0000, Font2, "Ｎ Ｕ Ｍ Ｂ Ｅ Ｒ　Ｏ Ｆ　Ｈ Ｉ Ｔ ：%d", nohit); //DrawFormatString(x2, 300, 0xff0000, "Ｎ Ｕ Ｍ Ｂ Ｅ Ｒ　Ｏ Ｆ　Ｈ Ｉ Ｔ ：%d", nohit);

		SetFontSize(24);
		DrawExtendFormatStringToHandle(x3, 350, 2.0, 2.0, 0x0000ff, Font2, "Ｄ Ａ Ｍ Ａ Ｇ Ｅ　Ｈ Ｉ Ｔ ：%d", damhit); //DrawFormatString(x3, 350, 0x0000ff, "Ｄ Ａ Ｍ Ａ Ｇ Ｅ　Ｈ Ｉ Ｔ ：%d", damhit);

		if (x3 <= 250) {
			SetFontSize(16);
			DrawExtendFormatStringToHandle(400, 550, 2.0, 2.0, 0xffffff, Font3, "Ｐ Ｕ Ｓ Ｈ　Ｔ Ｏ　Ｅ Ｎ Ｔ Ｅ Ｒ"); //DrawFormatString(400, 550, 0xffffff, "Ｐ Ｕ Ｓ Ｈ　Ｔ Ｏ　Ｅ Ｎ Ｔ Ｅ Ｒ");

			if (CHECK == 1) {
				StopSoundMem(BGM3);
				StopSoundMem(BGM4);
				PlaySoundMem(SE4, DX_PLAYTYPE_BACK);
				PlaySoundMem(BGM1, DX_PLAYTYPE_LOOP);
				return;
			}
		}

		ScreenFlip();	//表と裏（描画画面）の入れ換え
	}

}

float Root(int missx, int missy, int x, int y) {

	float d;

	d = sqrt((float)((x - missx) * (x - missx) + (y - missy) * (y - missy)));

	return d;

}
