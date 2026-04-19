#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <crypt.h>
#include "lib.h"

int main(){
    int choice;

    load();

    do{
        printf("\n****** LIBRARY MANAGEMENT SYSTEM ******\n");
        printf("-------- MENU --------\n");
        printf("1.About User\n");
        printf("2.Pay fine\n");
        printf("3.Search Book\n");
        printf("4.Request Book\n");
        printf("5.Add user\n");
        printf("6.Add book\n");
        printf("7.Delete Book\n");
        printf("8.Borrow Book\n");
        printf("9.Return Book\n");
        printf("10.Display Users\n");
        printf("11.Display Books\n");
        printf("12.Display Transactions\n");
        printf("13.View Requests\n");
        printf("14.Exit\n");
        printf("Enter your choice with corresponding number.\n");
        if (!read_int_input(NULL, &choice)) {
            if (feof(stdin)) {
                printf("Exiting...\n");
                break;
            }
            continue;
        }

        switch(choice) {
            case 1: about_user(); break;
            case 2: pay_fine();break;
            case 3: search_book(); break;
            case 4: request_book(); break;
            case 5: add_user(); break;
            case 6: add_book(); break;
            case 7: delete_book(); break;
            case 8: borrow_book(); break;
            case 9: return_book(); break;
            case 10: display_users(); break;
            case 11: display_books(); break;
            case 12: display_transactions(); break;
            case 13: view_requests(); break;
            case 14: 
                    save();
                printf("Exiting...\n");
                exit(0);

            default:
                printf("Invalid choice\n");
        }
    }
        while(choice!=14);
    return 0;
}
