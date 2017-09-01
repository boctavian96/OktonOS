/*

***Okton OS Konsole***
Version : 0.1
Author : Octavian

*/

void waitForInput()
{
    getInput();
}

void getInput()
{
    char* input = "hello";
    if (getCommand(input) == 1)
    {
        printf("FAILURE");
    }
    else 
    {
        printf("SUCCES");
    }
}

int getCommand(char* input)
{
    if(strcmp(input, "hello"))
    {
        printf("Ich liebe Ade");        
    }
    else
    {
        return 1;
    }
    return 0;
}

