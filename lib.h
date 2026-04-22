#include <stddef.h>
#include <time.h>

typedef struct Book{
    int book_id;
    char title[100];
    int total_copies;
    int available_copies;
    struct Book *next;
}Book;

typedef struct User{
    int user_id;
    char name[100];
    int fine;
    struct User*next;
}User;

typedef struct Transaction{
    int trans_id;
    int user_id;
    int book_id;
    time_t issue_time;
    time_t return_time;
    int returned;
    int fine_added;
    struct Transaction*next;
}Transaction;

typedef struct Request{
    int user_id;
    char title[100];
    struct Request *next;
}Request;

int about_user();
int search_book();
int pay_fine();
int request_book();
int add_user();
int add_book();
int delete_book();
int borrow_book();
int return_book();
int display_users();
int display_books();
int display_transactions();
int view_requests();
int undo_last_operation();
int save();
int load();
void clear_all_data();
int read_line_input(const char *prompt, char *buffer, size_t size);
int read_int_input(const char *prompt, int *value);
