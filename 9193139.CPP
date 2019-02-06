#include <iostream.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>
#include <dos.h> //delay()
#include <fstream.h>
#include <time.h> //total time taken by user

class score
{
    char name[20];
    int attempts;
    double time;
    double highscore;

    public:
        void assign(char string[20], int a, double t)
        {
            strcpy(name, string);
            attempts = a;
            time = t;
            highscore = 32000 - (59*(attempts)*(attempts)) - (19*(time)); //formula for highscore
        }

        void output()
        {
            cout << name << "\t" << highscore << endl;
        }

        double retScore()
        {
            return highscore;
        }

        char *retName()
        {
            return name;
        }
};

class master
{
    char answer[4];
    int red;
    int white;

    public:
        void assign(char code[], int r, int w)
        {
            strcpy(answer, code);
            red = r;
            white = w;
        }
        void output()
        {
            for (int i = 0; i < 4; i++)
            {
                cout << answer[i];
            }
            cout << ", RED: " << red << ", WHITE: " << white << endl;
        }
};

/*The following functions write() and read() are used
  to save the ATTEMPT HISTORY so that the user can
  analyze their previous attempts.

  Although this can be done simply by printing
  the previous attempts using "cout", the attempts
  MUST BE SAVED as the Turbo C++ graphics require
  the use of cleardevice() functions that delete
  all console outputs
*/

void write(char code[], int r1, int w1)
{
    ofstream f1;
    f1.open("okay.dat", ios::binary | ios::app);
    master A;
    A.assign(code, r1, w1);
    f1.write((char*)&A, sizeof(A));
}

void read()
{
    ifstream f;
    f.open("okay.dat", ios::binary | ios::in);
    master A;
    while (f.read((char*)&A, sizeof(A)))
    {
        A.output();
    }
    f.close();
}

void highscoreWrite(char name[20], int attempts, double time) //saves highscores in binary file scores.dat
{
    ofstream f; //write;
    f.open("scores.dat", ios::binary | ios::app);

    score A;
    A.assign(name, attempts, time);

    f.write((char*)&A, sizeof(A));

    f.close();
}

/*The following function is called while converting a double 
  (user's score) into a character array because outtextxy() 
  can work ONLY WITH char *arr or char arr[]
*/

int lengthOfNumber(double t)
{
    int n = 0;
    do
    {
        n++;
        t = int(t) / 10;
    } while (t != 0);

    return n;
}

void highscoreRead()
{
    ifstream f; //read;
    f.open("scores.dat", ios::binary);

    score A;
    score arr[100];
    int i = 0;
    while (f.read((char*)&A, sizeof(A)))
    {
        arr[i] = A;
        i++;
    }
    int total = i;

    //SORTS SCORES IN DESCENDING ORDER
    for (i = 0; i < total; i++)
    {
        int min = i;
        for (int j = i+1; j < total; j++)
        {
            if (arr[j].retScore() > arr[min].retScore())
            {
                min = j;
            }
        }
        if (min != i)
        {
            score temp = arr[min];
            arr[min] = arr[i];
            arr[i] = temp;
        }
    }

    //DETERMINES HIGHEST SCORE
    double highest = 0;
    for (i = 0; i < total; i++)
    {
        arr[i].output();
        if (arr[i].retScore() > highest)
        {
            highest = arr[i].retScore();
        }
    }

    //FOR BEST PLAYER (HIGHEST SCORER)
    char bestplayer[10];
    for (i = 0; i < total; i++)
    {
        if (arr[i].retScore() == highest)
        {
            strcpy(bestplayer, arr[i].retName());
        }
    }

    //CONVERT DOUBLE TO INT AND THEN INT TO DOUBLE
    int n = int(highest);

    char stringa[10];
    for (i = lengthOfNumber(highest)-1; i >= 0; i--) //USING lengthofNumber(double) function
    {
        if (n == 0)
        {
            break;
        }
        stringa[i] = (n%10)+48;
        n /= 10;
    }
    stringa[lengthOfNumber(highest)] = '\0'; //TO ENSURE NO JUNK VALUE IS PRINTED WITH outtextxy() 

    settextstyle(3, 0, 3);
    outtextxy(150, 200, "HIGHEST SCORER: ");
    outtextxy(350, 200, bestplayer);
    outtextxy(150, 250, "HIGHEST SCORE: ");
    outtextxy(350, 250, stringa);

    f.close();
}

int lSearch(char n, char string[], int size) //determines if element already exists in array
{
    for (int i = 0; i < size; i++)
    {
        if (n == string[i])
        {
            return 1;
        }
    }
    return 0;
}

void doubleToString(char s[], int size, double t) //CONVERT DOUBLE TO INT AND INT TO STRING
{
    int n = int(t);

    for (int i = lengthOfNumber(t) - 1; i >= 0; i--)
    {
        if (n == 0)
        {
            break;
        }
        s[i] = (n % 10) + 48;
        cout << s[i] << endl;
        getch();
        n /= 10;
    }
    s[lengthOfNumber(n)] = '\0';
    cout << s << endl;
}

void assignScore(int a, double t, double &highscore)
{
	highscore = 32000 - (59*(a)*(a)) - (19*(t)); //score calculation for main()
}

void main ()
{
    clrscr();

    //graphdriver is an integer that specifies the graphics driver to be used.

    /*graphmode is an integer that specifies the initial graphics mode 
      (unless *graphdriver equals DETECT; in which case, *graphmode is 
      set by initgraph to the highest resolution available for the 
      detected driver).
    */
     
    int gm = DETECT, gd = DETECT;
    initgraph(&gm,&gd,"\\tc\\bgi");

    //WELCOME PAGE
    settextstyle(3, 0, 8);
    outtextxy(100, 180, "MASTERMIND");
    settextstyle(3, 0, 1);
    outtextxy(180, 270, "BY ROLL NUMBER 9193139");
    settextstyle(3, 0, 2);
    outtextxy(260, 350, "1. PLAY");
    outtextxy(230, 380, "2. HIGHSCORES");

    char choice = getch(); //only accepts value, doesn't print it (unlike cin)

    if (choice == '1') //PLAY
    {
        cleardevice();

        outtextxy(270, 250, "Loading...");
        delay(500);

        cleardevice();

        settextstyle(3, 0, 8);
        outtextxy(50, 20, "HOW TO PLAY");

        settextstyle(2, 0, 5);
        outtextxy(10, 150, "I will randomly decide a 4-digit code from 0-9, without repetitions");

        settextstyle(3, 0, 2);
        outtextxy(180, 350, "Press any key to continue");
        getch(); //waits for user to press any key

        settextstyle(2, 0, 5);
        outtextxy(10, 180, "You are to guess the code using two clues");
        getch();

        outtextxy(30, 200, "1. If a number and its index are the same as in the code, you get a RED");
        getch();

        outtextxy(30, 220, "2. If the number is correct, but not the index, you get a WHITE");
        getch();

        outtextxy(10, 260, "The faster you arrive at the answer, the higher your score will be.");
        getch();

        cleardevice(); //clears graphics screen
        clrscr(); //clears console screen

        settextstyle(3, 0, 5);
        outtextxy(230, 200, "Loading...");
        delay(1000);

        cleardevice();
        clrscr();

        char string[4];

        randomize(); //initializes random() using time

        for (int i = 0; i < 4; i++)
        {
            int n = 1 + random(9);
            while (i != 0 && lSearch(char(n+48), string, 4))
            {
                n = 1 + random(9); //n is between 1 and 9
            }
            char x = n+48;
            if (i == 0)
            {
                string[i] = x;
            }
            else if (!lSearch(x, string, 4)) //AVOIDS REPETITIONS
            {
                string[i] = x;
            }
        }
        string[i] = '\0'; //TO ENSURE NO JUNK VALUE IS USED
        cout << string << endl;

        int gameWin = 1;
        int red = 0;
        int white = 0;

        int numberOfAttempts = 0;

        clock_t start = clock(); 
        //clock_t is a type suitable for storing the processor time
        //clock() returns the processor clock time

        clock_t stop;
        double time_taken;
        do
        {
            numberOfAttempts++;

            settextstyle(3, 0, 2);

            //DISPLAYED ON RIGHT, ALONGSIDE ATTEMPT-HISTORY
            outtextxy(300, 0, "Enter your guess below:");
            settextstyle(2, 0, 7);


            //CLOSE ANYTIME
            outtextxy(200, 400, "Enter 0000 to QUIT");

            cout << "\n\n";

            read();

            char guess[4];
            for (int j = 0; j < 4; j++)
            {
                guess[j] = getch();
            }
            guess[j] = '\0';

            settextstyle(5, 0, 3);
            outtextxy(370, 25, guess);
            stop = clock();

            getch();

            red = 0;
            white = 0;
            gameWin = 1;
            for (int k = 0; k < 4; k++)
            {
                if (guess[k] == string[k])
                {
                    red++;
                }
                else if (lSearch(guess[k], string, 4))
                {
                    white++;
                    gameWin = 0;
                }
                else
                {
                    gameWin = 0;
                }
            }

            if (!strcmp(guess, "0000"))
            {
                gameWin = 0;
                break;
            }

            settextstyle(3, 0, 3);
            outtextxy(170, 430, "Press any key to continue");
            getch();


            if (gameWin == 0)
            {
                write(guess, red, white);
            }

            cleardevice();
            clrscr();
        } while (gameWin != 1);


        stop = clock();
        time_taken = double(stop-start)/CLOCKS_PER_SEC; //difference is time elapsed

        //CLOCKS_PER_SEC represents the number of processor clocks per second.

        string[4] = '\0';

        if (gameWin != 1)
        {
            cleardevice();
            settextstyle(3, 0, 3);
            outtextxy(210, 140, "The answer was ");
            settextstyle(3, 0, 5);
            outtextxy(250, 170, string);
            settextstyle(3, 0, 2);
            outtextxy(200, 220, "Better luck next time!");
            cout << "Time Taken: " << time_taken;
            settextstyle(3, 0, 3);
            outtextxy(170, 430, "Press any key to continue");
            getch();
        }

        else if (gameWin == 1)
        {
            cleardevice();
            settextstyle(3, 0, 3);
            outtextxy(260, 140, "You got it!");
            settextstyle(3, 0, 5);
            outtextxy(265, 170, string);

            double score = 0;
            assignScore(numberOfAttempts, time_taken, score);

            int n = int(score);

            char stringa[10];
            for (int i = lengthOfNumber(score)-1; i >= 0; i--)
            {
                if (n == 0)
                {
                    break;
                }
                stringa[i] = (n%10)+48;
                n /= 10;
            }
            stringa[lengthOfNumber(score)] = '\0';

            cout << stringa << endl;

            outtextxy(180, 230, "Score: ");
            outtextxy(320, 230, stringa);
            settextstyle(3, 0, 3);
            outtextxy(170, 430, "Press any key to continue");
            getch();
            cleardevice();
            settextstyle(3, 0, 3);
            outtextxy(20, 220, "Would you like to enter your score (press y/n)");
            char hs = getch();
            if (hs == 'y')
            {
                cleardevice();
                char name[20];
                cout << "Enter your name: ";
                gets(name);
                highscoreWrite(name, numberOfAttempts, time_taken); //write into FILE in APPEND MODE
                cout << "Score successfully saved." << endl;
                settextstyle(3, 0, 3);
                outtextxy(170, 430, "Press any key to continue");
                getch();
            }
        }

        ofstream f;
        f.open("okay.dat", ios::binary | ios::out); //TO DELETE ATTEMPT HISTORY
        f.close();
    }

    else if (choice == '2')
    {
        cleardevice();
        cout << "Name\tHighscore\n";
        highscoreRead();
        getch();
    }

    cleardevice();
    settextstyle(3, 0, 5);
    outtextxy(70, 200, "THANK YOU FOR PLAYING");
    settextstyle(3, 0, 2);
    outtextxy(160, 430, "Press any key to close the app");
    getch();
    cleardevice();
    closegraph(); //deallocates memory allocated by the graphics system and restores the screen to orginal mode before initgraph()
}