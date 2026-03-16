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


/**
 *
 */
void Vita::exportToTex(const char templateFile[],
                       const char outputFile[]) const {

  std::ifstream in(templateFile);
  if (!in) {
    std::cout << "Error: Could not open template file.\n";
    return;
  }

  std::ofstream out(outputFile);
  if (!out) {
    std::cout << "Error: Could not open output file.\n";
    return;
  }

  char line[1024];

  char skillBlock[50][256];
  int skillBlockSize = 0;

  char jobBlock[50][256];
  int jobBlockSize = 0;

  bool inSkillBlock = false;
  bool inJobBlock = false;

  while (in.getline(line, 1024)) {

    if (std::strstr(line, "%BEGIN_SKILL%")) {

      inSkillBlock = true;
      skillBlockSize = 0;
      continue;
    }

    if (std::strstr(line, "%END_SKILL%")) {

      inSkillBlock = false;

      for (int i = 0; i < numSkills; ++i) {

        for (int j = 0; j < skillBlockSize; ++j) {

          char temp[256];
          std::strcpy(temp, skillBlock[j]);

          replacePlaceholder(temp, "<<SKILL>>", skills[i]);

          out << temp << "\n";
        }
      }
      continue;
    }

    if (inSkillBlock) {
      std::strcpy(skillBlock[skillBlockSize], line);
      skillBlockSize++;
      continue;
    }

    if (std::strstr(line, "%BEGIN_JOB%")) {

      inJobBlock = true;
      jobBlockSize = 0;
      continue;
    }

    if (std::strstr(line, "%END_JOB%")) {

      inJobBlock = false;

      Summa *curr = head;
      char dateBuffer[16];

      while (curr != nullptr) {

        for (int j = 0; j < jobBlockSize; ++j) {

          char temp[256];
          std::strcpy(temp, jobBlock[j]);

          replacePlaceholder(temp, "<<COMPANY>>", curr->company);
          replacePlaceholder(temp, "<<POSITION>>", curr->position);

          formatDate(curr->startDate, dateBuffer);
          replacePlaceholder(temp, "<<STARTDATE>>", dateBuffer);

          formatDate(curr->endDate, dateBuffer);
          replacePlaceholder(temp, "<<ENDDATE>>", dateBuffer);

          out << temp << "\n";

        }

        curr = curr->next;
      }

      continue;

    }

    if (inJobBlock) {

        std::strcpy(jobBlock[jobBlockSize], line);
        jobBlockSize++;
        continue;
    }

      replacePlaceholder(line, "<<NAME>>", (name ? name : ""));
      replacePlaceholder(line, "<<BIO>>",  (biog ? biog : ""));

      out << line << "\n";

  }
}
