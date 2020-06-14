/* o_main.c -- options menu */

#include "doomdef.h"
#include "p_local.h"
#include "st_main.h"
#include "r_local.h"

#define MOVEWAIT	5
#define ITEMSPACE	40
#define SLIDEWIDTH 90

#define SetVolPsx(vol) ((int)((vol*0x7f)/100))

char optionsnames[][16] = //8007491C
{
	"Music Volume",
	"Sound Volume",
	"Password",
	//"Configuration",
	"Mouse",
	"Control Pad",
	"Main Menu",
	"Restart Level"
};

typedef struct
{
	int	casepos; //*
	int x;		 //*4
	int y;		 //*8
} menuitem_t;

#define MAX_INTRO 6
#define MAX_SINGLE 7
#define MAX_NET 4

menuitem_t menu_intro[MAX_INTRO] = //80075328
{
	{ 0, 62, 55 },
	{ 1, 62, 95 },
	{ 2, 62, 135 },
	{ 3, 62, 160 },
	{ 4, 62, 185 },
	{ 5, 62, 210 },
};

menuitem_t menu_single[MAX_SINGLE] = //80075370
{
    { 0, 62, 45 },
	{ 1, 62, 85 },
	{ 2, 62, 125 },
	{ 3, 62, 145 },
	{ 4, 62, 165 },
	{ 5, 62, 185 },
	{ 6, 62, 205 },
};

menuitem_t menu_net[MAX_NET] = //800753C4
{
	{ 0, 62, 70 },
	{ 1, 62, 110 },
	{ 5, 62, 150 },
	{ 6, 62, 175 }
};

menuitem_t *menuitem;   //800780E4 puGp00000cd4
int itemlines;          //80077F40 uGp00000b30

/*
=================
=
= O_Start
=
=================
*/

void O_Start(void)//L8003F4B8()
{
	int i;

	S_StartSound(NULL, sfx_pistol);

	cursorframe = 0;

	for (i = 0; i < MAXPLAYERS; i++)
	{
		m_vframe1[i] = 0;
		cursorpos[i] = 0;
	}

	if (netgame == gt_single)
    {
        if (gamepaused)
		{
			menuitem = menu_single;
			itemlines = MAX_SINGLE;
		}
		else
		{
			menuitem = menu_intro;
			itemlines = MAX_INTRO;
		}
    }
	else
	{
		menuitem = menu_net;
		itemlines = MAX_NET;
	}
}

/*
=================
=
= O_Stop
=
=================
*/

void O_Stop(int exit)//L8003F578()
{
	int i;

	for (i = 0; i < MAXPLAYERS; i++)
		cursorpos[i] = 0;
}

/*
=================
=
= O_Ticker
=
=================
*/

int O_Ticker(void)//L8003F59C()
{
	int i, buttons, oldbuttons;

    /* animate skull */
    if ((gamevbls < (int)gametic) && ((gametic & 3) == 0))
        cursorframe ^= 1;

    //for (i = 0; i < MAXPLAYERS; i++)
    for (i=(MAXPLAYERS-1) ; i >= 0 ; i--)
    {
        if (playeringame[i] != 0)
        {
            buttons = Get_PadButtons(&ticbuttons[i]); //ticbuttons[i];
            oldbuttons = (unsigned short)oldticbuttons[i];

            if ((buttons != oldbuttons) && (buttons & (PAD_START|PAD_SELECT)))
            {
                S_StartSound(0, sfx_pistol);
                return ga_exit;
            }

            /* check for movement */
            if (!(buttons & PAD_ARROWS))
            {
                m_vframe1[i] = 0;
            }
            else
            {
                m_vframe1[i] -= vblsinframe[0];
                if (m_vframe1[i] <= 0)
                {
                    m_vframe1[i] = TICRATE;

                    if (buttons & PAD_DOWN)
                    {
                        cursorpos[i]++;
                        if ((itemlines - 1) < cursorpos[i])
                            cursorpos[i] = 0;

                        if (consoleplayer == i)
                            S_StartSound(NULL, sfx_pstop);
                    }
                    else if (buttons & PAD_UP)
                    {
                        cursorpos[i]--;
                        if (cursorpos[i] < 0)
                            cursorpos[i] = (itemlines - 1);

                        if (consoleplayer == i)
                            S_StartSound(NULL, sfx_pstop);
                    }
                }
            }

            switch ((menuitem + cursorpos[i])->casepos)
            {
                case 0:	// music volume
                    if (consoleplayer == i)
                    {
                        if (buttons & PAD_RIGHT)
                        {
                            MusVolume++;
                            if (MusVolume > 100)
                                MusVolume = 100;
                            else
                            {
                                S_SetMusicVolume(SetVolPsx(MusVolume));

                                if (MusVolume & 1)
                                    S_StartSound(NULL, sfx_stnmov);
                            }

                            CDVolume = (MusVolume * 0x3cff) / 100;
                        }
                        else if (buttons & PAD_LEFT)
                        {
                            MusVolume--;
                            if (MusVolume < 0)
                                MusVolume = 0;
                            else
                            {
                                S_SetMusicVolume(SetVolPsx(MusVolume));

                                if (MusVolume & 1)
                                    S_StartSound(NULL, sfx_stnmov);
                            }

                            CDVolume = (MusVolume * 0x3cff) / 100;
                        }
                    }
                    break;

                case 1:	// sound volume
                    if (consoleplayer == i)
                    {
                        if (buttons & PAD_RIGHT)
                        {
                            SfxVolume++;
                            if (SfxVolume > 100)
                                SfxVolume = 100;
                            else
                            {
                                S_SetSoundVolume(SetVolPsx(SfxVolume));

                                if (SfxVolume & 1)
                                    S_StartSound(NULL, sfx_stnmov);
                            }
                        }
                        else if (buttons & PAD_LEFT)
                        {
                            SfxVolume--;
                            if (SfxVolume < 0)
                                SfxVolume = 0;
                            else
                            {
                                S_SetSoundVolume(SetVolPsx(SfxVolume));

                                if (SfxVolume & 1)
                                    S_StartSound(NULL, sfx_stnmov);
                            }
                        }
                    }
                    break;
                case 2://Password
                    if ((buttons != oldbuttons) && (buttons & PAD_ACTION))
                    {
                        if (MiniLoop(PW_Start, PW_Stop, PW_Ticker, PW_Drawer) == ga_warped)
                            return ga_warped;
                    }
                    break;
                case 3://Configuration Mouse
                    if ((buttons != oldbuttons) && (buttons & PAD_ACTION) &&
                        ((*(unsigned int*)&PAD_Rbuf2[0]& 0xf0ff) == 0x1000))//Check Mouse
                    {
                        MiniLoop(CFM_Start, CFM_Stop, CFM_Ticker, CFM_Drawer);
                    }
                    break;
                case 4://Configuration Pad
                    if ((buttons != oldbuttons) && (buttons & PAD_ACTION))
                    {
                        MiniLoop(CF_Start, CF_Stop, CF_Ticker, CF_Drawer);
                    }
                    break;
                case 5://Main Menu
                    if (buttons & PAD_ACTION)
                    {
                        S_StartSound(0, sfx_pistol);
                        return ga_exitdemo;
                    }
                    break;
                case 6:// Restar Level
                    if (buttons & PAD_ACTION)
                    {
                        S_StartSound(0, sfx_pistol);
                        return ga_restart;
                    }
                    break;
                default:
                    break;
            }
        }
    }
	return ga_nothing;
}


/*
=================
=
= O_Drawer
=
=================
*/

void O_Drawer(void)//L8003FAE0()
{
	int i, j;
	int	xpos, ypos;
	menuitem_t *m_item;

	NextTextureCacheIdx();

	//Draw Backround MARB01 Pic
	for (ypos = 0; ypos < 4; ypos++)
	{
		for (xpos = 0; xpos < 4; xpos++)
		{
			DrawStaticImage(&marb01pic, xpos << 6, ypos << 6, palette[20]);
		}
	}

	if (gameaction == 0)
	{
		ST_DrawText(-1, 20, "Options");

		m_item = menuitem;
		for (i = 0; i < itemlines; i++)
		{
			ST_DrawText(m_item->x, m_item->y, optionsnames[m_item->casepos]);

			if (m_item->casepos < 2)
			{
				//Draw Bar Dial
				DrawImage(statuspic.vtpage, palette[16], m_item->x + 13, m_item->y + 20, 0, 184, 108, 11);

				if (m_item->casepos == 0)
					DrawImage(statuspic.vtpage, palette[16], MusVolume + m_item->x + 14, m_item->y + 20, 108, 184, 6, 11);
				else
					DrawImage(statuspic.vtpage, palette[16], SfxVolume + m_item->x + 14, m_item->y + 20, 108, 184, 6, 11);
			}

			m_item++;
		}

		//Draw Skull
		m_item = menuitem + cursorpos[consoleplayer];
		DrawImage(statuspic.vtpage, palette[16], m_item->x - 24, m_item->y - 2, (cursorframe * M_SKULL_W) + M_SKULL_VX, M_SKULL_VY, M_SKULL_W, M_SKULL_H);
	}

	UpdateDrawOTag();
	DrawRender();
}
