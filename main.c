#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME 50
#define MAX_ADDRESS 100

struct Account {
    int acc_no;
    char name[MAX_NAME];
    char address[MAX_ADDRESS];
    char phone[15];
    char acc_type[20];
    float balance;
    int dob_day, dob_month, dob_year;
    int deposit_day, deposit_month, deposit_year;
};

void create_account();
void deposit();
void withdraw();
void balance_inquiry();
void update_account();
void delete_account();
void view_all_accounts();
void add_transaction(int acc_no, char *type, float amount);
void view_transaction_history(int acc_no);
void display_menu();
void delay(int number_of_seconds);

int main() {
    int choice;
    while (1) {
        display_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                create_account();
                break;
            case 2:
                deposit();
                break;
            case 3:
                withdraw();
                break;
            case 4:
                balance_inquiry();
                break;
            case 5:
                update_account();
                break;
            case 6:
                delete_account();
                break;
            case 7:
                view_all_accounts();
                break;
            case 8:{
                int acc_no;
                printf("Enter account number: ");
                scanf("%d", &acc_no);
                view_transaction_history(acc_no);
                break;
            }
            case 9:
                printf("Thank you for using our banking system.\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
        delay(1);
    }
    return 0;
}

void display_menu() {
    printf("\n=== Banking System Menu ===\n");
    printf("1. Create Account\n");
    printf("2. Deposit\n");
    printf("3. Withdraw\n");
    printf("4. Balance Inquiry\n");
    printf("5. Update Account\n");
    printf("6. Delete Account\n");
    printf("7. View All Accounts\n");
    printf("8. View Transaction History\n");
    printf("9. Exit\n");
}

void create_account() {
    FILE *fp = fopen("accounts.txt", "a");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    struct Account acc;
    printf("Enter account number: ");
    scanf("%d", &acc.acc_no);
    printf("Enter name: ");
    scanf("%s", acc.name);
    printf("Enter address: ");
    scanf("%s", acc.address);
    printf("Enter phone: ");
    scanf("%s", acc.phone);
    printf("Enter account type (savings/current): ");
    scanf("%s", acc.acc_type);
    printf("Enter initial balance: ");
    scanf("%f", &acc.balance);
    printf("Enter date of birth (dd mm yyyy): ");
    scanf("%d %d %d", &acc.dob_day, &acc.dob_month, &acc.dob_year);
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    acc.deposit_day = tm.tm_mday;
    acc.deposit_month = tm.tm_mon + 1;
    acc.deposit_year = tm.tm_year + 1900;
    fprintf(fp, "%d %s %s %s %s %.2f %d %d %d %d %d %d\n", acc.acc_no, acc.name, acc.address, acc.phone, acc.acc_type, acc.balance, acc.dob_day, acc.dob_month, acc.dob_year, acc.deposit_day, acc.deposit_month, acc.deposit_year);
    fclose(fp);
    add_transaction(acc.acc_no, "CREATE", acc.balance);
    printf("Account created successfully.\n");
}

void deposit() {
    FILE *fp = fopen("accounts.txt", "r+");
    FILE *temp = fopen("temp.txt", "w");
    struct Account acc;
    int acc_no;
    float amount;
    printf("Enter account number: ");
    scanf("%d", &acc_no);
    printf("Enter amount to deposit: ");
    scanf("%f", &amount);
    int found = 0;
    while (fscanf(fp, "%d %s %s %s %s %f %d %d %d %d %d %d", &acc.acc_no, acc.name, acc.address, acc.phone, acc.acc_type, &acc.balance, &acc.dob_day, &acc.dob_month, &acc.dob_year, &acc.deposit_day, &acc.deposit_month, &acc.deposit_year) != EOF) {
        if (acc.acc_no == acc_no) {
            acc.balance += amount;
            found = 1;
            add_transaction(acc.acc_no, "DEPOSIT", amount);
            printf("Deposit successful. New balance: %.2f\n", acc.balance);
        }
        fprintf(temp, "%d %s %s %s %s %.2f %d %d %d %d %d %d\n", acc.acc_no, acc.name, acc.address, acc.phone, acc.acc_type, acc.balance, acc.dob_day, acc.dob_month, acc.dob_year, acc.deposit_day, acc.deposit_month, acc.deposit_year);
    }
    fclose(fp);
    fclose(temp);
    if (!found) {
        printf("Account not found.\n");
        remove("temp.txt");
    } else {
        remove("accounts.txt");
        rename("temp.txt", "accounts.txt");
    }
}

void withdraw() {
    FILE *fp = fopen("accounts.txt", "r+");
    FILE *temp = fopen("temp.txt", "w");
    struct Account acc;
    int acc_no;
    float amount;
    printf("Enter account number: ");
    scanf("%d", &acc_no);
    printf("Enter amount to withdraw: ");
    scanf("%f", &amount);
    int found = 0;
    while (fscanf(fp, "%d %s %s %s %s %f %d %d %d %d %d %d", &acc.acc_no, acc.name, acc.address, acc.phone, acc.acc_type, &acc.balance, &acc.dob_day, &acc.dob_month, &acc.dob_year, &acc.deposit_day, &acc.deposit_month, &acc.deposit_year) != EOF) {
        if (acc.acc_no == acc_no) {
            if (acc.balance >= amount) {
                acc.balance -= amount;
                found = 1;
                add_transaction(acc.acc_no, "WITHDRAW", amount);
                printf("Withdrawal successful. New balance: %.2f\n", acc.balance);
            } else {
                printf("Insufficient balance.\n");
            }
        }
        fprintf(temp, "%d %s %s %s %s %.2f %d %d %d %d %d %d\n", acc.acc_no, acc.name, acc.address, acc.phone, acc.acc_type, acc.balance, acc.dob_day, acc.dob_month, acc.dob_year, acc.deposit_day, acc.deposit_month, acc.deposit_year);
    }
    fclose(fp);
    fclose(temp);
    if (!found) {
        printf("Account not found.\n");
        remove("temp.txt");
    } else {
        remove("accounts.txt");
        rename("temp.txt", "accounts.txt");
    }
}

void balance_inquiry() {
    FILE *fp = fopen("accounts.txt", "r");
    struct Account acc;
    int acc_no;
    printf("Enter account number: ");
    scanf("%d", &acc_no);
    int found = 0;
    while (fscanf(fp, "%d %s %s %s %s %f %d %d %d %d %d %d", &acc.acc_no, acc.name, acc.address, acc.phone, acc.acc_type, &acc.balance, &acc.dob_day, &acc.dob_month, &acc.dob_year, &acc.deposit_day, &acc.deposit_month, &acc.deposit_year) != EOF) {
        if (acc.acc_no == acc_no) {
            printf("Account Holder: %s\nAddress: %s\nPhone: %s\nAccount Type: %s\nBalance: %.2f\nDOB: %d/%d/%d\n", acc.name, acc.address, acc.phone, acc.acc_type, acc.balance, acc.dob_day, acc.dob_month, acc.dob_year);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Account not found.\n");
    }
    fclose(fp);
}

void update_account() {
    FILE *fp = fopen("accounts.txt", "r+");
    FILE *temp = fopen("temp.txt", "w");
    struct Account acc;
    int acc_no;
    printf("Enter account number: ");
    scanf("%d", &acc_no);
    int found = 0;
    while (fscanf(fp, "%d %s %s %s %s %f %d %d %d %d %d %d", &acc.acc_no, acc.name, acc.address, acc.phone, acc.acc_type, &acc.balance, &acc.dob_day, &acc.dob_month, &acc.dob_year, &acc.deposit_day, &acc.deposit_month, &acc.deposit_year) != EOF) {
        if (acc.acc_no == acc_no) {
            found = 1;
            printf("Enter new address: ");
            scanf("%s", acc.address);
            printf("Enter new phone: ");
            scanf("%s", acc.phone);
            printf("Account updated successfully.\n");
        }
        fprintf(temp, "%d %s %s %s %s %.2f %d %d %d %d %d %d\n", acc.acc_no, acc.name, acc.address, acc.phone, acc.acc_type, acc.balance, acc.dob_day, acc.dob_month, acc.dob_year, acc.deposit_day, acc.deposit_month, acc.deposit_year);
    }
    fclose(fp);
    fclose(temp);
    if (!found) {
        printf("Account not found.\n");
        remove("temp.txt");
    } else {
        remove("accounts.txt");
        rename("temp.txt", "accounts.txt");
    }
}

void delete_account() {
    FILE *fp = fopen("accounts.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    struct Account acc;
    int acc_no;
    printf("Enter account number: ");
    scanf("%d", &acc_no);
    int found = 0;
    while (fscanf(fp, "%d %s %s %s %s %f %d %d %d %d %d %d", &acc.acc_no, acc.name, acc.address, acc.phone, acc.acc_type, &acc.balance, &acc.dob_day, &acc.dob_month, &acc.dob_year, &acc.deposit_day, &acc.deposit_month, &acc.deposit_year) != EOF) {
        if (acc.acc_no != acc_no) {
            fprintf(temp, "%d %s %s %s %s %.2f %d %d %d %d %d %d\n", acc.acc_no, acc.name, acc.address, acc.phone, acc.acc_type, acc.balance, acc.dob_day, acc.dob_month, acc.dob_year, acc.deposit_day, acc.deposit_month, acc.deposit_year);
        } else {
            found = 1;
        }
    }
    fclose(fp);
    fclose(temp);
    if (!found) {
        printf("Account not found.\n");
        remove("temp.txt");
    } else {
        remove("accounts.txt");
        rename("temp.txt", "accounts.txt");
        printf("Account deleted successfully.\n");
    }
}

void view_all_accounts() {
    FILE *fp = fopen("accounts.txt", "r");
    struct Account acc;
    printf("All Accounts:\n");
    while (fscanf(fp, "%d %s %s %s %s %f %d %d %d %d %d %d", &acc.acc_no, acc.name, acc.address, acc.phone, acc.acc_type, &acc.balance, &acc.dob_day, &acc.dob_month, &acc.dob_year, &acc.deposit_day, &acc.deposit_month, &acc.deposit_year) != EOF) {
        printf("Acc No: %d, Name: %s, Balance: %.2f\n", acc.acc_no, acc.name, acc.balance);
    }
    fclose(fp);
}

void add_transaction(int acc_no, char *type, float amount) {
    FILE *fp = fopen("transactions.txt", "a");
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(fp, "%d %s %.2f %d/%d/%d %d:%d:%d\n", acc_no, type, amount, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fclose(fp);
}

void view_transaction_history(int acc_no) {
    FILE *fp = fopen("transactions.txt", "r");
    int acc;
    char type[20];
    float amount;
    int day, month, year, hour, min, sec;
    printf("Transaction History for Account %d:\n", acc_no);
    while (fscanf(fp, "%d %s %f %d/%d/%d %d:%d:%d", &acc, type, &amount, &day, &month, &year, &hour, &min, &sec) != EOF) {
        if (acc == acc_no) {
            printf("%s %.2f on %d/%d/%d at %d:%d:%d\n", type, amount, day, month, year, hour, min, sec);
        }
    }
    fclose(fp);
}

void delay(int number_of_seconds) {
    int milli_seconds = 1000 * number_of_seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);
}
