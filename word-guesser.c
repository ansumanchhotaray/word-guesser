#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

const int length = 5;

int main(void)
{
    FILE *pWords1 = fopen("words.txt", "r");
    
    if (pWords1 == NULL)
    {
        printf("Could not open the words list.\n");
        return 1;
    }

    char word1[length + 1];
    int count = 0;

    while (fgets(word1, sizeof(word1), pWords1) != NULL)
    {
        count++;
    }

    if (count == 0)
    {
        printf("No words found in the words list.\n");
        fclose(pWords1);
        return 1;
    }

    fclose(pWords1);

    srand(time(NULL));
    int min = 0;
    int max = count / length + 1;
    int random = (rand() % (max - min + 1)) + min;

    FILE *pWords2 = fopen("words.txt", "r");

    int search = 0;
    char word2[length + 1];
    char answer[length + 1];

    while (fgets(word2, sizeof(word2), pWords2) != NULL) 
    {
        if (search == random) 
        {
            for (int i = 0; i < length + 1; i++)
            {
                answer[i] = word2[i];
            }
            break;
        }
        search++;
    }

    fclose(pWords2);

    for (int i = 0; answer[i] != '\0'; i++) 
    {
        answer[i] = toupper(answer[i]);
    }
    
    char correct = '2';
    char present = '1';
    char absent = '0';
    char guess[10] = "";

    printf("Guess the 5-letter word using the following hints:\n");
    printf("%c = Letter is in the word and in the correct spot.\n", correct);
    printf("%c = Letter is in the word but in the wrong spot.\n", present);
    printf("%c = Letter is not in the word in any spot.\n", absent);
    printf("Enter END to reveal the answer.\n\n");

    for (int attempts = 0; guess != answer; attempts++)
    {
        printf("%i: ", attempts + 1);
        scanf("%s", &guess);
        printf(" : ");

        for (int i = 0; guess[i] != '\0'; i++) 
        {
            guess[i] = toupper(guess[i]);
        }

        if (strcmp(guess, "END") == 0)
        {
            printf("%s is the answer.\n", answer);
            break;
        }

        bool letters = true;

        for (int i = 0; guess[i] != '\0'; i++) 
        {
            if (guess[i] < 'A' || guess[i] > 'Z')
            {
                letters = false;
                break;
            }
        }

        if (letters == false) 
        {
            printf("The word should contain only letters.\n");
            attempts--;
            continue;
        } 

        if (strlen(guess) != 5)
        {
            printf("The word should contain exactly 5 letters.\n");
            attempts--;
            continue;
        }

        FILE *pDict = fopen("dictionary.txt", "r");
        char bufferDict[1024] = {0};

        if (pDict == NULL)
        {
            printf("Could not open dictionary.\n");
            return 1;
        }

        bool exists = false;

        while (fgets(bufferDict, sizeof(bufferDict), pDict)) 
        {
            bufferDict[strcspn(bufferDict, "\n")] = '\0';

            if (strcmp(bufferDict, guess) == 0)
            {
                exists = true;
                break;
            }
        }

        fclose(pDict);

        if (exists == false) 
        {
            printf("The word is not in the dictionary.\n");
            attempts--;
            continue;
        } 

        int matches = 0;

        for (int i = 0; i < 5; i++)
        {
            if (guess[i] == answer[i])
            {
                printf("%c", correct);
                matches++;
            }
            else if (guess[i] != answer[i])
            {
                for (int j = 0; j < 5; j++)
                {
                    if (guess[i] == answer[j])
                    {
                        printf("%c", present);
                        break;
                    }
                    else if (j == 4 && guess[i] != answer[j])
                    {
                        printf("%c", absent);
                        break;
                    }
                }
            }
        }
        printf("\n");

        if (matches == 5)
        {
            printf("You win!\n");
            break;
        }
    }
}
