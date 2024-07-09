#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LENGTH 50
#define DATE_LENGTH 12

struct Book
{
    int id;
    char bookName[MAX_NAME_LENGTH];
    char authorName[MAX_NAME_LENGTH];
    char date[DATE_LENGTH];
};

struct Student
{
    int id;
    char sName[MAX_NAME_LENGTH];
    char sClass[MAX_NAME_LENGTH];
    int sRoll;
    char bookName[MAX_NAME_LENGTH];
    char date[DATE_LENGTH];
};

void addBook();
void displayBooks();
void removeBook();
void issueBook();
void displayIssuedBooks();

int main()
{
    int choice;
    do
    {
        printf("<== Library Management System ==>\n");
        printf("1. Add Book\n");
        printf("2. Display Available Books\n");
        printf("3. Remove Book\n");
        printf("4. Issue Book\n");
        printf("5. Display Issued Books\n");
        printf("0. Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addBook();
            break;
        case 2:
            displayBooks();
            break;
        case 3:
            removeBook();
            break;
        case 4:
            issueBook();
            break;
        case 5:
            displayIssuedBooks();
            break;
        case 0:
            printf("Exiting...");
            break;
        default:
            printf("Invalid Choice...\n\n");
        }
        printf("\nPress Enter to continue...");
        getchar();       // Clear the newline character from the input buffer
        getchar();       // Wait for the user to press Enter
        system("clear"); // Clear the screen (for Unix-like systems)
    } while (choice != 0);

    return 0;
}

void addBook()
{
    struct Book b;
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    snprintf(b.date, DATE_LENGTH, "%02d/%02d/%d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);

    FILE *fp = fopen("books.txt", "ab+");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter book id: ");
    scanf("%d", &b.id);

    printf("Enter book name: ");
    scanf("%s", b.bookName);

    printf("Enter author name: ");
    scanf("%s", b.authorName);

    fwrite(&b, sizeof(struct Book), 1, fp);
    fclose(fp);

    printf("Book Added Successfully\n");
}

void displayBooks()
{
    struct Book b;
    FILE *fp = fopen("books.txt", "rb");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    printf("<== Available Books ==>\n");
    printf("%-10s %-30s %-20s %s\n", "Book id", "Book Name", "Author", "Date");

    while (fread(&b, sizeof(struct Book), 1, fp) == 1)
    {
        printf("%-10d %-30s %-20s %s\n", b.id, b.bookName, b.authorName, b.date);
    }

    fclose(fp);
}

void removeBook()
{
    int id;
    printf("<== Remove Book ==>\n");
    printf("Enter Book id to remove: ");
    scanf("%d", &id);

    FILE *fp = fopen("books.txt", "rb");
    FILE *ft = fopen("temp.txt", "wb");
    if (fp == NULL || ft == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    struct Book b;
    int found = 0;
    while (fread(&b, sizeof(struct Book), 1, fp) == 1)
    {
        if (id == b.id)
        {
            found = 1;
        }
        else
        {
            fwrite(&b, sizeof(struct Book), 1, ft);
        }
    }

    fclose(fp);
    fclose(ft);

    if (found)
    {
        printf("Deleted Successfully.\n");
        remove("books.txt");
        rename("temp.txt", "books.txt");
    }
    else
    {
        printf("Record Not Found !\n");
    }
}

void issueBook()
{
    struct Student s;
    struct Book b;
    int book_id;
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    snprintf(s.date, DATE_LENGTH, "%02d/%02d/%d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);

    printf("<== Issue Book ==>\n");
    printf("Enter Book id to issue: ");
    scanf("%d", &book_id);

    FILE *fp = fopen("books.txt", "rb");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    int found = 0;
    while (fread(&b, sizeof(struct Book), 1, fp) == 1)
    {
        if (b.id == book_id)
        {
            strcpy(s.bookName, b.bookName);
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found)
    {
        printf("No book found with this id\n");
        return;
    }

    fp = fopen("issue.txt", "ab+");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Student Name: ");
    scanf("%s", s.sName);

    printf("Enter Student Class: ");
    scanf("%s", s.sClass);

    printf("Enter Student Roll: ");
    scanf("%d", &s.sRoll);

    fwrite(&s, sizeof(struct Student), 1, fp);
    fclose(fp);

    printf("Book Issued Successfully\n");
}

void displayIssuedBooks()
{
    struct Student s;
    FILE *fp = fopen("issue.txt", "rb");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    printf("<== Issued Books ==>\n");
    printf("%-10s %-30s %-20s %-10s %-30s %s\n", "Book id", "Name", "Class", "Roll", "Book Name", "Date");

    while (fread(&s, sizeof(struct Student), 1, fp) == 1)
    {
        printf("%-10d %-30s %-20s %-10d %-30s %s\n", s.id, s.sName, s.sClass, s.sRoll, s.bookName, s.date);
    }
    fclose(fp);


}