#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

struct user
{
    char phone[50];
    char ac[50];
    char password[50];
    char name[50];
    float balance;
};

void clear_screen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void generate_account_number(char *ac)
{
    srand((unsigned int)time(NULL));
    int random_number = rand() % 1000000;
    sprintf(ac, "AC%06d", random_number);
}

void login(struct user *usr)
{
    char phone[50], pword[50], filename[60];
    FILE *fp;

    printf("\nPhone number:\t");
    scanf("%s", phone);

    sprintf(filename, "%s.dat", phone);
    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("\nThere is no account registered with this phone number.\n");
        return;
    }

    printf("Password:\t");
    scanf("%s", pword);

    if (fread(usr, sizeof(struct user), 1, fp) != 1)
    {
        printf("\nError reading account information. Please try again.\n");
        fclose(fp);
        return;
    }

    fclose(fp);

    if (strcmp(pword, usr->password) == 0)
    {
        printf("\n\t\tWelcome, %s!\n", usr->name);
    }
    else
    {
        printf("\nInvalid password. Please try again.\n");
        usr->phone[0] = '\0'; // Mark login failure
    }
}

void help_section()
{
    clear_screen();
    printf("                *** HELP SECTION ***\n");
    printf("\n  Welcome to PPS Banking Zone! Here's how you can use the system:\n");
    printf("  1. Balance Inquiry: View your current account balance.\n");
    printf("  2. Deposit Cash: Add funds to your account.\n");
    printf("  3. Cash Withdrawal: Withdraw funds from your account.\n");
    printf("  4. Online Transfer: Send money to another account using their phone number.\n");
    printf("  5. Change Password: Update your account password for better security.\n");
    printf("  6. Help Section: View information about available features.\n");
    printf("\n  For any issues or assistance, feel free to contact customer support.\n");
    printf("\nPress Enter to return to the main menu...");
    getchar(); // consume newline
    getchar(); // wait for user to press Enter
}

int main()
{
    struct user usr, usr1;
    FILE *fp;
    char filename[60], phone[50], pword[50];
    int opt, choice;
    char cont = 'y';
    float amount;

    printf("\n\n               PPS BANKING ZONE\n");
    printf("          ___Welcome To Our Bank___\n");

    while (1)
    {
        printf("\nWhat do you want to do?");
        printf("\n1. Register an account");
        printf("\n2. Login to an account");
        printf("\n3. Exit");
        printf("\n\nYour choice:\t");
        scanf("%d", &opt);

        if (opt == 1)
        {
            clear_screen();
            printf("Enter your phone number:\t");
            scanf("%s", usr.phone);
            printf("Enter your name:\t");
            scanf(" %[^\n]%*c", usr.name);
            printf("Enter your new password:\t");
            scanf("%s", usr.password);
            usr.balance = 0.0;
            generate_account_number(usr.ac);

            sprintf(filename, "%s.dat", usr.phone);
            fp = fopen(filename, "w");
            if (fp && fwrite(&usr, sizeof(struct user), 1, fp) == 1)
            {
                printf("\nAccount successfully registered.");
                printf("\nYour account number is: %s\n", usr.ac);
                fclose(fp);
            }
            else
            {
                printf("\nError: Could not register the account. Please try again.\n");
                if (fp) fclose(fp);
            }

        }
        else if (opt == 2)
        {
            clear_screen();
            login(&usr);
            if (usr.phone[0] != '\0')   // Login success
            {
                while (cont == 'y')
                {
                    clear_screen();
                    printf("\n1. Balance Inquiry");
                    printf("\n2. Deposit Cash");
                    printf("\n3. Cash Withdrawal");
                    printf("\n4. Online Transfer");
                    printf("\n5. Change Password");
                    printf("\n6. Help Section");
                    printf("\n7. Exit");
                    printf("\n\nYour choice:\t");
                    scanf("%d", &choice);

                    switch (choice)
                    {
                    case 1:
                        printf("\nYour current balance is tk. %.2f\n", usr.balance);
                        break;
                    case 2:
                        printf("\nEnter the amount to deposit:\t");
                        scanf("%f", &amount);
                        usr.balance += amount;
                        sprintf(filename, "%s.dat", usr.phone);
                        fp = fopen(filename, "w");
                        fwrite(&usr, sizeof(struct user), 1, fp);
                        fclose(fp);
                        printf("\nSuccessfully deposited.\n");
                        break;
                    case 3:
                        printf("\nEnter the amount to withdraw:\t");
                        scanf("%f", &amount);
                        if (amount > usr.balance)
                        {
                            printf("\nInsufficient balance.\n");
                        }
                        else
                        {
                            usr.balance -= amount;
                            sprintf(filename, "%s.dat", usr.phone);
                            fp = fopen(filename, "w");
                            fwrite(&usr, sizeof(struct user), 1, fp);
                            fclose(fp);
                            printf("\nSuccessfully withdrawn tk. %.2f\n", amount);
                        }
                        break;
                    case 4:
                        printf("\nEnter the phone number to transfer to:\t");
                        scanf("%s", phone);
                        printf("\nEnter the amount to transfer:\t");
                        scanf("%f", &amount);
                        sprintf(filename, "%s.dat", phone);
                        fp = fopen(filename, "r");
                        if (fp == NULL)
                        {
                            printf("\nThe account does not exist.\n");
                        }
                        else
                        {
                            fread(&usr1, sizeof(struct user), 1, fp);
                            fclose(fp);
                            if (amount > usr.balance)
                            {
                                printf("\nInsufficient balance.\n");
                            }
                            else
                            {
                                usr1.balance += amount;
                                sprintf(filename, "%s.dat", phone);
                                fp = fopen(filename, "w");
                                fwrite(&usr1, sizeof(struct user), 1, fp);
                                fclose(fp);
                                usr.balance -= amount;
                                sprintf(filename, "%s.dat", usr.phone);
                                fp = fopen(filename, "w");
                                fwrite(&usr, sizeof(struct user), 1, fp);
                                fclose(fp);
                                printf("\nSuccessfully transferred tk. %.2f to %s\n", amount, phone);
                            }
                        }
                        break;
                    case 5:
                        printf("\nEnter your new password:\t");
                        scanf("%s", pword);
                        strcpy(usr.password, pword);
                        sprintf(filename, "%s.dat", usr.phone);
                        fp = fopen(filename, "w");
                        fwrite(&usr, sizeof(struct user), 1, fp);
                        fclose(fp);
                        printf("\nPassword successfully changed.\n");
                        break;
                    case 6:
                        help_section();
                        break;
                    case 7:
                        printf("\nThank you for using PPS Banking Zone. Goodbye!\n");
                        cont = 'n';
                        break;
                    default:
                        printf("\nInvalid option. Please try again.\n");
                    }

                    if (cont == 'y')
                    {
                        printf("\nDo you want to continue? [y/n]: ");
                        scanf(" %c", &cont);
                    }
                }
            }
        }
        else if (opt == 3)
        {
            printf("\nThank you for using PPS Banking Zone. Goodbye!\n");
            break;
        }
        else
        {
            printf("\nInvalid choice. Please try again.\n");
        }
    }
    return 0;
}
