#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

#define MAX_CONTACTS 10
#define NAME_LEN 50
#define PHONE_LEN 20
#define EMAIL_LEN 50
#define CATEGORY_LEN 30

struct Contact {
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char email[EMAIL_LEN];
    char birthday[11];        // ДД.ММ.ГГГГ
    char category[CATEGORY_LEN];
};

struct Contact contacts[MAX_CONTACTS];
int contact_count = 0;

void add_contact() {
    if (contact_count >= MAX_CONTACTS) {
        printf("Список контактов заполнен!\n");
        return;
    }

    printf("\n--- Добавление нового контакта ---\n");
    printf("Имя: ");
    scanf("%s", contacts[contact_count].name);
    printf("Номер телефона: ");
    scanf("%s", contacts[contact_count].phone);
    printf("Email: ");
    scanf("%s", contacts[contact_count].email);
    printf("Дата рождения (ДД.ММ.ГГГГ): ");
    scanf("%s", contacts[contact_count].birthday);
    printf("Категория (личные/работа/другое): ");
    scanf("%s", contacts[contact_count].category);

    contact_count++;
    printf("Контакт успешно добавлен!\n");
}

void print_contacts() {
    if (contact_count == 0) {
        printf("Список контактов пуст.\n");
        return;
    }

    printf("\n========== ВСЕ КОНТАКТЫ ==========\n");
    for (int i = 0; i < contact_count; i++) {
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
    int index;
    printf("Введите номер контакта для редактирования (1-%d): ", contact_count);
    scanf("%d", &index);

    if (index < 1 || index > contact_count) {
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

    switch (choice) {
    case 1:
        printf("Новое имя: ");
        scanf("%s", contacts[index].name);
        break;
    case 2:
        printf("Новый телефон: ");
        scanf("%s", contacts[index].phone);
        break;
    case 3:
        printf("Новый email: ");
        scanf("%s", contacts[index].email);
        break;
    case 4:
        printf("Новая дата рождения (ДД.ММ.ГГГГ): ");
        scanf("%s", contacts[index].birthday);
        break;
    case 5:
        printf("Новая категория: ");
        scanf("%s", contacts[index].category);
        break;
    default:
        printf("Неверный выбор!\n");
        return;
    }
    printf("Контакт успешно обновлён!\n");
}

void search_contacts() {
    int choice;
    printf("\nПоиск контактов\n");
    printf("1 - по имени\n");
    printf("2 - по категории\n");
    printf("Ваш выбор: ");
    scanf("%d", &choice);

    if (choice == 1) {
        char search_name[NAME_LEN];
        printf("Введите имя для поиска: ");
        scanf("%s", search_name);

        int found = 0;
        printf("\n--- Результаты поиска по имени '%s' ---\n", search_name);
        for (int i = 0; i < contact_count; i++) {
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
        if (!found) {
            printf("Контакты с именем '%s' не найдены.\n", search_name);
        }
    }
    else if (choice == 2) {
        char search_cat[CATEGORY_LEN];
        printf("Введите категорию для поиска: ");
        scanf("%s", search_cat);

        int found = 0;
        printf("\n--- Результаты поиска по категории '%s' ---\n", search_cat);
        for (int i = 0; i < contact_count; i++) {
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
        if (!found) {
            printf("Контакты с категорией '%s' не найдены.\n", search_cat);
        }
    }
    else {
        printf("Неверный выбор!\n");
    }
}

void print_menu() {
    printf("\n========== МЕНЮ УПРАВЛЕНИЯ КОНТАКТАМИ ==========\n");
    printf("1. Добавить новый контакт\n");
    printf("2. Показать все контакты\n");
    printf("3. Редактировать контакт\n");
    printf("4. Найти контакт\n");
    printf("5. Выход\n");
    printf("Выберите действие: ");
}

int main() {
#ifdef _WIN32
    SetConsoleCP(1251);          // Устанавливаем кодировку ввода
    SetConsoleOutputCP(1251);     // Устанавливаем кодировку вывода
#endif

    int choice;
    do {
        print_menu();
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            add_contact();
            break;
        case 2:
            print_contacts();
            break;
        case 3:
            if (contact_count > 0)
                edit_contact();
            else
                printf("Нет контактов для редактирования!\n");
            break;
        case 4:
            if (contact_count > 0)
                search_contacts();
            else
                printf("Нет контактов для поиска!\n");
            break;
        case 5:
            printf("Выход из программы...\n");
            break;
        default:
            printf("Неверный выбор. Попробуйте снова.\n");
        }
    } while (choice != 5);

    return 0;
}