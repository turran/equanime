#include "stdio.h"

/*
 * The parser that generates .c and .h
 *
 * Message without reply:
 * msg MyMessage {
 *   float f;
 * };
 *
 * Message with reply:
 * msg MyMessageAndReply {
 *   int i;
 *   char c;
 *   string str;
 * } => {
 *   int[] iarray;
 * };
 * 
 */

int main(int argc, char **argv)
{
	char *s_header;
	char *s_source;
	FILE *f_header;
	FILE *f_source;
}
