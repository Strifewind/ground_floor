#include "vita.h"

#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>


//void Vita::exportToTex(const char templateFile[], const char outFile[]) const;

//void replacePlaceholder(char line[], const char placeholder[], const char replacement[]);
//void writeJobBlock(std::ofstream &out, char jobBlock[][256], int blockLines) const;
//void writeSkillBlock(std::ofstream &out, char skillBlock[][256], int blockLines) const;

//bool isBeginJob(const char line[]);
//bool isEndJob(const char line[]);


/**
 *
 */
void replacePlaceholder(char line[], const char placeholder[], const char replacement[]) {

  char temp[1024];
  char *pos = strstr(line, placeholder);

  if (!pos) return;

  int index = pos - line;
  strncpy(temp, line, index);
  temp[index] = '\0';

  strcat(temp, replacement);

  strcat(temp, pos + strlen(placeholder));

  strcpy(line, temp);
}


