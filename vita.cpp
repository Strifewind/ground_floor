#include "Vita.h"

#include <iostream>
#include <limits>
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
