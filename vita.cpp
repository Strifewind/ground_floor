#include "Vita.h"

#include <iostream>
#include <limits>
#include <fstream>
#include <cctype>
#include <cstring>


/**
 *
 */
Vita::Vita() {

  head = nullptr;

  name = nullptr;
  biog = nullptr;
  skills = nullptr;

  numSkills = 0;
  skillCap = 0;
  loadFromToml(DATA_FILE);
}


/**
 *
 */
Vita::~Vita() {

  saveToToml(DATA_FILE);
  clear();
}


/**
 *
 */
void Vita::clear() {

  Summa* current = head;
  while (current != nullptr) {

    Summa * toDelete = current;
    current = current->next;

    delete [] toDelete->company;
    delete [] toDelete->position;
    delete toDelete;
  }

  head = nullptr;

  if (skills != nullptr) {

    for (int i = 0; i < numSkills; ++i) {

      delete [] skills[i];
    }
    delete [] skills;
    skills = nullptr;

  }
  numSkills = 0;
  skillCap = 0;

  if (name != nullptr) {

    delete [] name;
    name = nullptr;
  }

  if (biog != nullptr) {

    delete [] biog;
    biog = nullptr;
  }
}


/**
 *
 */
void Vita::addName(const char name[]) {

  if (this->name != nullptr) {
    delete [] this->name;
    this->name = nullptr;
  }

  int len = std::strlen(name);

  this->name = new char[len + 1];

  std::strcpy(this->name, name);
}


/**
 *
 */
void Vita::addBio(const char bio[]) {

  if (this->biog != nullptr) {
    delete [] this->biog;
    this->biog = nullptr;
  }
  int len = std::strlen(bio);

  this->biog = new char[len + 1];

  std::strcpy(this->biog, bio);
}


/** 
 *
 */
bool Vita::addSkill(const char skill[]) {

  if (skillCap == 0) {

    skillCap = 4;
    skills = new char*[skillCap];
  
  } else if (numSkills == skillCap) {

    increaseSize();
  }

  int len = std::strlen(skill);
  skills[numSkills] = new char[len + 1];

  std::strcpy(skills[numSkills], skill);

  ++numSkills;

  return true;
}


/**
 *
 */
void Vita::addSumma(const char company[],
                    const char position[],
                    const Date &start,
                    const Date &end) {

  Summa *newNode = new Summa;

  int lenCompany = std::strlen(company);
  newNode->company = new char[lenCompany + 1];
  std::strcpy(newNode->company, company);


  int lenPosition = std::strlen(position);
  newNode->position = new char[lenPosition + 1];
  std::strcpy(newNode->position, position);

  newNode->startDate  = start;
  newNode->endDate    = end;

  newNode->next = head;
  head = newNode;
}


/**
 *
 */
void Vita::displaySumma() {

  Summa *current = head;
  while (current != nullptr) {
    std::cout << "\nCompany:  " << current->company  << "\n"
              << "Position: " << current->position << "\n"
              << "Start: "
              << current->startDate.month << "/"
              << current->startDate.day   << "/"
              << current->startDate.year  << "\n"
              << "End:   "
              << current->endDate.month   << "/"
              << current->endDate.day     << "/"
              << current->endDate.year    << "\n"
              << "-------------------------" << std::endl;
    current = current->next;
  }
}


/**
 *
 */
void Vita::displaySkills() {

  std::cout << "Skills (" << numSkills << "):\n";
  for (int i = 0; i < numSkills; ++i) {

    std::cout << "  - " << skills[i] << std::endl;
  }
}


/**
 *
 */
void Vita::increaseSize() {

  int newSize = skillCap + 4;
  char** newSkills = new char*[newSize];

  for (int i = 0; i < numSkills; ++i) {
    newSkills[i] = skills[i];
  }

  delete [] skills;

  skills = newSkills;
  skillCap = newSize;
}


/**
 *
 */
void Vita::formatDate(const Date &d, char buffer[]) const {
  sprintf(buffer, "%02d/%02d/%04d", d.month, d.day, d.year);
}


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


/**
 *
 */
void Vita::saveToToml(const char filename[]) const {

  std::ofstream out(filename);
  if (!out) {
    std::cout << " Error: could not open TOML file for writing." << std::endl;
    return;
  }

  // ===== NAME =====
  out << "name = \"";
  if (name != nullptr) {
    out << name;
  }
  out << "\"\n\n";

  // ===== BIO (triple-quoted) =====
  out << "bio = \"\"\"\n";
  if (biog != nullptr) {
    out << biog << "\n";
  } else {
    out << "\n"; // empty bio
  }
  out << "\"\"\"\n\n";

  // ===== SKILLS ARRAY =====
  out << "skills = [\n";
  for (int i = 0; i < numSkills; ++i) {
    out << "  \"" << skills[i] << "\",\n";
  }
  out << "]\n\n";

  // ===== JOBS (linked list as repeated [[job]] tables) =====
  Summa *current = head;
  char dateBuffer[16];

  while (current != nullptr) {
    out << "[[job]]\n";
    out << "company = \"" << current->company  << "\"\n";
    out << "position = \"" << current->position << "\"\n";

    // Start date
    formatDate(current->startDate, dateBuffer);
    out << "start = \"" << dateBuffer << "\"\n";

    // End date
    formatDate(current->endDate, dateBuffer);
    out << "end   = \"" << dateBuffer << "\"\n\n";

    current = current->next;
  }

}


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
