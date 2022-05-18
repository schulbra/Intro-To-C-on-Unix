/*------------------------------------------------------------------------------------------------|
--      Name:           Brandon Schultz
---     Date:           1 - 8 - 21
----    Course:         CS 344 400 W2022
-----   Assignment:     1 :  Movies
------  Description:    This program:
------  - Reads a CSV file with movie data that is provided to the program as an argument
------  - Processes the data in the file to create structs to hold data for each movie
------  - Creates a linked list of all these structs
------  - Gives user choices to ask questions about the movies in the data
------  - Prints out the data about the movies per user choice
-----   - References: https://www.tutorialspoint.com/cprogramming/c_structures.htm
------              https://www.geeksforgeeks.org/comparator-function-of-qsort-in-c/
------              https://en.wikipedia.org/wiki/C_standard_library
------              https://www.tutorialspoint.com/cprogramming/c_type_casting.htm
------              https://stackoverflow.com/questions/5057987/whats-the-point-of-const-void
----                https://stackoverflow.com/questions/34842224/when-to-use-const-void
---                 http://www.cplusplus.com/reference/cstdlib/qsort/
--                  https://www.javatpoint.com/tokens-in-c
--                  https://stackoverflow.com/questions/15961253/c-correct-usage-of-strtok-r
--                  Students.c and various other sample code via Canvas
|------------------------------------------------------------------------------------------------*/
#include <stdio.h>         //   Function definitions used throughout program
#include <stdlib.h>
#include <string.h>

/*------------------------------------------------------------------------------------------------|
- Structure used to represent various movie attributes as defined in movies_sample_1.csv :
- Additional context is provided below :
|------------------------------------------------------------------------------------------------*/

struct Movie
{
    char *title;          //  Movie's title.
    int year;             //  Movie's year of release between 1900-2021.
    char *languages;      //  Language(s) movie is in.
    double rating;        //  Movies designated Rating value between 1-10
    struct Movie *next;   //  -------------------------------------------------------
}      Movie;

/*------------------------------------------------------------------------------------------------|
- Structure used to parse current line of movie data and create data structure representing movie
  item containg attributes listed above for "struct Movie"
- Additional context is provided below :
- Note, the token setup is heavily inspired by student.c from Canvas
|------------------------------------------------------------------------------------------------*/

struct Movie *createMovie(char *currLine)
{
    // -Create movie object on curr line:
    // -structs cannot have functions for themselves so ned to use function pointers.
    //   - struct Movie                     =   pointers base type
    //   -  *currMovie                      =   name of pointer variable =
    //   -  malloc(sizeof(struct Movie))    =   bytes allocated in mem for Movie
    struct Movie *currMovie = malloc(sizeof(struct Movie));
    // - strtok_r
    char *saveptr;
    //_______________________|                                              
    // Token 1 = Movie Title |_________________________________
    //-----------------------------------------------------------------
     char *token = strtok_r(currLine, ",", &saveptr);
     currMovie->title = calloc(strlen(token) + 1, sizeof(char));
     strcpy(currMovie->title, token);
    //______________________||                                              
    /// Token 2 = Movie Year  |____________
     token = strtok_r(NULL, ",", &saveptr);
     currMovie->year = atoi(token);
    //_____________________|||                                              
    //// Token 3 = Movie Lang ||_____________________________________
     token = strtok_r(NULL, ",", &saveptr);
     currMovie->languages = calloc(strlen(token) + 1, sizeof(char));
     strcpy(currMovie->languages, token);

    //_______________________||||
    ///// Token 4 = Movie Rating |||___________     
     token = strtok_r(NULL, "\n", &saveptr);
     currMovie->rating = strtod(token,NULL);

     currMovie->next = NULL;     // Next LL node = NULL in new Movie entry
    return currMovie;
}

/*------------------------------------------------------------------------------------------------|
- Structure used to return linked list of movies after parsing data line by line from
  movies_sample_1.csv / somethingElse.extension: 
|------------------------------------------------------------------------------------------------*/
struct Movie *processMoviesFileNameExtension(char *moviesFileNameExtension)
{
    // Opens and read from file designated in terminal, ideally  movies_sample_1.csv as no other
    // extensions tested but dont tell anyone
    FILE *movieFile = fopen(moviesFileNameExtension, "r");    // read from existing file
    char *currLine = NULL;
    size_t length = 0;
    size_t skipLine;
    char *token;
    struct Movie *Head = NULL;      // Head of Movie Linked List
    struct Movie *Tail = NULL;      // Tail of Movie Linked List

    //  Skip First line of CSV, then read line by line: 
    skipLine = getline(&currLine, & length, movieFile);
    while ((skipLine = getline(&currLine, &length, movieFile)) != -1)
     {
        //  newMovieNode = curr line of csv/file
        struct Movie *newMovieNode = createMovie(currLine);
        // -Check if node is first node in LL, if so, attach to
        // node the head and tail that mill make up start/end of LL.
        if (Head == NULL)
        {
         Head = newMovieNode;
         Tail = newMovieNode;
        }
        // -Add non-first node to list and move Tail, 
        else
        {
          Tail->next = newMovieNode;
          Tail = newMovieNode;
        }
     }
    free(currLine);   //release memory
    fclose(movieFile);// close file including movie list

    return Head;
}

/*------------------------------------------------------------------------------------------------|
- Structure used to display given Movie information:
|------------------------------------------------------------------------------------------------*/
void printCurrMovieInfo(struct Movie* currMovie)
{
  printf("%s, %d %s, %.1f\n", currMovie->title, currMovie->year, currMovie->languages,
  currMovie->rating);
}

/*------------------------------------------------------------------------------------------------|
- Structure used to display LL of Movies:
|------------------------------------------------------------------------------------------------*/
void printMovieList(struct Movie *list)
{
  // while list of movies in csv contains movies
  while (list != NULL)
  {
   // show movies until all movies from movie column 
   // have been included.
   printCurrMovieInfo(list);
   list = list->next;
  }
}
 // For managing currTotalMovieList of movies included in csv.
 int totalMovieList(struct Movie *list)
  {
  int currTotalMovieList=0;
  while (list != NULL)
  {
    currTotalMovieList++;
    list = list->next;
  }
  return currTotalMovieList;
  }

/*------------------------------------------------------------------------------------------------|
- Structure used to display list of movies released in designated year:
|------------------------------------------------------------------------------------------------*/
void printMovieYear(struct Movie *list, int inputOption)
{
  // Takes csv containing movie release years, eventually printing titles released per year
  // based on UI's four digit val.
  printf("\n");
  int inputValidationFail = 1;
  // While year entered matchs movie release year from csv...
  // Print title, and compare the next film in list's year of release
  // against the UI value until all 20 films have been checked
  while (list != NULL)
  {
    if (list->year == inputOption)
    {
     printf("%s\n",list->title);
     inputValidationFail = 0;
    }
    list = list->next;
  }
  // if year entered contains no released movies in csv,
  // tell the user to try harder and enter a better year:
  if(inputValidationFail)
  {
   printf("It doesn't look like anything was released in that year.  %i\n",inputOption);
  }

  printf("\n");

}

//-------------------------------------------------------------------------
// - Used to prevent displaying films released having a lower rating then 
// film with higher rating when years of release are equal to UI year val
//-------------------------------------------------------------------------
int yearMovieCheck(int yearUI,int* yearUIRepeatList)
{
  int i=0;
    // While film years still exist to compare 
    while(yearUIRepeatList[i]!=-1)
    {
      // If UI year = year of film previously compared
      if(yearUI == yearUIRepeatList[i])
      {
        // dont print it, skip to next comparison
        return 0;        
      }
      i++;
    }
  return 1;

}

//-------------------------------------------------------------------------
// - Used to display best rated film released during designated year:
//-------------------------------------------------------------------------
void printHighestYear(int yearUI,struct Movie *list)
{
 struct Movie *highest = malloc(sizeof(struct Movie));
 highest->rating = -1;

 while(list!=NULL)          // -while list of movies contains additional rows...
 {                          // -if UI year matches to a year of release val from
  if(list->year == yearUI)  // row in CSV, and if said rows rating val is > val 
  {                         // of currently hgihest rated film processed, assign 
   if(list->rating > highest->rating) // new title as the now highest rated film
   {                                  // released for given year and update rating
    highest->title = list->title;     // to reflect new best film of said year.
    highest->rating = list->rating;   // - Then move onto next comparison if list
   }                                  // in csv contains additional rows/ films
  }
 list=list->next;
 }
  // Display values/year to user:
  // update below-----------------------------------------------------------------
 printf("%i: %s , %.1f\n",yearUI,highest->title,highest->rating);
 free(highest);       //Clear memory after printing.
}


//-------------------------------------------------------------------------
// Adds years previously checked to repeat arr yearUIRepeatList:
//-------------------------------------------------------------------------
void addToYearUIRepList(int yearMovie,int* yearUIRepeatList)
{
  int i =0;
  while(yearUIRepeatList[i]!=-1)
  {
    i++;
  }
  // assign now checked year to spot in array of previosuly compared years
  yearUIRepeatList[i]=yearMovie;
}


//-------------------------------------------------------------------------
// Displays highest rated movie for all valid years via comparisons made using
// functions above. Additional context is provided below: 
//-------------------------------------------------------------------------
void topMovieRating(struct Movie *list)
{
  printf("\n");
  // - add 1 to count of movies total and allocate size in repeat list
  //   for soon to be comapred and eventually, previously checked year val:
  int currTotalMovieList = totalMovieList(list)+1;
  int* yearUIRepeatList = calloc(currTotalMovieList,sizeof(int));
  // - add movie year to particular index of repeat list
  for(int i=0;i<currTotalMovieList;i++)
  {
   yearUIRepeatList[i] = -1;
  }

  // While list contains valid rows from csv to compare...
  // If year being compared is in repeat list due to some other
  // film having the same release year, take film of same year
  // but having higher avg rating.Add year val to repeat list
  // and compare next row/year. Keep doing this as a means to
  // parse entire csv for highest rated film/year until no rows
  // remain and print now list containing all years with relases of
  // highest rated movie/year. Then, clear memory after printing.
  while (list != NULL)
  {
   if(yearMovieCheck(list->year,yearUIRepeatList))
   {
    printHighestYear(list->year,list);
    addToYearUIRepList(list->year,yearUIRepeatList);
   }
  list = list->next;
  }
  printf("\n\n");
  free(yearUIRepeatList);   //clear memory
}

// Print all movies and their years of release for given language
void printMoviesWithLanguage(struct Movie *list,char* language)
{
  int inputValidationFailLang = 1;
  // If a language match exists, print film title and year
  // move onto next comparison of list until all rows/movies
  // processed.
  while (list != NULL)
  {
    if (strstr(list->languages,language))
    {
      printf("%s , %d\n",list->title,list->year); // show titles
      inputValidationFailLang = 0;                   // no titles of given lang
  }
  list = list->next;
 }

// If no matches are found for UI lang val, tell them nothing was found:
if(inputValidationFailLang)
 {
  printf("Doesn't look like any language to me: %s\n",language);
 }
 printf("\n");
}

//
int main(int argc, char *argv[])
{
    struct Movie *list = processMoviesFileNameExtension(argv[1]);
    printf("\n");
    int numberOfMovies = totalMovieList(list);
    printf(" _________________________________________________________________________________  \n");
    printf("| -      -  Name: Brandon Schultz                                            ---- | \n");
    printf("| --     -  Date: 1-12-22                                                     --- | \n");
    printf("| ---    -  Assignment 1: Movies                                               -- | \n");
    printf("| ----   -  Description: What am I?  A Machine Clearly.                         - | \n");
    printf("| ---       You can call me AMC and I LOVE MOVIES...                            - | \n");
    printf("| --        listed in CSV files. You provide me the goods                      -- | \n");
    printf("| -         and I'll show you:                                                --- | \n");
    printf("| --         1. All Movies Released In Defined Year                          ---- | \n");
    printf("| ---        2. Highest Rated Movie In Defined Year                           --- | \n");
    printf("| ----       3. Title+Release Date of All Movies In Defined Year               -- | \n");
    printf("| ---        4. Quit Program                                                    - | \n");
    printf("| --         and if you dont like that well                                    -- | \n");
    printf("| -          THATS SHOW BUSINESS BABYYYYYYY                                   --- | \n");
    printf("|_________________________________________________________________________________| \n\n");
    printf(" |       - Processed (the goods) and...\n");
    printf(" ||      - Processed the file %s and parsed data for %i films.\n",argv[1],numberOfMovies);
    printf(" |||                \n");
    printf(" ||||    - Now what?\n");
    // printf("|________________________________________________________|\n");
    int inputOptionSelection;
    int inputOption;
    char* language;
    while(1==1)
    { //printf("  - Processed (the goods)
        printf("         _______________________________________________________________\n"); 
        printf("        |   - Valid Input = 1-4 ONLY    |    - Selection Description:   |\n");   
        printf("        |_______________________________________________________________|\n\n"); 
        printf("     1. Show movies released in the specified year                               ||||\n");                         
        printf("     2. Show highest rated movie for each year                                    |||\n");          
        printf("     3. Show the title and year of realease of all movies in a specific language   ||\n");
        printf("     4. Exit from the program                                                       |\n");
        printf("   \n        - Enter a choice from 1 to 4: ");
        scanf("%i",&inputOptionSelection);
       // printf("   - Choose using your hands \n");
        
      if (inputOptionSelection == 1)
      {
        printf("Enter the year for which you want to see movies: ");
        scanf("%i",&inputOption);
        printMovieYear(list,inputOption);
      }

      else if (inputOptionSelection == 2)
      {
        topMovieRating(list);
      }

      else if (inputOptionSelection == 3)
      {
        printf("      ---  Enter the language for which you want to see movies: ");
        scanf("%s",language);
        printMoviesWithLanguage(list,language);
      }
      
      else if (inputOptionSelection == 4)
      {
        return EXIT_SUCCESS;
      }

      else if (inputOptionSelection >= 5)
      {
        printf(" ---- Nothing was found...\n");
        printf(" ---- Try another input value: ");
        printf(" \n");    

      }
  }
}

