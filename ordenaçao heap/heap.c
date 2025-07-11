#include <stdio.h>

// vai garantir que o nó i e seus filhos formem um Max-Heap
void heapify(int arr[], int n, int i) {
    int maior = i;           // Inicializa o maior como raiz
    int esquerda = 2 * i + 1; 
    int direita = 2 * i + 2;  

    // aqui vai verificar quem é o maior: raiz, filho esquerdo  ou direito
    if (esquerda < n && arr[esquerda] > arr[maior])
        maior = esquerda;                                        //Esses dois if comparam a raiz
                                                                  //com seus filhos e atualizam o maior.
    if (direita < n && arr[direita] > arr[maior])
        maior = direita;

    // Se o maior não for a raiz
    if (maior != i) {
        int temp = arr[i];
        arr[i] = arr[maior];
        arr[maior] = temp;
        //Faz a troca de posições entre i e o maior filho
        
        heapify(arr, n, maior);
        //Depois chama heapify recursivamente para garantir que o restante do heap esteja correto.
    }
}

void heapSort(int arr[], int n) {

//Essa parte constrói o heap a partir do vetor
//Começa nos nós que têm filhos e aplica heapif
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    
    for (int i = n - 1; i > 0; i--) {
        //vai percorrer do fim ate o começoo
        int temp = arr[0]; //pega o maior valor, que está na raiz
        arr[0] = arr[i];   //troca com o ultimo elemente do heap
        arr[i] = temp;  //coloca o maior valor no fim 

        
        heapify(arr, i, 0);    //agr com valor reduzido, tamanho do heap é 1 
         //assim o maior vai para o fim, depois o segundo maior  e assim por diante 
    }
}

void imprimirArray(int arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    int arr[] = {12, 11, 13, 5, 6, 7};
    int n = sizeof(arr)/sizeof(arr[0]);

    printf("Array original:\n");
    imprimirArray(arr, n);

    heapSort(arr, n);

    printf("Array ordenado com Heap Sort:\n");
    imprimirArray(arr, n);
    return 0;
}
