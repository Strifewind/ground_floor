#include "vita.h"

#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>


void Vita::loadFromToml(const char filename[]) {

  clear();

  std::ifstream in(filename);
  if (!in) {

    std::cout << "Error opening TOML file.\n";
    return;
  }

  char line[512];

  bool inBio        = false;
  bool inSkills     = false;
  bool inJobBlock   = false;

  char bioBuffer[4096];
  bioBuffer[0] = '\0';

  char company[256];
  char position[256];
  Date startDate{};
  Date endDate{};

  bool haveCompany    = false;
  bool havePosition   = false;
  bool haveStart      = false;
  bool haveEnd        = false;

  while (in.getline(line, 512)) {

    char *p = line;
    while (*p != '\0' && std::isspace(static_cast<unsigned char>(*p))) {

      ++p;
    }

    if (inBio) {
      if (std::strncmp(p, "\"\"\"", 3) == 0) {

        addBio(bioBuffer);
        inBio = false;
        continue;
      }

      std::strcat(bioBuffer, p);
      std::strcat(bioBuffer, "\n");
      continue;
    }


    if (inSkills) {

      if (*p == ']') {
        inSkills = false;
        continue;
      }

      char *firstQuote = std::strchr(p, '\"');
      if (!firstQuote) {
        continue;
      }
      char *secondQuote = std::strchr(firstQuote + 1, '\"');
      if(!secondQuote) {
        continue;
      }

      char skill[256];
      int len = secondQuote - firstQuote - 1;
      if (len > 255) len = 255;
      std::strncpy(skill, firstQuote + 1, len);
      skill[len] = '\0';

      addSkill(skill);

      continue;
    }


    if (inJobBlock) {

      if (*p == '\0') {

        if (haveCompany && havePosition && haveStart && haveEnd) {
          addSumma(company, position, startDate, endDate);
        }

        haveCompany = havePosition = haveStart = haveEnd = false;
        inJobBlock = false;
        continue;
      }

      if(std::strncmp(p, "company", 7) == 0) {

        char *firstQuote = std::strchr(p, '\"');
        char *secondQuote = firstQuote ? std::strchr(firstQuote + 1, '\"') : nullptr;

        if (firstQuote && secondQuote) {

          int len = secondQuote - firstQuote - 1;
          if (len > 255) len = 255;
          std::strncpy(company, firstQuote + 1, len);
          company[len] = '\0';
          haveCompany = true;
        }
        continue;

      }

      if (std::strncmp(p, "position", 8) == 0) {

        char *firstQuote = std::strchr(p, '\"');
        char *secondQuote = firstQuote ? std::strchr(firstQuote + 1, '\"') : nullptr;

        if (firstQuote && secondQuote) {

          int len = secondQuote - firstQuote - 1;
          if (len > 255) len = 255;
          std::strncpy(position, firstQuote + 1, len);
          position[len] = '\0';
          havePosition = true;
        }
        continue;

      }


      if (std::strncmp(p, "start", 5) == 0) {

        char *firstQuote = std::strchr(p, '\"');
        char *secondQuote = firstQuote ? std::strchr(firstQuote + 1, '\"') : nullptr;

        if (firstQuote && secondQuote) {

          char dateStr[32];
          int len = secondQuote - firstQuote - 1;
          if (len > 31) len = 31;
          std::strncpy(dateStr, firstQuote + 1, len);
          dateStr[len] = '\0';

          int mm = 0, dd = 0, yy = 0;
          std::sscanf(dateStr, "%d/%d/%d", &mm, &dd, &yy);
          startDate.month   = mm;
          startDate.day     = dd;
          startDate.year    = yy;
          haveStart = true;
        }
        continue;

      }

      if (std::strncmp(p, "end", 3) == 0) {

        char *firstQuote = std::strchr(p, '\"');
        char *secondQuote = firstQuote ? std::strchr(firstQuote + 1, '\"') : nullptr;

        if (firstQuote && secondQuote) {

          char dateStr[32];
          int len = secondQuote - firstQuote - 1;
          if (len > 31) len = 31;
          std::strncpy(dateStr, firstQuote + 1, len);
          dateStr[len] = '\0';

          int mm = 0, dd = 0, yy = 0;
          std::sscanf(dateStr, "%d/%d/%d", &mm, &dd, &yy);
          endDate.month   = mm;
          endDate.day     = dd;
          endDate.year    = yy;
          haveEnd = true;
        }
        continue;
      }


      continue;
    }



    if (*p == '\0') {

      continue;
    }

    if (std::strncmp(p, "name", 4) == 0) {

      char *firstQuote = std::strchr(p, '\"');
      char *secondQuote = firstQuote ? std::strchr(firstQuote + 1, '\"') : nullptr;
      if (firstQuote && secondQuote) {

        char tempName[256];
        int len = secondQuote - firstQuote - 1;
        if (len > 255) len = 255;
        std::strncpy(tempName, firstQuote + 1, len);
        tempName[len] = '\0';
        addName(tempName);
      }
      continue;
    }

    if (std::strncmp(p, "bio", 3) == 0) {

      inBio = true;
      bioBuffer[0] = '\0';
      continue;
    }

    if (std::strncmp(p, "skills", 6) == 0) {

      inSkills = true;
      continue;
    }

    if (std::strncmp(p, "[[job]]", 7) == 0) {

      inJobBlock = true;

      haveCompany = havePosition = haveStart = haveEnd = false;
      continue;
    }
  }

  if (inJobBlock && haveCompany && havePosition && haveStart && haveEnd) {

    addSumma(company, position, startDate, endDate);
  }
}
