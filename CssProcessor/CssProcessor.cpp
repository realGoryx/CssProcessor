#include <iostream>
#include <cstdlib>

const int T = 8;

struct Attribute {
    char* name;
    char* value;
};

struct Selector {
    char* name;
    int number_of_attributes;
    Attribute* attributes;
};

struct CssBlock {
    int number_of_selectors;
    Selector* selectors;
};

struct CssNode {
    CssBlock blocks[T];
    int number_of_blocks;
    struct CssNode* prev;
    struct CssNode* next;
};

struct CssList {
    CssNode* head;
    CssNode* tail;
};

bool isWhitespace(char c) {
    return (c == ' ' || c == '\t' || c == '\n');
}

char* trim(char* word) {
    while (isWhitespace(*word)) {
        word++;

    }

    if (*word == '\0') {
        return word;

    }

    char* end = word;
    char* last_non_space = word;
    while (*end != '\0') {
        if (!isWhitespace(*end)) {
            last_non_space = end;

        }
        end++;

    }
    *(last_non_space + 1) = '\0';

    return word;
}

bool wordsEquality(const char* word1, const char* word2, size_t num_chars = SIZE_MAX) {
    size_t count = 0;
    while (*word1 && *word2 && count < num_chars) {
        if (*word1 != *word2) {
            return false;

        }
        ++word1;
        ++word2;
        ++count;

    }
    if (count == num_chars) {
        return true;

    }
    return *word1 == *word2;
}

size_t wordLength(const char* word) {
    size_t length = 0;
    while (word[length] != '\0') {
        ++length;

    }
    return length;
}

char* findCharInWord(char* word, char c) {
    while (*word != '\0') {
        if (*word == c) {
            return word;

        }
        ++word;

    }
    return nullptr;
}

char* copyWord(char* WordToCopy) {
    WordToCopy = trim(WordToCopy);
    size_t length = wordLength(WordToCopy);
    char* dest = static_cast<char*>(malloc(length + 1));
    if (dest != nullptr) {
        memcpy(dest, WordToCopy, length + 1);
    }
    return dest;
}

void freeCssList(CssList* list) {
    CssNode* current = list->head;
    CssNode* next;

    while (current != nullptr) {
        next = current->next;
        for (int i = 0; i < current->number_of_blocks; i++) {
            for (int j = 0; j < current->blocks[i].number_of_selectors; j++) {
                free(current->blocks[i].selectors[j].name);
                for (int k = 0; k < current->blocks[i].selectors[j].number_of_attributes; k++) {
                    free(current->blocks[i].selectors[j].attributes[k].name);
                    free(current->blocks[i].selectors[j].attributes[k].value);

                }
                free(current->blocks[i].selectors[j].attributes);

            }
            free(current->blocks[i].selectors);

        }
        free(current);
        current = next;

    }
    free(list);
}

int getNumberSection(CssList* list) {
    CssNode* current = list->head;
    CssNode* next;
    int numberSection = 0;

    while (current != nullptr) {
        next = current->next;
        numberSection += current->number_of_blocks;
        current = next;

    }
    return numberSection;
}

int getNumberOfSelectorsByIndex(CssList* list, int index) {
    CssNode* current = list->head;
    CssNode* next;
    int numberSelectors = -1;

    //    int originalIndex = index + 1;
    //
    //    int nodeIndex = originalIndex / T;
    //    int blockIndex = originalIndex % T;
    //    if(blockIndex > 0 && nodeIndex > 0){
    //        nodeIndex++;
    //    }
    //    if(blockIndex == 0  && nodeIndex > 0){
    //        blockIndex = 7;
    //    }
    //    if(blockIndex > 0){
    //        blockIndex--; // because array from 0
    //    }
    //
    //    int counter = 0;

    while (current != nullptr) {
        next = current->next;
        if (index < T) {
            if (index >= 0 && index < current->number_of_blocks) {
                //                if (numberSelectors < 0) {
                //                    numberSelectors = 0;
                //                }
                numberSelectors = current->blocks[index].number_of_selectors;

            }

        }
        current = next;
        index -= T;

    }
    return numberSelectors;
}

int getNumberOfAttributesInBlockByIndex(CssList* list, int index) {
    CssNode* current = list->head;
    CssNode* next;
    int numberOfAttributes = -1;

    while (current != nullptr) {
        next = current->next;
        if (index < T) {
            if (index >= 0 && index < current->number_of_blocks) {
                if (numberOfAttributes < 0) {
                    numberOfAttributes = 0;

                }
                int lastSelectorIndex = current->blocks[index].number_of_selectors - 1;
                numberOfAttributes += current->blocks[index].selectors[lastSelectorIndex].number_of_attributes;

            }

        }
        current = next;
        index -= T;

    }
    return numberOfAttributes;
}

char* getSelectorNameByBlockIndex(CssList* list, int blockIndex, int selectorIndex) {
    CssNode* current = list->head;
    CssNode* next;
    char* selectorName = nullptr;

    while (current != nullptr) {
        next = current->next;
        if (blockIndex < T) {
            if (blockIndex >= 0 && blockIndex < current->number_of_blocks) {
                if (selectorIndex >= 0 && selectorIndex < current->blocks[blockIndex].number_of_selectors) {
                    selectorName = current->blocks[blockIndex].selectors[selectorIndex].name;

                }

            }

        }
        current = next;
        blockIndex -= T;

    }
    return selectorName;
}

char* getAttributeValueByBlockIndex(CssList* list, int blockIndex, char* attrName) {
    CssNode* current = list->head;
    CssNode* next;
    char* attributeValue = nullptr;

    while (current != nullptr) {
        next = current->next;
        if (blockIndex < T) {
            if (blockIndex >= 0 && blockIndex < current->number_of_blocks) {
                int lastSelectorIndex = current->blocks[blockIndex].number_of_selectors - 1;
                for (int i = 0;
                    i < current->blocks[blockIndex].selectors[lastSelectorIndex].number_of_attributes; ++i) {
                    if (wordsEquality(attrName,
                        current->blocks[blockIndex].selectors[lastSelectorIndex].attributes[i].name) ==
                        1) {
                        attributeValue = current->blocks[blockIndex].selectors[lastSelectorIndex].attributes[i].value;

                    }

                }

            }

        }
        current = next;
        blockIndex -= T;

    }
    return attributeValue;
}

void printWholeList(CssList* list);

int getTotalNumberOfAttribute(CssList* list, char* attrName) {
    CssNode* current = list->head;
    CssNode* next;
    int totalNumberOfAttribute = 0;

    while (current != nullptr) {
        next = current->next;
        for (int i = 0; i < current->number_of_blocks; ++i) {
            int lastSelectorIndex = current->blocks[i].number_of_selectors - 1;
            for (int j = 0; j < current->blocks[i].selectors[lastSelectorIndex].number_of_attributes; ++j) {
                if (wordsEquality(attrName, current->blocks[i].selectors[lastSelectorIndex].attributes[j].name) == 1) {
                    totalNumberOfAttribute += 1;

                }

            }

        }
        current = next;

    }
    return totalNumberOfAttribute;
}

int getTotalNumberOfSelector(CssList* list, char* selectorName) {
    CssNode* current = list->head;
    CssNode* next;
    int totalNumberOfSelector = 0;

    while (current != nullptr) {
        next = current->next;
        for (int i = 0; i < current->number_of_blocks; ++i) {
            for (int j = 0; j < current->blocks[i].number_of_selectors; ++j) {
                if (wordsEquality(selectorName, current->blocks[i].selectors[j].name) == 1) {
                    totalNumberOfSelector += 1;

                }

            }

        }
        current = next;

    }
    return totalNumberOfSelector;
}

char* getAttributeValueForSelectorName(CssList* list, char* selectorName, char* attrName) {
    CssNode* current = list->head;
    CssNode* next;
    char* attributeValue = nullptr;

    while (current != nullptr) {
        next = current->next;
        for (int i = 0; i < current->number_of_blocks; ++i) {
            // because of only last selector keeps the attributes for a block
            // check if it's needed to iterate over all selector to find the name
            // or it's enough to check only last ones from all blocks
            bool selectorExists = false;
            int lastSelectorIndex = current->blocks[i].number_of_selectors - 1;
            for (int j = 0; j < current->blocks[i].number_of_selectors; ++j) {
                if (wordsEquality(selectorName, current->blocks[i].selectors[j].name) == 1) {
                    selectorExists = true;
                    break;

                }

            }
            if (selectorExists) {
                for (int j = 0; j < current->blocks[i].selectors[lastSelectorIndex].number_of_attributes; ++j) {
                    if (wordsEquality(attrName, current->blocks[i].selectors[lastSelectorIndex].attributes[j].name) ==
                        1) {
                        attributeValue = current->blocks[i].selectors[lastSelectorIndex].attributes[j].value; // possible of wordCopy

                    }

                }

            }

        }
        current = next;

    }
    return attributeValue;
}

void addCssBlock(CssList* list, CssBlock block) {
    if (list->tail == nullptr || list->tail->number_of_blocks == T) {
        auto new_node = static_cast<CssNode*>(malloc(sizeof(CssNode)));
        new_node->number_of_blocks = 0;
        new_node->prev = list->tail;
        new_node->next = nullptr;
        if (list->tail != nullptr) {
            list->tail->next = new_node;

        }
        else {
            list->head = new_node;

        }
        list->tail = new_node;

    }

    list->tail->blocks[list->tail->number_of_blocks++] = block;
}

void deleteCssBlock(CssList* list, int index) {
    int count = 0;
    CssNode* current = list->head;

    while (current != nullptr) {
        for (int i = 0; i < current->number_of_blocks; i++) {
            if (count == index) {
                for (int j = 0; j < current->blocks[i].number_of_selectors; j++) {
                    free(current->blocks[i].selectors[j].name);
                    for (int k = 0; k < current->blocks[i].selectors[j].number_of_attributes; k++) {
                        free(current->blocks[i].selectors[j].attributes[k].name);
                        free(current->blocks[i].selectors[j].attributes[k].value);

                    }
                    free(current->blocks[i].selectors[j].attributes);

                }
                free(current->blocks[i].selectors);

                current->number_of_blocks--;

                if (current->number_of_blocks > i) {
                    current->blocks[i] = current->blocks[current->number_of_blocks];

                }

                if (current->number_of_blocks == 0) {
                    if (current->prev != nullptr) {
                        current->prev->next = current->next;

                    }
                    else {
                        list->head = current->next;

                    }

                    if (current->next != nullptr) {
                        current->next->prev = current->prev;

                    }
                    else {
                        list->tail = current->prev;

                    }

                    free(current);

                }
                return;

            }
            count++;

        }
        current = current->next;

    }
}

void deleteAttributeFromBlock(CssList* list, int block_index, const char* attribute_name) {
    int count = 0;
    CssNode* current = list->head;

    while (current != nullptr) {
        for (int i = 0; i < current->number_of_blocks; i++) {
            if (count == block_index)
                for (int j = 0; j < current->blocks[i].number_of_selectors; j++) {
                    Selector* selector = &current->blocks[i].selectors[j];
                    for (int k = 0; k < selector->number_of_attributes; k++) {
                        if (wordsEquality(selector->attributes[k].name, attribute_name) == 1) {
                            free(selector->attributes[k].name);
                            free(selector->attributes[k].value);

                            selector->number_of_attributes--;

                            if (selector->number_of_attributes > k) {
                                selector->attributes[k] = selector->attributes[selector->number_of_attributes];

                            }

                            if (selector->number_of_attributes == 0) {
                                deleteCssBlock(list, block_index);

                            }
                            std::cout << block_index + 1 << ",D," << attribute_name << " == deleted\n";
                            return;

                        }

                    }

                }

        }
        count++;

    }
    current = current->next;
}

CssBlock parseCssSection(const char* input, CssList* list, bool* blockAdded) {
    CssBlock block;
    block.number_of_selectors = 0;
    block.selectors = nullptr;
    char selector_text[1024];
    char attribute_text[1024];
    char attribute_name[256];
    char attribute_value[256];
    const char* cursor = input;
    if (sscanf(cursor, "%[^{]{%[^}]", selector_text, attribute_text) == 2) {
        cursor += wordLength(selector_text) + wordLength(attribute_text) + 2;
        char* selector_start = selector_text;
        char* selector_end;
        while ((selector_end = findCharInWord(selector_start, ',')) != nullptr) {
            *selector_end = '\0';
            block.selectors = static_cast<Selector*>(realloc(block.selectors,
                sizeof(Selector) * (block.number_of_selectors + 1)));
            block.selectors[block.number_of_selectors].name = copyWord(selector_start);
            block.selectors[block.number_of_selectors].number_of_attributes = 0;
            block.selectors[block.number_of_selectors].attributes = nullptr;
            block.number_of_selectors++;
            selector_start = selector_end + 1;

        }
        block.selectors = static_cast<Selector*>(realloc(block.selectors,
            sizeof(Selector) * (block.number_of_selectors + 1)));
        block.selectors[block.number_of_selectors].name = copyWord(selector_start);
        block.selectors[block.number_of_selectors].number_of_attributes = 0;
        block.selectors[block.number_of_selectors].attributes = nullptr;
        block.number_of_selectors++;
        const char* attribute_cursor = attribute_text;
        while (sscanf(attribute_cursor, "%[^:]:%[^;];", attribute_name, attribute_value) == 2) {
            attribute_cursor += wordLength(attribute_name) + wordLength(attribute_value) + 2;
            int last_selector_index = block.number_of_selectors - 1;
            int attribute_index = block.selectors[last_selector_index].number_of_attributes;
            bool attribute_found = false;
            for (int i = 0; i < attribute_index; i++) {
                if (wordsEquality(block.selectors[last_selector_index].attributes[i].name, attribute_name) == 1) {
                    free(block.selectors[last_selector_index].attributes[i].value);
                    block.selectors[last_selector_index].attributes[i].value = copyWord(attribute_value);
                    attribute_found = true;
                    break;

                }

            }
            if (!attribute_found) {
                block.selectors[last_selector_index].attributes = static_cast<Attribute*>(realloc(
                    block.selectors[last_selector_index].attributes, sizeof(Attribute) * (attribute_index + 1)));
                block.selectors[last_selector_index].attributes[attribute_index].name = copyWord(attribute_name);
                block.selectors[last_selector_index].attributes[attribute_index].value = copyWord(attribute_value);
                block.selectors[last_selector_index].number_of_attributes++;

            }

        }
        if (*cursor == '}') {
            cursor++;

        }
        *blockAdded = true;

    }
    else if (wordsEquality(cursor, "{\n", 2) || wordsEquality(cursor, "}\n", 2)) {
        return CssBlock{
        };

    }
    else if (wordsEquality(cursor, "\n", 1) || wordsEquality(cursor, "}\t\t", 3)
        || wordsEquality(cursor, "\t}", 2) || wordsEquality(cursor, "\t\n", 2)) {
        return CssBlock{
        };

    }
    else if (sscanf(cursor, "%[^{\t\n]", selector_text) == 1) {
        char* selector_start = selector_text;
        char* selector_end;
        while ((selector_end = findCharInWord(selector_start, ',')) != nullptr) {
            *selector_end = '\0';
            block.selectors = static_cast<Selector*>(realloc(block.selectors,
                sizeof(Selector) * (block.number_of_selectors + 1)));
            block.selectors[block.number_of_selectors].name = copyWord(selector_start);
            block.selectors[block.number_of_selectors].number_of_attributes = 0;
            block.selectors[block.number_of_selectors].attributes = nullptr;
            block.number_of_selectors++;
            selector_start = selector_end + 1;

        }
        block.selectors = static_cast<Selector*>(realloc(block.selectors,
            sizeof(Selector) * (block.number_of_selectors + 1)));
        block.selectors[block.number_of_selectors].name = copyWord(selector_start);
        block.selectors[block.number_of_selectors].number_of_attributes = 0;
        block.selectors[block.number_of_selectors].attributes = nullptr;
        block.number_of_selectors++;
        *blockAdded = true;

    }
    else if (sscanf(cursor, "%[^:]:%[^;];", attribute_name, attribute_value)) {
        int last_block_index = list->tail->number_of_blocks - 1;
        auto last_block = &(list->tail->blocks[last_block_index]);
        int last_selector_index = last_block->number_of_selectors - 1;
        int attribute_index = last_block->selectors[last_selector_index].number_of_attributes;
        bool attribute_found = false;
        for (int i = 0; i < attribute_index; i++) {
            if (wordsEquality(last_block->selectors[last_selector_index].attributes[i].name, trim(attribute_name)) == 1) {
                free(last_block->selectors[last_selector_index].attributes[i].value);
                last_block->selectors[last_selector_index].attributes[i].value = copyWord(attribute_value);
                attribute_found = true;
                break;

            }

        }
        if (!attribute_found) {
            last_block->selectors[last_selector_index].attributes = static_cast<Attribute*>(realloc(
                last_block->selectors[last_selector_index].attributes, sizeof(Attribute) * (attribute_index + 1)));
            last_block->selectors[last_selector_index].attributes[attribute_index].name = copyWord(attribute_name);
            last_block->selectors[last_selector_index].attributes[attribute_index].value = copyWord(attribute_value);
            last_block->selectors[last_selector_index].number_of_attributes++;

        }
        *blockAdded = false;

    }
    return block;
}

void runCommand(CssList* list, const char* command) {
    char action;
    int index;
    int sub_index;
    char identifier[256];
    char token[256];

    //printWholeList(list);

    if (sscanf(command, "%c", identifier) == 1 && identifier[0] == '?') {
        std::cout << "? == " << getNumberSection(list) << "\n";

    }
    else if (sscanf(command, "%d,S,%c", &index, token) == 2 && token[0] == '?') {
        int numberOFSelectors = getNumberOfSelectorsByIndex(list, index - 1);
        if (numberOFSelectors == -1) {
            return;

        }
        std::cout << index << ",S,? == " << numberOFSelectors << "\n";

    }
    else if (sscanf(command, "%d,A,%c", &index, token) == 2 && token[0] == '?') {
        int numberOfAttributes = getNumberOfAttributesInBlockByIndex(list, index - 1);
        if (numberOfAttributes == -1) {
            return;

        }
        std::cout << index << ",A,? == " << numberOfAttributes << "\n";

    }
    else if (sscanf(command, "%d,S,%d", &index, &sub_index) == 2) {
        char* selectorName = getSelectorNameByBlockIndex(list, index - 1, sub_index - 1);
        if (selectorName == nullptr) {
            return;

        }
        std::cout << index << ",S," << sub_index << " == " << selectorName << "\n";

    }
    else if (sscanf(command, "%d,A,%[^EOF]", &index, identifier) == 2) {
        char* attributeValue = getAttributeValueByBlockIndex(list, index - 1, identifier);
        if (attributeValue == nullptr) {
            return;

        }
        std::cout << index << ",A," << identifier << " == " << attributeValue << "\n";

    }
    else if (sscanf(command, "%[^,],A,%c", identifier, token) == 2 && token[0] == '?') {
        int totalNumberOfAttribute = getTotalNumberOfAttribute(list, identifier);
        std::cout << identifier << ",A,? == " << totalNumberOfAttribute << "\n";

    }
    else if (sscanf(command, "%[^,],S,%c", identifier, token) == 2 && token[0] == '?') {
        int totalNumberOfSelector = getTotalNumberOfSelector(list, identifier);
        std::cout << identifier << ",S,? == " << totalNumberOfSelector << "\n";

    }
    else if (sscanf(command, "%[^,],E,%[^EOF]", identifier, token) == 2) {
        char* atrributeValueForSelector = getAttributeValueForSelectorName(list, identifier, token);
        if (atrributeValueForSelector != nullptr) {
            std::cout << identifier << ",E," << token << " == " << atrributeValueForSelector << "\n";

        }

    }
    else if (sscanf(command, "%d,D,%c", &index, token) == 2 && token[0] == '*') {
        deleteCssBlock(list, index - 1);
        std::cout << index << ",D,* == " << "deleted\n";

    }
    else if (sscanf(command, "%d,D,%[^EOF]", &index, identifier) == 2) {
        deleteAttributeFromBlock(list, index - 1, identifier);

    }
}

void printWholeList(CssList* list) {
    CssNode* current = list->head;
    CssNode* next;

    std::cout << "whole list:" << "\n";
    while (current != nullptr) {
        next = current->next;
        auto blocksLength = current->number_of_blocks;
        for (int i = 0; i < blocksLength; ++i) {
            for (int j = 0; j < current->blocks[i].number_of_selectors; ++j) {
                std::cout << "Selector name:" << current->blocks[i].selectors[j].name << "\n";
                for (int k = 0; k < current->blocks[i].selectors[j].number_of_attributes; ++k) {
                    std::cout << "Attribute: " << "\t" << current->blocks[i].selectors[j].attributes[k].name << " : "
                        << current->blocks[i].selectors[j].attributes[k].value << "\n";

                }

            }

        }
        current = next;

    }
}

int main() {
    CssList* css_list = static_cast<CssList*>(malloc(sizeof(CssList)));
    css_list->head = nullptr;
    css_list->tail = nullptr;
    char input[1024];
    bool blockAdded = false;

    while (fgets(input, sizeof(input), stdin) != nullptr) {
        if (wordsEquality(input, "****", 4) == 1) {
            continue;

        }
        else if (wordsEquality(input, "????", 4) == 1) {
            //printWholeList(css_list);
            while (fgets(input, sizeof(input), stdin) != nullptr && wordsEquality(input, "****", 4) != 1) {
                runCommand(css_list, trim(input));

            }

        }
        else {
            CssBlock block = parseCssSection(input, css_list, &blockAdded);
            if (blockAdded) {
                addCssBlock(css_list, block);
                blockAdded = false;

            }

        }

    }

    freeCssList(css_list);
    return 0;
}