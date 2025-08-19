#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

#define MAX_ITEMS 50
#define MAX_INV 100

typedef struct
{
    int item_num;
    char name[100];
    int price;
} Menu;

Menu menu[MAX_ITEMS];
int menu_size = 0;

void read_menu() // Jeet
{
    FILE *menu_file = fopen("menu.txt", "r");

    if (menu_file==NULL)
    {
        printf("Error: Could not open menu.txt\n");
        return;
    }

    while(fscanf(menu_file,"%d %s %d",&menu[menu_size].item_num,menu[menu_size].name,&menu[menu_size].price) == 3)
    {
        menu_size++;
        if(menu_size >= MAX_ITEMS)
            break;
    }
    fclose(menu_file);
}

void show_menu() // Jeet
{
    printf("\n------ Coffee Shop Menu ------\n");

    for (int i = 0; i < menu_size; i++)
    {
        printf("%d. %s -> %d BDT\n", menu[i].item_num, menu[i].name, menu[i].price);
    }
    printf("------------------------------\n");
}

typedef struct {
    int id;
    char name[100];
    int qty;
} InvItem;

InvItem inv[MAX_INV];
int inv_size = 0;

void show_inventory() {
    FILE *fp = fopen("inventory.txt", "r");//file name
    if (fp==NULL) {
        printf("Error: Could not open inventory.txt\n");
        inv_size = 0;
        return;
    }

    inv_size = 0;
    while (inv_size < MAX_INV &&
           fscanf(fp, "%d %99s %d", &inv[inv_size].id, inv[inv_size].name, &inv[inv_size].qty) == 3) {
        inv_size++;
    }
    fclose(fp);
    printf("\n----------- Inventory ------------\n");
    printf("%-5s %-20s %-10s\n", "ID", "Item", "Quantity");
    printf("----------------------------------------\n");

    for (int i = 0; i < inv_size; i++) {
        printf("%-5d %-20s %-10d\n", inv[i].id, inv[i].name, inv[i].qty);
    }

    if (inv_size == 0) {
        printf("(No items found)\n");
    }
}

void total_income() //Mahin
{
    FILE *fp;
    char item[100];
    int qty, price, total = 0;

    fp = fopen("orders.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return ;
    }
    while (fscanf(fp, "%*d. %d x %s -> %d BDT", &qty, item, &price) == 3)
    {
        total=total+price;
    }

    fclose(fp);

    printf("Total Income: %d BDT\n", total);
}

void customer_history() // Mahin
{
    FILE *fp = fopen("customer.txt", "r");
    if (fp == NULL)
    {
        printf("Error: Could not open file\n");
        return;
    }

    char name[100], phone[20];
    printf("\n----------- Customer History ------------\n");
    printf("%-20s %-15s\n","Customer Name","Phone Number");
    printf("----------------------------------------\n\n");
    while (fscanf(fp, "Name: %99[^,], Phone: %19s\n", name, phone) == 2)
    {
        printf("%-20s %-15s\n", name, phone);
    }

    fclose(fp);
}

void mainMenu() // Arib
{
    printf("\nWelcome to Coffee Shop\n");
    printf("\n1. Customer\n");
    printf("2. Admin\n");
    printf("\nEnter choice: ");
}

int take_order()
{
    int order_choice,quantity,total = 0;
    int more;
    static int order_num = 1;
    int order_quantity[MAX_ITEMS] = {0};
    int total_items = 0;

    do
    {
        show_menu();
        printf("\nEnter item number: ");
        scanf("%d", &order_choice);

        int found = -1;

        for (int i = 0; i < menu_size; i++)
        {
            if (menu[i].item_num == order_choice)
            {
                found = i;
                break;
            }
        }

        if (found == -1)
        {
            system("cls");
            printf("Invalid number. Try again.\n");
            take_order();
        }

        printf("Enter quantity: ");
        scanf("%d", &quantity);

        order_quantity[found] += quantity;
        total += menu[found].price * quantity;
        total_items += quantity;

        system("cls");
        printf("%d x %s added to order.\nSubtotal: %d BDT\n", quantity, menu[found].name, total);

more_or_not:
        printf("\nDo you want to order more?\n");
        printf("\n1. Yes");
        printf("\n2. No\n");
        printf("\nEnter the corresponding number: ");
        scanf("%d", &more);
        system("cls");
    }
    while (more == 1);

    if (more != 1 && more != 2)
    {
        printf("Invalid number. Try again!\n");
        goto more_or_not;
    }

    FILE *order_file = fopen("orders.txt", "r");
    if (order_file != NULL)
    {
        char line[256];
        int last_num = 0;

        while (fgets(line, sizeof(line), order_file))
        {
            int num;
            if (sscanf(line, "%d.", &num) == 1)
            {
                last_num = num;
            }
        }

        fclose(order_file);

        order_num = last_num + 1;
    }

    system("cls");
    printf("Here is your receipt.\n");
    printf("\n========== Receipt ==========\n");

    for (int i = 0; i < menu_size; i++)
    {
        if (order_quantity[i] > 0)
        {
            int item_total = order_quantity[i] * menu[i].price;

            printf("%d x %s = %d BDT\n", order_quantity[i], menu[i].name, item_total);

            FILE *order_file = fopen("orders.txt", "a");

            if (order_file == NULL)
            {
                printf("Error: Could not save order history.\n");
                return total;
            }

            fprintf(order_file, "%d. %d x %s -> %d BDT\n", order_num, order_quantity[i], menu[i].name, item_total);
            order_num++;

            fclose(order_file);
        }
    }

    printf("------------------------------\n");
    printf("Total amount: %d BDT\n", total);
    printf("=============================\n\n");

    if (total_items >= 3)
    {
        printf("Congratulations! You get a FREE Brownie! \n\n");
    }
    return total;
}

void member() // Jeet
{
    char mem_num[100];
    char mem_name[100];

    system("cls");
    printf("We need your name and phone number.\n");
    printf("\nYour name: ");
    getchar();
    fgets(mem_name, sizeof(mem_name), stdin);
    mem_name[strcspn(mem_name, "\n")] = '\0';
    printf("Your phone number: ");
    scanf("%s",mem_num);

    system("cls");
    printf("Congratulations, %s! You are now a member.\n",mem_name);

    FILE *member_file = fopen("customer.txt", "a");

    if (member_file == NULL)
    {
        printf("Error: Could not save member data.\n");
        return;
    }

    fprintf(member_file, "Name: %s , Phone: %s\n", mem_name, mem_num);

    fclose(member_file);
}

void customerMenu() // Arib + Mahin
{
    int choice;
    do
    {
        printf("1. Place Order\n");
        printf("0. Exit\n");
        printf("\nEnter choice: ");
        scanf("%d", &choice);
        printf("\n");

        switch(choice)
        {
        case 1:
            read_menu();
            take_order();

            {
                int mem_choice;
member_qna:
                printf("We also have a membership program. Do you want to become a member?\n\n");
                printf("1. Yes\n");
                printf("2. No\n");
                printf("\nEnter the corresponding number: ");
                scanf("%d",&mem_choice);

                if(mem_choice==1)
                {
                    member();
                    exit(0);
                }
                else if(mem_choice==2)
                {
                    system("cls");
                    printf("Thank you for your purchase. Make sure to come again!\n\n");
                    exit(0);
                }
                else
                {
                    system("cls");
                    printf("Invalid number. Try again.\n\n");
                    goto member_qna;
                }
            }
            break;

        case 0:
            return;
            break;

        default:
            printf("\nInvalid choice. Try again\n");
            break;
        }
    }
    while(choice != 0);
}

void adminMenu() // Arib
{
    int choice;
    do
    {
        printf("\n\n1. Check Inventory\n");

        printf("2. View Total Income\n");
        printf("3. View Customer History\n");
        printf("0. Exit\n");
        printf("\nEnter choice: ");
        scanf("%d", &choice);
        printf("\n");

        switch(choice)
        {
        case 1:
            system("cls");
            show_inventory();
            break;
        case 2:
            system("cls");
            total_income();
            break;
        case 3:
            system("cls");
            customer_history();
            break;
        case 0:
            exit(0);
            break;
        default:
            printf("Invalid choice. Try again");
            break;
        }
    }
    while(choice != 0);
}

int login() // Mahin
{
    char username[12], password[12];
    int attempts=3,i=0;
    for(attempts=3; attempts>0; attempts--)
    {
        i=0; // reset for each attempt
        printf("\nEnter Username: ");
        scanf("%s", username);

        printf("Enter Password: ");

        char ch;
        while (1)
        {
            ch = getch();
            if (ch == 13)
            {
                password[i] = '\0';
                printf("\n");
                break;
            }
            else if (ch == 8)
            {
                if (i > 0)
                {
                    i--;
                    printf("\b \b");
                }
            }
            else
            {
                password[i++] = ch;
                printf("*");
            }
        }

        if (strcmp(username, "admin") == 0 && strcmp(password, "1234") == 0)
        {
            system("cls");
            printf("Login successful. Welcome, %s!\n", username);
            return 1;
        }
        else
        {
            printf("\nIncorrect username or password. Attempts left: %d\n",attempts-1);
        }
    }

    printf("Too many failed attempts. Access denied.\n");
    return 0;
}

int main()
{
    int step1;

    mainMenu();
    scanf("%d", &step1);

    if(step1 == 1)
    {
        system("cls");
        customerMenu();
    }
    else if(step1 == 2)
        if(login() == 1)
        {
            adminMenu();
        }

    return 0;
}
