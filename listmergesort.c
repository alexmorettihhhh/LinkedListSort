#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

Node* create_node(int value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        exit(EXIT_FAILURE);
    }
    new_node->value = value;
    new_node->next = NULL;
    return new_node;
}

void prepend_node(Node** head, int value) {
    Node* new_node = create_node(value);
    new_node->next = *head;
    *head = new_node;
}

void append_node(Node** head, int value) {
    Node* new_node = create_node(value);
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    
    Node* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}

int delete_node(Node** head, int value) {
    if (*head == NULL) {
        return 0;
    }
    
    if ((*head)->value == value) {
        Node* temp = *head;
        *head = (*head)->next;
        free(temp);
        return 1;
    }
    
    Node* current = *head;
    while (current->next != NULL && current->next->value != value) {
        current = current->next;
    }
    
    if (current->next == NULL) {
        return 0;
    }
    
    Node* temp = current->next;
    current->next = current->next->next;
    free(temp);
    return 1;
}

void remove_duplicates(Node* head) {
    if (head == NULL) return;
    
    Node* current = head;
    while (current != NULL) {
        Node* runner = current;
        while (runner->next != NULL) {
            if (runner->next->value == current->value) {
                Node* temp = runner->next;
                runner->next = runner->next->next;
                free(temp);
            } else {
                runner = runner->next;
            }
        }
        current = current->next;
    }
}

Node* find_node(Node* head, int value) {
    Node* current = head;
    while (current != NULL) {
        if (current->value == value) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int get_length(Node* head) {
    int count = 0;
    Node* current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

Node* reverse_list(Node* head) {
    Node* prev = NULL;
    Node* current = head;
    Node* next = NULL;
    
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    
    return prev;
}

Node* merge(Node* left, Node* right) {
    Node dummy;
    Node* tail = &dummy;
    dummy.next = NULL;

    while (left != NULL && right != NULL) {
        if (abs(left->value) <= abs(right->value)) {
            tail->next = left;
            left = left->next;
        } else {
            tail->next = right;
            right = right->next;
        }
        tail = tail->next;
    }

    tail->next = (left != NULL) ? left : right;
    return dummy.next;
}

Node* merge_regular(Node* left, Node* right) {
    Node dummy;
    Node* tail = &dummy;
    dummy.next = NULL;

    while (left != NULL && right != NULL) {
        if (left->value <= right->value) {
            tail->next = left;
            left = left->next;
        } else {
            tail->next = right;
            right = right->next;
        }
        tail = tail->next;
    }

    tail->next = (left != NULL) ? left : right;
    return dummy.next;
}

Node* merge_sort(Node* head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }

    Node* slow = head;
    Node* fast = head->next;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    Node* mid = slow->next;
    slow->next = NULL;

    Node* left = merge_sort(head);
    Node* right = merge_sort(mid);

    return merge(left, right);
}

Node* merge_sort_regular(Node* head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }

    Node* slow = head;
    Node* fast = head->next;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    Node* mid = slow->next;
    slow->next = NULL;

    Node* left = merge_sort_regular(head);
    Node* right = merge_sort_regular(mid);

    return merge_regular(left, right);
}

Node* merge_lists(Node* list1, Node* list2) {
    if (list1 == NULL) return list2;
    if (list2 == NULL) return list1;
    
    Node* result = NULL;
    Node** current = &result;
    
    while (list1 != NULL) {
        append_node(current, list1->value);
        if (*current == NULL) {
            *current = create_node(list1->value);
        } else {
            Node* temp = *current;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = create_node(list1->value);
        }
        list1 = list1->next;
    }
    
    while (list2 != NULL) {
        if (*current == NULL) {
            *current = create_node(list2->value);
        } else {
            Node* temp = *current;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = create_node(list2->value);
        }
        list2 = list2->next;
    }
    
    return result;
}

void rotate_list(Node** head, int k) {
    if (*head == NULL || k <= 0) return;
    
    int length = get_length(*head);
    k = k % length;
    if (k == 0) return;
    
    Node* current = *head;
    for (int i = 1; i < length - k; i++) {
        current = current->next;
    }
    
    Node* new_head = current->next;
    current->next = NULL;
    
    current = new_head;
    while (current->next != NULL) {
        current = current->next;
    }
    
    current->next = *head;
    *head = new_head;
}

void save_to_file(Node* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Ошибка при открытии файла для записи\n");
        return;
    }
    
    Node* current = head;
    while (current != NULL) {
        fprintf(file, "%d ", current->value);
        current = current->next;
    }
    
    fclose(file);
    printf("Список сохранен в файл %s\n", filename);
}

void load_from_file(Node** head, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Ошибка при открытии файла для чтения\n");
        return;
    }
    
    free_list(*head);
    *head = NULL;
    
    int value;
    while (fscanf(file, "%d", &value) == 1) {
        append_node(head, value);
    }
    
    fclose(file);
    printf("Список загружен из файла %s\n", filename);
}

void print_list(Node* head) {
    if (head == NULL) {
        printf("Список пуст\n");
        return;
    }
    
    Node* current = head;
    while (current != NULL) {
        printf("%d ", current->value);
        current = current->next;
    }
    printf("\n");
}

void free_list(Node* head) {
    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }
}

void display_menu() {
    printf("\n--- Меню управления списком ---\n");
    printf("1. Добавить элемент в конец\n");
    printf("2. Добавить элемент в начало\n");
    printf("3. Удалить элемент по значению\n");
    printf("4. Найти элемент\n");
    printf("5. Отсортировать список по модулю\n");
    printf("6. Перевернуть список\n");
    printf("7. Вывести список\n");
    printf("8. Вывести длину списка\n");
    printf("9. Удалить дубликаты\n");
    printf("10. Отсортировать список по значению\n");
    printf("11. Циклически сдвинуть список\n");
    printf("12. Сохранить список в файл\n");
    printf("13. Загрузить список из файла\n");
    printf("14. Создать второй список\n");
    printf("15. Объединить списки\n");
    printf("0. Выход\n");
    printf("Выберите опцию: ");
}

int main() {
    Node* head = NULL;
    Node* second_list = NULL;
    int choice, value, k;
    char filename[100];
    bool has_second_list = false;
    
    while (1) {
        display_menu();
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                printf("Введите значение для добавления в конец: ");
                scanf("%d", &value);
                append_node(&head, value);
                printf("Элемент добавлен\n");
                break;
                
            case 2:
                printf("Введите значение для добавления в начало: ");
                scanf("%d", &value);
                prepend_node(&head, value);
                printf("Элемент добавлен\n");
                break;
                
            case 3:
                printf("Введите значение для удаления: ");
                scanf("%d", &value);
                if (delete_node(&head, value)) {
                    printf("Элемент удален\n");
                } else {
                    printf("Элемент не найден\n");
                }
                break;
                
            case 4:
                printf("Введите значение для поиска: ");
                scanf("%d", &value);
                if (find_node(head, value)) {
                    printf("Элемент найден\n");
                } else {
                    printf("Элемент не найден\n");
                }
                break;
                
            case 5:
                head = merge_sort(head);
                printf("Список отсортирован по модулю\n");
                break;
                
            case 6:
                head = reverse_list(head);
                printf("Список перевернут\n");
                break;
                
            case 7:
                printf("Текущий список: ");
                print_list(head);
                if (has_second_list) {
                    printf("Второй список: ");
                    print_list(second_list);
                }
                break;
                
            case 8:
                printf("Длина списка: %d\n", get_length(head));
                break;
                
            case 9:
                remove_duplicates(head);
                printf("Дубликаты удалены\n");
                break;
                
            case 10:
                head = merge_sort_regular(head);
                printf("Список отсортирован по значению\n");
                break;
                
            case 11:
                printf("Введите количество позиций для сдвига: ");
                scanf("%d", &k);
                rotate_list(&head, k);
                printf("Список сдвинут на %d позиций\n", k);
                break;
                
            case 12:
                printf("Введите имя файла для сохранения: ");
                scanf("%s", filename);
                save_to_file(head, filename);
                break;
                
            case 13:
                printf("Введите имя файла для загрузки: ");
                scanf("%s", filename);
                load_from_file(&head, filename);
                break;
                
            case 14:
                if (has_second_list) {
                    free_list(second_list);
                    second_list = NULL;
                }
                
                printf("Введите количество элементов второго списка: ");
                scanf("%d", &value);
                
                for (int i = 0; i < value; i++) {
                    int element;
                    printf("Введите элемент %d: ", i+1);
                    scanf("%d", &element);
                    append_node(&second_list, element);
                }
                
                has_second_list = true;
                printf("Второй список создан\n");
                break;
                
            case 15:
                if (!has_second_list) {
                    printf("Сначала создайте второй список\n");
                    break;
                }
                
                Node* merged = merge_lists(head, second_list);
                free_list(head);
                head = merged;
                
                printf("Списки объединены\n");
                break;
                
            case 0:
                free_list(head);
                if (has_second_list) {
                    free_list(second_list);
                }
                printf("Программа завершена\n");
                return 0;
                
            default:
                printf("Неверный выбор, попробуйте снова\n");
        }
    }

    return 0;
}
