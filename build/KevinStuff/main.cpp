//Operator class for testing functionality of DER agents.
// Three basic commands availabe, import, export, idle.


// INCLUDE
#include <iostream>
#include <string>
//#include <map>
//#include "operator.cpp"
//#include "operator.cpp"



//#include <vector>
#include <stdio.h>
#include <stdlib.h>
//#include <math.h>
#include "Operator.h"
//#include "UtilityK.hpp"
//Moved to header file.
/*
class Operator
{
public:
//    File *csvp;
    char fname[];

  // member functions
  // functions, include Import, Export, and Idle.
  void Import();
  void Export();
  void Idle();

private:
  // member properties

  };
*/

int main()
{

    Operator op("./timeActExt.csv");

    //std::cout << op.fname << std::endl;
   //         printf("\n Testing.\n");

    //op.dataLength = op.GetDataLength(op.fname);

    //printf("\n\%d",op.dataLength);

    //op.Idle();
    //op.time =
    //Operator op;

    //printf("\n\%s",op.fname);

    std::cout << op.fname << std::endl;
    //std::cout << a << std::endl;
    op.Loop();
        //std::cout << op.time << std::endl;



    //::cin.get();
    return 0;
};

/*
        char fname[] = "taction.csv";
        //printf("\n..\n..\n..\n %s \n %d \n..\n..\n..\n", str, &str);
        FILE *csvp;                                 //Creates a pointer to the file.
        csvp = fopen(fname, "r");    //Uses that pointer open my csv file.
            if (!csvp){                             //In case of error in loading, stop the program.
                printf("Error loading taction.csv.\n");
                return 0;
            }

        int counter= 0;
        int x = 3;
        for(;;){
            fscanf(csvp, "%*[^\n]\n", NULL);     // Skips to the next line.
            x = feof(csvp);
            //printf("\n\%d",x);
            counter++;
            if (x){
                printf("\n End of file reached.");
                printf("\n\%d",counter);
                break;
                }
            }
        fclose(csvp);


        int time[counter];
        char action[counter];

printf("\n Test\n.");
        //FILE *csvp2;
        csvp = fopen(fname, "r");    //Uses that pointer open my csv file.
            if (!csvp){                             //In case of error in loading, stop the program.
                printf("Error loading taction.csv.\n");
                return 0;
                }





    //Reading the data lines from CSV.
        for(int i = 0; i < counter ; i++){
            fscanf(csvp, "%d", &time[i]);       //Saves the time.
            fgetc(csvp);                        //Eats the comma.
            fgetc(csvp);                        //Eats the first letter.
            action[i] = fgetc(csvp);            //Saves the second letter.

            fscanf(csvp, "%*[^\n]\n", NULL);         // Skips to the next line.

            if (feof(csvp)){
                printf("\n End of file reached.");
                }
            }

        printf("\n %d    %c \n %d     %c\n %d    %c \n %d    %c \n %d    %c \n %d    %c \n %d   %c \n %d   %c ", time[10000], action[10000], time[20000],action[20000], time[30000],action[30000], time[40000],action[40000], time[50000],action[50000],  time[60000],action[60000],  time[70000],action[70000], time[80000],action[80000]);
     fclose(csvp);

//Close the .csv file.
        //fclose(csvp);

    //Input Portion Complete
        printf("\n Data finished parsing.\n");
*/
