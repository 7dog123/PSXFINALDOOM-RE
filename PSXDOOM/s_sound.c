/* s_sound.c */
#include "doomdef.h"
#include "p_local.h"

#define abs(x) ((x)<0 ? -(x) : (x))

int CD_TRACK[NUM_CD_MUSIC_TRACKS] = { //80074B30
    2,      // cdmusic_title_screen
    3,      // cdmusic_main_menu
    4,      // cdmusic_credits_demo
    5,      // cdmusic_intermission
    6,      // cdmusic_club_doom
    7,      // cdmusic_finale_final_doom
};

typedef struct mapaudio_s
{
	int   Sfx_lcd_indx;
	int   music_indx;
}mapaudio_t;

typedef struct music_s
{
	int   lcd_indx;
	int   midi_indx;
	int   rev_mode;
	short depthleft;
	short depthright;
}music_t;

enum Musics
{
    NO_MUSIC,
    MUSAMB01,
    MUSAMB02,
    MUSAMB03,
    MUSAMB04,
    MUSAMB05,
    MUSAMB06,
    MUSAMB07,
    MUSAMB08,
    MUSAMB09,
    MUSAMB10,
    MUSAMB11,
    MUSAMB12,
    MUSAMB13,
    MUSAMB14,
    MUSAMB15,
    MUSAMB16,
    MUSAMB17,
    MUSAMB18,
    MUSAMB19,
    MUSAMB20,
    MUSAMB21,
    MUSAMB22,
    MUSAMB23,
    MUSAMB24,
    MUSAMB25,
    MUSAMB26,
    MUSAMB27,
    MUSAMB28,
    MUSAMB29,
    MUSAMB30
};

static mapaudio_t Map_Audio[34] =  //800754FC
{
    { 0             , NO_MUSIC },
    { _MAP01_LCD    , MUSAMB01 },
    { _MAP02_LCD    , MUSAMB02 },
    { _MAP03_LCD    , MUSAMB03 },
    { _MAP04_LCD    , MUSAMB04 },
    { _MAP05_LCD    , MUSAMB05 },
    { _MAP06_LCD    , MUSAMB06 },
    { _MAP07_LCD    , MUSAMB07 },

    { _MAP10_LCD    , MUSAMB08 },//bug? beber�a ser _MAP08_LCD
    { _MAP08_LCD    , MUSAMB09 },//bug? beber�a ser _MAP09_LCD
    { _MAP09_LCD    , MUSAMB10 },//bug? beber�a ser _MAP10_LCD

    { _MAP11_LCD    , MUSAMB11 },
    { _MAP12_LCD    , MUSAMB12 },
    { _MAP13_LCD    , MUSAMB13 },
    { _MAP14_LCD    , MUSAMB14 },
    { _MAP15_LCD    , MUSAMB15 },
    { _MAP16_LCD    , MUSAMB16 },
    { _MAP17_LCD    , MUSAMB17 },
    { _MAP18_LCD    , MUSAMB18 },
    { _MAP19_LCD    , MUSAMB19 },
    { _MAP20_LCD    , MUSAMB20 },
    { _MAP21_LCD    , MUSAMB21 },
    { _MAP22_LCD    , MUSAMB22 },
    { _MAP23_LCD    , MUSAMB23 },
    { _MAP24_LCD    , MUSAMB24 },
    { _MAP25_LCD    , MUSAMB25 },
    { _MAP26_LCD    , MUSAMB26 },
    { _MAP27_LCD    , MUSAMB27 },
    { _MAP28_LCD    , MUSAMB28 },
    { _MAP29_LCD    , MUSAMB29 },
    { _MAP30_LCD    , MUSAMB30 },
    { _MAP60_LCD    , NO_MUSIC },
    { 0             , NO_MUSIC }
};


/* //DOOM
static music_t Music[31] = //8007530C
{
	{ 0  , 0  , PSXSPU_REV_MODE_OFF     , 0    , 0 },// No Music
	{ _MUSLEV1_LCD, 90 , PSXSPU_REV_MODE_SPACE   , 4095 , 4095 },// MUSAMB01
	{ _MUSLEV2_LCD, 91 , PSXSPU_REV_MODE_SPACE   , 4095 , 4095 },// MUSAMB02
	{ _MUSLEV3_LCD, 92 , PSXSPU_REV_MODE_STUDIO_B, 10239, 10239 },// MUSAMB03
	{ _MUSLEV4_LCD, 93 , PSXSPU_REV_MODE_HALL    , 6143 , 6143 },// MUSAMB04
	{ _MUSLEV5_LCD, 94 , PSXSPU_REV_MODE_STUDIO_A, 9215 , 9215 },// MUSAMB05
	{ _MUSLEV6_LCD, 95 , PSXSPU_REV_MODE_HALL    , 8191 , 8191 },// MUSAMB06
	{ _MUSLEV7_LCD, 96 , PSXSPU_REV_MODE_STUDIO_C, 9983 , 9983 },// MUSAMB07
	{ _MUSLEV8_LCD, 97 , PSXSPU_REV_MODE_STUDIO_B, 11775, 11775 },// MUSAMB08
	{ _MUSLEV9_LCD, 98 , PSXSPU_REV_MODE_STUDIO_C, 12287, 12287 },// MUSAMB09
	{ _MUSLEV10_LCD, 99 , PSXSPU_REV_MODE_SPACE   , 4095 , 4095 },// MUSAMB10
	{ _MUSLEV11_LCD, 100, PSXSPU_REV_MODE_HALL    , 8191 , 8191 },// MUSAMB11
	{ _MUSLEV12_LCD, 101, PSXSPU_REV_MODE_HALL    , 8191 , 8191 },// MUSAMB12
	{ _MUSLEV13_LCD, 102, PSXSPU_REV_MODE_SPACE   , 4095 , 4095 },// MUSAMB13
	{ _MUSLEV14_LCD, 103, PSXSPU_REV_MODE_HALL    , 8191 , 8191 },// MUSAMB14
	{ _MUSLEV15_LCD, 104, PSXSPU_REV_MODE_STUDIO_B, 10239, 10239 },// MUSAMB15
	{ _MUSLEV16_LCD, 105, PSXSPU_REV_MODE_SPACE   , 4095 , 4095 },// MUSAMB16
	{ _MUSLEV17_LCD, 106, PSXSPU_REV_MODE_HALL    , 8191 , 8191 },// MUSAMB17
	{ _MUSLEV18_LCD, 107, PSXSPU_REV_MODE_SPACE   , 4095 , 4095 },// MUSAMB18
	{ _MUSLEV19_LCD, 108, PSXSPU_REV_MODE_HALL    , 8191 , 8191 },// MUSAMB19
	{ _MUSLEV20_LCD, 109, PSXSPU_REV_MODE_STUDIO_C, 12287, 12287 },// MUSAMB20
	{ _MUSLEV19_LCD, 108, PSXSPU_REV_MODE_STUDIO_C, 12287, 12287 },// MUSAMB19
	{ _MUSLEV2_LCD, 91 , PSXSPU_REV_MODE_HALL    , 8191 , 8191 },// MUSAMB02
	{ _MUSLEV1_LCD, 90 , PSXSPU_REV_MODE_HALL    , 8191 , 8191 },// MUSAMB01
	{ _MUSLEV13_LCD, 102, PSXSPU_REV_MODE_HALL    , 8191 , 8191 },// MUSAMB13
	{ _MUSLEV7_LCD, 96 , PSXSPU_REV_MODE_STUDIO_B, 10239, 10239 },// MUSAMB07
	{ _MUSLEV16_LCD, 105, PSXSPU_REV_MODE_HALL    , 8191 , 8191 },// MUSAMB16
	{ _MUSLEV5_LCD, 94 , PSXSPU_REV_MODE_STUDIO_C, 12287, 12287 },// MUSAMB05
	{ _MUSLEV1_LCD, 90 , PSXSPU_REV_MODE_STUDIO_C, 12287, 12287 },// MUSAMB01
	{ _MUSLEV17_LCD, 106, PSXSPU_REV_MODE_STUDIO_C, 12287, 12287 },// MUSAMB17
	{ 0  , 0  , PSXSPU_REV_MODE_OFF     , 0    , 0 },// No Music
};*/

//FINAL DOOM
music_t Music[32] = //80075310
{
	{ 0  , 0  , PSXSPU_REV_MODE_OFF     , 0    , 0 },           // No Music
	{ _MUSLEV23_LCD, 112, PSXSPU_REV_MODE_HALL      , 8191 , 8191 },// MUSAMB23
	{ _MUSLEV29_LCD, 118, PSXSPU_REV_MODE_STUDIO_C  , 9983 , 9983 },// MUSAMB29
	{ _MUSLEV24_LCD, 113, PSXSPU_REV_MODE_SPACE     , 8191 , 8191 },// MUSAMB24
	{ _MUSLEV30_LCD, 119, PSXSPU_REV_MODE_SPACE     , 4095 , 4095 },// MUSAMB30
	{ _MUSLEV21_LCD, 110, PSXSPU_REV_MODE_SPACE     , 8191 , 8191 },// MUSAMB21
	{ _MUSLEV27_LCD, 116, PSXSPU_REV_MODE_HALL      , 8191 , 8191 },// MUSAMB27
	{ _MUSLEV25_LCD, 114, PSXSPU_REV_MODE_SPACE     , 4095 , 4095 },// MUSAMB25
	{ _MUSLEV28_LCD, 117, PSXSPU_REV_MODE_HALL      , 8191 , 8191 },// MUSAMB28
	{ _MUSLEV22_LCD, 111, PSXSPU_REV_MODE_SPACE     , 8191 , 8191 },// MUSAMB22
	{ _MUSLEV26_LCD, 115, PSXSPU_REV_MODE_SPACE     , 4095 , 4095 },// MUSAMB26
	{ _MUSLEV1_LCD , 90 , PSXSPU_REV_MODE_SPACE     , 4095 , 4095 },// MUSAMB01
	{ _MUSLEV2_LCD , 91 , PSXSPU_REV_MODE_SPACE     , 4095 , 4095 },// MUSAMB02
	{ _MUSLEV3_LCD , 92 , PSXSPU_REV_MODE_STUDIO_B  , 10239, 10239},// MUSAMB03
	{ _MUSLEV4_LCD , 93 , PSXSPU_REV_MODE_HALL      , 6143 , 6143 },// MUSAMB04
	{ _MUSLEV5_LCD , 94 , PSXSPU_REV_MODE_STUDIO_A  , 9215 , 9215 },// MUSAMB05
	{ _MUSLEV6_LCD , 95 , PSXSPU_REV_MODE_HALL      , 8191 , 8191 },// MUSAMB06
	{ _MUSLEV7_LCD , 96 , PSXSPU_REV_MODE_STUDIO_C  , 9983 , 9983 },// MUSAMB07
	{ _MUSLEV8_LCD , 97 , PSXSPU_REV_MODE_STUDIO_B  , 11775, 11775},// MUSAMB08
	{ _MUSLEV9_LCD , 98 , PSXSPU_REV_MODE_STUDIO_C  , 12287, 12287},// MUSAMB09
	{ _MUSLEV10_LCD, 99 , PSXSPU_REV_MODE_STUDIO_B  , 10239, 10239},// MUSAMB10
	{ _MUSLEV11_LCD, 100, PSXSPU_REV_MODE_HALL      , 8191 , 8191 },// MUSAMB11
	{ _MUSLEV12_LCD, 101, PSXSPU_REV_MODE_STUDIO_C  , 12287, 12287},// MUSAMB12
	{ _MUSLEV13_LCD, 102, PSXSPU_REV_MODE_SPACE     , 4095 , 4095 },// MUSAMB13
	{ _MUSLEV14_LCD, 103, PSXSPU_REV_MODE_HALL      , 8191 , 8191 },// MUSAMB14
	{ _MUSLEV15_LCD, 104, PSXSPU_REV_MODE_STUDIO_B  , 10239, 10239},// MUSAMB15
	{ _MUSLEV16_LCD, 105, PSXSPU_REV_MODE_SPACE     , 4095 , 4095 },// MUSAMB16
	{ _MUSLEV17_LCD, 106, PSXSPU_REV_MODE_HALL      , 8191 , 8191 },// MUSAMB17
	{ _MUSLEV18_LCD, 107, PSXSPU_REV_MODE_SPACE     , 4095 , 4095 },// MUSAMB18
	{ _MUSLEV22_LCD, 111, PSXSPU_REV_MODE_STUDIO_C  , 12287, 12287},// MUSAMB22
	{ _MUSLEV26_LCD, 115, PSXSPU_REV_MODE_STUDIO_C  , 12287, 12287},// MUSAMB26
	{ 0  , 0  , PSXSPU_REV_MODE_OFF     , 0    , 0 },// No Music
};

extern int psxsettings[SNDHW_TAG_MAX*2];
extern int *settings_list[2];

extern SampleBlock doomsfxblk;//8007eac0
extern SampleBlock mapsfxblk;//8007ec54

extern NoteState tmp_notestate;//0x8007e93c

#define WMD_SIZE 36000//26000
static int  mem_size = WMD_SIZE;//80077C34
static char mem_pntr[WMD_SIZE];//800783ac

#define SRAM_START 4112
int sram_pos = SRAM_START; //80077C48
int sram_load = 0;//80077C50

void *trackptr; //80077C4C

static int lcd_file_indx = 0;//80077C40
static int music_indx = 0;  //80077C44

static int snd_vol = 0;//80077C54 iGp00000844
static int snd_pan = 0;//80077C58 iGp00000848

static int update_snd = 0; //80077C5C

/*
==================
=
= S_SetSoundVolume
=
==================
*/

void S_SetSoundVolume (int volume) //L8003FE18()
{
    wess_master_sfx_vol_set(volume);
}

/*
==================
=
= S_SetMusicVolume
=
==================
*/

void S_SetMusicVolume(int volume)//L8003FE38()
{
    int cd_volume;

	wess_master_mus_vol_set(volume);

    cd_volume = volume << 7;
    if (0x3cff < cd_volume)
        cd_volume = 0x3cff;

	psxspu_set_cd_vol(cd_volume);
}

/*
==================
=
= S_StopMusic
=
==================
*/

void S_StopMusic(void)//L8003FE80()
{
	if (music_indx != 0)
		wess_seq_stop(Music[music_indx].midi_indx);
}

/*
==================
=
= S_StartMusic
=
==================
*/

void S_StartMusic(void)//L8003FEBC()
{
	S_StopMusic();
	if (music_indx != 0)
		wess_seq_trigger(Music[music_indx].midi_indx);
}

/*
==================
=
= S_InitBlock
=
==================
*/

void S_InitBlock(SampleBlock *sampblk)//8003FF04
{
    sampblk->numsamps = 0;
}

/*
==================
=
= S_DoBlockClear
=
==================
*/

void S_DoBlockClear(SampleBlock *sampblk)//L8003FF0C()
{
    while(sampblk->numsamps)
	{
		sampblk->numsamps--;

		//if no other buffers have the sound, clear position
		wess_dig_set_sample_position(sampblk->sampindx[sampblk->numsamps], (char *)0);
	}
}

void S_Lcd_Load(int map_indx)//L8003ff84()
{
	int sram_pos_tmp, master_vol;
	if (lcd_file_indx != map_indx)
	{
		if (lcd_file_indx != 0)
		{
			if (music_indx != 0)
			{
				S_StopMusic();
				do{} while (wess_seq_status(Music[music_indx].midi_indx) != SEQUENCE_INACTIVE);
				wess_seq_range_free(90, 30);
			}
			S_DoBlockClear(&mapsfxblk);
		}
		if (map_indx == LASTLEVEL)
		{
			S_DoBlockClear(&doomsfxblk);
			sram_load = 0;
			sram_pos = SRAM_START;
		}
		else
		{
			if (sram_load == 0)
			{
				sram_pos = wess_dig_lcd_load((char *)_DOOMSFX_LCD, (void *)SRAM_START, &doomsfxblk, 0);
				sram_pos += SRAM_START;
				sram_load = 1;
			}
		}
		sram_pos_tmp = sram_pos;

		music_indx = Map_Audio[map_indx].music_indx;
		if (music_indx == 0)
		{
			psxspu_init_reverb(0, 0, 0, 0, 0);
		}
		else
		{
		    /* Save Master Volume */
		    master_vol = psxspu_get_master_vol();

            psxspu_set_master_vol(0);
			psxspu_init_reverb(Music[music_indx].rev_mode, Music[music_indx].depthleft, Music[music_indx].depthright, 0, 0);
			wess_seq_load(Music[music_indx].midi_indx, trackptr);
			sram_pos_tmp += wess_dig_lcd_load((char *)Music[music_indx].lcd_indx, (void *)sram_pos_tmp, &mapsfxblk, 0);

			/* Restore Master Volume */
			psxspu_set_master_vol(master_vol);
		}

		lcd_file_indx = map_indx;
		if ((char *)Map_Audio[map_indx].Sfx_lcd_indx != (char *)0x0)
		{
			wess_dig_lcd_load((char *)Map_Audio[map_indx].Sfx_lcd_indx, (void *)sram_pos_tmp, &mapsfxblk, 0);
		}
	}
}

/*
==================
=
= S_Pause
=
==================
*/

void S_Pause(void)//L800401B4()
{
    wess_seq_pauseall (YesMute, &tmp_notestate);
}

/*
==================
=
= S_Resume
=
==================
*/

void S_Resume(void)//L800401DC()
{
    wess_seq_restartall (&tmp_notestate);
}

/*
==================
=
= S_StopSound
=
==================
*/

void S_StopSound(unsigned long sequence_type)//L80040204()
{
    wess_seq_stoptype(sequence_type);
}

/*
==================
=
= S_StopAll
=
==================
*/

void S_StopAll(void)//L80040224()
{
    wess_seq_stopall();
}

/*
==================
=
= S_AdjustSoundParams
=
= Exclusive Psx Doom
==================
*/

// when to clip out sounds
// Does not fit the large outdoor areas.
#define S_CLIPPING_DIST (1124<<FRACBITS)	/* Clip sounds beyond this distance */
#define S_CLOSE_DIST (100 << FRACBITS)		/* Too close! */
#define S_ATTENUATOR ((S_CLIPPING_DIST-S_CLOSE_DIST)>>FRACBITS)

#define REVERB_OFF	1

int volume; //80077C54
int pan; //80077C54
void S_AdjustSoundParams(mobj_t *origin, int sound_id, TriggerPlayAttr *attr)//L80040244()
{
	fixed_t    dx, dy;
	fixed_t    dist_approx;
	char       vol;
	char       pan;
	angle_t    angle;
	mobj_t*    listener;
	TriggerPlayAttr t_attr;

    if ((NUMSFX-1) < sound_id)
        return;

    listener = players[consoleplayer].mo;

    vol = 127;

    if (origin == NULL)
    {
       pan = 64;
    }
    else
    {
        pan = 64;
        if (origin != listener)
        {
            dx = abs(origin->x - listener->x);
            dy = abs(origin->y - listener->y);
            dist_approx = (dx + dy) - ((dx < dy ? dx : dy) >> 1);

            if(dist_approx > S_CLIPPING_DIST)
                return;

            // angle of source to listener
            angle = R_PointToAngle2(listener->x, listener->y, origin->x, origin->y);

            if (angle <= listener->angle)
                angle += 0xffffffff;

            angle -= listener->angle;

            // stereo separation
            pan = 128 - ((finesine[angle >> ANGLETOFINESHIFT] * 96) >> FRACBITS) >> 1;

            // volume calculation
            if(dist_approx < S_CLOSE_DIST)
                vol = 127;
            else
                vol = (((S_CLIPPING_DIST - dist_approx) >> FRACBITS) * 127) / S_ATTENUATOR;

            if ((origin->x == listener->x) && (origin->y == listener->y))
                pan = 64;

            if (!(vol > 0))
                return;
        }
    }

    //Check Reverb Flag Sector
    if (origin && (origin->subsector->sector->flags & REVERB_OFF))
        t_attr.reverb = 0;
    else
        t_attr.reverb = 127;

    t_attr.mask = (TRIGGER_VOLUME|TRIGGER_PAN|TRIGGER_REVERB);
    t_attr.volume = vol;
    t_attr.pan = pan;
    attr = &t_attr;
    snd_vol = vol;
    snd_pan = pan;
    wess_seq_trigger_type_special(sound_id, (unsigned long)origin, attr);
}

/*
==================
=
= S_StartSound
=
==================
*/

void S_StartSound(mobj_t *origin, int sound_id)//L80040470()
{
	S_AdjustSoundParams(origin, sound_id, 0);
}

/*
==================
=
= S_UpdateSounds
=
==================
*/

void S_UpdateSounds(void) //L80040470()
{
  update_snd += 1;
}

/*
==================
=
= PsxSoundInit
=
==================
*/

void PsxSoundInit(int sfx_volume, int mus_volume, void *data)//L80040488()
{
    PsxCd_File *fileptr;
    //char *wmd_data;

    wess_init();
    psxcd_init();

    fileptr = psxcd_open((char*)_DOOMSND_WMD);

    psxcd_read(data, fileptr->file.size, fileptr);
	psxcd_close(fileptr);

	S_InitBlock(&doomsfxblk);
	S_InitBlock(&mapsfxblk);

	//Init wmd_data
	//wmd_data = Z_Alloc(0x1e000, PU_STATIC, 0);
	wess_load_module(data, mem_pntr, mem_size, settings_list);

	//printf("wess_dig_lcd_loader_init\n");
	wess_dig_lcd_loader_init(wess_get_master_status());

	//printf("wess_seq_loader_init\n");
	wess_seq_loader_init(wess_get_master_status(), (char*)_DOOMSND_WMD, YesOpenSeqHandle);

	//printf("wess_seq_range_load\n");
	//printf("wess_get_wmd_end %X\n", (int)wess_get_wmd_end);
	trackptr = wess_get_wmd_end();
	trackptr += wess_seq_range_load(0, 90, wess_get_wmd_end());
	//printf("trackptr %X\n", (int)trackptr);

    //Free wmd_data tmp
    //Z_Free(wmd_data);

    S_SetSoundVolume(sfx_volume);
    S_SetMusicVolume(mus_volume);

    sram_load = 0;
    sram_pos = wess_dig_lcd_load((char *)_DOOMSFX_LCD,(void *)SRAM_START, &doomsfxblk, 0);
    sram_pos += SRAM_START;

    //printf("sram_pos %d\n",sram_pos);
    sram_load = 1;
}


/*
==================
=
= PsxSoundExit
=
==================
*/

void PsxSoundExit (void)//L800405C8()
{
	return;
}
