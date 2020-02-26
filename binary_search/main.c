/** Implementacja drzewa binarych poszukiwań
 *  AHE moduł #4 ANSI C
 */
#include <stdio.h>
#include <stdlib.h>


/** Podstawowa struktura definiująca węzeł drzewa
 * data - przechowuje dane do porównania
 * left - wskaźnik do lewego dziecka drzewa
 * right - wskaźnik do prawego dziecka drzewa
 */
typedef struct node_s
{
    int data;
    struct node_s* left;
    struct node_s* right;
} node;

/** Funkcja tworzy pusty węzeł alokując pamięć na stosie
	inicjalizując dane przekazane jako argument data
	wskaźnik na lewe i prawe dziecko ustawia na NULL (pusty)
*/
node* create_node(int data)
{
    // Zaalokuj pamięć na strukturę na stercie
    node *new_node = (node*)malloc(sizeof(node));
    // Jeśli nie można zaalokować pamięci to zwróć NULL
    if(new_node == NULL)
    {
        return NULL;
    }
    // Wypełnij pole dana wartościa przekazaną jako argument
    new_node->data = data;
    // Ustaw lewe i prawe dziecko jako puste
    new_node->left = NULL;
    new_node->right = NULL;
    // Zwróć wskaźnik na utworzony element
    return new_node;
}

/** Funkcja porównuje dwie liczby całkowite
	jeśli paramer left jest większy od right zwraca wartość dodatnią
    jeśli parametre left jest mniejszy od right zwraca wartość ujemną
    jeśli oba elementy są takie same zwraca wartośc równą 0
*/
int compare(int left,int right)
{
    if(left > right)
        return 1;
    if(left < right)
        return -1;
    return 0;
}

/** Funkcja wstawia nowy element do drzewa binarnych poszukiwań wstawiając
    go na odpowiednie miejsce zgodnie z definicją i regułami drzewa BST
    jako argument pierwszy root przyjmuje wskaźnik do korzenia, natomiast jako
    argument drugi przyjmuje wartosć jaką chcemy wstawić.
	Jeśli drzewo binarne jest puste wystarczy utworzyć nowy element, natomiast
    w przeciwnym wypadku rozpoczynamy od węzła głównego.
    Znajdujemy właściwą pozycję dla nowego węzła porównując klucz nowego
    węzła z kluczem węzła głównego. Jeśli klucz nowego węzła jest mniejszy
	niż klucz węzła głównego, przechodzimy do lewego poddrzewa. Jeśli klucz
	nowego węzła jest większy niż klucz węzła głównego, przechodzimy do prawego poddrzewa.
	Robimy ten krok rekurencyjnie, aż znajdziemy poprawną pozycję w drzewie,
	aby wstawić nowy węzeł.
*/

node* insert_node(node *root, int data)
{
    if(root == NULL)
    {
        root = create_node(data);
    }
    else
    {
        int is_left  = 0;
        int r        = 0;
        node* curr_node = root;
        node* prev_node   = NULL;
        while(curr_node)
        {
            r = compare(data,curr_node->data);
            prev_node = curr_node;
            if(r < 0)
            {
                is_left = 1;
                curr_node = curr_node->left;
            }
            else if(r > 0)
            {
                is_left = 0;
                curr_node = curr_node->right;
            }
        }
        if(is_left)
            prev_node->left = create_node(data);
        else
            prev_node->right = create_node(data);
    }
    return root;
}

/** Funkcja usuwa dany węzeł z drzewa jako argument root przyjmuje
	wskaźnik do korzenia drzewa natomiast jako argument data przyjmuje
	wartość w drzewie jaką chcemy usunąć.
	Usunięcie istniejącego węzła w drzewie wyszukiwania binarnego jest nieco
	bardziej skomplikowane. Mamy trzy przypadki, które powinniśmy wziąć pod uwagę:
	Przypadek 1. Dla węzła nie zawiarającego liści, czyli
	Węzeł, który nie ma elementów podrzędnych, możemy go zwyczajnie usunąć bez
    żadnych konsekwencji dla struktry drzewa.
	Przypadek 2. Jesli chcemy usunąć węzeł, który ma jeden węzeł potomny,
	zamieniamy go z węzłem potomnym a następnie usuwamy.
	Przypadek 3 (Najtrudniejszy). Aby usunąć węzeł, który ma dwa węzły potomne,,
	znajdujemy jego następny węzeł, który jest następnym węzłem w kolejności
	uporządkowania drzewa i zastępujemy go węzłem o odpowiadającej kolejności.
*/

node* delete_node(node* root, int data)
{
    //Jeśli korzeń nie ustawiony to kończ z błędem
    if(root == NULL)
        return NULL;
    node *curr_node;
    // Porównaj zawartość węzła z wartością do usunięcia
    int r = compare(data,root->data);
    //Jeśli mniejszy przejdz rekursywnie do lewego poddrzewa
    if( r < 0)
        root->left = delete_node( root->left, data);
        // Jeśli większy przejdz rekursywnie do prawego poddrzewa
    else if( r > 0 )
        root->right = delete_node(root->right,data);
        // Jeśli równy oznacza to że znaleziono odpowiedni element do usunięcia
    else
    {
        // Jeśli tylko lewe dziecko
        if (root->left == NULL)
        {
            // Usuń ten węzeł i przepisz wartość
            // Oraz zwolnij pamięć zajmowaną przez węzeł
            curr_node = root->right;
            free(root);
            root = curr_node;
        }
            // Jeśli tylko prawe dziecko
        else if (root->right == NULL)
        {
            // Usuń ten węzeł i przepisz wartość oraz zwolnij pamięć
            // zajmowaną przez węzeł
            curr_node = root->left;
            free(root);
            root = curr_node;
        }
        else    //Jeśli istnieje prawy oraz lewy podwęzeł
        {
            curr_node = root->right;
            node *parent = NULL;
            //Przejdz do ostatniego węzła lewego poddrzewa
            while(curr_node->left != NULL)
            {
                parent = curr_node;
                curr_node = curr_node->left;
            }
            // Przepisz wartość z tego węzła
            root->data = curr_node->data;
            // Wywołaj rekurencyjnie usunięcie prawego lub lewego podddrzewa
            if (parent != NULL)
                parent->left = delete_node(parent->left, parent->left->data);
            else
                root->right = delete_node(root->right, root->right->data);
        }
    }
    return root;
}

/** Szukaj elementu zawierającego daną wartość
 *  Jako parametr root podajemy korzeń drzewa
 *  Jako parametr data podajemy wartość jaką chcemy wyszukać
 *  zwraca wartośc znalezionego węzła
 *  Aby wyszukać określony klucz w drzewie wyszukiwania binarnego,
 *  rozpoczynamy od węzła głównego. Jeśli drzewo jest puste, klucz nie istnieje.
 *  W przeciwnym przypadku, jeśli klucz węzła głównego jest równy elementowi szukanemu,
 *  wyszukiwanie zakończy się pomyślnie, kończymy wyszukiwanie.
 *  Jeśli klucz jest mniejszy niż klucz węzła głównego, przeszukujemy lewe poddrzewo
 *  Podobnie, jeśli klucz jest większy niż klucz węzła głównego, przeszukujemy
 *  prawe poddrzewo. Powtarzamy ten krok rekurencyjnie, aż klucz
 *  zostanie znaleziony lub poddrzewo ma wartość NULL.
 */
node* search(node *root,const int data)
{
    // Cwiczenie 4:  Uzupełnij kod wyszukujący zadany element w drzewie BST
    if(root == NULL)
        return NULL;

    int r;
    node* curr_node = root;
    while(curr_node != NULL)
    {
        r = compare(data, curr_node->data);
        if(r<0)
            curr_node = curr_node -> left;
        else if(r>0)
            curr_node = curr_node -> right;
        else
            return curr_node;
    }
    return curr_node;

}


/** Wyświetla drzewo na ekranie
 * jako argument przyjmuje wskaźnik do węzła
 * Funkcja przchodzi przez całe poddrzewo i wywołuje siebie rekurencyjnie
 * dla lewego ora dla prawego poddrzewa
 */
void display_tree(node* nd)
{
    if (nd == NULL)
        return;
    printf("%d",nd->data);
    if(nd->left != NULL)
        printf("(L:%d)",nd->left->data);
    if(nd->right != NULL)
        printf("(R:%d)",nd->right->data);
    printf("\n");
    display_tree(nd->left);
    display_tree(nd->right);
}

/** Usuwa całą zawartość drzewa.
 * Aby usunąć drzewo należy przejśc od korzenia do końca drzewa
 * oraz usunąć wszystkie elementy tego drzewa.
 */
void _remove_all(node* root)
{
    // Ćwiczenie 4:  Uzupełnij kod usuwający całą zawartość drzewa
    if(root != NULL)
    {
        _remove_all(root->left);
        _remove_all(root->right);
        free(root);
    }

}

void remove_all( node** root)
{
    _remove_all(*root);
    *root = NULL;
}


int main(void)
{
    //Inicjalizujemy zmienne lokalne
    node* root = NULL;
    //Dodajemy przykładowe elementy do drzewa binarnego
    root = insert_node(root,8);
    root = insert_node(root,10);
    root = insert_node(root,3);
    root = insert_node(root,1);
    root = insert_node(root,6);
    root = insert_node(root,14);
    root = insert_node(root,4);
    root = insert_node(root,7);
    root = insert_node(root,13);
    //Wyświetlamy zawartość drzewa binarnego
    display_tree(root);
    //Wyszukaj zadany element w drzewie binarnym
    for( int key=0;; )
    {
        node* s;
        printf("Podaj wartosc klucza do wyszukania. Wpisz -1 aby zakonczyć: ");
        scanf("%d",&key);
        if( key < 0 ) break;
        s = search(root,key);
        if(s != NULL)
        {
            printf("Znaleziono element %i",s->data);
            if(s->left != NULL)
                printf("(L: %i)",s->left->data);
            if(s->right != NULL)
                printf("(R: %i)",s->right->data);
            printf("\n");
        }
        else
        {
            printf("wezel %i nie zostal znaleziony\n",key);
        }
    }
    // Usuwanie danego elementu
    for( int key=0;;)
    {
        printf("Podaj wartosc klucza do usuniecia. Wpisz -1 aby zakonczyć: ");
        scanf("%d",&key);
        if( key < 0 ) break;
        root = delete_node(root,key);
        /* display the tree */
        if(root != NULL) display_tree(root);
        else break;
    }
    // Usuwanie wszystkich elementów i wyświetlenie
    remove_all(&root);
    if( root )
        display_tree(root);
    else
        printf("Drzewo puste\n");
    return 0;
}
