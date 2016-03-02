#include "ShipDefParser.h"
#include <stdio.h>
#include <string.h>

void ShipDefParseLine(char * line, ShipDef *def) {
	switch (line[1]) {
	case 'M':{
		switch (line[2]) {
		case 'A':{
			switch (line[4]) {
			case 'C':{
				sscanf(line, "%*[^:]:%d]", &def->mMaxCrew);
				break;
			}
			case 'W':{
				sscanf(line, "%*[^:]:%d]", &def->mMaxWeight);
				break;
			}
			}
			break;
		}
		case 'I':{
			switch (line[4]) {
			case 'C':{
				sscanf(line, "%*[^:]:%d]", &def->mMinCrew);
				break;
			}
			case 'W':{
				sscanf(line, "%*[^:]:%d]", &def->mMinWeight);
				break;
			}
			}
			break;
		}
		}
		break;
	}
	case 'N':{
		sscanf(line, "%*[^:]:%[^\]]", &def->mName);
		break;
	}
	}
}

void LoadShipDef(char * path, ShipDef *def) {
	FILE * file = fopen(path, "rt");
	char line[120];
	while (fgets(line, 120, file) != NULL) {
		ShipDefParseLine(line, def);
	}
}
