#include "valid.h"
#include "Vita.h"

#include <iostream>
#include <limits>
#include <cctype>
#include <cstring>

void printIntro();
void printMenu();
void printOutro();

void getName(Vita &vita);
void getBio(Vita &vita);
void getJob(Vita &vita);
void getSkills(Vita &vita);


int main() {

  printIntro();
  Vita vitas;
  bool isRunning = true;

  getName(vitas);
  getBio(vitas);

  while (isRunning) {

    printMenu();
    int choice = readIntInRange("Please enter your choice: ", 1, 5);

    switch (choice) {
      case 1:

        getJob(vitas);
        break;

      case 2:

        getSkills(vitas);
        break;

      case 3:

        vitas.displaySumma();
        break;

      case 4:

        vitas.displaySkills();
        break;

      case 5:
        isRunning = false;
        printOutro();
        break;
    }
  }

  return 0;
}


/**
 *
 */
void printIntro() {

  std::cout << "\t=== Resume Builder ===\n"
            << "Using groud floor enter your skills and biography, provide previous \n"
            << "job histories to store.\n" << "\n Use tex template to print \n"
            << "doc with formatted text." << std::endl;
}


/**
 *
 */
void printMenu() {

  std::cout << "--- Menu options ---\n"
            << "  1) Add new Job details.\n"
            << "  2) Add skills to list.\n"
            << "  3) Display vita.\n"
            << "  4) Display skills.\n"
            << "  5) Quit\n" << std::endl;
}


/**
 *
 */
void printOutro(){

  std::cout << "Thank you for using ground floor.\n"
            << " You have entered " << " amount of vita" << std::endl;
}


/**
 *
 */
void getName(Vita &vita) {

  char name[256];

  readCharArray("Enter your name: ", name, 256);
  vita.addName(name);
}


/**
 *
 */
void getBio(Vita &vita) {

  char bio[256];

  readCharArray("Enter a breif biography for your resume: ", bio, 256);
  vita.addBio(bio);
}


/**
 *
 */
void getJob(Vita &vita) {

  char company[256];
  char position[256];
  Date start, end;

  readCharArray("Enter the company name: ", company, 256);
  readCharArray("Enter the position held: ", position, 256);

  readDate(start.month, start.day, start.year);
  readDate(end.month, end.day, end.year);

  vita.addSumma(company, position, start, end);
}


/**
 *
 */
void getSkills(Vita &vita) {

  char skill[256];

  do {

    readCharArray("Enter a skill (or 'q' to quit): ", skill, 256, false);

    if (std::strcmp(skill, "q") != 0 && skill[0] != '\0') {
      vita.addSkill(skill);
    }
  } while (std::strcmp(skill, "q") != 0 && skill[0] != '\0');
}
