// -*- coding: utf-8 -*-
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"

struct Contact* contacts = NULL;
int size = 0;
int capacity = 0;

void expandArray(int additional) {
    int newCapacity = capacity + additional;
    struct Contact* newArray = (struct Contact*)malloc(newCapacity * sizeof(struct Contact));
    if (newArray == NULL) {
        printf("Ошибка выделения памяти!\n");
        return;
    }
    for (int i = 0; i < size; i++)
        newArray[i] = contacts[i];
    free(contacts);
    contacts = newArray;
    capacity = newCapacity;
    printf("Массив расширен до %d элементов.\n", capacity);
}

void pushBack(struct Contact newContact) {
    if (size == capacity)
        expandArray(1);
    contacts[size] = newContact;
    size++;
}

void popBack() {
    if (size == 0) {
        printf("Список контактов пуст, удалять нечего.\n");
        return;
    }
    size--;
    printf("Последний контакт удалён. Текущее количество: %d\n", size);
}

void clearArray() {
    if (contacts != NULL) {
        free(contacts);
        contacts = NULL;
        size = 0;
        capacity = 0;
        printf("Вся память освобождена, список очищен.\n");
    }
}

void manualFree() {
    if (contacts == NULL || size == 0) {
        printf("Память уже освобождена или список пуст.\n");
        return;
    }
    printf("Освободить всю память? (y/n): ");
    char ch;
    scanf(" %c", &ch);
    if (ch == 'y' || ch == 'Y')
        clearArray();
    else
        printf("Операция отменена.\n");
}

// ---------- Работа с файлами ----------
void saveToFile() {
    FILE* f = fopen(SAVE_FILE, "w");
    if (f == NULL) {
        printf("Ошибка открытия файла для записи!\n");
        return;
    }
    fprintf(f, "%d\n", size);
    for (int i = 0; i < size; i++) {
        fprintf(f, "%s\n", contacts[i].name);
        fprintf(f, "%s\n", contacts[i].phone);
        fprintf(f, "%s\n", contacts[i].email);
        fprintf(f, "%s\n", contacts[i].birthday);
        fprintf(f, "%s\n", contacts[i].category);
    }
    fclose(f);
    printf("Контакты сохранены в файл %s.\n", SAVE_FILE);
}

void loadFromFile() {
    FILE* f = fopen(SAVE_FILE, "r");
    if (f == NULL) {
        printf("Файл %s не найден. Начинаем с пустого списка.\n", SAVE_FILE);
        return;
    }
    int newSize;
    fscanf(f, "%d", &newSize);
    if (newSize <= 0) {
        fclose(f);
        return;
    }
    clearArray();
    capacity = (newSize > INITIAL_CAPACITY) ? newSize : INITIAL_CAPACITY;
    contacts = (struct Contact*)malloc(capacity * sizeof(struct Contact));
    if (contacts == NULL) {
        printf("Ошибка выделения памяти!\n");
        fclose(f);
        return;
    }
    size = 0;
    for (int i = 0; i < newSize; i++) {
        struct Contact tmp;
        fscanf(f, "%s", tmp.name);
        fscanf(f, "%s", tmp.phone);
        fscanf(f, "%s", tmp.email);
        fscanf(f, "%s", tmp.birthday);
        fscanf(f, "%s", tmp.category);
        pushBack(tmp);
    }
    fclose(f);
    printf("Загружено %d контактов из файла %s.\n", size, SAVE_FILE);
}

// ---------- Функции работы с контактами ----------
void add_contact() {
    struct Contact newContact;
    printf("\n--- Добавление нового контакта ---\n");
    printf("Имя: ");
    scanf("%s", newContact.name);
    printf("Телефон: ");
    scanf("%s", newContact.phone);
    printf("Email: ");
    scanf("%s", newContact.email);
    printf("Дата рождения (ДД.ММ.ГГГГ): ");
    scanf("%s", newContact.birthday);
    printf("Категория (личные/работа/другое): ");
    scanf("%s", newContact.category);
    // Очистка буфера после ввода (на случай, если остался символ новой строки)
    while (getchar() != '\n');
    pushBack(newContact);
    printf("Контакт успешно добавлен!\n");
}

void print_contacts() {
    if (size == 0) {
        printf("Список контактов пуст.\n");
        return;
    }
    printf("\n========== ВСЕ КОНТАКТЫ ==========\n");
    for (int i = 0; i < size; i++) {
        printf("Контакт #%d\n", i + 1);
        printf("  Имя: %s\n", contacts[i].name);
        printf("  Телефон: %s\n", contacts[i].phone);
        printf("  Email: %s\n", contacts[i].email);
        printf("  Дата рождения: %s\n", contacts[i].birthday);
        printf("  Категория: %s\n", contacts[i].category);
        printf("---------------------------------\n");
    }
}

void edit_contact() {
    if (size == 0) {
        printf("Нет контактов для редактирования.\n");
        return;
    }
    int index;
    printf("Введите номер контакта для редактирования (1-%d): ", size);
    scanf("%d", &index);
    while (getchar() != '\n'); // очистка буфера
    if (index < 1 || index > size) {
        printf("Неверный номер!\n");
        return;
    }
    index--;
    printf("\nРедактирование контакта #%d\n", index + 1);
    printf("Текущие данные:\n");
    printf("1. Имя: %s\n", contacts[index].name);
    printf("2. Телефон: %s\n", contacts[index].phone);
    printf("3. Email: %s\n", contacts[index].email);
    printf("4. Дата рождения: %s\n", contacts[index].birthday);
    printf("5. Категория: %s\n", contacts[index].category);
    int choice;
    printf("\nВыберите поле для редактирования (1-5): ");
    scanf("%d", &choice);
    while (getchar() != '\n');
    switch (choice) {
    case 1: printf("Новое имя: "); scanf("%s", contacts[index].name); break;
    case 2: printf("Новый телефон: "); scanf("%s", contacts[index].phone); break;
    case 3: printf("Новый email: "); scanf("%s", contacts[index].email); break;
    case 4: printf("Новая дата рождения (ДД.ММ.ГГГГ): "); scanf("%s", contacts[index].birthday); break;
    case 5: printf("Новая категория: "); scanf("%s", contacts[index].category); break;
    default: printf("Неверный выбор!\n"); return;
    }
    while (getchar() != '\n');
    printf("Контакт успешно обновлён!\n");
}

void search_contacts() {
    if (size == 0) {
        printf("Список контактов пуст.\n");
        return;
    }
    int choice;
    printf("\nПоиск контактов\n1 - по имени\n2 - по категории\nВаш выбор: ");
    scanf("%d", &choice);
    while (getchar() != '\n');
    if (choice == 1) {
        char search_name[NAME_LEN];
        printf("Введите имя для поиска: ");
        scanf("%s", search_name);
        int found = 0;
        printf("\n--- Результаты поиска по имени '%s' ---\n", search_name);
        for (int i = 0; i < size; i++) {
            if (strcmp(contacts[i].name, search_name) == 0) {
                printf("Контакт #%d\n", i + 1);
                printf("  Имя: %s\n", contacts[i].name);
                printf("  Телефон: %s\n", contacts[i].phone);
                printf("  Email: %s\n", contacts[i].email);
                printf("  Дата рождения: %s\n", contacts[i].birthday);
                printf("  Категория: %s\n", contacts[i].category);
                printf("---------------------------------\n");
                found = 1;
            }
        }
        if (!found) printf("Контакты с именем '%s' не найдены.\n", search_name);
    }
    else if (choice == 2) {
        char search_cat[CATEGORY_LEN];
        printf("Введите категорию для поиска: ");
        scanf("%s", search_cat);
        int found = 0;
        printf("\n--- Результаты поиска по категории '%s' ---\n", search_cat);
        for (int i = 0; i < size; i++) {
            if (strcmp(contacts[i].category, search_cat) == 0) {
                printf("Контакт #%d\n", i + 1);
                printf("  Имя: %s\n", contacts[i].name);
                printf("  Телефон: %s\n", contacts[i].phone);
                printf("  Email: %s\n", contacts[i].email);
                printf("  Дата рождения: %s\n", contacts[i].birthday);
                printf("  Категория: %s\n", contacts[i].category);
                printf("---------------------------------\n");
                found = 1;
            }
        }
        if (!found) printf("Контакты с категорией '%s' не найдены.\n", search_cat);
    }
    else {
        printf("Неверный выбор!\n");
    }
}

void sortContacts() {
    if (size < 2) {
        printf("Недостаточно контактов для сортировки.\n");
        return;
    }
    int field;
    printf("\nВыберите поле для сортировки:\n");
    printf("1 - по имени\n2 - по телефону\n3 - по email\n4 - по дате рождения\n5 - по категории\n");
    printf("Ваш выбор: ");
    scanf("%d", &field);
    while (getchar() != '\n');
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            int needSwap = 0;
            switch (field) {
            case 1: needSwap = (strcmp(contacts[j].name, contacts[j + 1].name) > 0); break;
            case 2: needSwap = (strcmp(contacts[j].phone, contacts[j + 1].phone) > 0); break;
            case 3: needSwap = (strcmp(contacts[j].email, contacts[j + 1].email) > 0); break;
            case 4: needSwap = (strcmp(contacts[j].birthday, contacts[j + 1].birthday) > 0); break;
            case 5: needSwap = (strcmp(contacts[j].category, contacts[j + 1].category) > 0); break;
            default: printf("Неверный выбор!\n"); return;
            }
            if (needSwap) {
                struct Contact temp = contacts[j];
                contacts[j] = contacts[j + 1];
                contacts[j + 1] = temp;
            }
        }
    }
    printf("Сортировка выполнена.\n");
}

void insertAtIndex() {
    if (size == capacity) expandArray(1);
    int pos;
    printf("Введите позицию для вставки (1..%d): ", size + 1);
    scanf("%d", &pos);
    while (getchar() != '\n');
    if (pos < 1 || pos > size + 1) {
        printf("Неверная позиция!\n");
        return;
    }
    for (int i = size; i >= pos; i--) {
        contacts[i] = contacts[i - 1];
    }
    struct Contact newContact;
    printf("\n--- Ввод нового контакта ---\n");
    printf("Имя: "); scanf("%s", newContact.name);
    printf("Телефон: "); scanf("%s", newContact.phone);
    printf("Email: "); scanf("%s", newContact.email);
    printf("Дата рождения (ДД.ММ.ГГГГ): "); scanf("%s", newContact.birthday);
    printf("Категория (личные/работа/другое): "); scanf("%s", newContact.category);
    while (getchar() != '\n');
    contacts[pos - 1] = newContact;
    size++;
    printf("Контакт вставлен на позицию %d.\n", pos);
}

void deleteAtIndex() {
    if (size == 0) {
        printf("Список контактов пуст.\n");
        return;
    }
    int pos;
    printf("Введите номер контакта для удаления (1..%d): ", size);
    scanf("%d", &pos);
    while (getchar() != '\n');
    if (pos < 1 || pos > size) {
        printf("Неверный номер!\n");
        return;
    }
    for (int i = pos - 1; i < size - 1; i++) {
        contacts[i] = contacts[i + 1];
    }
    size--;
    printf("Контакт удалён. Текущее количество: %d\n", size);
}