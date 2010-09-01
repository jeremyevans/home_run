
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


#line 84 "date_parser.c"
static const char _date_parser_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	7, 1, 9, 1, 10, 1, 11, 1, 
	12, 1, 13, 1, 14, 1, 15, 1, 
	16, 1, 17, 1, 18, 1, 21, 1, 
	23, 1, 24, 1, 25, 1, 26, 1, 
	27, 1, 28, 1, 29, 1, 31, 2, 
	0, 22, 2, 7, 9, 2, 8, 7, 
	2, 18, 21, 2, 19, 18, 2, 20, 
	18, 2, 29, 31, 2, 30, 29, 3, 
	8, 7, 9, 3, 19, 18, 21, 3, 
	20, 18, 21, 3, 30, 29, 31
};

static const short _date_parser_key_offsets[] = {
	0, 0, 20, 22, 24, 26, 28, 30, 
	32, 34, 36, 40, 42, 44, 45, 47, 
	49, 50, 52, 54, 56, 58, 59, 61, 
	63, 65, 68, 70, 72, 74, 77, 80, 
	96, 100, 102, 103, 106, 108, 110, 112, 
	114, 117, 119, 120, 122, 124, 125, 127, 
	129, 131, 133, 135, 137, 139, 141, 143, 
	145, 147, 149, 153, 155, 159, 161, 165, 
	167, 169, 171, 173, 175, 177, 179, 183, 
	185, 187, 188, 195, 204, 206, 225, 229, 
	231, 237, 239, 241, 243, 245, 247, 248, 
	250, 252, 253, 255, 257, 259, 261, 263, 
	265, 267, 269, 271, 273, 275, 277, 279, 
	283, 285, 289, 291, 295, 297, 299, 301, 
	303, 305, 307, 328, 349, 351, 353, 355, 
	357, 361, 363, 367, 369, 371, 373, 375, 
	380, 383, 389, 394, 397, 401, 404, 409, 
	414, 417, 423, 448, 473, 476, 479, 482, 
	489, 494
};

static const char _date_parser_trans_keys[] = {
	32, 45, 48, 51, 70, 77, 83, 84, 
	87, 102, 109, 115, 116, 119, 9, 13, 
	49, 50, 52, 57, 48, 57, 48, 57, 
	48, 57, 48, 57, 45, 47, 48, 49, 
	49, 57, 45, 47, 48, 51, 49, 50, 
	49, 57, 48, 57, 58, 48, 53, 48, 
	57, 58, 48, 53, 48, 57, 48, 57, 
	48, 57, 58, 48, 57, 48, 57, 48, 
	52, 50, 48, 49, 48, 57, 48, 49, 
	48, 50, 48, 49, 57, 47, 48, 57, 
	65, 68, 70, 74, 77, 78, 79, 83, 
	97, 100, 102, 106, 109, 110, 111, 115, 
	80, 85, 112, 117, 82, 114, 47, 45, 
	48, 57, 48, 57, 48, 57, 48, 57, 
	48, 57, 50, 48, 49, 48, 57, 58, 
	48, 53, 48, 57, 58, 48, 53, 48, 
	57, 48, 57, 48, 57, 48, 57, 48, 
	57, 48, 52, 71, 103, 69, 101, 67, 
	99, 69, 101, 66, 98, 65, 85, 97, 
	117, 78, 110, 76, 78, 108, 110, 65, 
	97, 82, 89, 114, 121, 79, 111, 86, 
	118, 67, 99, 84, 116, 69, 101, 80, 
	112, 48, 57, 48, 49, 50, 57, 82, 
	114, 73, 105, 44, 32, 48, 51, 9, 
	13, 49, 50, 32, 48, 51, 9, 13, 
	49, 50, 52, 57, 49, 57, 32, 65, 
	68, 70, 74, 77, 78, 79, 83, 97, 
	100, 102, 106, 109, 110, 111, 115, 9, 
	13, 80, 85, 112, 117, 82, 114, 32, 
	45, 9, 13, 48, 57, 48, 57, 48, 
	57, 48, 57, 48, 57, 48, 57, 58, 
	48, 53, 48, 57, 58, 48, 53, 48, 
	57, 48, 57, 48, 57, 48, 57, 48, 
	57, 84, 116, 48, 52, 71, 103, 69, 
	101, 67, 99, 69, 101, 66, 98, 65, 
	85, 97, 117, 78, 110, 76, 78, 108, 
	110, 65, 97, 82, 89, 114, 121, 79, 
	111, 86, 118, 67, 99, 84, 116, 69, 
	101, 80, 112, 32, 65, 68, 70, 74, 
	77, 78, 79, 83, 97, 100, 102, 106, 
	109, 110, 111, 115, 9, 13, 48, 57, 
	32, 65, 68, 70, 74, 77, 78, 79, 
	83, 97, 100, 102, 106, 109, 110, 111, 
	115, 9, 13, 48, 49, 48, 57, 48, 
	49, 79, 111, 78, 110, 65, 85, 97, 
	117, 84, 116, 72, 85, 104, 117, 85, 
	117, 69, 101, 69, 101, 68, 100, 32, 
	84, 116, 9, 13, 32, 9, 13, 32, 
	50, 9, 13, 48, 49, 32, 43, 45, 
	9, 13, 32, 9, 13, 32, 58, 9, 
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
	1, 0, 0, 0, 0, 1, 0, 0, 
	0, 1, 0, 0, 0, 1, 1, 16, 
	4, 2, 1, 1, 0, 0, 0, 0, 
	1, 0, 1, 0, 0, 1, 0, 0, 
	0, 0, 0, 0, 0, 2, 2, 2, 
	2, 2, 4, 2, 4, 2, 4, 2, 
	2, 2, 2, 2, 2, 0, 0, 2, 
	2, 1, 3, 3, 0, 17, 4, 2, 
	2, 0, 0, 0, 0, 0, 1, 0, 
	0, 1, 0, 0, 0, 0, 0, 0, 
	2, 0, 2, 2, 2, 2, 2, 4, 
	2, 4, 2, 4, 2, 2, 2, 2, 
	2, 2, 17, 17, 0, 0, 2, 2, 
	4, 2, 4, 2, 2, 2, 2, 3, 
	1, 2, 3, 1, 2, 1, 3, 3, 
	1, 2, 15, 15, 1, 1, 1, 5, 
	3, 3
};

static const char _date_parser_range_lengths[] = {
	0, 3, 1, 1, 1, 1, 0, 0, 
	1, 0, 1, 1, 1, 0, 1, 1, 
	0, 1, 1, 1, 1, 0, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 0, 
	0, 0, 0, 1, 1, 1, 1, 1, 
	1, 1, 0, 1, 1, 0, 1, 1, 
	1, 1, 1, 1, 1, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 1, 2, 0, 
	0, 0, 2, 3, 1, 1, 0, 0, 
	2, 1, 1, 1, 1, 1, 0, 1, 
	1, 0, 1, 1, 1, 1, 1, 1, 
	0, 1, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 2, 2, 1, 1, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 1, 
	1, 2, 1, 1, 1, 1, 1, 1, 
	1, 2, 5, 5, 1, 1, 1, 1, 
	1, 1
};

static const short _date_parser_index_offsets[] = {
	0, 0, 18, 20, 22, 24, 26, 29, 
	32, 34, 37, 41, 43, 45, 47, 49, 
	51, 53, 55, 57, 59, 61, 63, 65, 
	67, 69, 72, 74, 76, 78, 81, 84, 
	101, 106, 109, 111, 114, 116, 118, 120, 
	122, 125, 127, 129, 131, 133, 135, 137, 
	139, 141, 143, 145, 147, 149, 152, 155, 
	158, 161, 164, 169, 172, 177, 180, 185, 
	188, 191, 194, 197, 200, 203, 205, 208, 
	211, 214, 216, 222, 229, 231, 250, 255, 
	258, 263, 265, 267, 269, 271, 273, 275, 
	277, 279, 281, 283, 285, 287, 289, 291, 
	293, 296, 298, 301, 304, 307, 310, 313, 
	318, 321, 326, 329, 334, 337, 340, 343, 
	346, 349, 352, 372, 392, 394, 396, 399, 
	402, 407, 410, 415, 418, 421, 424, 427, 
	432, 435, 440, 445, 448, 452, 455, 460, 
	465, 468, 473, 494, 515, 518, 521, 524, 
	531, 536
};

static const unsigned char _date_parser_indicies[] = {
	0, 2, 3, 5, 7, 8, 9, 10, 
	11, 7, 8, 9, 10, 11, 0, 4, 
	6, 1, 12, 1, 13, 1, 14, 1, 
	15, 1, 16, 16, 1, 17, 18, 1, 
	19, 1, 20, 20, 1, 21, 23, 22, 
	1, 24, 1, 25, 1, 26, 1, 27, 
	1, 28, 1, 29, 1, 30, 1, 31, 
	1, 32, 1, 33, 1, 34, 1, 35, 
	1, 36, 1, 25, 1, 38, 37, 1, 
	24, 1, 24, 1, 19, 1, 13, 39, 
	1, 40, 14, 1, 41, 42, 43, 44, 
	45, 46, 47, 48, 41, 42, 43, 44, 
	45, 46, 47, 48, 1, 49, 50, 49, 
	50, 1, 51, 51, 1, 52, 1, 53, 
	54, 1, 55, 1, 56, 1, 57, 1, 
	58, 1, 60, 59, 1, 61, 1, 62, 
	1, 63, 1, 64, 1, 65, 1, 66, 
	1, 67, 1, 68, 1, 69, 1, 70, 
	1, 71, 1, 61, 1, 51, 51, 1, 
	72, 72, 1, 51, 51, 1, 73, 73, 
	1, 51, 51, 1, 74, 75, 74, 75, 
	1, 51, 51, 1, 51, 51, 51, 51, 
	1, 76, 76, 1, 51, 51, 51, 51, 
	1, 77, 77, 1, 51, 51, 1, 78, 
	78, 1, 51, 51, 1, 79, 79, 1, 
	51, 51, 1, 39, 1, 39, 13, 1, 
	80, 80, 1, 81, 81, 1, 82, 1, 
	83, 84, 86, 82, 85, 1, 83, 84, 
	88, 82, 87, 89, 1, 89, 1, 89, 
	90, 91, 92, 93, 94, 95, 96, 97, 
	90, 91, 92, 93, 94, 95, 96, 97, 
	89, 1, 98, 99, 98, 99, 1, 100, 
	100, 1, 100, 101, 100, 102, 1, 103, 
	1, 104, 1, 105, 1, 106, 1, 107, 
	1, 108, 1, 109, 1, 110, 1, 111, 
	1, 112, 1, 113, 1, 114, 1, 115, 
	1, 116, 1, 117, 1, 118, 118, 1, 
	107, 1, 100, 100, 1, 119, 119, 1, 
	100, 100, 1, 120, 120, 1, 100, 100, 
	1, 121, 122, 121, 122, 1, 100, 100, 
	1, 100, 100, 100, 100, 1, 123, 123, 
	1, 100, 100, 100, 100, 1, 124, 124, 
	1, 100, 100, 1, 125, 125, 1, 100, 
	100, 1, 126, 126, 1, 100, 100, 1, 
	89, 90, 91, 92, 93, 94, 95, 96, 
	97, 90, 91, 92, 93, 94, 95, 96, 
	97, 89, 89, 1, 89, 90, 91, 92, 
	93, 94, 95, 96, 97, 90, 91, 92, 
	93, 94, 95, 96, 97, 89, 89, 1, 
	89, 1, 89, 1, 127, 127, 1, 81, 
	81, 1, 128, 127, 128, 127, 1, 81, 
	81, 1, 129, 130, 129, 130, 1, 81, 
	81, 1, 81, 81, 1, 131, 131, 1, 
	81, 81, 1, 133, 134, 134, 132, 1, 
	132, 132, 1, 132, 38, 132, 37, 1, 
	135, 136, 136, 135, 1, 137, 137, 1, 
	138, 139, 138, 1, 138, 138, 1, 140, 
	141, 141, 140, 1, 142, 141, 141, 142, 
	1, 143, 143, 1, 106, 145, 106, 144, 
	1, 146, 147, 147, 149, 149, 150, 149, 
	149, 151, 149, 149, 150, 149, 149, 151, 
	146, 148, 148, 148, 148, 1, 152, 147, 
	147, 149, 149, 150, 149, 149, 151, 149, 
	149, 150, 149, 149, 151, 152, 148, 148, 
	148, 148, 1, 153, 153, 1, 154, 154, 
	1, 155, 155, 1, 155, 156, 156, 156, 
	156, 155, 1, 155, 156, 156, 155, 1, 
	155, 118, 118, 155, 1, 0
};

static const unsigned char _date_parser_trans_targs[] = {
	1, 0, 2, 29, 69, 70, 3, 71, 
	118, 120, 122, 125, 3, 4, 5, 6, 
	7, 8, 28, 9, 10, 11, 26, 27, 
	127, 13, 14, 15, 16, 17, 18, 130, 
	20, 21, 22, 23, 131, 12, 24, 30, 
	31, 32, 54, 56, 58, 61, 63, 65, 
	67, 33, 53, 34, 35, 36, 37, 37, 
	38, 39, 132, 41, 52, 42, 43, 44, 
	45, 46, 47, 134, 49, 50, 51, 136, 
	55, 57, 59, 60, 62, 64, 66, 68, 
	72, 73, 74, 75, 76, 116, 117, 114, 
	115, 77, 78, 99, 101, 103, 106, 108, 
	110, 112, 79, 98, 80, 81, 82, 82, 
	83, 84, 137, 86, 87, 88, 89, 90, 
	91, 138, 93, 94, 95, 140, 142, 100, 
	102, 104, 105, 107, 109, 111, 113, 119, 
	121, 123, 124, 126, 128, 129, 25, 128, 
	19, 128, 133, 40, 135, 48, 135, 133, 
	85, 97, 139, 92, 142, 143, 144, 145, 
	139, 141, 141, 141, 96
};

static const char _date_parser_trans_actions[] = {
	0, 0, 1, 55, 55, 55, 1, 19, 
	19, 19, 19, 19, 0, 0, 0, 0, 
	0, 3, 3, 0, 0, 5, 5, 5, 
	0, 0, 0, 9, 0, 0, 11, 0, 
	0, 0, 0, 0, 0, 7, 7, 0, 
	0, 39, 39, 39, 39, 39, 39, 39, 
	39, 0, 0, 0, 0, 41, 41, 0, 
	0, 0, 0, 43, 43, 0, 0, 45, 
	0, 0, 47, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 21, 21, 21, 21, 21, 
	21, 0, 23, 23, 23, 23, 23, 23, 
	23, 23, 0, 0, 0, 25, 25, 0, 
	0, 0, 0, 0, 0, 29, 0, 0, 
	31, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 15, 
	13, 61, 0, 0, 51, 49, 0, 76, 
	27, 27, 35, 33, 33, 33, 33, 33, 
	0, 67, 0, 70, 0
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
	0, 0, 0, 0, 0, 0, 0, 17, 
	17, 17, 58, 79, 53, 53, 73, 53, 
	91, 37, 64, 37, 83, 37, 87, 87, 
	87, 87
};

static const int date_parser_start = 1;
static const int date_parser_first_final = 127;
static const int date_parser_error = 0;

static const int date_parser_en_main = 1;


#line 185 "date_parser.rl"


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

  
#line 460 "date_parser.c"
	{
	cs = date_parser_start;
	}

#line 244 "date_parser.rl"
  
#line 467 "date_parser.c"
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
#line 89 "date_parser.rl"
	{ t_iso_year = p; }
	break;
	case 1:
#line 90 "date_parser.rl"
	{ t_iso_month = p; }
	break;
	case 2:
#line 91 "date_parser.rl"
	{ t_iso_day = p; }
	break;
	case 3:
#line 92 "date_parser.rl"
	{ t_iso_hour = p; }
	break;
	case 4:
#line 93 "date_parser.rl"
	{ t_iso_minute = p; }
	break;
	case 5:
#line 94 "date_parser.rl"
	{ t_iso_second = p; }
	break;
	case 6:
#line 95 "date_parser.rl"
	{ t_iso_zone = p; }
	break;
	case 7:
#line 97 "date_parser.rl"
	{ iso_state |= RHRR_ISO_TIME_SET; }
	break;
	case 8:
#line 98 "date_parser.rl"
	{
    t_iso_zone_end = p;
    iso_state |= RHRR_ISO_ZONE_SET;
  }
	break;
	case 10:
#line 118 "date_parser.rl"
	{ t_rfc_wday = p; }
	break;
	case 11:
#line 119 "date_parser.rl"
	{ t_rfc_day = p; }
	break;
	case 12:
#line 120 "date_parser.rl"
	{ t_rfc_month = p; }
	break;
	case 13:
#line 121 "date_parser.rl"
	{ t_rfc_year = p; }
	break;
	case 14:
#line 122 "date_parser.rl"
	{ t_rfc_hour = p; }
	break;
	case 15:
#line 123 "date_parser.rl"
	{ t_rfc_minute = p; }
	break;
	case 16:
#line 124 "date_parser.rl"
	{ t_rfc_second = p; }
	break;
	case 17:
#line 125 "date_parser.rl"
	{ t_rfc_zone = p; }
	break;
	case 18:
#line 127 "date_parser.rl"
	{ rfc_state |= RHRR_RFC_TIME_SET; }
	break;
	case 19:
#line 128 "date_parser.rl"
	{
    t_rfc_zone_end = p;
    rfc_state |= RHRR_RFC_ZONE_SET | RHRR_RFC_ZONE_NUM_SET;
  }
	break;
	case 20:
#line 132 "date_parser.rl"
	{
    t_rfc_zone_end = p;
    rfc_state |= RHRR_RFC_ZONE_SET | RHRR_RFC_ZONE_NAME_SET;
  }
	break;
	case 22:
#line 155 "date_parser.rl"
	{ t_clf_day = p; }
	break;
	case 23:
#line 156 "date_parser.rl"
	{ t_clf_month = p; }
	break;
	case 24:
#line 157 "date_parser.rl"
	{ t_clf_year = p; }
	break;
	case 25:
#line 158 "date_parser.rl"
	{ t_clf_hour = p; }
	break;
	case 26:
#line 159 "date_parser.rl"
	{ t_clf_minute = p; }
	break;
	case 27:
#line 160 "date_parser.rl"
	{ t_clf_second = p; }
	break;
	case 28:
#line 161 "date_parser.rl"
	{ t_clf_zone = p; }
	break;
	case 29:
#line 163 "date_parser.rl"
	{ clf_state |= RHRR_CLF_TIME_SET; }
	break;
	case 30:
#line 164 "date_parser.rl"
	{
    t_clf_zone_end = p;
    clf_state |= RHRR_CLF_ZONE_SET;
  }
	break;
#line 669 "date_parser.c"
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
	case 7:
#line 97 "date_parser.rl"
	{ iso_state |= RHRR_ISO_TIME_SET; }
	break;
	case 8:
#line 98 "date_parser.rl"
	{
    t_iso_zone_end = p;
    iso_state |= RHRR_ISO_ZONE_SET;
  }
	break;
	case 9:
#line 102 "date_parser.rl"
	{ parsers |= RHRR_ISO_PARSER; }
	break;
	case 18:
#line 127 "date_parser.rl"
	{ rfc_state |= RHRR_RFC_TIME_SET; }
	break;
	case 19:
#line 128 "date_parser.rl"
	{
    t_rfc_zone_end = p;
    rfc_state |= RHRR_RFC_ZONE_SET | RHRR_RFC_ZONE_NUM_SET;
  }
	break;
	case 20:
#line 132 "date_parser.rl"
	{
    t_rfc_zone_end = p;
    rfc_state |= RHRR_RFC_ZONE_SET | RHRR_RFC_ZONE_NAME_SET;
  }
	break;
	case 21:
#line 136 "date_parser.rl"
	{ parsers |= RHRR_RFC_PARSER; }
	break;
	case 29:
#line 163 "date_parser.rl"
	{ clf_state |= RHRR_CLF_TIME_SET; }
	break;
	case 30:
#line 164 "date_parser.rl"
	{
    t_clf_zone_end = p;
    clf_state |= RHRR_CLF_ZONE_SET;
  }
	break;
	case 31:
#line 168 "date_parser.rl"
	{ parsers |= RHRR_CLF_PARSER; }
	break;
#line 737 "date_parser.c"
		}
	}
	}

	_out: {}
	}

#line 245 "date_parser.rl"

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
        if (iso_state & RHRR_ISO_ZONE_SET) {
          zone = t_iso_zone;
          zone_len = t_iso_zone_end - zone;
          offset = atol(zone) * 3600 + atol(zone + 4) * 60;
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
