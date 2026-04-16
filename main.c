#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#ifdef _WIN32
#include <windows.h>
#endif

void print_menu() {
    printf("\n========== МЕНЮ УПРАВЛЕНИЯ КОНТАКТАМИ (ФАЙЛЫ + МОДУЛИ) ==========\n");
    printf("1. Добавить новый контакт\n");
    printf("2. Показать все контакты\n");
    printf("3. Редактировать контакт\n");
    printf("4. Найти контакт\n");
    printf("5. Удалить последний контакт\n");
    printf("6. Освободить память (ручной режим)\n");
    printf("7. СОРТИРОВАТЬ контакты\n");
    printf("8. ВСТАВИТЬ контакт на позицию\n");
    printf("9. УДАЛИТЬ контакт по индексу\n");
    printf("10. Загрузить из файла\n");
    printf("11. Сохранить в файл\n");
    printf("12. Выход\n");
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

    // Загружаем контакты из файла при старте
    loadFromFile();

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
        case 7: sortContacts(); break;
        case 8: insertAtIndex(); break;
        case 9: deleteAtIndex(); break;
        case 10: loadFromFile(); break;
        case 11: saveToFile(); break;
        case 12:
            printf("Выход из программы...\n");
            saveToFile();       // автосохранение перед выходом
            clearArray();
            break;
        default:
            printf("Неверный выбор. Попробуйте снова.\n");
        }
    } while (choice != 12);

    return 0;
}