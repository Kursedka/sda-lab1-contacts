#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

// Структура контакта (как в предыдущих работах)
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

// Узел бинарного дерева поиска
struct TreeNode {
    struct Contact data;
    struct TreeNode* left;
    struct TreeNode* right;
};

// ---------- Очередь для BFS ----------
struct QueueNode {
    struct TreeNode* treeNode;
    struct QueueNode* next;
};

struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
};

void initQueue(struct Queue* q) {
    q->front = q->rear = NULL;
}

int isEmptyQueue(struct Queue* q) {
    return q->front == NULL;
}

void enqueue(struct Queue* q, struct TreeNode* node) {
    struct QueueNode* newNode = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    newNode->treeNode = node;
    newNode->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    }
    else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

struct TreeNode* dequeue(struct Queue* q) {
    if (isEmptyQueue(q)) return NULL;
    struct QueueNode* temp = q->front;
    struct TreeNode* node = temp->treeNode;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp);
    return node;
}

// ---------- Функции дерева ----------

// Создание нового узла
struct TreeNode* createNode(struct Contact data) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Вставка в BST по имени (строковое сравнение)
struct TreeNode* insert(struct TreeNode* root, struct Contact data) {
    if (root == NULL) {
        return createNode(data);
    }
    // Сравниваем имена (можно использовать strcmp)
    if (strcmp(data.name, root->data.name) < 0) {
        root->left = insert(root->left, data);
    }
    else if (strcmp(data.name, root->data.name) > 0) {
        root->right = insert(root->right, data);
    }
    else {
        // Если имя уже существует, не вставляем (можно вывести сообщение)
        printf("Контакт с именем '%s' уже существует!\n", data.name);
    }
    return root;
}

// Поиск узла по имени
struct TreeNode* search(struct TreeNode* root, const char* name) {
    if (root == NULL) return NULL;
    int cmp = strcmp(name, root->data.name);
    if (cmp == 0) return root;
    else if (cmp < 0) return search(root->left, name);
    else return search(root->right, name);
}

// Прямой обход (preorder) – NLR
void preorder(struct TreeNode* root) {
    if (root != NULL) {
        printf("Имя: %s, Тел: %s, Email: %s, Дата: %s, Категория: %s\n",
            root->data.name, root->data.phone, root->data.email,
            root->data.birthday, root->data.category);
        preorder(root->left);
        preorder(root->right);
    }
}

// Центрированный обход (inorder) – LNR
void inorder(struct TreeNode* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("Имя: %s, Тел: %s, Email: %s, Дата: %s, Категория: %s\n",
            root->data.name, root->data.phone, root->data.email,
            root->data.birthday, root->data.category);
        inorder(root->right);
    }
}

// Обратный обход (postorder) – LRN
void postorder(struct TreeNode* root) {
    if (root != NULL) {
        postorder(root->left);
        postorder(root->right);
        printf("Имя: %s, Тел: %s, Email: %s, Дата: %s, Категория: %s\n",
            root->data.name, root->data.phone, root->data.email,
            root->data.birthday, root->data.category);
    }
}

// Обход в ширину (BFS) через очередь
void bfs(struct TreeNode* root) {
    if (root == NULL) return;
    struct Queue q;
    initQueue(&q);
    enqueue(&q, root);
    while (!isEmptyQueue(&q)) {
        struct TreeNode* current = dequeue(&q);
        printf("Имя: %s, Тел: %s, Email: %s, Дата: %s, Категория: %s\n",
            current->data.name, current->data.phone, current->data.email,
            current->data.birthday, current->data.category);
        if (current->left != NULL) enqueue(&q, current->left);
        if (current->right != NULL) enqueue(&q, current->right);
    }
}

// Освобождение памяти дерева (postorder)
void freeTree(struct TreeNode* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

// ---------- Функции ввода контакта ----------
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

// ---------- Меню ----------
void printMenu() {
    printf("\n========== МЕНЮ (БИНАРНОЕ ДЕРЕВО ПОИСКА) ==========\n");
    printf("1. Добавить контакт\n");
    printf("2. Вывести все контакты (inorder - отсортировано по имени)\n");
    printf("3. Прямой обход (preorder)\n");
    printf("4. Обратный обход (postorder)\n");
    printf("5. Обход в ширину (BFS)\n");
    printf("6. Поиск контакта по имени\n");
    printf("7. Выход\n");
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
        while (getchar() != '\n'); // очистка буфера

        switch (choice) {
        case 1: {
            struct Contact newContact = inputContact();
            root = insert(root, newContact);
            printf("Контакт добавлен в дерево.\n");
            break;
        }
        case 2:
            if (root == NULL)
                printf("Дерево пусто.\n");
            else {
                printf("\n=== Центрированный обход (inorder, по возрастанию имени) ===\n");
                inorder(root);
            }
            break;
        case 3:
            if (root == NULL)
                printf("Дерево пусто.\n");
            else {
                printf("\n=== Прямой обход (preorder) ===\n");
                preorder(root);
            }
            break;
        case 4:
            if (root == NULL)
                printf("Дерево пусто.\n");
            else {
                printf("\n=== Обратный обход (postorder) ===\n");
                postorder(root);
            }
            break;
        case 5:
            if (root == NULL)
                printf("Дерево пусто.\n");
            else {
                printf("\n=== Обход в ширину (BFS) ===\n");
                bfs(root);
            }
            break;
        case 6: {
            if (root == NULL) {
                printf("Дерево пусто.\n");
                break;
            }
            char searchName[NAME_LEN];
            printf("Введите имя для поиска: ");
            scanf("%s", searchName);
            struct TreeNode* found = search(root, searchName);
            if (found != NULL) {
                printf("Контакт найден:\n");
                printf("Имя: %s, Телефон: %s, Email: %s, Дата: %s, Категория: %s\n",
                    found->data.name, found->data.phone, found->data.email,
                    found->data.birthday, found->data.category);
            }
            else {
                printf("Контакт с именем '%s' не найден.\n", searchName);
            }
            break;
        }
        case 7:
            printf("Выход из программы...\n");
            freeTree(root);
            break;
        default:
            printf("Неверный выбор. Попробуйте снова.\n");
        }
    } while (choice != 7);

    return 0;
}