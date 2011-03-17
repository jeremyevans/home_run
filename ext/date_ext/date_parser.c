
#line 1 "date_parser.rl"
#include "date_ext.h"

#ifdef RHR_ENCODING
extern int rhrd_encoding_index;
#endif

#define RHRR_ISO_PARSER 0x1
#define RHRR_RFC_PARSER 0x2
#define RHRR_CLF_PARSER 0x4

#define RHRR_ISO_TIME_SET 0x1
#define RHRR_ISO_ZONE_SET 0x2
#define RHRR_ISO_SEC_FRACTION_SET 0x4

#define RHRR_RFC_TIME_SET 0x1
#define RHRR_RFC_ZONE_SET 0x2
#define RHRR_RFC_ZONE_NUM_SET 0x4
#define RHRR_RFC_ZONE_NAME_SET 0x8

#define RHRR_CLF_TIME_SET 0x1
#define RHRR_CLF_ZONE_SET 0x2

#define RHRR_BAD_OFFSET 86400

extern const char * rhrd__abbr_month_names[];
extern const char * rhrd__abbr_day_names[];

extern VALUE rhrd_sym_hour;
extern VALUE rhrd_sym_mday;
extern VALUE rhrd_sym_min;
extern VALUE rhrd_sym_mon;
extern VALUE rhrd_sym_offset;
extern VALUE rhrd_sym_sec;
extern VALUE rhrd_sym_sec_fraction;
extern VALUE rhrd_sym_wday;
extern VALUE rhrd_sym_year;
extern VALUE rhrd_sym_zone;

const char * rhrd__rfc_zone_names[] = {"UT", "GMT", "EST", "EDT", "CST", "CDT", "MST", "MDT", "PST", "PDT",
  "A", "B", "C", "D", "E", "F", "G", "H", "I", "K", "L", "M",
  "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
long rhrd__rfc_zone_offsets[] = {0, 0, -18000, -14400, -21600, -18000, -25200, -21600, -28800, -25200,
  3600, 7200, 10800, 14400, 18000, 21600, 25200, 28800, 32400, 36000, 39600, 43200,
  -3600, -7200, -10800, -14400, -18000, -21600, -25200, -28800, -32400, -36000, -39600, -43200, 0};

long rhrd__rfc_zone_offset(char * str) {
  int i;

  for(i = 0; i < 36; i++) {
    if(strcasecmp(rhrd__rfc_zone_names[i], str) == 0) {
      return rhrd__rfc_zone_offsets[i];
    }
  }

  return RHRR_BAD_OFFSET;
}

long rhrd__month_num(char * str) {
  int i;

  for(i = 1; i < 13; i++) {
    if(strncasecmp(str, rhrd__abbr_month_names[i], 3) == 0) {
      return i;
    }
  }

  return atol(str);
}

long rhrd__weekday_num(char * str) {
  int i;

  for(i = 0; i < 7; i++) {
    if(strncasecmp(str, rhrd__abbr_day_names[i], 3) == 0) {
      return i;
    }
  }

  return 7;
}


#line 86 "date_parser.c"
static const char _date_parser_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	7, 1, 8, 1, 11, 1, 12, 1, 
	13, 1, 14, 1, 15, 1, 16, 1, 
	17, 1, 18, 1, 19, 1, 20, 1, 
	23, 1, 25, 1, 26, 1, 27, 1, 
	28, 1, 29, 1, 30, 1, 31, 1, 
	33, 2, 0, 24, 2, 8, 11, 2, 
	9, 7, 2, 9, 8, 2, 10, 8, 
	2, 20, 23, 2, 21, 20, 2, 22, 
	20, 2, 31, 33, 2, 32, 31, 3, 
	9, 8, 11, 3, 10, 8, 11, 3, 
	21, 20, 23, 3, 22, 20, 23, 3, 
	32, 31, 33
};

static const short _date_parser_key_offsets[] = {
	0, 0, 20, 22, 24, 26, 28, 30, 
	32, 34, 36, 40, 42, 44, 45, 47, 
	49, 50, 52, 54, 56, 58, 60, 62, 
	65, 67, 69, 71, 74, 77, 93, 97, 
	99, 100, 103, 105, 107, 109, 111, 114, 
	116, 117, 119, 121, 122, 124, 126, 128, 
	130, 132, 134, 136, 138, 140, 142, 144, 
	146, 150, 152, 156, 158, 162, 164, 166, 
	168, 170, 172, 174, 176, 180, 182, 184, 
	185, 192, 201, 203, 222, 226, 228, 234, 
	236, 238, 240, 242, 244, 245, 247, 249, 
	250, 252, 254, 256, 258, 260, 262, 264, 
	266, 268, 270, 272, 274, 276, 280, 282, 
	286, 288, 292, 294, 296, 298, 300, 302, 
	304, 325, 346, 348, 350, 352, 354, 358, 
	360, 364, 366, 368, 370, 372, 377, 380, 
	386, 392, 398, 401, 406, 413, 420, 427, 
	434, 441, 448, 453, 457, 460, 465, 470, 
	473, 479, 504, 529, 532, 535, 538, 545, 
	550
};

static const char _date_parser_trans_keys[] = {
	32, 45, 48, 51, 70, 77, 83, 84, 
	87, 102, 109, 115, 116, 119, 9, 13, 
	49, 50, 52, 57, 48, 57, 48, 57, 
	48, 57, 48, 57, 45, 47, 48, 49, 
	49, 57, 45, 47, 48, 51, 49, 50, 
	49, 57, 48, 57, 58, 48, 53, 48, 
	57, 58, 48, 53, 48, 57, 48, 57, 
	48, 57, 48, 57, 48, 52, 50, 48, 
	49, 48, 57, 48, 49, 48, 50, 48, 
	49, 57, 47, 48, 57, 65, 68, 70, 
	74, 77, 78, 79, 83, 97, 100, 102, 
	106, 109, 110, 111, 115, 80, 85, 112, 
	117, 82, 114, 47, 45, 48, 57, 48, 
	57, 48, 57, 48, 57, 48, 57, 50, 
	48, 49, 48, 57, 58, 48, 53, 48, 
	57, 58, 48, 53, 48, 57, 48, 57, 
	48, 57, 48, 57, 48, 57, 48, 52, 
	71, 103, 69, 101, 67, 99, 69, 101, 
	66, 98, 65, 85, 97, 117, 78, 110, 
	76, 78, 108, 110, 65, 97, 82, 89, 
	114, 121, 79, 111, 86, 118, 67, 99, 
	84, 116, 69, 101, 80, 112, 48, 57, 
	48, 49, 50, 57, 82, 114, 73, 105, 
	44, 32, 48, 51, 9, 13, 49, 50, 
	32, 48, 51, 9, 13, 49, 50, 52, 
	57, 49, 57, 32, 65, 68, 70, 74, 
	77, 78, 79, 83, 97, 100, 102, 106, 
	109, 110, 111, 115, 9, 13, 80, 85, 
	112, 117, 82, 114, 32, 45, 9, 13, 
	48, 57, 48, 57, 48, 57, 48, 57, 
	48, 57, 48, 57, 58, 48, 53, 48, 
	57, 58, 48, 53, 48, 57, 48, 57, 
	48, 57, 48, 57, 48, 57, 84, 116, 
	48, 52, 71, 103, 69, 101, 67, 99, 
	69, 101, 66, 98, 65, 85, 97, 117, 
	78, 110, 76, 78, 108, 110, 65, 97, 
	82, 89, 114, 121, 79, 111, 86, 118, 
	67, 99, 84, 116, 69, 101, 80, 112, 
	32, 65, 68, 70, 74, 77, 78, 79, 
	83, 97, 100, 102, 106, 109, 110, 111, 
	115, 9, 13, 48, 57, 32, 65, 68, 
	70, 74, 77, 78, 79, 83, 97, 100, 
	102, 106, 109, 110, 111, 115, 9, 13, 
	48, 49, 48, 57, 48, 49, 79, 111, 
	78, 110, 65, 85, 97, 117, 84, 116, 
	72, 85, 104, 117, 85, 117, 69, 101, 
	69, 101, 68, 100, 32, 84, 116, 9, 
	13, 32, 9, 13, 32, 50, 9, 13, 
	48, 49, 32, 43, 45, 46, 9, 13, 
	32, 58, 9, 13, 48, 57, 32, 9, 
	13, 32, 9, 13, 48, 57, 32, 43, 
	45, 9, 13, 48, 57, 32, 43, 45, 
	9, 13, 48, 57, 32, 43, 45, 9, 
	13, 48, 57, 32, 43, 45, 9, 13, 
	48, 57, 32, 43, 45, 9, 13, 48, 
	57, 32, 43, 45, 9, 13, 48, 57, 
	32, 43, 45, 9, 13, 32, 58, 9, 
	13, 32, 9, 13, 32, 43, 45, 9, 
	13, 32, 43, 45, 9, 13, 32, 9, 
	13, 32, 50, 9, 13, 48, 49, 32, 
	43, 45, 67, 69, 71, 77, 80, 85, 
	99, 101, 103, 109, 112, 117, 9, 13, 
	65, 73, 75, 90, 97, 105, 107, 122, 
	32, 43, 45, 67, 69, 71, 77, 80, 
	85, 99, 101, 103, 109, 112, 117, 9, 
	13, 65, 73, 75, 90, 97, 105, 107, 
	122, 32, 9, 13, 32, 9, 13, 32, 
	9, 13, 32, 68, 83, 100, 115, 9, 
	13, 32, 77, 109, 9, 13, 32, 84, 
	116, 9, 13, 0
};

static const char _date_parser_single_lengths[] = {
	0, 14, 0, 0, 0, 0, 2, 2, 
	0, 2, 2, 0, 0, 1, 0, 0, 
	1, 0, 0, 0, 0, 0, 0, 1, 
	0, 0, 0, 1, 1, 16, 4, 2, 
	1, 1, 0, 0, 0, 0, 1, 0, 
	1, 0, 0, 1, 0, 0, 0, 0, 
	0, 0, 0, 2, 2, 2, 2, 2, 
	4, 2, 4, 2, 4, 2, 2, 2, 
	2, 2, 2, 0, 0, 2, 2, 1, 
	3, 3, 0, 17, 4, 2, 2, 0, 
	0, 0, 0, 0, 1, 0, 0, 1, 
	0, 0, 0, 0, 0, 0, 2, 0, 
	2, 2, 2, 2, 2, 4, 2, 4, 
	2, 4, 2, 2, 2, 2, 2, 2, 
	17, 17, 0, 0, 2, 2, 4, 2, 
	4, 2, 2, 2, 2, 3, 1, 2, 
	4, 2, 1, 1, 3, 3, 3, 3, 
	3, 3, 3, 2, 1, 3, 3, 1, 
	2, 15, 15, 1, 1, 1, 5, 3, 
	3
};

static const char _date_parser_range_lengths[] = {
	0, 3, 1, 1, 1, 1, 0, 0, 
	1, 0, 1, 1, 1, 0, 1, 1, 
	0, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 0, 0, 0, 
	0, 1, 1, 1, 1, 1, 1, 1, 
	0, 1, 1, 0, 1, 1, 1, 1, 
	1, 1, 1, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 1, 2, 0, 0, 0, 
	2, 3, 1, 1, 0, 0, 2, 1, 
	1, 1, 1, 1, 0, 1, 1, 0, 
	1, 1, 1, 1, 1, 1, 0, 1, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	2, 2, 1, 1, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 1, 1, 2, 
	1, 2, 1, 2, 2, 2, 2, 2, 
	2, 2, 1, 1, 1, 1, 1, 1, 
	2, 5, 5, 1, 1, 1, 1, 1, 
	1
};

static const short _date_parser_index_offsets[] = {
	0, 0, 18, 20, 22, 24, 26, 29, 
	32, 34, 37, 41, 43, 45, 47, 49, 
	51, 53, 55, 57, 59, 61, 63, 65, 
	68, 70, 72, 74, 77, 80, 97, 102, 
	105, 107, 110, 112, 114, 116, 118, 121, 
	123, 125, 127, 129, 131, 133, 135, 137, 
	139, 141, 143, 145, 148, 151, 154, 157, 
	160, 165, 168, 173, 176, 181, 184, 187, 
	190, 193, 196, 199, 201, 204, 207, 210, 
	212, 218, 225, 227, 246, 251, 254, 259, 
	261, 263, 265, 267, 269, 271, 273, 275, 
	277, 279, 281, 283, 285, 287, 289, 292, 
	294, 297, 300, 303, 306, 309, 314, 317, 
	322, 325, 330, 333, 336, 339, 342, 345, 
	348, 368, 388, 390, 392, 395, 398, 403, 
	406, 411, 414, 417, 420, 423, 428, 431, 
	436, 442, 447, 450, 454, 460, 466, 472, 
	478, 484, 490, 495, 499, 502, 507, 512, 
	515, 520, 541, 562, 565, 568, 571, 578, 
	583
};

static const unsigned char _date_parser_indicies[] = {
	0, 2, 3, 5, 7, 8, 9, 10, 
	11, 7, 8, 9, 10, 11, 0, 4, 
	6, 1, 12, 1, 13, 1, 14, 1, 
	15, 1, 16, 16, 1, 17, 18, 1, 
	19, 1, 20, 20, 1, 21, 23, 22, 
	1, 24, 1, 25, 1, 26, 1, 27, 
	1, 28, 1, 29, 1, 30, 1, 31, 
	1, 32, 1, 33, 1, 34, 1, 25, 
	1, 36, 35, 1, 24, 1, 24, 1, 
	19, 1, 13, 37, 1, 38, 14, 1, 
	39, 40, 41, 42, 43, 44, 45, 46, 
	39, 40, 41, 42, 43, 44, 45, 46, 
	1, 47, 48, 47, 48, 1, 49, 49, 
	1, 50, 1, 51, 52, 1, 53, 1, 
	54, 1, 55, 1, 56, 1, 58, 57, 
	1, 59, 1, 60, 1, 61, 1, 62, 
	1, 63, 1, 64, 1, 65, 1, 66, 
	1, 67, 1, 68, 1, 69, 1, 59, 
	1, 49, 49, 1, 70, 70, 1, 49, 
	49, 1, 71, 71, 1, 49, 49, 1, 
	72, 73, 72, 73, 1, 49, 49, 1, 
	49, 49, 49, 49, 1, 74, 74, 1, 
	49, 49, 49, 49, 1, 75, 75, 1, 
	49, 49, 1, 76, 76, 1, 49, 49, 
	1, 77, 77, 1, 49, 49, 1, 37, 
	1, 37, 13, 1, 78, 78, 1, 79, 
	79, 1, 80, 1, 81, 82, 84, 80, 
	83, 1, 81, 82, 86, 80, 85, 87, 
	1, 87, 1, 87, 88, 89, 90, 91, 
	92, 93, 94, 95, 88, 89, 90, 91, 
	92, 93, 94, 95, 87, 1, 96, 97, 
	96, 97, 1, 98, 98, 1, 98, 99, 
	98, 100, 1, 101, 1, 102, 1, 103, 
	1, 104, 1, 105, 1, 106, 1, 107, 
	1, 108, 1, 109, 1, 110, 1, 111, 
	1, 112, 1, 113, 1, 114, 1, 115, 
	1, 116, 116, 1, 105, 1, 98, 98, 
	1, 117, 117, 1, 98, 98, 1, 118, 
	118, 1, 98, 98, 1, 119, 120, 119, 
	120, 1, 98, 98, 1, 98, 98, 98, 
	98, 1, 121, 121, 1, 98, 98, 98, 
	98, 1, 122, 122, 1, 98, 98, 1, 
	123, 123, 1, 98, 98, 1, 124, 124, 
	1, 98, 98, 1, 87, 88, 89, 90, 
	91, 92, 93, 94, 95, 88, 89, 90, 
	91, 92, 93, 94, 95, 87, 87, 1, 
	87, 88, 89, 90, 91, 92, 93, 94, 
	95, 88, 89, 90, 91, 92, 93, 94, 
	95, 87, 87, 1, 87, 1, 87, 1, 
	125, 125, 1, 79, 79, 1, 126, 125, 
	126, 125, 1, 79, 79, 1, 127, 128, 
	127, 128, 1, 79, 79, 1, 79, 79, 
	1, 129, 129, 1, 79, 79, 1, 131, 
	132, 132, 130, 1, 130, 130, 1, 130, 
	36, 130, 35, 1, 133, 134, 134, 135, 
	133, 1, 136, 138, 136, 137, 1, 136, 
	136, 1, 136, 136, 137, 1, 139, 140, 
	140, 139, 141, 1, 139, 140, 140, 139, 
	142, 1, 139, 140, 140, 139, 143, 1, 
	139, 140, 140, 139, 144, 1, 139, 140, 
	140, 139, 145, 1, 139, 140, 140, 139, 
	146, 1, 139, 140, 140, 139, 1, 147, 
	148, 147, 1, 147, 147, 1, 149, 150, 
	150, 149, 1, 151, 150, 150, 151, 1, 
	152, 152, 1, 104, 154, 104, 153, 1, 
	155, 156, 156, 158, 158, 159, 158, 158, 
	160, 158, 158, 159, 158, 158, 160, 155, 
	157, 157, 157, 157, 1, 161, 156, 156, 
	158, 158, 159, 158, 158, 160, 158, 158, 
	159, 158, 158, 160, 161, 157, 157, 157, 
	157, 1, 162, 162, 1, 163, 163, 1, 
	164, 164, 1, 164, 165, 165, 165, 165, 
	164, 1, 164, 165, 165, 164, 1, 164, 
	116, 116, 164, 1, 0
};

static const unsigned char _date_parser_trans_targs[] = {
	1, 0, 2, 27, 67, 68, 3, 69, 
	116, 118, 120, 123, 3, 4, 5, 6, 
	7, 8, 26, 9, 10, 11, 24, 25, 
	125, 13, 14, 15, 16, 17, 18, 128, 
	20, 129, 130, 12, 22, 28, 29, 30, 
	52, 54, 56, 59, 61, 63, 65, 31, 
	51, 32, 33, 34, 35, 35, 36, 37, 
	139, 39, 50, 40, 41, 42, 43, 44, 
	45, 141, 47, 48, 49, 143, 53, 55, 
	57, 58, 60, 62, 64, 66, 70, 71, 
	72, 73, 74, 114, 115, 112, 113, 75, 
	76, 97, 99, 101, 104, 106, 108, 110, 
	77, 96, 78, 79, 80, 80, 81, 82, 
	144, 84, 85, 86, 87, 88, 89, 145, 
	91, 92, 93, 147, 149, 98, 100, 102, 
	103, 105, 107, 109, 111, 117, 119, 121, 
	122, 124, 126, 127, 23, 126, 19, 132, 
	126, 21, 131, 126, 19, 133, 134, 135, 
	136, 137, 138, 140, 38, 142, 46, 142, 
	140, 83, 95, 146, 90, 149, 150, 151, 
	152, 146, 148, 148, 148, 94
};

static const char _date_parser_trans_actions[] = {
	0, 0, 1, 57, 57, 57, 1, 21, 
	21, 21, 21, 21, 0, 0, 0, 0, 
	0, 3, 3, 0, 0, 5, 5, 5, 
	0, 0, 0, 9, 0, 0, 11, 0, 
	0, 0, 0, 7, 7, 0, 0, 41, 
	41, 41, 41, 41, 41, 41, 41, 0, 
	0, 0, 0, 43, 43, 0, 0, 0, 
	0, 45, 45, 0, 0, 47, 0, 0, 
	49, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 23, 23, 23, 23, 23, 23, 0, 
	25, 25, 25, 25, 25, 25, 25, 25, 
	0, 0, 0, 27, 27, 0, 0, 0, 
	0, 0, 0, 31, 0, 0, 33, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 17, 15, 13, 
	69, 0, 0, 66, 63, 0, 0, 0, 
	0, 0, 0, 0, 0, 53, 51, 0, 
	84, 29, 29, 37, 35, 35, 35, 35, 
	35, 0, 75, 0, 78, 0
};

static const char _date_parser_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 19, 19, 19, 
	60, 91, 91, 91, 87, 87, 87, 87, 
	87, 87, 87, 55, 55, 81, 55, 103, 
	39, 72, 39, 95, 39, 99, 99, 99, 
	99
};

static const int date_parser_start = 1;
static const int date_parser_first_final = 125;
static const int date_parser_error = 0;

static const int date_parser_en_main = 1;


#line 192 "date_parser.rl"


VALUE rhrd__ragel_parse(char * p, long len) {
  VALUE hash;
  long state = 0;
  long parsers = 0;

  long year = 0;
  long month = 0;
  long day = 0;
  long wday = 0;

  long hour = 0;
  long minute = 0;
  long second = 0;
  double sec_fraction = 0;

  VALUE rzone;
  char * zone = NULL;
  long zone_len = 0;
  long offset = 0;

  long iso_state = 0;
  char * t_iso_year = NULL;
  char * t_iso_month = NULL;
  char * t_iso_day = NULL;
  char * t_iso_hour= NULL;
  char * t_iso_minute = NULL;
  char * t_iso_second = NULL;
  char * t_iso_sec_fraction = NULL;
  char * t_iso_zone = NULL;
  char * t_iso_zone_end = NULL;

  long rfc_state = 0;
  char * t_rfc_wday = NULL;
  char * t_rfc_year = NULL;
  char * t_rfc_month = NULL;
  char * t_rfc_day = NULL;
  char * t_rfc_hour= NULL;
  char * t_rfc_minute = NULL;
  char * t_rfc_second = NULL;
  char * t_rfc_zone = NULL;
  char * t_rfc_zone_end = NULL;

  long clf_state = 0;
  char * t_clf_year = NULL;
  char * t_clf_month = NULL;
  char * t_clf_day = NULL;
  char * t_clf_hour= NULL;
  char * t_clf_minute = NULL;
  char * t_clf_second = NULL;
  char * t_clf_zone = NULL;
  char * t_clf_zone_end = NULL;

  int cs = 0;
  char * eof;
  char * pe;
  pe = p + len;
  eof = pe;

  
#line 486 "date_parser.c"
	{
	cs = date_parser_start;
	}

#line 253 "date_parser.rl"
  
#line 493 "date_parser.c"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _date_parser_trans_keys + _date_parser_key_offsets[cs];
	_trans = _date_parser_index_offsets[cs];

	_klen = _date_parser_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _date_parser_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += ((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _date_parser_indicies[_trans];
	cs = _date_parser_trans_targs[_trans];

	if ( _date_parser_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _date_parser_actions + _date_parser_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 91 "date_parser.rl"
	{ t_iso_year = p; }
	break;
	case 1:
#line 92 "date_parser.rl"
	{ t_iso_month = p; }
	break;
	case 2:
#line 93 "date_parser.rl"
	{ t_iso_day = p; }
	break;
	case 3:
#line 94 "date_parser.rl"
	{ t_iso_hour = p; }
	break;
	case 4:
#line 95 "date_parser.rl"
	{ t_iso_minute = p; }
	break;
	case 5:
#line 96 "date_parser.rl"
	{ t_iso_second = p; }
	break;
	case 6:
#line 97 "date_parser.rl"
	{ t_iso_sec_fraction = p; }
	break;
	case 7:
#line 98 "date_parser.rl"
	{ t_iso_zone = p; }
	break;
	case 8:
#line 100 "date_parser.rl"
	{ iso_state |= RHRR_ISO_TIME_SET; }
	break;
	case 9:
#line 101 "date_parser.rl"
	{ iso_state |= RHRR_ISO_SEC_FRACTION_SET; }
	break;
	case 10:
#line 102 "date_parser.rl"
	{
    t_iso_zone_end = p;
    iso_state |= RHRR_ISO_ZONE_SET;
  }
	break;
	case 12:
#line 125 "date_parser.rl"
	{ t_rfc_wday = p; }
	break;
	case 13:
#line 126 "date_parser.rl"
	{ t_rfc_day = p; }
	break;
	case 14:
#line 127 "date_parser.rl"
	{ t_rfc_month = p; }
	break;
	case 15:
#line 128 "date_parser.rl"
	{ t_rfc_year = p; }
	break;
	case 16:
#line 129 "date_parser.rl"
	{ t_rfc_hour = p; }
	break;
	case 17:
#line 130 "date_parser.rl"
	{ t_rfc_minute = p; }
	break;
	case 18:
#line 131 "date_parser.rl"
	{ t_rfc_second = p; }
	break;
	case 19:
#line 132 "date_parser.rl"
	{ t_rfc_zone = p; }
	break;
	case 20:
#line 134 "date_parser.rl"
	{ rfc_state |= RHRR_RFC_TIME_SET; }
	break;
	case 21:
#line 135 "date_parser.rl"
	{
    t_rfc_zone_end = p;
    rfc_state |= RHRR_RFC_ZONE_SET | RHRR_RFC_ZONE_NUM_SET;
  }
	break;
	case 22:
#line 139 "date_parser.rl"
	{
    t_rfc_zone_end = p;
    rfc_state |= RHRR_RFC_ZONE_SET | RHRR_RFC_ZONE_NAME_SET;
  }
	break;
	case 24:
#line 162 "date_parser.rl"
	{ t_clf_day = p; }
	break;
	case 25:
#line 163 "date_parser.rl"
	{ t_clf_month = p; }
	break;
	case 26:
#line 164 "date_parser.rl"
	{ t_clf_year = p; }
	break;
	case 27:
#line 165 "date_parser.rl"
	{ t_clf_hour = p; }
	break;
	case 28:
#line 166 "date_parser.rl"
	{ t_clf_minute = p; }
	break;
	case 29:
#line 167 "date_parser.rl"
	{ t_clf_second = p; }
	break;
	case 30:
#line 168 "date_parser.rl"
	{ t_clf_zone = p; }
	break;
	case 31:
#line 170 "date_parser.rl"
	{ clf_state |= RHRR_CLF_TIME_SET; }
	break;
	case 32:
#line 171 "date_parser.rl"
	{
    t_clf_zone_end = p;
    clf_state |= RHRR_CLF_ZONE_SET;
  }
	break;
#line 703 "date_parser.c"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	const char *__acts = _date_parser_actions + _date_parser_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 8:
#line 100 "date_parser.rl"
	{ iso_state |= RHRR_ISO_TIME_SET; }
	break;
	case 9:
#line 101 "date_parser.rl"
	{ iso_state |= RHRR_ISO_SEC_FRACTION_SET; }
	break;
	case 10:
#line 102 "date_parser.rl"
	{
    t_iso_zone_end = p;
    iso_state |= RHRR_ISO_ZONE_SET;
  }
	break;
	case 11:
#line 106 "date_parser.rl"
	{ parsers |= RHRR_ISO_PARSER; }
	break;
	case 20:
#line 134 "date_parser.rl"
	{ rfc_state |= RHRR_RFC_TIME_SET; }
	break;
	case 21:
#line 135 "date_parser.rl"
	{
    t_rfc_zone_end = p;
    rfc_state |= RHRR_RFC_ZONE_SET | RHRR_RFC_ZONE_NUM_SET;
  }
	break;
	case 22:
#line 139 "date_parser.rl"
	{
    t_rfc_zone_end = p;
    rfc_state |= RHRR_RFC_ZONE_SET | RHRR_RFC_ZONE_NAME_SET;
  }
	break;
	case 23:
#line 143 "date_parser.rl"
	{ parsers |= RHRR_RFC_PARSER; }
	break;
	case 31:
#line 170 "date_parser.rl"
	{ clf_state |= RHRR_CLF_TIME_SET; }
	break;
	case 32:
#line 171 "date_parser.rl"
	{
    t_clf_zone_end = p;
    clf_state |= RHRR_CLF_ZONE_SET;
  }
	break;
	case 33:
#line 175 "date_parser.rl"
	{ parsers |= RHRR_CLF_PARSER; }
	break;
#line 775 "date_parser.c"
		}
	}
	}

	_out: {}
	}

#line 254 "date_parser.rl"

  switch(parsers) {
    case RHRR_ISO_PARSER:
      year = atol(t_iso_year);
      month = atol(t_iso_month);
      day = atol(t_iso_day);
      state |= RHRR_YEAR_SET | RHRR_MONTH_SET | RHRR_DAY_SET;
      if (iso_state & RHRR_ISO_TIME_SET) {
        hour = atol(t_iso_hour);
        minute = atol(t_iso_minute);
        second = atol(t_iso_second);
        state |= RHRR_HOUR_SET | RHRR_MINUTE_SET | RHRR_SECOND_SET;
        if (iso_state & RHRR_ISO_SEC_FRACTION_SET) {
          sec_fraction = rb_cstr_to_dbl(t_iso_sec_fraction, Qfalse);
          state |= RHRR_SEC_FRACTION_SET;
        }
        if (iso_state & RHRR_ISO_ZONE_SET) {
          zone = t_iso_zone;
          zone_len = t_iso_zone_end - zone;
          if (zone_len == 3) { /* case for postgresql +03 */
            offset = atol(zone) * 3600;
          } else if (zone_len == 6) { /* standart iso */
            offset = atol(zone) * 3600 + atol(zone + 4) * 60;
          } else if (zone_len == 5) { /* case for sqlite +0300 */
            char hours[3] = { zone[1], zone[2], 0};
            offset = atol(hours) * 3600 + atol(zone + 3) * 60;
            offset *= (zone[0] == '-') ? -1 : 1;
          }
          state |= RHRR_ZONE_SET | RHRR_OFFSET_SET;
        }
      }
      break;
    case RHRR_RFC_PARSER:
      wday = rhrd__weekday_num(t_rfc_wday);
      day = atol(t_rfc_day);
      month = rhrd__month_num(t_rfc_month);
      year = atol(t_rfc_year);
      state |= RHRR_WDAY_SET | RHRR_YEAR_SET | RHRR_MONTH_SET | RHRR_DAY_SET;
      if (rfc_state & RHRR_RFC_TIME_SET) {
        hour = atol(t_rfc_hour);
        minute = atol(t_rfc_minute);
        second = atol(t_rfc_second);
        state |= RHRR_HOUR_SET | RHRR_MINUTE_SET | RHRR_SECOND_SET;
        if (rfc_state & RHRR_RFC_ZONE_SET) {
          zone = t_rfc_zone;
          zone_len = t_rfc_zone_end - zone;
          state |= RHRR_ZONE_SET;
          if (rfc_state & RHRR_RFC_ZONE_NUM_SET) {
            offset = atol(zone);
            offset = (offset/100) * 3600 + (labs(offset) % 100) * 60 * (offset < 0 ? -1 : 1);
            state |= RHRR_OFFSET_SET;
          } else if (rfc_state & RHRR_RFC_ZONE_NAME_SET) {
            offset = rhrd__rfc_zone_offset(zone);
            if (offset == RHRR_BAD_OFFSET) {
              state = 0;
            } else {
              state |= RHRR_OFFSET_SET;
            }
          }
        }
      }
      break;
    case RHRR_CLF_PARSER:
      year = atol(t_clf_year);
      month = rhrd__month_num(t_clf_month);
      day = atol(t_clf_day);
      state |= RHRR_YEAR_SET | RHRR_MONTH_SET | RHRR_DAY_SET;
      if (clf_state & RHRR_ISO_TIME_SET) {
        hour = atol(t_clf_hour);
        minute = atol(t_clf_minute);
        second = atol(t_clf_second);
        state |= RHRR_HOUR_SET | RHRR_MINUTE_SET | RHRR_SECOND_SET;
        if (clf_state & RHRR_ISO_ZONE_SET) {
          zone = t_clf_zone;
          zone_len = t_clf_zone_end - zone;
          offset = atol(zone);
          offset = (offset/100) * 3600 + (labs(offset) % 100) * 60 * (offset < 0 ? -1 : 1);
          state |= RHRR_ZONE_SET | RHRR_OFFSET_SET;
        }
      }
      break;
  }

  if (!state) {
    return Qnil;
  }

  hash = rb_hash_new();
  if(state & RHRR_WDAY_SET) {
    rb_hash_aset(hash, rhrd_sym_wday, LONG2NUM(wday));
  } 
  if(state & RHRR_YEAR_SET) {
    rb_hash_aset(hash, rhrd_sym_year, LONG2NUM(year));
  } 
  if(state & RHRR_MONTH_SET) {
    rb_hash_aset(hash, rhrd_sym_mon, LONG2NUM(month));
  } 
  if(state & RHRR_DAY_SET) {
    rb_hash_aset(hash, rhrd_sym_mday, LONG2NUM(day));
  } 
  if(state & RHRR_HOUR_SET) {
    rb_hash_aset(hash, rhrd_sym_hour, LONG2NUM(hour));
  } 
  if(state & RHRR_MINUTE_SET) {
    rb_hash_aset(hash, rhrd_sym_min, LONG2NUM(minute));
  } 
  if(state & RHRR_SECOND_SET) {
    rb_hash_aset(hash, rhrd_sym_sec, LONG2NUM(second));
  } 
  if(state & RHRR_SEC_FRACTION_SET) {
    rb_hash_aset(hash, rhrd_sym_sec_fraction, rb_float_new(sec_fraction));
  }
  if(state & RHRR_ZONE_SET) {
    rzone = rb_str_new(zone, zone_len);
    rb_hash_aset(hash, rhrd_sym_zone, rzone);
    if (state & RHRR_OFFSET_SET) {
        rb_hash_aset(hash, rhrd_sym_offset, LONG2NUM(offset));
    } else {
      rzone = rhrd_s_zone_to_diff(rzone, rzone);
      if(RTEST(rzone)) {
        rb_hash_aset(hash, rhrd_sym_offset, rzone);
      }
    }
  } 

  return hash;
}
