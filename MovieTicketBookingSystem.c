#include <stdio.h>
#include <string.h>

int main() {
    float Standard = 150.0;
    float Premium = 250.0;
    float Recliner = 400.0;

    char name[50];
    int ticket_count;
    int category;
    float ticket_price; 
    float total_cost; 
    float discount = 0.0; 
    float final_cost;
    char category_type[20];

    printf("Enter name: ");
    scanf("%49s", name);

    printf("Enter number of tickets: ");
    scanf("%d", &ticket_count);

    if (ticket_count < 1 || ticket_count > 20) {
        printf("Error: Ticket count must be between 1 and 20.\n");
        return 1;
    }

    printf("Ticket Menu : \n");
    printf("1 Standard - 150 \n");
    printf("2 Premium - 250 \n");
    printf("3 Recliner - 400 \n");
    printf("Enter category : ");
    scanf("%d", &category);
    switch (category) {
        case 1:
            ticket_price = Standard;
            strcpy(category_type, "Standard");
            break;
        case 2:
            ticket_price = Premium;
            strcpy(category_type, "Premium");
            break;
        case 3:
            ticket_price = Recliner;
            strcpy(category_type, "Recliner");
            break;
        default:
            printf("Error: Invalid ticket category.\n");
            return 1;
    }

    total_cost = ticket_count * ticket_price;

    if (ticket_count >= 5) discount = total_cost * 0.10;

    final_cost = total_cost - discount;

    printf("\nCustomer: %s\n", name);
    printf("Ticket Category: %s\n", category_type);
    printf("Tickets: %d\n", ticket_count);
    printf("Subtotal: %.2f\n", total_cost);
    printf("Discount: %.2f\n", discount);
    printf("Final Bill: %.2f\n", final_cost);

    return 0;
}