
#define F_WIDTH         150
#define F_HEIGHT        150

#define GSCREEN_WIDTH   800
#define GSCREEN_HEIGHT  600

#define BASECOLOR   0x00475790

#if CFG_GUI > 0
EXTERN VOID YIDrawLineH(INT x1, INT x2, INT y, UINT32 color);
EXTERN VOID YIDrawLineV(INT x, INT y1, INT y2, UINT32 color);
EXTERN VOID YIDrawPoint(INT x, INT y, UINT32 color);
EXTERN VOID YIDrawLine(INT iX, INT iY, INT iToX, INT iToY, UINT32 iColor);
EXTERN VOID YIDrawRect(INT x1, INT y1, INT x2, INT y2, UINT32 color);
EXTERN VOID YIDrawWindow(INT iX, INT iY, INT iToX, INT iToY, UINT32 dwStyle);

EXTERN VOID YIDrawTextOutHz(UINT8 nChar1, UINT8 nChar2, INT x, INT y, UINT32 nColorFT, UINT32 nColorBK);
EXTERN VOID YIDrawTextOutEn(CHAR nChar, INT x, INT y, UINT32 nColorFT, UINT32 nColorBK);

/*STATIC*/ UINT INLINE Rand()
{
    g_st_rand_num = (g_st_rand_num*123+59);
    return g_st_rand_num;
}

STATIC YI_STATUS YICALL ColorThread(PVOID data)
{
    /*INT i,j;
    UINT32 nColor;*/
    while(1)
    {
        INT x1, y1, x2, y2;
        x1 = Rand()%F_WIDTH;
        x2 = Rand()%F_WIDTH;
        y1 = Rand()%F_HEIGHT;
        y2 = Rand()%F_HEIGHT;

        YIDrawFillRect(GSCREEN_WIDTH-x1, y1, GSCREEN_WIDTH-x2, y2, Rand());
        YIKeSRMWait(yi_g_nKeCurrentThreadId, 4, YI_WAIT_WITH_TIMEOUT_UNINTERRUPTED);
    }
    return E_OK; 
}

EXTERN INT yi_g_arCpuPfmGTbl[];
EXTERN INT yi_g_nCpuPfmGPos;

STATIC YI_STATUS YICALL GlShowThread(PVOID data)
{

    while(1)
    {
        INT i,j = 0;
        for(i =  yi_g_nCpuPfmGPos; i < F_WIDTH; i++, j++)
        {
            YIDrawLineV(GSCREEN_WIDTH-2*F_WIDTH+j, 50, F_HEIGHT-yi_g_arCpuPfmGTbl[i], 0x00000000);
            YIDrawLineV(GSCREEN_WIDTH-2*F_WIDTH+j, F_HEIGHT-yi_g_arCpuPfmGTbl[i], F_HEIGHT, 0x0000ff00);
        }
        for(i = 0; i < yi_g_nCpuPfmGPos; i++, j++)
        {
            YIDrawLineV(GSCREEN_WIDTH-2*F_WIDTH+j, 50, F_HEIGHT-yi_g_arCpuPfmGTbl[i], 0x00000000);
            YIDrawLineV(GSCREEN_WIDTH-2*F_WIDTH+j, F_HEIGHT-yi_g_arCpuPfmGTbl[i], F_HEIGHT, 0x0000ff00);
        }
       YIKeSRMWait(yi_g_nKeCurrentThreadId, 10, YI_WAIT_WITH_TIMEOUT_UNINTERRUPTED);
    }
#if 0
    INT i,j;
    UINT32 nColor;
    INT x1, y1, x2, y2;
    x1=x2=y1=y2 = 0;
    while(1)
    {
        x1 = Rand()%F_WIDTH;
        y1 = Rand()%F_HEIGHT;

        /* YIDrawLine(GSCREEN_WIDTH-F_WIDTH-x1, y1, GSCREEN_WIDTH-F_WIDTH-x2, y2, Rand());*/
        YIDrawFillRect(GSCREEN_WIDTH-F_WIDTH-x1, y1, GSCREEN_WIDTH-F_WIDTH-x2, y2, Rand());
        /* YIDrawPoint(GSCREEN_WIDTH-F_WIDTH-x1, y1, Rand());*/
        x2 = x1;
        y2 = y1;
        YIKeSRMWait(yi_g_nKeCurrentThreadId, 10, YI_WAIT_WITH_TIMEOUT_UNINTERRUPTED);
    }
#endif /* 0 */
    return E_OK; 
}

#define NSW_STARTX  (GSCREEN_WIDTH-2*F_WIDTH-8)
#define NSW_STARTY  (F_WIDTH+20)
#define NSW_ENDX    (GSCREEN_WIDTH-13)
#define NSW_ENDY    (4*F_WIDTH-20)

#define NSC_STARTX  (NSW_STARTX+4)
#define NSC_STARTY  (NSW_STARTY+23)
#define NSC_ENDX    (NSW_ENDX-3)
#define NSC_ENDY    (NSW_ENDY-3)

#define NSC_BGCLO   0x00ffffff
#define NSC_FTCLO   0x00000000

#define ASCII_WIDTH     8
#define ASCII_HEIGHT    16

EXTERN CHAR yi_g_szNovel[];
STATIC PCHAR yi_g_pNovelPos = yi_g_szNovel;

STATIC VOID SNMovePos(INT nDir)
{
    while(nDir)
    {
        INT nCount;
        for(nCount = 0; nCount < (NSC_ENDX - NSC_STARTX)/ASCII_WIDTH; nCount++)
        {
            if(nDir > 0)
            {
                if(*yi_g_pNovelPos == '\n')
                {
                    yi_g_pNovelPos ++;
                    break;
                }
                yi_g_pNovelPos++;
            }
            else
            {
                 if(*yi_g_pNovelPos == '\n')
                {
                    yi_g_pNovelPos --;
                    break;
                }
                yi_g_pNovelPos--;
            }
        }
        if(nCount >= (NSC_ENDX - NSC_STARTX)/ASCII_WIDTH)
        {
            if(*yi_g_pNovelPos == '\n')
            {
                if(nDir > 0 )yi_g_pNovelPos++;
                else yi_g_pNovelPos--;
            }
        }
        if(nDir > 0)
        {
            nDir--;
        }
        else
        {
            nDir++;
        }
    }
}


STATIC VOID SNShowNovel()
{
    PCHAR pCurText ;
    INT nLine = 0;
/*    INT x,y;
    for(y = 0; y < (NSC_ENDY-NSC_STARTY)/ASCII_HEIGHT; y++)
    {
        for(x = 0; x < (NSC_ENDX - NSC_STARTX)/ASCII_WIDTH; x++)
        {
            if(*(PUINT8)pCurText == '\n')
            {
                YIDrawTextOutEn('\0', NSC_STARTX+x*ASCII_WIDTH, NSC_STARTY+(nLine-1)*ASCII_HEIGHT,
                             NSC_FTCLO, NSC_BGCLO);
                
            }
        }
    }
      */  

        pCurText = yi_g_pNovelPos;
        while(nLine < (NSC_ENDY-NSC_STARTY) /ASCII_HEIGHT)
        {
           INT nRow = 0;
           do{
                if(*(PUINT8)pCurText == '\n')
                {
                    INT nRow2 = nRow;
                    while(nRow2 < (NSC_ENDX - NSC_STARTX) / ASCII_WIDTH)
                    {
                        YIDrawTextOutEn('\0', NSC_STARTX+nRow2*ASCII_WIDTH, NSC_STARTY+nLine*ASCII_HEIGHT,
                                 NSC_FTCLO, NSC_BGCLO);
                        nRow2++; 
                    }
                    if(nRow != 0 || *(pCurText-1) != '\n')
                        nLine++;
                    pCurText++;
                    nRow = 0;

                    break;
                }
                else if(*(PUINT8)pCurText > 127 &&  *(PUINT8)(pCurText+1) > 127)
                {
                    if(nRow+1 < (NSC_ENDX - NSC_STARTX)/ASCII_WIDTH)
                    {
                        
                        YIDrawTextOutHz(*pCurText, *(pCurText+1),NSC_STARTX+nRow*ASCII_WIDTH,
                            NSC_STARTY+nLine*ASCII_HEIGHT, NSC_FTCLO, NSC_BGCLO);
                        nRow++;
                        nRow++;
                        pCurText++;
                        pCurText++;
                    }
                    else
                    {
                        YIDrawTextOutEn('\0', NSC_STARTX+nRow*ASCII_WIDTH, NSC_STARTY+nLine*ASCII_HEIGHT,
                                 NSC_FTCLO, NSC_BGCLO);
                        nLine++;
                        break;
                    }
                }
                else
                {
                    YIDrawTextOutEn(*pCurText, NSC_STARTX+nRow*ASCII_WIDTH, NSC_STARTY+nLine*ASCII_HEIGHT,
                                 NSC_FTCLO, NSC_BGCLO);
                    nRow++;
                    pCurText++;

               }
             }while(nRow < (NSC_ENDX - NSC_STARTX) / ASCII_WIDTH);
            if(nRow >= (NSC_ENDX - NSC_STARTX) / ASCII_WIDTH)
            {
                 if(*pCurText == '\n' )
                 {
                    pCurText++;
                 }
                 nLine++;
            }
        }
    }

STATIC YI_SEMAPHORE g_stSNSem;

STATIC YI_STATUS YICALL NovelShowThread(PVOID data)
{
   YIKeSemInit(&g_stSNSem, 0);
    
   while(1)
    {
        YIDrawWindow(NSW_STARTX, NSW_STARTY , NSW_ENDX, NSW_ENDY, 1);

        YIDrawLineH(NSW_STARTX+2, NSW_ENDX-1, NSW_STARTY+21, 0x007f7f7f);
        YIDrawLineH(NSW_STARTX+3, NSW_ENDX-2, NSW_STARTY+22, 0x00000000);

        YIDrawLineV(NSW_STARTX+2, NSW_STARTY+22, NSW_ENDY-1, 0x007f7f7f);
        YIDrawLineV(NSW_STARTX+3, NSW_STARTY+21, NSW_ENDY-2, 0x00000000);

        YIDrawLineH(NSW_STARTX+2, NSW_ENDX-1, NSW_ENDY-1, 0x00ffffff);
        YIDrawLineH(NSW_STARTX+3, NSW_ENDX-2, NSW_ENDY-2, 0x00a0a0a0);

        YIDrawLineV(NSW_ENDX-2, NSW_STARTY+22, NSW_ENDY-1, 0x00ffffff);
        YIDrawLineV(NSW_ENDX-1, NSW_STARTY+21, NSW_ENDY-2, 0x00a0a0a0);

        /* YIDrawFillRect(NSW_STARTX+4, NSW_STARTY+23, NSW_ENDX-3, NSW_ENDY-3, 0x00ffffff);*/

        SNShowNovel();
 
        YIKeSemLock(&g_stSNSem);
        //SNMovePos(1);
        //YIKeSRMWait(yi_g_nKeCurrentThreadId, 100, YI_WAIT_WITH_TIMEOUT_UNINTERRUPTED);
    } 
}
#endif /* CFG_GUI */
