#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

// Структура контакта
#define NAME_LEN 50
#define PHONE_LEN 20
#define EMAIL_LEN 50
#define CATEGORY_LEN 30

struct Contact {
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char email[EMAIL_LEN];
    char birthday[11];
    char category[CATEGORY_LEN];
};

// Узел бинарного дерева
struct TreeNode {
    struct Contact data;
    struct TreeNode* left;
    struct TreeNode* right;
};

// ---------- Функции дерева ----------
struct TreeNode* createNode(struct Contact data) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    return newNode;
}

struct TreeNode* insert(struct TreeNode* root, struct Contact data) {
    if (root == NULL) return createNode(data);
    if (strcmp(data.name, root->data.name) < 0)
        root->left = insert(root->left, data);
    else if (strcmp(data.name, root->data.name) > 0)
        root->right = insert(root->right, data);
    else
        printf("Контакт с именем '%s' уже существует!\n", data.name);
    return root;
}

struct TreeNode* search(struct TreeNode* root, const char* name) {
    if (root == NULL) return NULL;
    int cmp = strcmp(name, root->data.name);
    if (cmp == 0) return root;
    else if (cmp < 0) return search(root->left, name);
    else return search(root->right, name);
}

// ---------- Новые функции ----------

// Поиск минимального элемента (самый левый узел)
struct TreeNode* findMin(struct TreeNode* root) {
    if (root == NULL) return NULL;
    while (root->left != NULL)
        root = root->left;
    return root;
}

// Поиск максимального элемента (самый правый узел)
struct TreeNode* findMax(struct TreeNode* root) {
    if (root == NULL) return NULL;
    while (root->right != NULL)
        root = root->right;
    return root;
}

// Высота дерева
int treeHeight(struct TreeNode* root) {
    if (root == NULL) return 0;
    int leftHeight = treeHeight(root->left);
    int rightHeight = treeHeight(root->right);
    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

// Удаление узла по имени (ключу)
struct TreeNode* deleteNode(struct TreeNode* root, const char* name) {
    if (root == NULL) return NULL;
    int cmp = strcmp(name, root->data.name);
    if (cmp < 0)
        root->left = deleteNode(root->left, name);
    else if (cmp > 0)
        root->right = deleteNode(root->right, name);
    else {
        // Узел найден
        // Случай 1: нет детей или один ребёнок
        if (root->left == NULL) {
            struct TreeNode* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            struct TreeNode* temp = root->left;
            free(root);
            return temp;
        }
        // Случай 2: два ребёнка – ищем минимальный в правом поддереве
        struct TreeNode* minNode = findMin(root->right);
        root->data = minNode->data;          // копируем данные
        root->right = deleteNode(root->right, minNode->data.name); // удаляем тот узел
    }
    return root;
}

// Сохранение дерева в файл (preorder с маркерами NULL)
void saveTreeToFile(FILE* f, struct TreeNode* root) {
    if (root == NULL) {
        fprintf(f, "NULL\n");
        return;
    }
    fprintf(f, "%s %s %s %s %s\n", root->data.name, root->data.phone,
        root->data.email, root->data.birthday, root->data.category);
    saveTreeToFile(f, root->left);
    saveTreeToFile(f, root->right);
}

// Загрузка дерева из файла (рекурсивное чтение)
struct TreeNode* loadTreeFromFile(FILE* f) {
    char line[256];
    if (!fgets(line, sizeof(line), f)) return NULL;
    line[strcspn(line, "\n")] = '\0';
    if (strcmp(line, "NULL") == 0) return NULL;
    struct Contact data;
    sscanf(line, "%s %s %s %s %s", data.name, data.phone, data.email,
        data.birthday, data.category);
    struct TreeNode* node = createNode(data);
    node->left = loadTreeFromFile(f);
    node->right = loadTreeFromFile(f);
    return node;
}

// Освобождение памяти всего дерева
void freeTree(struct TreeNode* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

// ---------- Функции ввода и меню ----------
struct Contact inputContact() {
    struct Contact newContact;
    printf("\n--- Ввод нового контакта ---\n");
    printf("Имя: "); scanf("%s", newContact.name);
    printf("Телефон: "); scanf("%s", newContact.phone);
    printf("Email: "); scanf("%s", newContact.email);
    printf("Дата рождения (ДД.ММ.ГГГГ): "); scanf("%s", newContact.birthday);
    printf("Категория (личные/работа/другое): "); scanf("%s", newContact.category);
    return newContact;
}

void inorder(struct TreeNode* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("Имя: %s, Тел: %s, Email: %s, Дата: %s, Категория: %s\n",
            root->data.name, root->data.phone, root->data.email,
            root->data.birthday, root->data.category);
        inorder(root->right);
    }
}

void printMenu() {
    printf("\n========== МЕНЮ (БИНАРНОЕ ДЕРЕВО – РАСШИРЕННЫЕ ОПЕРАЦИИ) ==========\n");
    printf("1. Добавить контакт\n");
    printf("2. Вывести все контакты (inorder)\n");
    printf("3. Удалить контакт по имени\n");
    printf("4. Найти минимальный элемент\n");
    printf("5. Найти максимальный элемент\n");
    printf("6. Вычислить высоту дерева\n");
    printf("7. Сохранить дерево в файл\n");
    printf("8. Загрузить дерево из файла\n");
    printf("9. Выход\n");
    printf("Выберите действие: ");
}

int main() {
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif

    struct TreeNode* root = NULL;
    int choice;

    do {
        printMenu();
        scanf("%d", &choice);
        while (getchar() != '\n');

        switch (choice) {
        case 1: {
            struct Contact newContact = inputContact();
            root = insert(root, newContact);
            printf("Контакт добавлен.\n");
            break;
        }
        case 2:
            if (root == NULL) printf("Дерево пусто.\n");
            else {
                printf("\n=== Все контакты (inorder) ===\n");
                inorder(root);
            }
            break;
        case 3: {
            if (root == NULL) {
                printf("Дерево пусто.\n");
                break;
            }
            char name[NAME_LEN];
            printf("Введите имя контакта для удаления: ");
            scanf("%s", name);
            struct TreeNode* found = search(root, name);
            if (found == NULL) {
                printf("Контакт не найден.\n");
            }
            else {
                root = deleteNode(root, name);
                printf("Контакт удалён.\n");
            }
            break;
        }
        case 4: {
            struct TreeNode* minNode = findMin(root);
            if (minNode == NULL) printf("Дерево пусто.\n");
            else printf("Минимальный элемент: %s\n", minNode->data.name);
            break;
        }
        case 5: {
            struct TreeNode* maxNode = findMax(root);
            if (maxNode == NULL) printf("Дерево пусто.\n");
            else printf("Максимальный элемент: %s\n", maxNode->data.name);
            break;
        }
        case 6: {
            int h = treeHeight(root);
            printf("Высота дерева: %d\n", h);
            break;
        }
        case 7: {
            if (root == NULL) {
                printf("Дерево пусто, нечего сохранять.\n");
                break;
            }
            FILE* f = fopen("tree.txt", "w");
            if (f == NULL) {
                printf("Ошибка открытия файла для записи.\n");
                break;
            }
            saveTreeToFile(f, root);
            fclose(f);
            printf("Дерево сохранено в файл tree.txt.\n");
            break;
        }
        case 8: {
            FILE* f = fopen("tree.txt", "r");
            if (f == NULL) {
                printf("Файл tree.txt не найден.\n");
                break;
            }
            freeTree(root);
            root = loadTreeFromFile(f);
            fclose(f);
            printf("Дерево загружено из файла.\n");
            break;
        }
        case 9:
            printf("Выход из программы...\n");
            freeTree(root);
            break;
        default:
            printf("Неверный выбор. Попробуйте снова.\n");
        }
    } while (choice != 9);

    return 0;
}