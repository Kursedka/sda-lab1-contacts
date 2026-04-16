// -*- coding: utf-8 -*-
#ifndef DATA_H
#define DATA_H

#define NAME_LEN 50
#define PHONE_LEN 20
#define EMAIL_LEN 50
#define CATEGORY_LEN 30
#define INITIAL_CAPACITY 2
#define SAVE_FILE "contacts.txt"

struct Contact {
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char email[EMAIL_LEN];
    char birthday[11];   // ƒƒ.ÃÃ.√√√√
    char category[CATEGORY_LEN];
};

extern struct Contact* contacts;
extern int size;
extern int capacity;

void expandArray(int additional);
void pushBack(struct Contact newContact);
void popBack();
void clearArray();
void manualFree();
void saveToFile();
void loadFromFile();
void add_contact();
void print_contacts();
void edit_contact();
void search_contacts();
void sortContacts();
void insertAtIndex();
void deleteAtIndex();

#endif