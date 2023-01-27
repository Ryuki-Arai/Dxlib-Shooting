//�w�b�_�[�t�@�C���ǂݍ���
#include "Dxlib.h"
#include "math.h"

//�R���p�C�����̌x���\���}��
#pragma warning( disable:4305 4244 4305 )

void Game(void); //�Q�[���{��
void Menu(void); //���j���[
void Result(void); //���U���g
float Root(int,int,int,int);

//�}�N��
#define PI 3.141592 //�~����
#define RIGHT CheckHitKey(KEY_INPUT_RIGHT) //�E
#define LEFT CheckHitKey(KEY_INPUT_LEFT) //��
#define UP CheckHitKey(KEY_INPUT_UP) //��
#define DOWN CheckHitKey(KEY_INPUT_DOWN) //��
#define CHECK CheckHitKey(KEY_INPUT_RETURN) //����
#define SHOOT CheckHitKey(KEY_INPUT_SPACE) //���C
#define PAUSE CheckHitKey(KEY_INPUT_BACK) //�|�[�Y

//�ėp�ϐ��錾
int set = 0; //�I�����p�ϐ�
int keywait = 0; //���͑ҋ@(�����܂ň�莞�ԑ҂p)
int cf; //�N���A�t���O -1=NULL 0=FALSE 1=TRUE
int nextf = 0; //�����ҋ@�ϐ�
int damhit; //��e��
int nohit; //���e��
int skincolor[10] = {0xff0000,0xffc0cb,0xffd700,0xadff2f,0x008000,0x0000ff,0x800080,0xffa500,0x808080,0xffffff}; //���@�F(�ԁE��E�����E���΁E�΁E�E���E���E�D�E��)
int bulletcolor[5] = {0xff00ff,0x00ff00,0x00ffff,0xffff00,0xffffff}; //�e�F(�}�[���^�E���C���E�V�A���E���E��)
int cl = 0, bl = 0; //�F�ԍ�
char es[3][50] = { {"�d �` �r �x" }, {"�m �n �l �` �k"}, {"�g �` �q �c"} }; //��Փx
char em[3][50] = { {"�k �n �v"},{"�m �n �l �` �k"},{"�g �h �f �g"} }; //�G�@�ړ����x
int esl = 1, emove = 1; //�G�@��Փx�E���x
int FONT;
int BGM1, BGM2, BGM3, BGM4;
int SE1, SE2, SE3, SE4, SE5, SE6, SE7, SE8, SE9;

//�^�C�g�����(���C�����[�v)
int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nC)
{
	SetMainWindowText("Assalt BlitZ");
	SetWindowIconID(102);
	SetGraphMode(1120, 630, 32);		//��ʃT�C�Y�ݒ�(�ǂꂩ�ЂƂ�I��)
	ChangeWindowMode(TRUE);				//�E�B���h�E���[�h�ŋN���A�R�����g�A�E�g�Ńt���X�N���[��
	if (DxLib_Init() == -1) return -1;	//DX���C�u����������
	SetDrawScreen(DX_SCREEN_BACK);		//����ʂɕ`��

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
	//���C�����[�v�E�^�C�g�����
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)	//OS����I���w�������邩�AESC�L�[���������܂ŌJ��Ԃ�
	{
		nextf++;
		keywait++;
		ClearDrawScreen();	//�`���ʁi����ʁj�����ŃN���A

		//�㉺�L�[�őI��
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
		DrawExtendFormatStringToHandle(202, 98, 10.0, 10.0, 0x880015, Font, "�`�r�r�`�t�k�s\n  �a�k�h�s�y");
		DrawExtendFormatStringToHandle(198, 102, 10.0, 10.0, 0xFFC90E, Font, "�`�r�r�`�t�k�s\n  �a�k�h�s�y");

		if (set == 1) DrawExtendFormatStringToHandle(400, 400, 2.0, 2.0, 0xffffff, Font, "�� �f �` �l �d�@�r �s �` �q �s��");
		else DrawExtendFormatStringToHandle(400, 400, 2.0, 2.0, 0xffffff, Font, "�@ �f �` �l �d�@�r �s �` �q �s");
		if (set == 2) DrawExtendFormatStringToHandle(460, 450, 2.0, 2.0, 0xffffff, Font, "�� �n �o �s �h �n �m ��");
		else DrawExtendFormatStringToHandle(460, 450, 2.0, 2.0, 0xffffff, Font, "�@ �n �o �s �h �n �m");
		if (set == 3) DrawExtendFormatStringToHandle(400, 500, 2.0, 2.0, 0xffffff, Font, "�� �f �` �l �d�@�b �k �n �r �d ��");
		else DrawExtendFormatStringToHandle(400, 500, 2.0, 2.0, 0xffffff, Font, "�@ �f �` �l �d�@�b �k �n �r �d");
		//�G���^�[�L�[�������Č���
		if (CheckHitKey(KEY_INPUT_RETURN) == 1) {
			PlaySoundMem(SE4, DX_PLAYTYPE_BACK);
			switch (set)
			{
			case 1:
				set = 0; //�Q�[���J�n
				StopSoundMem(BGM1);
				Game();
				break;
			case 2:
				set = 0; //���j���[
				Menu();
				break;
			case 3:
				DxLib_End(); //�Q�[���I��
				break;
			}

		}

		if (nextf > 100) nextf = 0;
		if (nextf < 50) DrawExtendFormatStringToHandle(400, 600, 2.0, 2.0, 0xffffff, Font, "�o �t �r �g�@�s �n�@�d �m �s �d �q");

		ScreenFlip();	//�\�Ɨ��i�`���ʁj�̓��ꊷ��

	}


	//DX���C�u�����I������
	DxLib_End();

	return 0;

}

//���j���[���
void Menu(void)
{
	//�ϐ��錾
	int Font1 = CreateFontToHandle("PixelMplus10-Regular.ttf", 12, 9);
	int Font2 = CreateFontToHandle("PixelMplus10-Regular.ttf", 16, 9);
	int Font3 = CreateFontToHandle("PixelMplus10-Regular.ttf", 9, 4);

	int y1 = -30, y2 = -30, y3 = -30; //�A�j���[�V�����ϐ�
	int selectf = 0; //����t���O
	int opset = 3;

	//�F�X������
	set = 1;
	keywait = 0;

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)	//OS����I���w�������邩�AESC�L�[���������܂ŌJ��Ԃ�
	{
		nextf++;
		keywait++;
		ClearDrawScreen();	//�`���ʁi����ʁj�����ŃN���A

		//���E�L�[�Ńy�[�W�ړ�
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
		
		//������@����
		if (set == 1) {
			//�㉺�L�[�Őݒ�ӏ���I��
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
			DrawExtendFormatStringToHandle(300, 50, 2.0, 2.0, 0xffffff, Font1, "�f �` �l �d�@�n �o �d �q �` �s �h �n �m");

			DrawExtendFormatStringToHandle(150, 160, 2.0, 2.0, 0xffffff, Font2, "�` �b �s �h �n �m �F ���^�� �j �d �x");
			DrawExtendFormatStringToHandle(150, 240, 2.0, 2.0, 0xffffff, Font2, "�r �g �n �n �s �F �r �o �` �r �d�@�j �d �x");
			DrawExtendFormatStringToHandle(150, 320, 2.0, 2.0, 0xffffff, Font2, "�o �` �t �r �d �F �a �` �b �j�@�j �d �x");
		}
		//�G�@�ݒ�
		else if (set == 2) {
			//�㉺�L�[�Őݒ�ӏ���I��
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
			//���E�L�[�Őݒ�ύX(�G�@��Փx)
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
			//���E�L�[�Őݒ�ύX(�G�@�ړ����x)
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

			//��ʕ`��
			DrawExtendFormatStringToHandle(300, 50, 2.0, 2.0, 0xffffff, Font1, "�b �n �m �e �h �f �t �q �` �s �h �n �m");

			DrawExtendFormatStringToHandle(100, 120, 2.0, 2.0, 0xffffff, Font2, "�d �m �d �l �x�@�r �d �s");
			if (opset == 1) DrawExtendFormatStringToHandle(140, 210, 2.0, 2.0, 0xffffff, Font2, "�r �s �q �d �m �f �s �g : ��%s��", es[esl]);
			else DrawExtendFormatStringToHandle(150, 230, 2.0, 2.0, 0xffffff, Font2, "�r �s �q �d �m �f �s �g : �@%s�@", es[esl]);
			if (opset == 2) DrawExtendFormatStringToHandle(140, 300, 2.0, 2.0, 0xffffff, Font2, "�r �o �d �d �c �F ��%s��", em[emove]);
			else DrawExtendFormatStringToHandle(150, 300, 2.0, 2.0, 0xffffff, Font2, "�r �o �d �d �c �F �@%s�@", em[emove]);


		}
		//���@�ݒ�
		else if (set == 3) {
			//�㉺�L�[�Őݒ����I��
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
			//���E�L�[�Őݒ�ύX(���@�F)
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
			//���E�L�[�Őݒ�ύX(�e�F)
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

			//��ʕ`��
			DrawExtendFormatStringToHandle(300, 50, 2.0, 2.0, 0xffffff, Font1, "�b �n �m �e �h �f �t �q �` �s �h �n �m");

			DrawExtendFormatStringToHandle(100, 120, 2.0, 2.0, 0xffffff, Font2, "�b �n �k �n �q�@�r �d �k �d �b �s");
			if(opset==1) DrawExtendFormatStringToHandle(140, 220, 2.0, 2.0, 0xffffff, Font2, "�r �j �h �m �F ���@�@�@�@�@��");
			else DrawExtendFormatStringToHandle(150, 230, 2.0, 2.0, 0xffffff, Font2, "�r �j �h �m �F");
			if(opset==2) DrawExtendFormatStringToHandle(140, 340, 2.0, 2.0, 0xffffff, Font2, "�a �t �k �k �d �s �F ���@�@�@�@�@��");
			else DrawExtendFormatStringToHandle(150, 350, 2.0, 2.0, 0xffffff, Font2, "�a �t �k �k �d �s �F");
			DrawTriangle(530, 230 - 40 / sqrt(2), 530 - 40, 230 + 40, 530 + 40, 230 + 40, skincolor[cl], TRUE);
			DrawCircle(630, 360, 10, bulletcolor[bl], TRUE);
		}

		//��ʕ`��
		if (opset == 3) {
			DrawExtendFormatStringToHandle(480, 550, 2.0, 2.0, 0xffffff, Font3, "�@�o �` �f �d�@\n���@ %d�^ 3�@��", set);
		}
		else {
			DrawExtendFormatStringToHandle(480, 550, 2.0, 2.0, 0xffffff, Font3, "�@�o �` �f �d�@\n�@�@ %d�^ 3�@�@", set);
		}

		if (opset == 4) {
			DrawExtendFormatStringToHandle(480, 600, 2.0, 2.0, 0xffffff, Font3, "< �a �` �b �j >");
			if (CHECK == 1) {
				PlaySoundMem(SE4, DX_PLAYTYPE_BACK);
				set = 0;
				return;
			}
		}
		else {
			DrawExtendFormatStringToHandle(480, 600, 2.0, 2.0, 0xffffff, Font3, "  �a �` �b �j  ");
		}
		

		ScreenFlip();	//�\�Ɨ��i�`���ʁj�̓��ꊷ��
	}

}

//�Q�[������
void Game(void) {

	cf = -1;//�N���A�t���O������
	nohit = 0;
	damhit = 0;

	int Font1 = CreateFontToHandle("PixelMplus10-Regular.ttf", 24, 6); //72
	int Font2 = CreateFontToHandle("PixelMplus10-Regular.ttf", 16, 6); //64
	int Font3 = CreateFontToHandle("PixelMplus10-Regular.ttf", 14, 6); //58
	int Font4 = CreateFontToHandle("PixelMplus10-Regular.ttf", 14, 8); //42
	int Font5 = CreateFontToHandle("PixelMplus10-Regular.ttf", 10, 4); //20

	//�ϐ��錾
	int i; //���[�v�����p
	int f; //�C�x���g�t���O
	int jx = 560, jy = 650, jl = 0; //���@�p
	int mx = 0, my = 0, mf = 0; //�~�T�C��(�ʏ�)
	int rmf[100], rmw[100], rmx[100], rmy[100], rmwait = 0; //�~�T�C��(�A��)2%
	int tmx = 0, tmy = 0, tmf = 0, tmwait = 0; //�~�T�C��(�ǔ�)3%
	float tmr; //�~�T�C��(�ǔ�)3%
	float smx[100], smy[100], smr[100]; //�~�T�C��(�U�e)5%
	int smf[100], smwait = 0; //�~�T�C��(�U�e)5%
	int kmx = 0, kmy = 0, kmf = 0; //�~�T�C��(����)10%
	int remx = 0, remy = 0, remf = 0, ref = 0; //�񕜋�20%
	int a = 100; //���@�~�T�C���Q�[�W�����p
	int itemx = 0, itemy = 0, itemf = 0, item = 0;
	int ex = GetRand(1020) + 50, ey = -20, ed = 1, el = 0; //�G�@�p
	int emx = 0, emy = 0, emf = 0; //�G�~�T�C��
	int eef = 0;
	int b = 100; //�G�@�~�T�C���Q�[�W�����p
	float d; //�����蔻��p

	int bf = 0; //���@�~�T�C���ύX�t���O

	int gh; //�|�[�Y���̃O���t�B�b�N�ۑ�
	int pf = 0; //�|�[�Y�t���O

	//�J�E���g�_�E���p�ϐ�
	int now;
	int count;

	//�~�T�C���p�ϐ�������
	for (i = 0; i < 99; i++) {
		rmf[i] = 0;
		rmw[i] = 0;
		smf[i] = 0;
	}

	//��̃O���t�B�b�N�f�[�^���쐬(1120�~630)
	gh = MakeGraph(1120, 630);

	//�J�E���g�_�E������
	now = GetNowCount();
	while (GetNowCount() - now < 4000) {
		ClearDrawScreen();	//�`���ʁi����ʁj�����ŃN���A

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

		//���@�̗́E�e�ۃQ�[�W
		DrawBox(30, 600, 1050, 620, 0xffffff, TRUE);
		DrawBox(30, 600, jl * 2 + 50, 620, 0x00ff00, TRUE);
		DrawTriangle(jl * 2 + 30, 600, jl * 2 + 50, 600, jl * 2 + 50, 620, 0xffffff, TRUE);
		DrawBox(10, 580, 330, 600, 0xffffff, TRUE);
		if (count > 3000)DrawBox(10, 580, a * 3 + 30, 600, bulletcolor[bl], TRUE);
		DrawTriangle(a * 3 + 10, 580, a * 3 + 30, 580, a * 3 + 30, 600, 0xffffff, TRUE);
		DrawTriangle(10, 620, 10, 580, 50, 620, 0x000000, TRUE);
		DrawTriangle(310, 580, 330, 580, 330, 600, 0x000000, TRUE);
		DrawTriangle(1030, 600, 1050, 600, 1050, 620, 0x000000, TRUE);
		//�G�@�̗́E�e�ۃQ�[�W
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
			DrawExtendFormatStringToHandle(530, 280, 3.0, 3.0, 0xffffff, Font4, "�R");
		}
		else if (count < 2000) {
			DrawExtendFormatStringToHandle(520, 270, 4.0, 4.0, 0xffffff, Font3, "�Q");
		}
		else if (count < 3000) {
			DrawExtendFormatStringToHandle(515, 265, 4.0, 4.0, 0xffffff, Font2, "�P");
		}
		else if (count < 4000) {
			DrawExtendFormatStringToHandle(370, 260, 3.0, 3.0, 0xffffff, Font1, "�r�s�`�q�s�I");
		}

		ScreenFlip();
	}

	PlaySoundMem(BGM2, DX_PLAYTYPE_LOOP);

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)	//OS����I���w�������邩�AESC�L�[���������܂ŌJ��Ԃ�
	{

		rmwait++; //�~�T�C�����ˑҋ@(�A��)
		smwait++; //�~�T�C�����ˑҋ@(�U�e)
		eef++;


		ClearDrawScreen();	//�`���ʁi����ʁj�����ŃN���A

		//�Q�[���I�[�o�[����
		if (cf == 0) {
			nextf++;
			//���@�`��
			if (nextf % 2 == 0) {
				DrawTriangle(jx, jy - 40 / sqrt(2), jx - 40, jy + 40, jx + 40, jy + 40, skincolor[cl], TRUE);
			}
			jy++;

			//�G�@�`��
			DrawTriangle(ex, ey + 40 / sqrt(2), ex - 40, ey - 40, ex + 40, ey - 40, 0x0000ff, TRUE);
			ex = ex + 5 * ed;
			if (ex >= 1070) ed *= -1;
			if (ex <= 50) ed *= -1;

			//���@�̗́E�e�ۃQ�[�W
			DrawBox(30, 600, 1050, 620, 0xffffff, TRUE);
			DrawBox(30, 600, jl + 50, 620, 0x00ff00, TRUE);
			DrawTriangle(jl + 30, 600, jl + 50, 600, jl + 50, 620, 0xffffff, TRUE);
			DrawBox(10, 580, 330, 600, 0xffffff, TRUE);
			DrawBox(10, 580, a * 3 + 30, 600, bulletcolor[bl], TRUE);
			DrawTriangle(a * 3 + 10, 580, a * 3 + 30, 580, a * 3 + 30, 600, 0xffffff, TRUE);
			DrawTriangle(10, 620, 10, 580, 50, 620, 0x000000, TRUE);
			DrawTriangle(310, 580, 330, 580, 330, 600, 0x000000, TRUE);
			DrawTriangle(1030, 600, 1050, 600, 1050, 620, 0x000000, TRUE);
			//�G�@�̗́E�e�ۃQ�[�W
			DrawBox(1090, 10, 70, 30, 0xffffff, TRUE);
			DrawBox(1090, 10, 1070 - el, 30, 0x00ff00, TRUE);
			DrawTriangle(1070 - el, 10, 1070 - el, 30, 1090 - el, 30, 0xffffff, TRUE);
			DrawBox(1110, 30, 790, 50, 0xffffff, TRUE);
			DrawBox(1110, 30, 1090 - b * 3, 50, 0x00ffff, TRUE);
			DrawTriangle(1090 - b * 3, 30, 1090 - b * 3, 50, 1110 - b * 3, 50, 0xffffff, TRUE);
			DrawTriangle(1070, 10, 1110, 10, 1110, 50, 0x000000, TRUE);
			DrawTriangle(790, 30, 790, 50, 810, 50, 0x000000, TRUE);
			DrawTriangle(70, 10, 70, 30, 90, 30, 0x000000, TRUE);

			DrawExtendFormatStringToHandle(350, 200, 3.0, 3.0, 0x0000ff, Font1, " �f�`�l�d \n�n�u�d�q�c");

			if (jy > 700) {
				Result();
				return;
			}
		}
		//�Q�[���N���A����
		else if (cf == 1) {
			nextf++;

			//���@�`��
			DrawTriangle(jx, jy - 40 / sqrt(2), jx - 40, jy + 40, jx + 40, jy + 40, skincolor[cl], TRUE);

			//�G�@�`��
			if (nextf % 2 == 0) {
				DrawTriangle(ex, ey + 40 / sqrt(2), ex - 40, ey - 40, ex + 40, ey - 40, 0x0000ff, TRUE);
			}
			ey--;

			//���@�̗́E�e�ۃQ�[�W
			DrawBox(30, 600, 1050, 620, 0xffffff, TRUE);
			DrawBox(30, 600, jl + 50, 620, 0x00ff00, TRUE);
			DrawTriangle(jl + 30, 600, jl + 50, 600, jl + 50, 620, 0xffffff, TRUE);
			DrawBox(10, 580, 330, 600, 0xffffff, TRUE);
			DrawBox(10, 580, a * 3 + 30, 600, bulletcolor[bl], TRUE);
			DrawTriangle(a * 3 + 10, 580, a * 3 + 30, 580, a * 3 + 30, 600, 0xffffff, TRUE);
			DrawTriangle(10, 620, 10, 580, 50, 620, 0x000000, TRUE);
			DrawTriangle(310, 580, 330, 580, 330, 600, 0x000000, TRUE);
			DrawTriangle(1030, 600, 1050, 600, 1050, 620, 0x000000, TRUE);
			//�G�@�̗́E�e�ۃQ�[�W
			DrawBox(1090, 10, 70, 30, 0xffffff, TRUE);
			DrawBox(1090, 10, 1070 - el, 30, 0x00ff00, TRUE);
			DrawTriangle(1070 - el, 10, 1070 - el, 30, 1090 - el, 30, 0xffffff, TRUE);
			DrawBox(1110, 30, 790, 50, 0xffffff, TRUE);
			DrawBox(1110, 30, 1090 - b * 3, 50, 0x00ffff, TRUE);
			DrawTriangle(1090 - b * 3, 30, 1090 - b * 3, 50, 1110 - b * 3, 50, 0xffffff, TRUE);
			DrawTriangle(1070, 10, 1110, 10, 1110, 50, 0x000000, TRUE);
			DrawTriangle(790, 30, 790, 50, 810, 50, 0x000000, TRUE);
			DrawTriangle(70, 10, 70, 30, 90, 30, 0x000000, TRUE);

			DrawExtendFormatStringToHandle(350, 200, 3.0, 3.0, 0xff0000, Font1, "�@�f�`�l�d�@\n�b�k�d�`�q�I");

			if (ey < -20) {
				Result();
				return;
			}


		}
		//�ʏ폈��
		else {
			nextf++;
			SetFontSize(16);

			//���@�`��
			//if (UP == 1 && jy >= 10 + 40 / sqrt(2)) jy -= 10;
			//if (DOWN == 1 && jy <= 580) jy += 10;
			if (RIGHT == 1 && jx <= 1070) jx += 10;
			if (LEFT == 1 && jx >= 50) jx -= 10;
			DrawTriangle(jx, jy - 40 / sqrt(2), jx - 40, jy + 40, jx + 40, jy + 40, skincolor[cl], TRUE);

			//���@�~�T�C������(�ʏ�)
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
				//�G�@�ւ̓����蔻��
				d = Root(mx, my, ex, ey);//sqrt((float)((ex - mx) * (ex - mx) + (ey - my) * (ey - my)));
				if (d <= 50.0) {
					DrawCircle(mx, my, 30, 0xffff00, TRUE);
					mf = 0;
					a = 100;
					nohit++;
					//�G�_���[�W����
					el--;
					//�C�x���g����
					f = GetRand(99); //0�`99�Ń����_���Ƀt���O�𗧂Ă�
					if (f < 20) ref = 1; // �t���t��20������kf��1�ɂ���(20%)
					else if (f < 40) item = 1; // �t���t��20�ȏ�40������rf��1�ɂ���(1%)
				}
			}

			//�񕜃C�x���g���� �m��20%
			if (remf == 0 && ref == 1) {
				remx = ex;
				remy = ey + 40 / sqrt(2);
				remf = 1;
			}
			if (remf == 1) {
				SetFontSize(24);
				DrawFormatString(remx - 12, remy - 12, 0x00ff00, "�{");
				//DrawCircle(remx, remy, 10, GetColor(0, GetRand(255), 0), TRUE);
				remy++;
				ref = 0;
				if (remy >= 640) remf = 0;
				//���@�ւ̓����蔻��
				d = Root(remx, remy, jx, jy);//d = sqrt((float)((jx - remx) * (jx - remx) + (jy - remy) * (jy - remy)));
				if (d <= 50.0) {
					PlaySoundMem(SE5, DX_PLAYTYPE_BACK);
					remf = 0;
					if (jl < 500)jl += 5;
					DrawBox(0, 0, 1120, 630, 0x00ff00, TRUE);
				}
			}

			//����e�C�x���g���� �m��20%
			if (itemf == 0 && item == 1) {
				itemx = ex;
				itemy = ey + 40 / sqrt(2);
				itemf = 1;
			}
			if (itemf == 1) {
				SetFontSize(36);
				DrawFormatString(itemx - 18, itemy - 18, GetRand(0xffffff), "�H");
				//DrawCircle(remx, remy, 10, GetColor(0, GetRand(255), 0), TRUE);
				itemy++;
				item = 0;
				if (itemy >= 640) itemf = 0;
				//���@�ւ̓����蔻��
				d = Root(itemx, itemy, jx, jy);//d = sqrt((float)((jx - itemx) * (jx - itemx) + (jy - itemy) * (jy - itemy)));
				if (d <= 50.0) {
					PlaySoundMem(SE6, DX_PLAYTYPE_BACK);
					itemf = 0;
					f = GetRand(9); //0�`9�Ń����_���Ƀt���O�𗧂Ă�
					if (f < 2)	bf = 1; // �t���t��2������rf��1�ɂ���(1%)
					else if (f < 3) bf = 2; // �t���t��3������tf��1�ɂ���(3%)
					else if (f < 5) bf = 3; // �t���t��5������sf��1�ɂ���(5%)
					else if (f < 10) bf = 4; // �t���t��10������sf��1�ɂ���(10%)
					DrawBox(0, 0, 1120, 630, 0xffffff, TRUE);
				}
			}

			//���@�~�T�C������(�A��)�m��2%
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
					//�G�@�ւ̓����蔻��
					d = Root(rmx[i], rmy[i], ex, ey);//d = sqrt((float)((ex - rmx[i]) * (ex - rmx[i]) + (ey - rmy[i]) * (ey - rmy[i])));
					if (d <= 50.0) {
						PlaySoundMem(SE3, DX_PLAYTYPE_BACK);
						DrawCircle(rmx[i], rmy[i], 30, 0xffff00, TRUE);
						rmf[i] = 0;
						nohit++;
						//�G�_���[�W����
						el -= 15;
						//�񕜃C�x���g����
						f = GetRand(99); //0�`99���烉���_���Ƀt���O�𗧂Ă�
						if (f < 20) ref = 1; // �t���t��20������kf��1�ɂ���(20%)
					}
					if (a <= 0 && rmf[i] == 0) {
						bf = 0;
					}
				}
			}
			//���@�~�T�C������(�ǔ�)�m��3%
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
					//�G�_���[�W����
					el -= 5;
					//�񕜃C�x���g����
					f = GetRand(99); //0�`99���烉���_���Ƀt���O�𗧂Ă�
					if (f < 20) ref = 1; // �t���t��20������kf��1�ɂ���(20%)
				}
				if (a <= 0 && tmf == 0) {
					bf = 0;
				}
			}

			////�~�T�C������(�U�e)�m��5%
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
						//�G�_���[�W����
						el -= 3;
						//�񕜃C�x���g����
						f = GetRand(99); //0�`99���烉���_���Ƀt���O�𗧂Ă�
						if (f < 20) ref = 1; // �t���t��20������kf��1�ɂ���(20%)
					}
					if (a <= 0 && smf[0] == 0) {
						bf = 0;
					}
				}
			}

			//�~�T�C������(�U�e)�m��5%
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
			//			//�G�_���[�W����
			//			el -= 3;
			//			//�񕜃C�x���g����
			//			f = GetRand(99); //0�`99���烉���_���Ƀt���O�𗧂Ă�
			//			if (f < 20) ref = 1; // �t���t��20������kf��1�ɂ���(20%)
			//		}
			//		if (a <= 0 && smf[0] == 0) {
			//			bf = 0;
			//		}
			//	}
			//}

			//�~�T�C������(����)�m��10%
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
				//�G�@�ւ̓����蔻��
				d = Root(kmx, kmy, ex, ey); //d = sqrt((float)((ex - kmx) * (ex - kmx) + (ey - kmy) * (ey - kmy)));
				if (d <= 50.0) {
					PlaySoundMem(SE3, DX_PLAYTYPE_BACK);
					DrawCircle(kmx, kmy, 30, 0xffff00, TRUE);
					kmf = 0;
					nohit++;
					//�G�_���[�W����
					el -= 10;
					//�񕜃C�x���g����
					f = GetRand(99); //0�`99���烉���_���Ƀt���O�𗧂Ă�
					if (f < 20) ref = 1; // �t���t��20������kf��1�ɂ���(20%)
				}
				if (a <= 0 && kmf == 0) {
					bf = 0;
				}
			}

			//�G�@�`��
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

			//�G�@�~�T�C������
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
				//���@�ւ̓����蔻��
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

			//������ʔ������o��
			if (bf == 0) DrawExtendFormatStringToHandle(330, 580, 2.0, 2.0, bulletcolor[bl], Font5, "NORMAL:%d",a/100);
			if (bf == 1) DrawExtendFormatStringToHandle(330, 580, 2.0, 2.0, bulletcolor[bl], Font5, "RAPID:%d",a/2);
			if (bf == 2) DrawExtendFormatStringToHandle(330, 580, 2.0, 2.0, bulletcolor[bl], Font5, "TRACK:%d",a/20);
			if (bf == 4) DrawExtendFormatStringToHandle(330, 580, 2.0, 2.0, bulletcolor[bl], Font5, "SPEED:%d",a/10);
			if (bf == 3) DrawExtendFormatStringToHandle(330, 580, 2.0, 2.0, bulletcolor[bl], Font5, "SCATTER:%d",a);

			//���@�̗́E�e�ۃQ�[�W
			DrawBox(30, 600, 1050, 620, 0xffffff, TRUE);
			DrawBox(30, 600, jl*2 + 50, 620, 0x00ff00, TRUE);
			DrawTriangle(jl * 2 + 30, 600, jl * 2 + 50, 600, jl * 2 + 50, 620, 0xffffff, TRUE);
			DrawBox(10, 580, 330, 600, 0xffffff, TRUE);
			DrawBox(10, 580, a * 3 + 30, 600, bulletcolor[bl], TRUE);
			DrawTriangle(a * 3 + 10, 580, a * 3 + 30, 580, a * 3 + 30, 600, 0xffffff, TRUE);
			DrawTriangle(10, 620, 10, 580, 50, 620, 0x000000, TRUE);
			DrawTriangle(310, 580, 330, 580, 330, 600, 0x000000, TRUE);
			DrawTriangle(1030, 600, 1050, 600, 1050, 620, 0x000000, TRUE);
			//�G�@�̗́E�e�ۃQ�[�W
			DrawBox(1090, 10, 70, 30, 0xffffff, TRUE);
			DrawBox(1090, 10, 1070 - el * 2, 30, 0x00ff00, TRUE);
			DrawTriangle(1070 - el * 2, 10, 1070 - el * 2, 30, 1090 - el * 2, 30, 0xffffff, TRUE);
			DrawBox(1110, 30, 790, 50, 0xffffff, TRUE);
			DrawBox(1110, 30, 1090 - b * 3, 50, 0x00ffff, TRUE);
			DrawTriangle(1090 - b * 3, 30, 1090 - b * 3, 50, 1110 - b * 3, 50, 0xffffff, TRUE);
			DrawTriangle(1070, 10, 1110, 10, 1110, 50, 0x000000, TRUE);
			DrawTriangle(790, 30, 790, 50, 810, 50, 0x000000, TRUE);
			DrawTriangle(70, 10, 70, 30, 90, 30, 0x000000, TRUE);


			//�|�[�Y�@�\
			if (PAUSE == 1)	//����������������������������������������������
			{
				StopSoundMem(BGM2);
				GetDrawScreenGraph(0, 0, 1120, 630, gh);
				pf = 0;
				PlaySoundMem(SE9,DX_PLAYTYPE_BACK);
				
				while (ProcessMessage() == 0 && pf != 1)	//OS����I���w�������邩�AB�L�[���������܂ŌJ��Ԃ�
				{
					SetFontSize(16);
					//ClearDrawScreen();	//�`���ʁi����ʁj�����ŃN���A
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
					DrawExtendFormatStringToHandle(100, 240, 4.0, 4.0, GetRand(0xffffff), Font2, "�o�@�`�@�t�@�r�@�d");

					SetFontSize(16);
					if (set == 1) DrawExtendFormatStringToHandle(750, 190, 2.0, 2.0, 0xffffff, Font5, "�a�`�b�j�@�s�h�s�k�d");
					else DrawExtendFormatStringToHandle(800, 200, 2.0, 2.0, 0xffffff, Font5, "�a�`�b�j�@�s�h�s�k�d");
					if (set == 2) DrawExtendFormatStringToHandle(750, 490, 2.0, 2.0, 0xffffff, Font5, "�f�`�l�d�@�q�d�r�s�`�q�s");
					else DrawExtendFormatStringToHandle(800, 500, 2.0, 2.0, 0xffffff, Font5, "�f�`�l�d�@�q�d�r�s�`�q�s");

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

		ScreenFlip();	//�\�Ɨ��i�`���ʁj�̓��ꊷ��

	}

}

//���U���g����(�����߂�����t����)
void Result(void)
{

	int Font1 = CreateFontToHandle("PixelMplus10-Regular.ttf", 16, 6);
	int Font2 = CreateFontToHandle("PixelMplus10-Regular.ttf", 12, 6);
	int Font3 = CreateFontToHandle("PixelMplus10-Regular.ttf", 10, 6);

	int x1 = 1130, x2 = 1130, x3 = 1130;

	if (cf == 1) PlaySoundMem(BGM3, DX_PLAYTYPE_LOOP);
	if (cf == 0) PlaySoundMem(BGM4, DX_PLAYTYPE_LOOP);

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)	//OS����I���w�������邩�AESC�L�[���������܂ŌJ��Ԃ�
	{
		ClearDrawScreen();	//�`���ʁi����ʁj�����ŃN���A

		if (x1 >= 50) x1 -= 10;
		if (x1 <= 50 && x2 > 200) x2 -= 10;
		if (x2 <= 200 && x3 > 250) x3 -= 10;

		switch (cf)
		{
		case 0:
			SetFontSize(32);
			DrawExtendFormatStringToHandle(x1, 100, 2.0, 2.0, 0x0000ff, Font1, "�f �` �l �d�@�e �` �h �k �d �c"); //DrawFormatString(x1, 100, 0x0000ff, "�f �` �l �d�@�e �` �h �k �d �c");
			SetFontSize(24);
			DrawExtendFormatStringToHandle(x1 + 50, 150, 2.0, 2.0, 0x00ffff, Font2, "�k �d �u �d �k �F %s �| %s", es[esl], em[emove]); //DrawFormatString(x1 + 50, 150, 0x00ffff, "�k �d �u �d �k �F %s �| %s", es[esl], em[emove]);

			break;
		case 1:
			SetFontSize(32);
			DrawExtendFormatStringToHandle(x1, 100, 2.0, 2.0, skincolor[cl], Font1, "�f �` �l �d�@�r �t �b �b �d �r �r �e �t �k"); //DrawFormatString(x1, 100, skincolor[cl], "�f �` �l �d�@�r �t �b �b �d �r �r �e �t �k");
			SetFontSize(24);
			DrawExtendFormatStringToHandle(x1 + 50, 150, 2.0, 2.0, bulletcolor[bl], Font2, "�k �d �u �d �k �F %s �| %s", es[esl], em[emove]); //DrawFormatString(x1 + 50, 150, bulletcolor[bl], "�k �d �u �d �k �F %s �| %s", es[esl], em[emove]);

			break;
		}

		SetFontSize(24);
		DrawExtendFormatStringToHandle(x2, 300, 2.0, 2.0, 0xff0000, Font2, "�m �t �l �a �d �q�@�n �e�@�g �h �s �F%d", nohit); //DrawFormatString(x2, 300, 0xff0000, "�m �t �l �a �d �q�@�n �e�@�g �h �s �F%d", nohit);

		SetFontSize(24);
		DrawExtendFormatStringToHandle(x3, 350, 2.0, 2.0, 0x0000ff, Font2, "�c �` �l �` �f �d�@�g �h �s �F%d", damhit); //DrawFormatString(x3, 350, 0x0000ff, "�c �` �l �` �f �d�@�g �h �s �F%d", damhit);

		if (x3 <= 250) {
			SetFontSize(16);
			DrawExtendFormatStringToHandle(400, 550, 2.0, 2.0, 0xffffff, Font3, "�o �t �r �g�@�s �n�@�d �m �s �d �q"); //DrawFormatString(400, 550, 0xffffff, "�o �t �r �g�@�s �n�@�d �m �s �d �q");

			if (CHECK == 1) {
				StopSoundMem(BGM3);
				StopSoundMem(BGM4);
				PlaySoundMem(SE4, DX_PLAYTYPE_BACK);
				PlaySoundMem(BGM1, DX_PLAYTYPE_LOOP);
				return;
			}
		}

		ScreenFlip();	//�\�Ɨ��i�`���ʁj�̓��ꊷ��
	}

}

float Root(int missx, int missy, int x, int y) {

	float d;

	d = sqrt((float)((x - missx) * (x - missx) + (y - missy) * (y - missy)));

	return d;

}
