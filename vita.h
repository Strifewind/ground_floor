#ifndef VITA_H
#define VITA_H

const char DATA_FILE[] = "vita.toml";

struct Date {
  int day;
  int month;
  int year;
};

struct Summa {
  char *company;
  char *position;
  Date startDate;
  Date endDate;
  Summa *next;
};

// The summa should be a linked list of Summa struct nodes
// The class vita will init the list and summa's
// hold definitions on how to list or display the summa's
//
// The Skills need to be a dynamic array of characters
// The biog is a single block of char "text"
class Vita {
  private:
    Summa *head;

    int numSkills;
    int skillCap;
    char **skills;
    char *biog;
    char *name;

    void increaseSize();
    void clear();
    void formatDate(const Date &d, char buffer[]) const;

  public:
    Vita();
    ~Vita();
    void reset();

    void addName(const char name[]);
    void addBio(const char bio[]);

    bool addSkill(const char skill[]);

    void addSumma(const char company[],
                  const char position[],
                  const Date &start,
                  const Date &end);
    
    bool hasName() const { return name != nullptr; }
    bool hasBio()  const { return biog != nullptr; }

    void displaySumma();
    void displaySkills();
    void display();

    void exportToTex(const char templateFile[], const char outFile[]) const;
    void saveToToml(const char filename[]) const;
    void loadFromToml(const char filename[]);
};
#endif
