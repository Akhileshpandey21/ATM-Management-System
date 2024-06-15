#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// Global variables
float balance = 1000.0;
int accountNumber = 12345;
int pin = 1234;
char accountHolderName[50] = "John Doe";
float transactions[100];
int recipientAccounts[100];
int numTransactions = 0;
float amount;
char overdraftProtection = 'N'; // Added overdraft protection, 'Y' to enable, 'N' to disable

// Password for menu access
int password = 12345; // Change this to your desired password

// Function to initialize recipientAccounts array with zeros
void initializeRecipientAccounts() {
    for (int i = 0; i < 100; i++) {
        recipientAccounts[i] = 0;
    }
}

void initializeTransaction() {
    for (int i = 0; i < 100; i++) {
        transactions[i] = 0;
    }
}

// Function to display the menu after password check
void displayMenuWithPassword() {
    printf("\n**** Welcome to My ATM ****\n");
    printf("1. Check Balance\n");
    printf("2. Withdraw Money\n");
    printf("3. Deposit Money\n");
    printf("4. Change PIN\n");
    printf("5. View Account Information\n");
    printf("6. Modify Account Holder Name\n");
    printf("7. Enable/Disable Overdraft Protection\n");
    printf("8. Transfer Funds\n");
    printf("9. Mini Statement\n");
    printf("10. Logout\n");
    printf("11. Exit\n");
    printf("************\n");
}

// Function to validate PIN
int validatePIN(int enteredPIN) {
    return enteredPIN == pin;
}

// Function to display account information
void displayAccountInfo() {
    printf("Account Number: %d\n", accountNumber);
    printf("Account Holder Name: %s\n", accountHolderName);
    printf("Current Balance: $%.2f\n", balance);
    printf("Overdraft Protection: %c\n", overdraftProtection);
}
// Function to record a recipient account for a transfer and the transaction amount
void recordTransaction(float amount, int recipientAccount) {
    transactions[numTransactions] = amount;
    recipientAccounts[numTransactions] = recipientAccount;
    numTransactions++;
}

// Function to perform fund transfer
void transferFunds() {
    int targetAccount;

    printf("Enter the target account number: ");
    scanf("%d", &targetAccount);

    if (targetAccount == accountNumber) {
        printf("Cannot transfer funds to the same account.\n");
        return;
    }

    printf("Enter the amount to transfer: $");
    scanf("%f", &amount);

    if (amount > 0 && amount <= balance) {
        balance -= amount;
        printf("Transferred $%.2f to account %d. Your new balance is $%.2f\n", amount, targetAccount, balance);
        recordTransaction(-amount, targetAccount);
    } else {
        if (overdraftProtection == 'N') {
            printf("Invalid amount or insufficient balance.\n");
        } else {
            // Allow overdraft
            balance -= amount;
            printf("Transferred $%.2f to account %d. Your new balance is $%.2f\n", amount, targetAccount, balance);
            recordTransaction(-amount, targetAccount);
        }
    }
}


// Function to modify account holder's name
void modifyAccountHolderName() {
    printf("Enter the new account holder's name: ");
    scanf(" %49[^\n]", accountHolderName); // Read up to 49 characters (49 + 1 for null terminator)
}

// Function to enable or disable overdraft protection
void enableDisableOverdraftProtection() {
    printf("Enable or disable overdraft protection? (Y/N): ");
    scanf("%c", &overdraftProtection);


    overdraftProtection = toupper(overdraftProtection); // Convert to uppercase
}

// Function to display a mini statement
void miniStatement() {
    printf("Transaction History:\n");
    for (int i = 0; i < numTransactions; i++) {
        printf("Transaction %d: $%.2f\n", i + 1, transactions[i]);
        if (recipientAccounts[i] != 0) {
            printf(" (Transfer to Account %d)", recipientAccounts[i]);
        }
        printf("\n");
    }
}

int main() {
    int choice;
    int enteredPIN;
    float withdrawAmount;
    float depositAmount;
    int passwordAttempts = 3; // Number of password attempts allowed
    int loggedIn = 0; //Pin
    // Ask for the password
    printf("Enter the password: ");
    scanf("%d", &enteredPIN);

    while (enteredPIN != password && passwordAttempts > 0) {
        printf("Incorrect password. %d attempts remaining.\n", passwordAttempts);
        printf("Enter the password: ");
        scanf("%d", &enteredPIN);
        passwordAttempts--;
    }

    if (enteredPIN != password) {
        printf("Incorrect password. Exiting...\n");
        return 0; // Exit the program
    }

    initializeRecipientAccounts();
    initializeTransaction();

    do {
        if (!loggedIn) {
            // Ask for PIN only if not logged in
            printf("Enter your PIN: ");
            scanf("%d", &enteredPIN);
            if (validatePIN(enteredPIN)) {
                loggedIn = 1; // Set logged in
            } else
            {   printf("Invalid PIN. Please try again.\n");
                continue;
            }
        }





        displayMenuWithPassword(); // Use the password-protected menu

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            if (loggedIn) {
                printf("Your balance is $%.2f\n", balance);
            } else {
                printf("Please log in to check your balance.\n");


            }
            // printf("Your balance is $%.2f\n", balance);
            break;
        case 2:
            if(loggedIn) {
                printf("Enter the amount to withdraw: $");
                scanf("%f", &withdrawAmount);
                if (withdrawAmount > 0 && withdrawAmount <= balance) {
                    balance -= withdrawAmount;
                    printf("Withdrawn $%.2f. Your new balance is $%.2f\n", withdrawAmount, balance);
                    recordTransaction(-withdrawAmount, 0);
                } else {
                    if (overdraftProtection == 'N') {
                        printf("Invalid amount or insufficient balance.\n");
                    } else {
                        // Allow overdraft
                        balance -= withdrawAmount;
                        printf("Withdrawn $%.2f (including overdraft). Your new balance is $%.2f\n", withdrawAmount, balance);
                        recordTransaction(-withdrawAmount, 0);
                    }
                }
            } else {
                printf("Please log in to withdraw amount.\n");



            }
            break;
        case 3:
            printf("Enter the amount to deposit: $");
            scanf("%f", &depositAmount);
            if (depositAmount > 0) {
                balance += depositAmount;
                printf("Deposited $%.2f. Your new balance is $%.2f\n", depositAmount, balance);
                recordTransaction(depositAmount, 0);
            } else {
                printf("Invalid amount.\n");
            }
            break;
        case 4:
            if(loggedIn) {
                printf("Enter your current PIN: ");
                scanf("%d", &enteredPIN);
                if (validatePIN(enteredPIN)) {
                    printf("Enter a new 4-digit PIN: ");
                    scanf("%d", &pin);
                    printf("PIN successfully changed.\n");
                } else {
                    printf("Invalid PIN. PIN change failed.\n");
                }
            }
            else {
                printf("Please log in to change pin.\n");


            }
            break;
        case 5:
            if(loggedIn) {
                displayAccountInfo();
            }
            else {
                printf("Please log in to check your balance.\n");



            }
            break;
        case 6:
            if(loggedIn) {
                modifyAccountHolderName();
                printf("Account holder's name updated.\n");
            } else {
                printf("Please log in to modify your data.\n");


            }
            break;
        case 7:
            if(loggedIn) {
                enableDisableOverdraftProtection();
                printf("Overdraft protection updated.\n");

            } else {
                printf("Please log in to check your balance.\n");



            }
            break;
        case 8:
            if(loggedIn) {
                transferFunds();
            }
            else {
                printf("Please log in to check your balance.\n");


            }
            break;
        case 9:
            if(loggedIn) {
                miniStatement();
            } else {
                printf("Please log in to check your balance.\n");



            }
            break;
        case 10:
            loggedIn = 0; // Log out
            printf("Logged out successfully.\n");
            break;
        case 11:
            printf("Thank you for using My ATM. Goodbye!\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
    while (choice != 11);

    return 0;
}