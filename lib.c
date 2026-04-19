#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <crypt.h>
#include <ctype.h>
#include <time.h>
#include "lib.h"

Book *book_head=NULL;
User *user_head=NULL;
Transaction *trans_head=NULL;
Request *req_head=NULL;
int next_bookid=1;
int next_userid=1;
int next_transaction_id=1;

int read_line_input(const char *prompt,char *buffer,size_t size);

int case_not_care(char *left,char *right){
    while(*left && *right){
        if(tolower((char)*left)!=tolower((char)*right)){
            return 0;
        }
        left++;
        right++;
    }
    if(*left=='\0' && *right=='\0'){
        return 1;
    }
    return 0;
}

int check_admin_password(){
    FILE *fp;
    char password[100];
    char admin_hash[100];

    if(!read_line_input("Enter admin password: ",password,sizeof(password))){
        return 0;
    }

    fp=fopen("Admin.txt","r");
    if(!fp){
        printf("Couldn't open admin password file.\n");
        return 0;
    }

    if(fgets(admin_hash,sizeof(admin_hash),fp)==NULL){
        fclose(fp);
        printf("Couldn't read admin password.\n");
        return 0;
    }
    fclose(fp);

    admin_hash[strcspn(admin_hash,"\n")]='\0';

    char*password_hash=crypt(password,admin_hash);
    if(password_hash && strcmp(admin_hash,password_hash)==0){
        return 1;
    }

    printf("Wrong password.\n");
    return 0;
}

void clear_all_data(){
    Book *btemp;
    User *utemp;
    Transaction *ttemp;
    Request *rtemp;

    while(book_head){
        btemp=book_head;
        book_head=book_head->next;
        free(btemp);
    }

    while(user_head){
        utemp=user_head;
        user_head=user_head->next;
        free(utemp);
    }

    while(trans_head){
        ttemp=trans_head;
        trans_head=trans_head->next;
        free(ttemp);
    }

    while(req_head){
        rtemp=req_head;
        req_head=req_head->next;
        free(rtemp);
    }

    next_bookid=1;
    next_userid=1;
    next_transaction_id=1;
}

int read_line_input(const char *prompt,char *buffer,size_t size){
    size_t len; // length of string read into buffer
    char ch;     // for clearing extra input from stdin
    int start=0;
    int end;

    if(prompt){
        printf("%s", prompt);
    }
 
    if(fgets(buffer, size, stdin)==NULL){
        if(feof(stdin)){                    //true when input reaches EOF
            printf("\nInput closed. Returning to menu.\n");
        } else {                            //some I/O problem
            printf("Failed to read input.\n");
        }
        return 0;
    }

    len=strlen(buffer);

    if(len>0 && buffer[len-1]!='\n'){       //input longer than buffer,ignore input.
        while((ch=getchar())!='\n' && ch!=EOF){
        }
        printf("Input is too long.\n");
        buffer[0]='\0';
        return 0;
    }

    buffer[strcspn(buffer, "\n")] = '\0';
    len = strlen(buffer);

                                            //remove leading spaces
    while(buffer[start]==' ' || buffer[start]=='\t'){
        start++;
    }
                                            //remove trailing spaces
    end=(int)strlen(buffer)-1;
    while(end>=start && (buffer[end]==' ' || buffer[end]=='\t')){
        buffer[end]='\0';
        end--;
    }

    if(start>0){
        memmove(buffer,buffer+start,strlen(buffer+start)+1);
    }

    return 1;
}

int read_int_input(const char *prompt,int *value){
    char buffer[128];
    char *endptr;   //stores where strtol() stopped
    long parsed;    //the integer value read

    if(!read_line_input(prompt,buffer,sizeof(buffer))){
        return 0;
    }

    parsed=strtol(buffer,&endptr,10); 
    if(buffer[0]=='\0' || *endptr!='\0'){  //when string is empty or there are non digits
        printf("Invalid input\n");
        return 0;
    }

    *value=(int)parsed;
    return 1;
}

void print_datentime(time_t t){
    struct tm *tim=localtime(&t);   
    char buffer[100];
    strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",tim);
    printf("%-25s",buffer);
}

Book* find_book(int book_id){
    Book*temp=book_head;
    while(temp){
        if(temp->book_id==book_id){
            return temp;
        }
        temp=temp->next;
    }
    return NULL;
}

Book* find_book_by_title(char title[]){
    Book*temp=book_head;
    while(temp){
        if(case_not_care(temp->title,title)){
            return temp;
        }
        temp=temp->next;
    }
    return NULL;
}

User* find_user_by_title(char title[]){
    User*temp=user_head;
    while(temp){
        if(strcmp(temp->name,title)==0){
            return temp;
        }
        temp=temp->next;
    }
    return NULL;
}
    
User* find_user(int user_id){
    User*temp=user_head;
    while(temp){
        if(temp->user_id==user_id)
            return temp;
        temp=temp->next;
    }
    return NULL;
}

Transaction* find_active_transaction(int user_id,int book_id) {
    Transaction *temp=trans_head;
    while(temp) {
        if(temp->user_id==user_id && temp->book_id==book_id && temp->returned==0)
            return temp;
        temp=temp->next;
    }
    return NULL;
}

int add_user() {
    char name[100];
    if(!read_line_input("Enter user name: ", name, sizeof(name))){
        return 0;
    }

    User *new_user=(User*)malloc(sizeof(User));

    new_user->user_id=next_userid++;
    new_user->fine=0;
    strcpy(new_user->name,name);

//add to front
    new_user->next=user_head;
    user_head=new_user;

    printf("Welcome!! System generated ID=%d\n",new_user->user_id);
    return 1;
}

int add_book(){
    int copies;
    char title[100];
    Book*b;

    if(!check_admin_password()){
        return 0;
    }

    if(!read_line_input("Enter book title: ", title, sizeof(title))){
        return 0;
    }

    b=find_book_by_title(title);

    if(b){              //book already exists so increase number of copies.
        int extra;
        printf("Book already exists.\n");
        if(!read_int_input("Enter additional copies: ",&extra)){
            return 0;
        }

        if(extra<=0) {
            printf("Nothing added\n");
            return 0;
        }
        b->total_copies+=extra;
        b->available_copies+=extra;
        printf("Copies updated successfully.\n");
        return 1;
    }
    //new book 
    if(!read_int_input("Enter number of copies: ", &copies)){
        return 0;
    }

    if(copies <= 0) {
        printf("Nothing added\n");
        return 0;
    }

    Book *newb=(Book*)malloc(sizeof(Book));
    newb->book_id=next_bookid++;
    strcpy(newb->title, title);
    newb->total_copies=copies;
    newb->available_copies=copies;

    //add to front
    newb->next=book_head;
    book_head=newb;

    printf("Book added successfully.\n");
    return 1;
}

int delete_book(){
    char title[100];
    char query[5];
    int copies;
    if(!check_admin_password()){
        return 0;
    }

    if(!read_line_input("Enter book title to delete: ", title, sizeof(title))){
        return 0;
    }

    Book*curr=book_head;
    Book*prev=NULL;

    while(curr){
        if(case_not_care(title,curr->title)){
            break;
        }
        prev=curr;
        curr=curr->next;
    }

    if(!curr){
        printf("Book not found.\n");
        return 0;
    }
    printf("Available copies: %d\n",curr->available_copies);

    if(!read_line_input("Do u want to delete the book completely? [y/n]: ",query,sizeof(query))){
        return 0;
    }

    if(case_not_care("y",query)){
        if(curr->total_copies!=curr->available_copies){
            printf("Book cannot be deleted as already some copies have been issued.\n");
            return 0;
        }
        if(prev==NULL){
            book_head=curr->next;
        }
        else{
            prev->next=curr->next;
        }
        free(curr);
        printf("Book deleted succesfully\n");
        return 1;}

    else if(case_not_care("n",query)){

        if(!read_int_input("How many copies do u want to delete? : ",&copies)) return 0;

        if(copies >= curr->available_copies || copies<=0){
            printf("Invalid number\n");
            return 0;
        }
        curr->available_copies-=copies;
        printf("Updated number of available copies: %d\n",curr->available_copies);
        return 1;
    }
    return 0;
}

int borrow_book(){
    int user_id;
    char title[100];
    time_t now;

    if(!read_int_input("Enter user id: ",&user_id)){
        return 0;
    }
    if(!read_line_input("Enter book title: ",title,sizeof(title))){
        return 0;
    }

    User*u=find_user(user_id);
    if(!u){
        printf("User not found :(\n");
        return 0;
    }

    Book*b=find_book_by_title(title);
    if(!b){
        printf("Book not found. You can request the book.\n");
        return 0;
    }

    if(b->available_copies<=0){
        printf("No copies available. You can request the book.\n");
        return 0;
    }

    if(find_active_transaction(user_id,b->book_id)){
        printf("You already have this book :/\n");
        return 0;
    }

    Transaction *t=(Transaction*)malloc(sizeof(Transaction));
    
    now=time(NULL);

    t->trans_id=next_transaction_id++;
    t->user_id=user_id;
    t->book_id=b->book_id;
    t->issue_time=now;
    t->return_time=0;
    t->returned=0;
    t->fine_added=0;
//add front
    t->next=trans_head;
    trans_head=t;

    b->available_copies--;

    printf("Book issued successfully. Transaction ID=%d\n",t->trans_id);
    return 1;
}

int return_book(){
    int user_id,late_days,fine,total_days;
    char title[100];
    time_t now;
    double diff;

    if(!read_int_input("Enter user id: ",&user_id)){
        return 0;
    }
    if(!read_line_input("Enter book title: ",title,sizeof(title))){
        return 0;
    }

    User*u=find_user(user_id);
    if(!u){
        printf("User not found.\n");
        return 0;
    }

    Book*b=find_book_by_title(title);
    if(!b){
        printf("Book not found.\n");
        return 0;
    }

    Transaction*t=find_active_transaction(user_id,b->book_id);

    if(!t){
        printf("No active issue record found for this book and user.\n");
        return 0;
    }

    now=time(NULL);
    t->return_time=now;
    t->returned=1;

    b->available_copies++;

    diff=difftime(t->return_time,t->issue_time);
    total_days=(int)(diff/86400);

    if(total_days>1){
        late_days=total_days-1;
        fine=late_days*5;
        u->fine+=fine;
        t->fine_added=fine;
        printf("Book returned successfully.\n");
        printf("Returned late by %d days.\n",late_days);
        printf("Fine added=Rs %d\n",fine);
    }
    
    else{
        t->fine_added=0;
        printf("Book returned successfully.\n");
    }

    return 1;
}

int search_book(){
    char title[100];
    Book*temp;

    if(!read_line_input("Enter book title to search: ", title, sizeof(title))){
        return 0;
    }

    temp=book_head;
    while(temp){
        if(case_not_care(temp->title,title)){
            printf("\nBook found:\n");
            printf("Book ID: %d\n",temp->book_id);
            printf("Title: %s\n",temp->title);
            printf("Total copies: %d\n",temp->total_copies);
            printf("Available copies: %d\n",temp->available_copies);
            return 1;
        }
        temp=temp->next;
    }

    printf("Book not found.\n");
    return 0;
}

int display_books(){
    Book*temp=book_head;

    if(!temp){
        printf("No books in library!!!\n");
        return 0;
    }

    printf("\n==========BOOK-LIST==========\n");
    while(temp){
        printf("Book ID: %d\n",temp->book_id);
        printf("Title: %s\n",temp->title);
        printf("Total copies: %d\n",temp->total_copies);
        printf("Available copies: %d\n",temp->available_copies);
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        temp=temp->next;
    }

    return 1;
}

int how_much_fine(Transaction*t){
    time_t now;
    double diff;
    int total_days;
    int late_days;

    if(t->returned){
        return t->fine_added;
    }

    now=time(NULL);
    diff=difftime(now,t->issue_time);
    total_days=(int)(diff/60);
    if(total_days<=1){
        return 0;
    }
    late_days=total_days-1;
    return late_days*5;
}

int live_overdue_for_user(int user_id) {
    Transaction *temp=trans_head;
    int total=0;

    while (temp) {
        if (temp->user_id==user_id && temp->returned==0) {
            total+=how_much_fine(temp);
        }
        temp = temp->next;
    }

    return total;
}

int display_users(){
    User*temp=user_head;

    if(!temp){
        printf("No users registered.\n");
        return 0;
    }

    printf("\n==========USER-LIST==========\n");
    while(temp){
        printf("User ID: %d\n",temp->user_id);
        printf("Name: %s\n",temp->name);
        printf("Payable Fine: Rs %d\n",temp->fine);
        printf("Current Overdue: Rs %d\n",live_overdue_for_user(temp->user_id));
        printf("------------------------------\n");
        temp=temp->next;
    }

    return 1;
}

int display_transactions(){
    Transaction*temp=trans_head;

    if(!temp){
        printf("No transactions found.\n");
        return 0;
    }

    printf("\n==========TRANSACTION LIST==========\n");

    while(temp){
        printf("Transaction ID: %d\n",temp->trans_id);
        printf("User ID: %d\n",temp->user_id);
        printf("Book ID: %d\n",temp->book_id);
        printf("Issue time: ");
        print_datentime(temp->issue_time);

        printf("\nReturn time: ");
        if(temp->returned){
            print_datentime(temp->return_time);
        }
        else{printf("Not returned yet");}
        printf("\n");
        printf("Returned: %s\n",temp->returned ? "Yes" : "No");
        printf("Current Fine: Rs %d\n",how_much_fine(temp));
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

        temp=temp->next;
    }
     return 1;
}

int request_book(){
    int user_id;
    char title[100];
    User*u;
    Request*r;
    Book*b;

    if(!read_int_input("Enter user id: ", &user_id)){
        return 0;
    }
    if(!read_line_input("Enter book title: ", title, sizeof(title))){
        return 0;
    }

    u=find_user(user_id);
    if(!u){
        printf("User not found.\n");
        return 0;
    }

    b=find_book_by_title(title);

    if(b){
        if(b->available_copies>0){
            printf("Book is available in library.Your request is denied :(\n");
            return 0;
        }
        else{
            printf("Book exists but currently no copies are available.Your request is added.\n");
        }
    }
    else{
        printf("Book does not exist in library. Your request is added.\n");
    }

    r=(Request*)malloc(sizeof(Request));

    r->user_id=user_id;
    strcpy(r->title,title);

//add front
    r->next=req_head;
    req_head=r;

    printf("Book request added successfully.\n");
    return 1;
}

int view_requests(){
    Request*temp=req_head;

    if(!temp){
        printf("No book requests found.\n");
        return 0;
    }

    printf("\n==========BOOK REQUESTS==========\n");
    while(temp){
        printf("User ID: %d\n",temp->user_id);
        printf("Book Title: %s\n",temp->title);
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        temp=temp->next;
    }

    return 1;
}

int save(){
    FILE *fb,*fu,*ft,*fr;
    Book*tb;
    User*tu;
    Transaction*tt;
    Request*tr;

    fb=fopen("Books.txt","w");
    fu=fopen("Users.txt","w");
    ft=fopen("Transactions.txt","w");
    fr=fopen("Requests.txt","w");

    if(!fb || !fu || !ft || !fr){
        printf("Couldn't open files.\n");
        return 0;
    }
    tb=book_head;
    while(tb){
        fprintf(fb,"%d|%s|%d|%d\n",tb->book_id,tb->title,tb->total_copies,tb->available_copies);
        tb=tb->next;
    }
    tu=user_head;
    while(tu){
        fprintf(fu,"%d|%s|%d\n",tu->user_id,tu->name,tu->fine);
        tu=tu->next;
    }
    tt=trans_head;
    while(tt){
        fprintf(ft,"%d|%d|%d|%lld|%lld|%d|%d\n",tt->trans_id,tt->user_id,tt->book_id,(long long)tt->issue_time,(long long)tt->return_time,tt->returned,tt->fine_added);
        tt=tt->next;
    }
    tr=req_head;
    while(tr){
        fprintf(fr,"%d|%s\n",tr->user_id,tr->title);
        tr=tr->next;
    }
    fclose(fb);
    fclose(fu);
    fclose(ft);
    fclose(fr);
    printf("Saved successfully.\n");
    return 1;
}

int load(){
    FILE *fb,*fu,*ft,*fr;
    int max_book_id=0;
    int max_user_id=0;
    int max_trans_id=0;
    
    fb=fopen("Books.txt","r");
    fu=fopen("Users.txt","r");
    ft=fopen("Transactions.txt","r");
    fr=fopen("Requests.txt","r");
    
    if(!fb || !fu || !ft || !fr){
        printf("Couldn't open files.\n");
        if(fb) fclose(fb);
        if(fu) fclose(fu);
        if(ft) fclose(ft);
        if(fr) fclose(fr);
        return 0;
    }
    char line[300];

    clear_all_data();

    Book *btail=NULL;
    User *utail=NULL;
    Transaction *ttail=NULL;
    Request *rtail=NULL;

    //Book
    while(fgets(line,sizeof(line),fb)){
        Book*bnew=malloc(sizeof(Book));
        if(sscanf(line,"%d|%[^|]|%d|%d",&bnew->book_id,bnew->title,&bnew->total_copies,&bnew->available_copies)!=4){
            free(bnew);
            if(btail) btail->next=NULL;
            continue;
        }
        bnew->next=NULL;
        if(book_head==NULL){
            book_head=bnew;
            btail=bnew;
        }
        else{
            btail->next=bnew;
            btail=bnew;
        }

        if(bnew->book_id > max_book_id){
            max_book_id=bnew->book_id;
        }
    }
    //User
    while(fgets(line,sizeof(line),fu)){
        User *unew=malloc(sizeof(User));

        if(sscanf(line,"%d|%[^|]|%d",&unew->user_id,unew->name,&unew->fine)!=3){
            free(unew);
            if(utail) utail->next=NULL;
            continue;
        }
        unew->next=NULL;
        if(user_head==NULL){
            user_head=unew;
            utail=unew;
        }
        else{
            utail->next=unew;
            utail=unew;
        }

        if(unew->user_id > max_user_id){
            max_user_id=unew->user_id;
        }
    }
    //Request
    while(fgets(line,sizeof(line),fr)){
         Request *rnew=malloc(sizeof(Request));

        if(sscanf(line,"%d|%[^|]",&rnew->user_id,rnew->title)!=2){
            free(rnew);
            if(rtail) rtail->next=NULL;
            continue;
        }
        rnew->next=NULL;
        if(req_head==NULL){
            req_head=rnew;
            rtail=rnew;
        }
        else{
            rtail->next=rnew;
            rtail=rnew;
        }
    }
    //Trasaction
    while(fgets(line,sizeof(line),ft)){
        Transaction*tnew=malloc(sizeof(Transaction));

        if(sscanf(line,"%d|%d|%d|%lld|%lld|%d|%d",&tnew->trans_id,&tnew->user_id,&tnew->book_id,(long long*)&tnew->issue_time,(long long*)&tnew->return_time,&tnew->returned,&tnew->fine_added)!=7){
            free(tnew);
            if(ttail) ttail->next=NULL;
            continue;
        }
        tnew->next=NULL;
        if(trans_head==NULL){
            trans_head=tnew;
            ttail=tnew;
        }
        else{
            ttail->next=tnew;
            ttail=tnew;
        }

        if(tnew->trans_id > max_trans_id){
            max_trans_id=tnew->trans_id;
        }
    }

    next_bookid=max_book_id+1;
    next_userid=max_user_id+1;
    next_transaction_id=max_trans_id+1;

    fclose(fb);
    fclose(fu);
    fclose(ft);
    fclose(fr);

    printf("Loaded successfully.\n");
    return 1;
}

int about_user(){
    char name[50];
    if(!read_line_input("Enter your name\n", name, sizeof(name))){
        return 0;
    }

    User*user=find_user_by_title(name);

    if(!user){
        printf("User not found\n");
        return 0;
    }
    
    printf("User ID: %d\n",user->user_id);

    Transaction *temp=trans_head;
    
    int flag1=1;int firsttime=1;
    
    while(temp){
        if(temp->user_id==user->user_id){
            if(firsttime){
            printf("_________________________________________________________________________\n");
            printf("%-20s %-5s %14s %25s\n","BOOK","FINE","ISSUE TIME","RETURN TIME");firsttime=0;
            printf("--------------------------------------------------------------------------\n");}
            Book*b=find_book(temp->book_id);
            printf("%-20s %-5d ",b->title,how_much_fine(temp));
            print_datentime(temp->issue_time);
            if(temp->returned==0){printf("Not returned yet.");}
            else{
                print_datentime(temp->return_time);}
            printf("\n");
            flag1=0;
        }       
        temp=temp->next;
    }
    if(flag1) printf("---No books--- \n");
    printf("Payable Fine: Rs %d\n",user->fine);
    printf("Current Overdue: Rs %d\n",live_overdue_for_user(user->user_id));
    return 1;
}

int pay_fine(){
    int user_id;
    int amount;
    User*u;

    if(!read_int_input("Enter user id: ",&user_id)){
        return 0;
    }
    u=find_user(user_id);
    if(!u){
        printf("User not found\n");
        return 0;
    }
    if(u->fine<=0){
        printf("No fine due.\n");
        return 0;
    }
    printf("Payable fine: Rs %d\n",u->fine);
    printf("Current overdue on unreturned books: Rs %d\n",live_overdue_for_user(user_id));
    
    if(!read_int_input("Amount: ",&amount)){
        return 0;
    }
    if(amount<=0){
        printf("Invalid amount.\n");
        return 0;
    }
    if(amount>u->fine){
        printf("Amount cannot be more than fine.Your fine is Rs %d\n",u->fine);
        return 0;
    }

    u->fine-=amount;
    printf("Payment successful.\n");
    return 1;
}







   











