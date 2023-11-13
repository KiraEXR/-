#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 26
#define MAX_CODE_LENGTH 20
#define MAX_CODE_LENGTH 50

int nodeIndex = 0;
// 哈夫曼树节点
typedef struct HuffmanNode {
    char data;
    int frequency;
    int index;  // 添加节点序号
    struct HuffmanNode* left, * right, * parent;  
} HuffmanNode;


// 哈夫曼编码表
typedef struct {
    char character;
    char code[MAX_CODE_LENGTH];
} HuffmanCode;

// 用于保存字符频率的数组
int charFrequency[MAX_CHAR];

// 初始化字符频率数组
void initializeFrequency() {
    for (int i = 0; i < MAX_CHAR; i++) {
        charFrequency[i] = 0;
    }
}

// 统计字符频率
void countFrequency(char* str) {
    while (*str) {
        if (*str >= 'a' && *str <= 'z') {
            charFrequency[*str - 'a']++;
        }
        str++;
    }
}

// 创建哈夫曼树节点
HuffmanNode* createNode(char data, int frequency) {
    HuffmanNode* node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    node->data = data;
    node->frequency = frequency;
    node->index = ++nodeIndex;
    node->left = node->right = node->parent=NULL;
    return node;
}

// 选择两个最小的节点
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

// 构建哈夫曼树
HuffmanNode* buildHuffmanTree(HuffmanNode** nodes, int size) {
    int min1, min2;

    while (size > 1) {
        selectNodes(nodes, size, &min1, &min2);
        HuffmanNode* newNode = createNode('-', nodes[min1]->frequency + nodes[min2]->frequency);
        newNode->left = nodes[min1];
        newNode->right = nodes[min2];

        nodes[min1]->parent = newNode;  // 设置左子树的父节点为新节点
        nodes[min2]->parent = newNode;  // 设置右子树的父节点为新节点

        nodes[min1] = newNode;
        nodes[min2] = nodes[size - 1];  // 将最后一个节点移动到被替换的位置
        nodes[size - 1] = NULL;  // 将最后一个位置置为空
        size--;
    }

    return nodes[0];
}

// 生成哈夫曼编码
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
    // 在最后设置 null 字符
}
// 打印字符频率
void printCharFrequency(HuffmanCode* codes) {
    for (int i = 0; i < MAX_CHAR; i++) {
        if (charFrequency[i] > 0) {
            printf("%c:%d ", 'a' + i, charFrequency[i]);
        }
    }
    printf("\n");
}
// 中序遍历输出哈夫曼树的存储结构
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

// 打印哈夫曼编码
void printHuffmanCode(HuffmanCode* codes) {
    for (int i = 0; i < MAX_CHAR; i++) {
        if (charFrequency[i] > 0) {
            printf("%c:%s ", codes[i].character, codes[i].code);
        }
    }
    printf("\n");
}

// 压缩字符串
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

// 解压字符串
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


// 释放哈夫曼树的内存
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
        printf("请输入字符串（输入0停止）：");
        gets_s(inputString, sizeof(inputString));
        nodeIndex = 0;

        if (inputString[0] == '0') {
            break;
        }

        initializeFrequency();
        countFrequency(inputString);

        // 创建哈夫曼树节点数组
        HuffmanNode* nodes[MAX_CHAR];
        int size = 0;

        for (int i = 0; i < MAX_CHAR; i++) {
            if (charFrequency[i] > 0) {
                nodes[size++] = createNode('a' + i, charFrequency[i]);
            }
        }

        // 构建哈夫曼树
        HuffmanNode* huffmanRoot = buildHuffmanTree(nodes, size);

        // 生成哈夫曼编码
        HuffmanCode codes[MAX_CHAR];
        char currentCode[MAX_CODE_LENGTH];
        generateHuffmanCode(huffmanRoot, codes, currentCode, 0);

        // 输出字符频率
        printCharFrequency(codes);

        // 输出哈夫曼树结构
        printf("序号||字符||权值||父母节点序号||左节点序号||右节点序号\n");
        printHuffmanTreeInOrder(huffmanRoot);

        // 输出哈夫曼编码     
        printHuffmanCode(codes);

        // 输出压缩后的字符串
        char compressedString[MAX_CODE_LENGTH];
        printf("压缩字符串：");
        compressString(compressedString,inputString, codes);
        printf("\n");

        // 输出解压后的字符串
        printf("解压字符串：");
        decompressString(compressedString, huffmanRoot);
        printf("\n");

        // 释放内存
        freeHuffmanTree(huffmanRoot);
    }
    return 0;
}
