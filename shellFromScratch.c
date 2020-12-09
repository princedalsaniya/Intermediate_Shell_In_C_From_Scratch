// designing c program to implement the shell
/*
    TO-DO:
        grep --n.
        diff.
        proper commenting.
        tail.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXCHAR 1000    //maximum number of character in a command.
#define MAXARGS 10      //maximum number of argument can be in one command
char *parcedArgsSpace[MAXARGS];     //container type array to store each arguments.
char HISTORY[20][MAXCHAR];          //array to store each command for history. 
int countParcedSpace = 0, indexOfSwitch = -1, history_count = 0;    
//how many arguments are there in one command, index of argument where it finds the switch, tracking the numbers of commands in history array.

void clear();

//startup screen of the shell.
void initialization()
{
    clear();
    printf("*************************************\n");
    printf("WELCOME TO SHELL CREATED BY TEAM OS\n");
    printf("*************************************\n");
}

//taking input String.
int takeInput(char inputStr[])
{
    char temp[MAXCHAR];
    printf("\n\n>>>");
    scanf("%[^\n]%*c", temp); //it will take input untill \n arrives.

    //if the input is non empty then add this to history and also set it as input string.
    if (strlen(temp) != 0)
    {
        strcpy(inputStr, temp);
        strcpy(HISTORY[history_count], temp);
        history_count++;
        return 0;
    }
    //or else ingore it and just loop back to take input.
    else
    {
        return 1;
    }
}

//tokenizing the command inputed by space and store them in parcedArgsSpace array.
void parceSpace(char str[]){
    int i;

    for (i = 0; i < MAXARGS; i++){
        char *x = strsep(&str, " ");

        if (x == NULL)
            break;
        else {
            parcedArgsSpace[i] = x;
            countParcedSpace++;
        }
        if (strlen(parcedArgsSpace[i]) == 0) {
            i--;
            countParcedSpace--;
        }
    }
    // printf("size is %d", countParcedSpace);
    // for (i=0; i<countParcedSpace; i++){
    //     printf("%d. %s", i, parced[i]);
    // }
}

//finding the switch and if that is present then store it's index.
int checkForSwitch(char str[]){
    int i;
    for (i = 0; i < countParcedSpace; i++) {
        char *ptr = strstr(parcedArgsSpace[i], "--");
        if (ptr != NULL) {
            indexOfSwitch = i;
            return 0;
        }
    }
    return 1;
}

//clearing the token which stores. Once we used that we will clear it and store new arguments whennext command comes.
void clearParcedSpace(){
    for (size_t i = 0; i < MAXARGS; i++)
        parcedArgsSpace[i] = NULL;

    countParcedSpace = 0;
    indexOfSwitch = -1;
}

// proccesing str for space and switch.
void processString(char str[]){
    parceSpace(str);
    //0 if there is switch. 1 if there is no switch.
    checkForSwitch(str);
}

//------------------------IMPLEMENTING COMMANDS HERE.-----------------------//
void help(){
    printf("----------------Welcome to shell created as a part of OS Project.----------------\n");
    printf("We are Prince, Mithilesh, Shaili, Yagnik, tried to implement basic shell in c language.\n");
    printf("List of the commands we are supporting : \n");
    printf("1.exit\n");
    printf("2.help\n");
    printf("3.clear\n");
    printf("4.cat\n");
    printf("5.cat --b\n");
    printf("6.man\n");
    printf("7.rm\n");
    printf("8.calc\n");
    printf("9.cmp\n");
    printf("10.diff\n");
    printf("11.head\n");
    printf("12.echo\n");
    printf("13.history\n");
    printf("14.cd\n");
    printf("15.pwd\n");
    printf("16.wc --w\n");
    printf("17.wc --l\n");
    printf("18.wc --c\n");
    printf("19.wc --o\n");
    printf("20.grep --l\n");
    printf("Thanks for using this.");
}

void clear(){
    printf("\033[H\033[J");
}

void cat(){
    FILE *file;
    char line[1000];

    file = fopen(parcedArgsSpace[1], "r");

    if(file == NULL){
        printf("File dosen't exist. Try other file.");
        return;
    }
        
    printf("Output of the CAT command : \n");

    // reading file line by line entering to buffer line
    while (fscanf(file, "%[^\n]\n", line) != EOF)
        printf("%s\n", line);

    fclose(file);
}

void rm(){
    if (countParcedSpace < 2){
        printf("You are entring the wrong format. Check manual by \"man rm\".");
        return;
    }
    int flag = remove(parcedArgsSpace[1]);
    if (flag != 0)
        printf("Sorry. Either the file can't be deleted or it doesn't exist.\n");
    else
        printf("File %s deleted successfully!\n", parcedArgsSpace[1]);
}

void calc(){
    if (countParcedSpace < 2){
        printf("wrong format. chackout man page for this.");
        exit(0);
    }

    float opt1, opt2, ans;
    int opr1, opr2;
    int ch = 0;
    
    if (strcmp(parcedArgsSpace[2], "/") == 0)
        ch = 1;
    else if (strcmp(parcedArgsSpace[2], "*") == 0)
        ch = 2;
    else if (strcmp(parcedArgsSpace[2], "-") == 0)
        ch = 3;
    else if (strcmp(parcedArgsSpace[2], "+") == 0)
        ch = 4;
    else if (strcmp(parcedArgsSpace[2], "mod") == 0)
        ch = 5;
    else{
        printf("Invalid operator to perform calculation. Try +,-,*,/,mod");
        exit(0);
    }
    switch (ch){
        case 1:
            opt1 = atof(parcedArgsSpace[1]);
            opt2 = atof(parcedArgsSpace[3]);
            ans = opt1 / opt2;
            printf("%f / %f = %f", opt1, opt2, ans);
            return;
        case 2:
            opt1 = atof(parcedArgsSpace[1]);
            opt2 = atof(parcedArgsSpace[3]);
            ans = opt1 * opt2;
            printf("%f * %f = %f", opt1, opt2, ans);
            return;
        case 4:
            opt1 = atof(parcedArgsSpace[1]);
            opt2 = atof(parcedArgsSpace[3]);
            ans = opt1 + opt2;
            printf("%f + %f = %f", opt1, opt2, ans);
            return;
        case 3:
            opt1 = atof(parcedArgsSpace[1]);
            opt2 = atof(parcedArgsSpace[3]);
            ans = opt1 - opt2;
            printf("%f - %f = %f", opt1, opt2, ans);
            return;
        case 5:
            opr1 = atoi(parcedArgsSpace[1]);
            opr2 = atoi(parcedArgsSpace[3]);
            ans = opr1 % opr2;
            printf("%d mod %d = %f", opr1, opr2, ans);
            return;
        default:
            return;
    }
}

void cmp(){
    //opening 2 files to compare.
    FILE *fp1 = fopen(parcedArgsSpace[1], "r");
    FILE *fp2 = fopen(parcedArgsSpace[2], "r");

    if (fp1 == NULL || fp2 == NULL){
        printf("any one or both File/s can not found...");
        exit(0);
    }

    char ch1, ch2;
    int error_count = 0;
    int count1 = 0;     //for calculating the size of both files.
    int count2 = 0;
    while ((ch1 = fgetc(fp1)) != EOF)
        count1++;
    while ((ch2 = fgetc(fp2)) != EOF)
        count2++;
    if (count1 != count2){
        printf("%s and %s aren't have same size. So, both are not same.", parcedArgsSpace[1], parcedArgsSpace[2]);
        exit(0);
    }

    fclose(fp1);
    fclose(fp2);
    
    fp1 = fopen(parcedArgsSpace[1], "r");
    fp2 = fopen(parcedArgsSpace[2], "r");

    while ((ch1 = fgetc(fp1)) != EOF && (ch2 = fgetc(fp2)) != EOF){
        if (ch1 != ch2){
            printf("Both files are not same.\n");
            return;
        }
    }

    printf("Both files are same.");
    fclose(fp1);
    fclose(fp2);

    return;
}

void diff(){
    FILE *fp1 = fopen(parcedArgsSpace[1], "r");
    FILE *fp2 = fopen(parcedArgsSpace[2], "r");

    if (fp1 == NULL || fp2 == NULL){
        printf("any one or both File/s can not found...");
        exit(0);
    }

    char ch1, ch2;
    int error_count = 0, column = 0, line = 1;
    int count1 = 0;
    int count2 = 0;
    while ((ch1 = fgetc(fp1)) != EOF)
        count1++;
    while ((ch2 = fgetc(fp2)) != EOF)
        count2++;
    if (count1 != count2){
        printf("%s and %s aren't have same size", parcedArgsSpace[1], parcedArgsSpace[2]);
        exit(0);
    }

    fclose(fp1);
    fclose(fp2);
    
    fp1 = fopen(parcedArgsSpace[1], "r");
    fp2 = fopen(parcedArgsSpace[2], "r");

    while ((ch1 = fgetc(fp1)) != EOF && (ch2 = fgetc(fp2)) != EOF){
        column++;
        if (ch1 == '\n' && ch2 == '\n'){
            line++;
            column = 0;
        }

        if (ch1 != ch2){
            error_count++;
            printf("Different at Line : %d and Character : %d.", line, column);
        }
    }

    if (error_count == 0)
        printf("Both file have no difference. Both are identical.");

    fclose(fp1);
    fclose(fp2);

    return;
}

void head(){
    //number of lines we want to print from the file.
    int cnt = atoi(parcedArgsSpace[2]);
    FILE *fp;
    char *line;

    fp = fopen(parcedArgsSpace[1], "r");
    while ((fgets(line, 1000, fp)) != NULL){
        if (cnt == 0)
            return;
        printf("%s", line);
        cnt--;
    }
}

void tail(){
    //this is not working couurently. So, please ignor this.
    FILE *fp;
    int cnt = 0, len = 0, start_storing = -1, i;
    char *ans[atoi(parcedArgsSpace[2])];

    fp = fopen(parcedArgsSpace[1], "r");
    if (fp == NULL)
        printf("Enter valid filename.");

    char *line;
    while ((fgets(line, 1000, fp)) != NULL)
    {
        len++;
    }

    fclose(fp);
    fp = fopen(parcedArgsSpace[1], "r");

    if (len <= atoi(parcedArgsSpace[2]))
    {
        while ((fgets(line, 1000, fp)) != NULL)
            printf("%s", line);
    }
    else
    {
        i = 0;
        start_storing = len - atoi(parcedArgsSpace[2]);
        while ((fgets(line, 1000, fp)) != NULL)
        {
            if (cnt >= start_storing)
            {
                ans[i] = line;
                i++;
            }
            cnt++;
        }
    }

    fclose(fp);
    return;
}

void echo(){
    int i;
    printf("Output of ECHO command : \n");
    for (i = 1; i < countParcedSpace; i++)
        printf("%s ", parcedArgsSpace[i]);
    return;
}

void history(){
    int i;
    printf("Printing your history from start. highest number will be your last command.\n");
    for (i = 0; i < history_count; i++)
        printf("%d. %s\n", i + 1, HISTORY[i]);
    return;
}

void cd(){
    char current[100];
    getcwd(current, 100);

    chdir(parcedArgsSpace[1]);
    return;
}

void pwd(){
    char current[100];
    getcwd(current, 100);

    printf("Your present working directory is : %s", current);
}

void cat_b(){
    FILE *file;
    char line[1000];

    // remember file should exist
    file = fopen(parcedArgsSpace[1], "r");

    printf("Output of CAT command with -b switch : \n");

    // reading file line by line entering to buffer line
    int cnt = 1;
    while (fscanf(file, "%[^\n]\n", line) != EOF){
        printf("%d ", cnt);
        printf("%s\n", line);
        cnt++;
    }
    // closing file pointer.
    fclose(file);
}

void wc_words(){
    FILE *fp;
    int words = 0;
    char tempch;
    int flag = 1;

    fp = fopen(parcedArgsSpace[2], "r");
    if (fp == NULL){
        printf("This file doesn't exist.");
        return;
    }
    else{
        while ((tempch = fgetc(fp)) != EOF){
            flag = 0;
            if (tempch == ' ' || tempch == '\t' || tempch == '\n' || tempch == '\0')
                words++;
        }
        if (flag)
            printf("oops! this file is empty.");
        else
            printf("wooh! thats a lot of work. This file have %d words.", words + 1);
        fclose(fp);
    }
}

void wc_lines(){
    FILE *fp;
    int lines = 0;
    char tempch;
    int flag = 1;

    fp = fopen(parcedArgsSpace[2], "r");
    if (fp == NULL){
        printf("This file doesn't exist.");
        return;
    }
    else{
        while ((tempch = fgetc(fp)) != EOF){
            flag = 0;
            if (tempch == '\n' || tempch == '\0')
                lines++;
        }
        if (flag)
            printf("oops! this file is empty.");
        else
            printf("wooh! thats a lot of work. This file have %d lines.", lines + 1);
    }
}

void wc_chars(){
    FILE *fp;
    int chars = 0;
    char tempch;
    int flag = 1;

    fp = fopen(parcedArgsSpace[2], "r");
    if (fp == NULL){
        printf("This file doesn't exist.");
        return;
    }
    else{
        while ((tempch = fgetc(fp)) != EOF){
            flag = 0;
            chars++;
        }
        if (chars == 0)
            printf("oops! this file is empty.");
        else
            printf("wooh! thats a lot of work. This file have %d characters.", chars);
    }
}

void wc_only_chars(){
    FILE *fp;
    int chars = 0;
    char tempch;
    int flag = 1;

    fp = fopen(parcedArgsSpace[2], "r");
    if (fp == NULL){
        printf("This file doesn't exist.");
        return;
    }
    else{
        while ((tempch = fgetc(fp)) != EOF){
            flag = 0;
            if (tempch != ' ' && tempch != '\t' && tempch != '\n' && tempch != '\0')
                chars++;
        }
        if (chars == 0)
            printf("oops! this file is empty.");
        else
            printf("wooh! thats a lot of work. This file have %d characters.", chars);
    }
}

void grep_l(){
    FILE *fp;
    fp = fopen(parcedArgsSpace[3], "r");
    int flag = 1;

    while (flag){
        if (feof(fp))
            flag = 0;
        char *line;
        fgets(line, 1000, fp);
        if (strstr(line, parcedArgsSpace[1]) != NULL)
            printf("%s", line);
    }
    fclose(fp);
    return;
}

void grep_n(){
    //this command is also buggy. So, ignore this also.
    FILE *fp;
    fp = fopen(parcedArgsSpace[3], "r");
    char *line;
    char *position; //position of occurence.
    int cnt = 0, index;

    while ((fgets(line, 1000, fp)) != NULL)
    {
        index = 0;
        while ((position = strstr(line, parcedArgsSpace[1])) != NULL)
        {
            index = (position - line) + 1;
            cnt++;
        }
    }

    printf("%s this word has been found %d times in this file.", parcedArgsSpace[1], cnt);
}

//----------------------------ALL MANUALS ARE HERE.---------------------------//
void man_cat(){
    printf("\nCommand name : \"cat\".\n");
    printf("Command work : It prints the file given in the argument.\n");
    printf("Way to use : \"cat hello.txt\"\n\n");
    printf("Switches you can use : \n");
    printf("\t--b : It will put the linenumbers in front of every line. And also it will show non-empty lines only.\n\n");
    printf("\tWay to use : \"cat hello.txt --b\"");
}

void man_clear(){
    printf("\nCommand name : \"clear\".\n");
    printf("Command work : It clears the entire screen.\n");
    printf("Way to use : \"clear\"");
}

void man(){
    int noOfMans = 2, i, indexOfMan = -1;
    char *listOfMans[noOfMans];
    char flag;

    //all the name of commands for which you have created the manual pages.
    listOfMans[0] = "cat";
    listOfMans[1] = "clear";

    //finidng the index of command for priniting manual.
    for (i = 0; i < noOfMans; i++){
        if (strcmp(listOfMans[i], parcedArgsSpace[1]) == 0){
            indexOfMan = i;
            break;
        }
    }

    printf("Welcome to manual of %s command. Here it is : \n", parcedArgsSpace[1]);

    switch (indexOfMan){
        case 0:
            man_cat();
            break;
        case 1:
            man_clear();
            break;
        default:
            printf("There is an error loading this manual.\nEither you haven't entered in right format or right command.\nEither way we are sorry for your inconvinience.");
            break;
    }
}

//execution of commands is here.
int executeCmd(){
    //number of total commands, temp i, index of command initially, switch vala commands ni index.
    int noOfCmds = 28, i, indexOfCmd = -1, si = 21;
    char *listOfCmds[noOfCmds];

    listOfCmds[0] = "exit";      //
    listOfCmds[1] = "help";      //
    listOfCmds[2] = "clear";     //
    listOfCmds[3] = "cat";       //
    listOfCmds[4] = "man";       //
    listOfCmds[5] = "kill";      //
    listOfCmds[6] = "rm";        //
    listOfCmds[7] = "calc";      //
    listOfCmds[8] = "cmp";       //
    listOfCmds[9] = "diff";      //
    listOfCmds[10] = "head";     //
    listOfCmds[11] = "tail";     //
    listOfCmds[12] = "echo";     //
    listOfCmds[13] = "ps";       //
    listOfCmds[14] = "ls";       //
    listOfCmds[15] = "date";     //
    listOfCmds[16] = "mv";       //
    listOfCmds[17] = "history";  //
    listOfCmds[18] = "cd";       //
    listOfCmds[19] = "pwd";      //
    listOfCmds[20] = "cut";      //
    listOfCmds[21] = "cat --b";  //
    listOfCmds[22] = "wc --w";   //
    listOfCmds[23] = "wc --l";   //
    listOfCmds[24] = "wc --c";   //
    listOfCmds[25] = "wc --o";   //
    listOfCmds[26] = "grep --l"; //
    listOfCmds[27] = "grep --n"; //

    if (indexOfSwitch == -1)
    {
        //for nonswitch commands only.
        for (i = 0; i < noOfCmds; i++)
        {
            if (strcmp(parcedArgsSpace[0], listOfCmds[i]) == 0)
            {
                indexOfCmd = i;
                break;
            }
        }
    }
    else
    {
        //for commands with switches only.
        char *cmd = parcedArgsSpace[0];
        char *sw = parcedArgsSpace[indexOfSwitch];
        char *space = " ";
        char *fcmd = (char *)malloc(2 + strlen(cmd) + strlen(sw));
        strcpy(fcmd, cmd);
        strcat(fcmd, space);
        strcat(fcmd, sw);

        for (i = si; i < noOfCmds; i++)
        {
            if (strcmp(fcmd, listOfCmds[i]) == 0)
            {
                indexOfCmd = i;
                // printf("command index : %d and command : %s",indexOfCmd, listOfCmds[indexOfCmd]);
                break;
            }
        }
    }

    switch (indexOfCmd)
    {
    case 0:
        printf("\nHave a nice day. Goodbye.\n");
        exit(0);
    case 1:
        help();
        return 1;
    case 2:
        clear();
        return 1;
    case 3:
        cat();
        return 1;
    case 4:
        man();
        return 1;
    case 6:
        rm();
        return 1;
    case 7:
        calc();
        return 1;
    case 8:
        cmp();
        return 1;
    case 9:
        diff();
        return 1;
    case 10:
        head();
        return 1;
    case 11:
        tail();
        return 1;
    case 12:
        echo();
        return 1;
    case 17:
        history();
        return 1;
    case 18:
        cd();
        return 1;
    case 19:
        pwd();
        return 1;
    case 21:
        cat_b();
        return 1;
    case 22:
        wc_words();
        return 1;
    case 23:
        wc_lines();
        return 1;
    case 24:
        wc_chars();
        return 1;
    case 25:
        wc_only_chars();
        return 1;
    case 26:
        grep_l();
        return 1;
    case 27:
        printf("entered switch.");
        grep_n();
        return 1;
    default:
        return 0;
    }
}

int main(){
    initialization();
    while (1){
        char inputStr[MAXCHAR];
        int i;

        if (takeInput(inputStr) == 0){
            processString(inputStr);
            //Just for testing perspose only.
            // printf("number of arguments is %d\n", countParcedSpace);
            // for (i=0; i<countParcedSpace; i++){
            //     printf("%d. %s\n", i, parcedArgsSpace[i]);
            // }
            if (executeCmd() == 0)
                printf("There is an error in executing command. Sorry.");
            clearParcedSpace();
        }
    }
    return 0;
}