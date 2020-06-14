/* c_main.c -- credits intro */

#include "doomdef.h"
#include "r_local.h"

psxobj_t idcredpic[2];  //80097910, 80097930
psxobj_t wmscredpic[2]; //80097950, 80097970
psxobj_t levelcredpic;  //8009a6f0

int c_vframe1 = 0;	    //80077AFC iGp000006ec
int c_yscroll;		    //80078080 uGp00000c70
int c_next;		        //80077FCC uGp00000bbc

/*
=======================
=
= C_Start
=
=======================
*/

void C_Start(void)//L80036E68()
{
    ImageToVram(&titlepic, "TITLE", 0);
	ImageToVram(&idcredpic[0], "IDCRED1", 0);
	ImageToVram(&idcredpic[1], "IDCRED2", 0);
	ImageToVram(&wmscredpic[0], "WMSCRED1", 0);
	ImageToVram(&wmscredpic[1], "WMSCRED2", 0);
	ImageToVram(&levelcredpic, "LEVCRED2", 0);

	c_yscroll = 240;//80078080
	c_next = 0;//80077FCC

	psxcd_play_at_andloop(CD_TRACK[cdmusic_credits_demo],CDVolume,0,0,CD_TRACK[cdmusic_credits_demo],CDVolume,0,0);
	do {} while (psxcd_elapsed_sectors() == 0);
}

/*
=======================
=
= C_Stop
=
=======================
*/

void C_Stop(int exit)//L80036F5C()
{
	psxcd_stop();
}

/*
=======================
=
= C_Ticker
=
=======================
*/

int C_Ticker(void)//L80036F7C()
{
    int exit;

    exit = ga_nothing;

    if (ticbuttons[0])
    {
        exit = ga_exit;
    }
    else
    {
        c_vframe1 -= vblsinframe[0];
        if (c_vframe1 <= 0)
        {
            c_vframe1 = 2;
            c_yscroll--;
            if (c_next > -1)
            {
                if (c_next < 2)
                {
                    if (c_yscroll < -256)
                    {
                        c_vframe1 = 2;
                        c_next += 1;
                        c_yscroll = 240;
                    }
                }
                else
                {
                    if (c_next != 2)
                    {
                        c_vframe1 = 2;
                    }
                    else if (c_yscroll < -256)
                    {
                        c_vframe1 = 2;
                        exit = ga_exitdemo;
                    }
                }
            }
        }
    }

    return exit;
}

/*
=======================
=
= C_Draw
=
=======================
*/

void C_Draw(void) //L8003701C()
{
    int xpos;
    int pal;
    psxobj_t *pic;

    NextTextureCacheIdx();

    if (c_next == 0)
    {
        DrawStaticImage(&titlepic,0,0,(int)palette[17]);
        pic = &levelcredpic;
        pal = palette[19];
        xpos = 11;
    }
    else if (c_next == 1)
    {
        DrawStaticImage(&wmscredpic[0],0,0,(int)palette[19]);
        pic = &wmscredpic[1];
        pal = palette[19];
        xpos = 5;
    }
    else if (c_next == 2)
    {
        DrawStaticImage(&idcredpic[0],0,0,(int)palette[18]);
        pic = &idcredpic[1];
        pal = palette[19];
        xpos = 9;
    }

    DrawStaticImage(pic, xpos, c_yscroll, pal);

    UpdateDrawOTag();
    DrawRender();
}
