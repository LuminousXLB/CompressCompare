#pragma once

#include "string.h"

//#define DEBUG

//#define LZCoderDBG
//#define IOBaseDBG

//#define __FUNCTION_ENTRANCE__ printf("---- [%s : %4d] %s\n", __FILE__, __LINE__, __FUNCTION__);
#define __FUNCTION_ENTRANCE__ printf("\t [%4d] |>>  | %s\n", __LINE__, __FUNCTION__);
#define __FUNCTION_EXIT__ printf("\t [%4d] |  <<| %s\n", __LINE__, __FUNCTION__);
#define __FUNCTION_HERE__ printf("\t [%4d] \t| -- | %s\n", __LINE__, __FUNCTION__);

//#ifdef __PRETTY_FUNCTION__
//#undef __FUNCTION_ENTRANCE__
//#define __FUNCTION_ENTRANCE__ printf("---- [%s : %4d] %s\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);
//#endif

//#ifdef __FUNCSIG__
//#undef __FUNCTION_ENTRANCE__
//#define __FUNCTION_ENTRANCE__ printf("---- [%s : %4d] %s\n", __FILE__, __LINE__, __FUNCSIG__);
//#endif // __FUNCSIG__

//#define LZEncoderInputDBG
//#define LZEncoderOutputDBG
//#define LZDecoderInputDBG
//#define LZDecoderOutputDBG

//#define ArithEncoderProbDBG
//#define ArithEncoderCodeDBG

//#define ArithDecoderProbDBG
//#define ArithDecoderCodeDBG
//#define ArithDecoderDuraDBG
//#define ArithDecoderOutDBG

#define __DBGHeader__ printf("\t[%s : %4d] ", __FUNCTION__, __LINE__);

#define CMD_TOOL
