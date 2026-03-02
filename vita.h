#ifndef VITA_H
#define VITA_H

const int VITA_SIZE = 3;

struct Date {
  int day;
  int month;
  int year;
};

struct Summa {
  char company[];
  char lastPositionHeld[];
  Date startDate;
  Date endDate;
  char otherPositionsHeld[];
  char dailyTasks[];
  char jobDescription[];
  char techStack[];
};

class Vita {
  private:
    Summa * summas;
    int numSumma;
    char skills[];
    char biog[];

  public:
    Vita();
    ~Vita();

    void addSumma();
    void displaySumma();
    void printSumma();
    void increaseSize();

    void loadFile();
    void saveFile();
};
#endif
