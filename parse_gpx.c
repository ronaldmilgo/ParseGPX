#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>

// defining globally
#define BUFFER_SIZE 100
char tag_buffer[BUFFER_SIZE];
int tag_index = 0;
char attr_buffer[BUFFER_SIZE];
int attr_index = 0;
char value_buffer[BUFFER_SIZE];
int value_index = 0;
char ele_buffer[BUFFER_SIZE];
int ele_index = 0;
char time_buffer[BUFFER_SIZE];
int time_index = 0;
char lat[BUFFER_SIZE];
char lon[BUFFER_SIZE];
char ele[BUFFER_SIZE];
char time[BUFFER_SIZE];
char comma[BUFFER_SIZE * 2]; // multiplying by two to handle any extras!(where I store the altered after comma)

void replace_commas(char *str) {
    int i, j = 0; //j keeps track of current location(with replacements)

	for (i = 0; str[i] != '\0'; i++){
		if (str[i] == ','){
			strcpy(&comma[j], "&comma;");
			j += 7; 
		} else{
			comma[j++] = str[i];
		}
	}
	comma[j] = '\0';
	strcpy(str, comma);
}
enum state {TOP, TAGNAME, DECLARATION, WS_TAG, ATTR_NAME, WS_ATTRNAME, EQ, ATTRVALUE_SQ, ATTRVALUE_DQ, WS_EQ, CLOSING_TAGNAME, ERROR}; // states 



int main(void)
{
  // initial state is outside of any tag, and not inside any elements
  // of interest
  enum state curr = TOP; // tracks the current state of the program. Initially set to TOP.
  


  bool inside_trkpt = false, inside_ele = false, inside_time = false;

  int ch;
  while ((ch = fgetc(stdin)) != EOF) //main loop that reads the input character by character. ch stores character from the input.
    {
		// printf("char: %c, %i\n", ch, curr);
      // one case in the switch per state
      switch (curr)
	{
	case TOP:
	  // what do we do when outside any tag and we see ch?
	  if (ch == '<'){
		if (inside_trkpt && inside_ele){
			// printf("ele: %s\n", ele_buffer);
			replace_commas(ele_buffer);
			strcpy(ele, ele_buffer);
			inside_ele = false;
			tag_index = 0;
			ele_index = 0;
		} else if (inside_trkpt && inside_time){
			// printf("time: %s\n", time_buffer);
			replace_commas(time_buffer);
			strcpy(time, time_buffer);
			inside_time = false;
			time_index = 0;
			ele_index = 0;
			printf("%s,%s,%s,%s\n", lat, lon, ele, time);
		}
		curr = TAGNAME;
	  } else if (inside_trkpt && inside_ele){
		// printf("ele_buffer: %s\n", ele_buffer);
		ele_buffer[ele_index++] = ch;
		ele_buffer[ele_index] = '\0';
	  } else if (inside_trkpt && inside_time){
		
		time_buffer[time_index++] = ch;
		time_buffer[time_index] = '\0';
		// printf("time_buffer: %s\n", time_buffer);
	  } 
	  break;

	// define cases for additional states you define here 
	case TAGNAME:
		if (isalpha(ch)){
			tag_buffer[tag_index++] = ch; 
			tag_buffer[tag_index] = '\0'; // null-terminate for buffer safety
			// printf("tag_buffer: %s\n", tag_buffer);
		} else if (isspace(ch)){ // whitespace indicates end of tagnames
			if (strncasecmp(tag_buffer, "trkpt", 5) == 0){
				// printf("Yaay trkpt!\n");
				inside_trkpt = true;
			} if (strcasecmp(tag_buffer, "ele") == 0){
				// printf("Yaay ele!\n");
				inside_ele = true;
			} else if (strcasecmp(tag_buffer, "time") == 0){
				// printf("Yaay time!\n");
				inside_time = true;
			}
			curr = WS_TAG;
		} else if (ch == '>'){
			if (strncasecmp(tag_buffer, "ele", 3) == 0){
				// printf("Yaay ele!\n");
				inside_ele = true;
			} else if (strncasecmp(tag_buffer, "time", 4) == 0  && inside_trkpt){
				// printf("Yaay time!\n");
				inside_time = true;	
			}
			tag_index = 0;
			curr = TOP;
		} else if (ch == '!'){
			curr = DECLARATION;
		} else if (ch == '?'){
			curr = DECLARATION;
		} else if (ch == '/'){
			curr = CLOSING_TAGNAME;
		}
		break;
	
	case DECLARATION:
		if (ch == '>'){
			curr = TOP;
		}
		break;

	case WS_TAG:
		if (strncasecmp(tag_buffer, "trkpt", 5) == 0){
			if (isspace(ch)){
			// continue skipping whitespace
		} else if (isalpha(ch)){
			attr_index = 0;
			attr_buffer[attr_index++] = ch; // store the first alphabet of the attribute name
			// printf("attr_buffer: %s\n", attr_buffer);
			curr = ATTR_NAME;
		} else if (ch == '>'){
			curr = TOP;
		}
		} else if (ch == '>'){
			curr = TOP;
			tag_index = 0;
		}
		break;

	case ATTR_NAME:
		if (isalpha(ch)){ //keep track of the attribute name
			attr_buffer[attr_index++] = ch;
			// printf("attr_buffer: %s\n", attr_buffer);
		} else if (isspace(ch)){
			attr_buffer[attr_index] = '\0';
			curr = WS_ATTRNAME;
		} else if (ch == '='){
			attr_buffer[attr_index] = '\0';
			curr = EQ;
		} else if (ch == '>'){
			attr_buffer[attr_index] = '\0';
			curr = TOP;
		}
		break;

	case WS_ATTRNAME:
		if (isspace(ch)){

		} else if (ch == '='){
			curr = EQ;
		}
		break;

	case EQ:
		if (ch == '\''){
			curr = ATTRVALUE_SQ;
		} else if (ch == '\"'){
			curr = ATTRVALUE_DQ;
		} else if (isspace(ch)){
			curr = WS_EQ;
		}
		break;
	
	case ATTRVALUE_SQ:
		if (ch == '\''){
			value_buffer[value_index] =	'\0'; // terminate string
			if (strncmp(attr_buffer, "lat", 3) == 0){
				strcpy(lat, value_buffer);
				// printf("Attribute: %s: %s\n", attr_buffer, value_buffer);
				memset(value_buffer, 0, BUFFER_SIZE);
				memset(attr_buffer, 0, BUFFER_SIZE);
			} else if (strncmp(attr_buffer, "lon", 3) ==0){
				strcpy(lon, value_buffer);
				// printf("Attribute: %s: %s\n", attr_buffer, value_buffer);
				memset(value_buffer, 0, BUFFER_SIZE);
				memset(attr_buffer, 0, BUFFER_SIZE);
			}
			attr_index = 0;
			value_index = 0;
			tag_index = 0;
			curr = WS_TAG;
		} else { 
			// append character to the value_buffer
			if (value_index < BUFFER_SIZE - 1 ){
				value_buffer[value_index++] = ch;
				// printf("value_buffer: %s\n", value_buffer);
			};
			
		}
		break;

	case ATTRVALUE_DQ:
		if (ch == '\"'){
			value_buffer[value_index] =	'\0'; // terminate string
			if (strncmp(attr_buffer, "lat", 3) == 0){
				replace_commas(value_buffer);
				strcpy(lat, value_buffer);
				// printf("Attribute: %s: %s\n", attr_buffer, value_buffer);
				memset(value_buffer, 0, BUFFER_SIZE);
				memset(attr_buffer, 0, BUFFER_SIZE);
			} else if (strncmp(attr_buffer, "lon", 3) ==0){
				replace_commas(value_buffer);
				strcpy(lon, value_buffer);
				// printf("Attribute: %s: %s\n", attr_buffer, value_buffer);
				memset(value_buffer, 0, BUFFER_SIZE);
				memset(attr_buffer, 0, BUFFER_SIZE);
			}
			attr_index = 0;
			value_index = 0;
			tag_index = 0;
			curr = WS_TAG;
		} else { 
			// append character to the value_buffer
			if (value_index < BUFFER_SIZE - 1 ){
				value_buffer[value_index++] = ch;
				// printf("value_buffer: %s\n", value_buffer);
			};
			
		}
		break;

	case WS_EQ:
		if (isspace(ch)){

		} else if (ch == '\"'){
			curr = ATTRVALUE_DQ;
		} else if (ch == '\''){
			curr = ATTRVALUE_SQ;
		}
		break;
	
	case CLOSING_TAGNAME:
		if (ch == '>'){
			curr = TOP;
		}
		break;	
	case ERROR:
	  // we can send things here as a precaution when things
	  // are in the wrong place and then keep doing nothing
	  // until the end of file
	  break;
	}
    }
}