#include "ShipDefParser.h"
#include <stdio.h>
#include <string.h>

void ShipDefParseLine(char * line, ShipDef *def) {
switch(line[0]) {
case 'M':{
switch(line[1]) {
case 'A':{
switch(line[3]) {
case 'C':{
sscanf(line, "%*[^:]:%d]", &dst->mMaxCrew);
break;
}
case 'W':{
sscanf(line, "%*[^:]:%d]", &dst->mMaxWeight);
break;
}
}
break;
}
case 'I':{
switch(line[3]) {
case 'C':{
sscanf(line, "%*[^:]:%d]", &dst->mMinCrew);
break;
}
case 'W':{
sscanf(line, "%*[^:]:%d]", &dst->mMinWeight);
break;
}
}
break;
}
}
break;
}
case 'N':{
sscanf(line, "%*[^:]:%[^\]]", &dst->mName);
break;
}
}
}

void LoadShipDef(char * path, ShipDef *def) {
FILE * file = fopen(path, "rt");
char line[120];
while(fgets(line, 120, file) != NULL) {
ShipDefParseLine(line, def);
}
}
