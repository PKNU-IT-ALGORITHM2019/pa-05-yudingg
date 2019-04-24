#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX 1000

typedef struct Tree {
	char word[MAX];
	char c[MAX];
	char meaning[MAX];
	Tree *left;
	Tree *right;
	Tree *p;
}Tree;

Tree *root = NULL;

int n = 0;

Tree* find(Tree *r, char key[]);
void add(Tree *root, Tree*client);
void create();
void inorder(Tree *r);
void remove(Tree *r);
void load();
Tree* successor(Tree* r);
int main() {

	load();

	char command[MAX],save[MAX], removeWord[MAX];
	Tree *p;
	while (1) {
		printf("$ ");
		scanf(" %s", command);

		if (strcmp(command, "size") == 0) printf("%d\n", n);
		else if (strcmp(command, "find") == 0) {
			scanf("%s", save);
			p = find(root, save);
			if(p==NULL) printf("Not found.\n");
			else printf("%s\n", p->meaning);
		}
		else if (strcmp(command, "add") == 0) create();
		else if (strcmp(command, "delete") == 0) {
			scanf("%s", save);
			p = find(root, save);
			if (p == NULL) printf("Not found.\n");
			else {
				remove(p);
				printf("Deleted successfully.\n");
			}
		}
		else if (strcmp(command, "deleteall") == 0) {
			int removeNum = 0;
			scanf("%s", save);
			FILE*fp2 = fopen(save, "r");
			while (fscanf(fp2, "%s", removeWord) != EOF) {
				p = find(root, removeWord);
				remove(p);
				removeNum++;
			}
			fclose(fp2);
			printf("%d words were deleted successfully.\n", removeNum);
		}
		else if (strcmp(command, "exit") == 0) break;
	}

	inorder(root);

	return 0;

}

void inorder(Tree *r) {

	if (r != NULL) {
		inorder(r->left);
		printf("%s\n", r->word);
		inorder(r->right);
	}
}

void load() {

	char check;
	char buf[MAX], command[MAX];
	char *token;
	FILE*fp = fopen("shuffled_dict.txt", "r");

	while (!(feof(fp))) {
		if (fgets(buf, MAX, fp) <= 0 || strcmp(buf, "\n") == 0)
			continue;
		n++;
		Tree* node = (Tree*)malloc(sizeof(Tree));
		token = strtok(buf, "(");
		int i = 0;
		if (!isalpha(buf[0])) {			//단어의 맨 앞이 알파벳이 아닌 것 제거
			while (1) {
				buf[i] = buf[i + 1];
				if (buf[i + 1]==' ') {	//공백 제거
					buf[i] = '\0';
					break;
				}
				i++;
			}
		}
		else {							//단어 공백 제거
			while (buf[i] != ' ') i++;
			buf[i] = '\0';
		}
		strcpy(node->word, token);
		token = strtok(NULL, ")");
		if (token == NULL) strcpy(node->c, "\0");
		else strcpy(node->c, token);
		token = strtok(NULL, "\n");
		if (token == NULL) {
			strcpy(node->meaning, node->c);
			strcpy(node->c, "\0");
		}
		else strcpy(node->meaning, token);
		node->left = NULL;
		node->right = NULL;
		node->p = NULL;

		if (root == NULL) {
			node->p = NULL;
			root = node;
		}
		else add(root, node);
	}
	fclose(fp);

}

void add(Tree *root, Tree*client) {

	if (strcmp(root->word, client->word) < 0) {
		if (root->right != NULL)
			add(root->right, client);
		else {
			root->right = client;
			client->p = root;
		}
	}
	else if (strcmp(root->word, client->word) > 0) {
		if (root->left != NULL)
			add(root->left, client);
		else {
			root->left = client;
			client->p = root;
		}
	}
}

Tree* find(Tree *r, char key[]) {

	if (strcmp(r->word, key) == 0)
		return r;
	else if (strcmp(r->word, key) < 0) {
		if (r->right == NULL) return NULL;
		return find(r->right, key);
	}
	else {
		if (r->left == NULL) return NULL;
		return find(r->left, key);
	}

}

void create() {

	char s[MAX];
	int i;
	Tree *p = (Tree*)malloc(sizeof(Tree));

	printf("word: ");
	fgetc(stdin);
	fgets(s, MAX, stdin);
	for (i = 0; s[i] != '\n'; i++);	//'\n' 찾아서 지우기
	s[i] = '\0';
	strcpy(p->word, s);

	printf("class: ");
	fgets(s, MAX, stdin);
	if (s == NULL) strcpy(p->c, "\0");
	else {
		for (i = 0; s[i] != '\n'; i++);
		s[i] = '\0';
		strcpy(p->c, s);
	}
	printf("meaing: ");
	fgets(s, MAX, stdin);
	for (i = 0; s[i] != '\n'; i++);
	s[i] = '\0';
	strcpy(p->meaning, s);

	p->left = NULL;
	p->right = NULL;
	p->p = NULL;
	add(root, p);

}

void remove(Tree *p) {

	if (p == NULL)
		printf("error.\n");
	else if (p->left == NULL&&p->right == NULL) {
		if (strcmp(p->p->word, p->word) < 0)
			p->p->right = NULL;
		else p->p->left = NULL;
	}
	else if (p->left == NULL&&p->right!=NULL) {
		if (strcmp(p->p->word, p->word) < 0)
			p->p->right = p->right;
		else p->p->left = p->right;
	}
	else if (p->left != NULL&&p->right == NULL) {
		if (strcmp(p->p->word, p->word) < 0)
			p->p->right = p->left;
		else p->p->left = p->left;
	}
	else {
		Tree *s = successor(p->right);
		strcpy(p->word, s->word);
		strcpy(p->c, s->c);
		strcpy(p->meaning, s->meaning);
		remove(s);
	}

}

Tree* successor(Tree* r) {

	if (r->left == NULL)
		return r;
	else return successor(r->left);

}