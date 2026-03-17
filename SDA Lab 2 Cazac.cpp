#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

#define NAME_LEN 50
#define PHONE_LEN 20
#define EMAIL_LEN 50
#define CATEGORY_LEN 30
#define INITIAL_CAPACITY 2   // начальный размер динамического массива

struct Contact {
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char email[EMAIL_LEN];
    char birthday[11];        // ДД.ММ.ГГГГ
    char category[CATEGORY_LEN];
};

struct Contact* contacts = NULL;  // динамический массив
int size = 0;                     // текущее количество контактов
int capacity = 0;                  // выделенная ёмкость

// Функция расширения массива (без realloc)
void expandArray(int additional) {
    int newCapacity = capacity + additional;
    struct Contact* newArray = (struct Contact*)malloc(newCapacity * sizeof(struct Contact));
    if (newArray == NULL) {
        printf("Ошибка выделения памяти!\n");
        return;
    }
    // Копируем старые данные
    for (int i = 0; i < size; i++) {
        newArray[i] = contacts[i];
    }
    // Освобождаем старую память
    free(contacts);
    // Перенаправляем указатель
    contacts = newArray;
    capacity = newCapacity;
    printf("Массив расширен до %d элементов.\n", capacity);
}

// Добавление контакта в конец
void pushBack(struct Contact newContact) {
    if (size == capacity) {
        expandArray(1);  // расширяем на 1 элемент
    }
    contacts[size] = newContact;
    size++;
}

// Удаление последнего контакта
void popBack() {
    if (size == 0) {
        printf("Список контактов пуст, удалять нечего.\n");
        return;
    }
    size--;
    printf("Последний контакт удалён. Текущее количество: %d\n", size);
    // При желании можно уменьшить capacity (не требуется по заданию)
}

// Очистка всего списка и освобождение памяти
void clearArray() {
    if (contacts != NULL) {
        free(contacts);
        contacts = NULL;
        size = 0;
        capacity = 0;
        printf("Вся память освобождена, список очищен.\n");
    }
}

// Ручное освобождение памяти (вызывается из меню)
void manualFree() {
    if (contacts == NULL || size == 0) {
        printf("Память уже освобождена или список пуст.\n");
        return;
    }
    printf("Освободить всю память? (y/n): ");
    char ch;
    scanf(" %c", &ch);
    if (ch == 'y' || ch == 'Y') {
        clearArray();
    }
    else {
        printf("Операция отменена.\n");
    }
}

// ---------- Функции ввода/вывода (адаптированы для динамического массива) ----------

void add_contact() {
    struct Contact newContact;
    printf("\n--- Добавление нового контакта ---\n");
    printf("Имя: ");
    scanf("%s", newContact.name);
    printf("Номер телефона: ");
    scanf("%s", newContact.phone);
    printf("Email: ");
    scanf("%s", newContact.email);
    printf("Дата рождения (ДД.ММ.ГГГГ): ");
    scanf("%s", newContact.birthday);
    printf("Категория (личные/работа/другое): ");
    scanf("%s", newContact.category);
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
    if (size == 0) {
        printf("Список контактов пуст.\n");
        return;
    }
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

void print_menu() {
    printf("\n========== МЕНЮ УПРАВЛЕНИЯ КОНТАКТАМИ (ДИНАМИЧЕСКИЙ МАССИВ) ==========\n");
    printf("1. Добавить новый контакт\n");
    printf("2. Показать все контакты\n");
    printf("3. Редактировать контакт\n");
    printf("4. Найти контакт\n");
    printf("5. Удалить последний контакт\n");
    printf("6. Освободить память (ручной режим)\n");
    printf("7. Выход\n");
    printf("Выберите действие: ");
}

int main() {
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif

    // Начальное выделение памяти
    capacity = INITIAL_CAPACITY;
    contacts = (struct Contact*)malloc(capacity * sizeof(struct Contact));
    if (contacts == NULL) {
        printf("Ошибка выделения памяти!\n");
        return 1;
    }
    size = 0;

    int choice;
    do {
        print_menu();
        scanf("%d", &choice);
        switch (choice) {
        case 1: add_contact(); break;
        case 2: print_contacts(); break;
        case 3: edit_contact(); break;
        case 4: search_contacts(); break;
        case 5: popBack(); break;
        case 6: manualFree(); break;
        case 7:
            printf("Выход из программы...\n");
            clearArray();  // обязательное освобождение перед выходом
            break;
        default:
            printf("Неверный выбор. Попробуйте снова.\n");
        }
    } while (choice != 7);

    return 0;
}