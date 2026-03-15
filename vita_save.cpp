#include "vita.h"

#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>

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

