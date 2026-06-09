#ifndef SUMA_H
#define SUMA_H


class Summa {
  Public:

    int createSumma();
    int readSumma();
    int updateSumma();
    int deleteSumma();

  Private:

    char *company;
    char* position;
    Date startDate;
    Date endDate;
    

};

#endif
