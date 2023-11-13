#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 26
#define MAX_CODE_LENGTH 20
#define MAX_CODE_LENGTH 50

int nodeIndex = 0;
// ���������ڵ�
typedef struct HuffmanNode {
    char data;
    int frequency;
    int index;  // ��ӽڵ����
    struct HuffmanNode* left, * right, * parent;  
} HuffmanNode;


// �����������
typedef struct {
    char character;
    char code[MAX_CODE_LENGTH];
} HuffmanCode;

// ���ڱ����ַ�Ƶ�ʵ�����
int charFrequency[MAX_CHAR];

// ��ʼ���ַ�Ƶ������
void initializeFrequency() {
    for (int i = 0; i < MAX_CHAR; i++) {
        charFrequency[i] = 0;
    }
}

// ͳ���ַ�Ƶ��
void countFrequency(char* str) {
    while (*str) {
        if (*str >= 'a' && *str <= 'z') {
            charFrequency[*str - 'a']++;
        }
        str++;
    }
}

// �������������ڵ�
HuffmanNode* createNode(char data, int frequency) {
    HuffmanNode* node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    node->data = data;
    node->frequency = frequency;
    node->index = ++nodeIndex;
    node->left = node->right = node->parent=NULL;
    return node;
}

// ѡ��������С�Ľڵ�
void selectNodes(HuffmanNode** nodes, int size, int* min1, int* min2) {
    int i;
    *min1 = *min2 = -1;

    for (i = 0; i < size; i++) {
        if (nodes[i] != NULL&&nodes[i]->frequency!=0) {
            if (*min1 == -1 || nodes[i]->frequency < nodes[*min1]->frequency) {
                *min2 = *min1;
                *min1 = i;
            }
            else if (*min2 == -1 || nodes[i]->frequency < nodes[*min2]->frequency) {
                *min2 = i;
            }
        }
    }
}

// ������������
HuffmanNode* buildHuffmanTree(HuffmanNode** nodes, int size) {
    int min1, min2;

    while (size > 1) {
        selectNodes(nodes, size, &min1, &min2);
        HuffmanNode* newNode = createNode('-', nodes[min1]->frequency + nodes[min2]->frequency);
        newNode->left = nodes[min1];
        newNode->right = nodes[min2];

        nodes[min1]->parent = newNode;  // �����������ĸ��ڵ�Ϊ�½ڵ�
        nodes[min2]->parent = newNode;  // �����������ĸ��ڵ�Ϊ�½ڵ�

        nodes[min1] = newNode;
        nodes[min2] = nodes[size - 1];  // �����һ���ڵ��ƶ������滻��λ��
        nodes[size - 1] = NULL;  // �����һ��λ����Ϊ��
        size--;
    }

    return nodes[0];
}

// ���ɹ���������
void generateHuffmanCode(HuffmanNode* root, HuffmanCode* codes, char* currentCode, int depth) {
    if (root->left == NULL && root->right == NULL) {
        currentCode[depth] = '\0';
        int index = root->data - 'a';
        codes[index].character = root->data;
        strcpy_s(codes[index].code, sizeof(codes[index].code), currentCode);
        return;
    }
    currentCode[depth] = '0';
    generateHuffmanCode(root->left, codes, currentCode, depth + 1);
    currentCode[depth] = '1';
    generateHuffmanCode(root->right, codes, currentCode, depth + 1);
    // ��������� null �ַ�
}
// ��ӡ�ַ�Ƶ��
void printCharFrequency(HuffmanCode* codes) {
    for (int i = 0; i < MAX_CHAR; i++) {
        if (charFrequency[i] > 0) {
            printf("%c:%d ", 'a' + i, charFrequency[i]);
        }
    }
    printf("\n");
}
// �������������������Ĵ洢�ṹ
void printHuffmanTreeInOrder(HuffmanNode* root) {
    if (root == NULL) {
        return;
    }

    printHuffmanTreeInOrder(root->left);

    printf("%d || %c || %d ||", root->index, root->data, root->frequency);

    if (root->parent) {
        printf(" %d ||", root->parent->index);
    }
    else {
        printf(" 0 ||");
    }

    if (root->left) {
        printf(" %d ||", root->left->index);
    }
    else {
        printf(" 0 ||");
    }

    if (root->right) {
        printf(" %d\n", root->right->index);
    }
    else {
        printf(" 0\n");
    }

    printHuffmanTreeInOrder(root->right);
}

// ��ӡ����������
void printHuffmanCode(HuffmanCode* codes) {
    for (int i = 0; i < MAX_CHAR; i++) {
        if (charFrequency[i] > 0) {
            printf("%c:%s ", codes[i].character, codes[i].code);
        }
    }
    printf("\n");
}

// ѹ���ַ���
void compressString(char* compressedString,char* str, HuffmanCode* codes) {
    while (*str) {
        if (*str >= 'a' && *str <= 'z') {
            int index = *str - 'a';
            printf("%s", codes[index].code);          
            strcat(compressedString, codes[index].code);
        }
        str++;
    }
    compressedString[strlen(compressedString)] = '\0';
}

// ��ѹ�ַ���
void decompressString(char* str, HuffmanNode* root) {
    HuffmanNode* current = root;

    while (*str) {
        if (*str == '0') {
            current = current->left;
        }
        else if (*str == '1') {
            current = current->right;
        }

        if (current->left == NULL && current->right == NULL) {
            printf("%c", current->data);
            current = root;
        }

        str++;
    }
}


// �ͷŹ����������ڴ�
void freeHuffmanTree(HuffmanNode* root) {
    if (root == NULL) {
        return;
    }
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    free(root);
}

int main() {
    char inputString[100];
    while (1) {
        printf("�������ַ���������0ֹͣ����");
        gets_s(inputString, sizeof(inputString));
        nodeIndex = 0;

        if (inputString[0] == '0') {
            break;
        }

        initializeFrequency();
        countFrequency(inputString);

        // �������������ڵ�����
        HuffmanNode* nodes[MAX_CHAR];
        int size = 0;

        for (int i = 0; i < MAX_CHAR; i++) {
            if (charFrequency[i] > 0) {
                nodes[size++] = createNode('a' + i, charFrequency[i]);
            }
        }

        // ������������
        HuffmanNode* huffmanRoot = buildHuffmanTree(nodes, size);

        // ���ɹ���������
        HuffmanCode codes[MAX_CHAR];
        char currentCode[MAX_CODE_LENGTH];
        generateHuffmanCode(huffmanRoot, codes, currentCode, 0);

        // ����ַ�Ƶ��
        printCharFrequency(codes);

        // ������������ṹ
        printf("���||�ַ�||Ȩֵ||��ĸ�ڵ����||��ڵ����||�ҽڵ����\n");
        printHuffmanTreeInOrder(huffmanRoot);

        // �������������     
        printHuffmanCode(codes);

        // ���ѹ������ַ���
        char compressedString[MAX_CODE_LENGTH];
        printf("ѹ���ַ�����");
        compressString(compressedString,inputString, codes);
        printf("\n");

        // �����ѹ����ַ���
        printf("��ѹ�ַ�����");
        decompressString(compressedString, huffmanRoot);
        printf("\n");

        // �ͷ��ڴ�
        freeHuffmanTree(huffmanRoot);
    }
    return 0;
}
