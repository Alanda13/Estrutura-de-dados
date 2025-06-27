//gerenciamento de prioridade de emergencia

//Heap Sort foi usado para ordenar a lista de chamadas
// com base na prioridade, garantindo que a chamada mais urgente seja tratada primeiro.

#include <stdio.h>

void heapify(int arr[], int n, int i) {
    int maior = i;
    int esquerda = 2 * i + 1;
    int direita = 2 * i + 2;

    if (esquerda < n && arr[esquerda] > arr[maior])
        maior = esquerda;

    if (direita < n && arr[direita] > arr[maior])
        maior = direita;

    if (maior != i) {
        int temp = arr[i];
        arr[i] = arr[maior];
        arr[maior] = temp;

        heapify(arr, n, maior);
    }
}

void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        heapify(arr, i, 0);
    }
}

void imprimirChamadas(int prioridades[], int n) {
    for (int i = 0; i < n; i++)
        printf("Chamada com prioridade %d\n", prioridades[i]);
}

int main() {
    // Prioridades das chamadas (quanto maior, mais urgente)
    int chamadas[] = {4, 10, 3, 5, 1, 7, 8};
    int n = sizeof(chamadas)/sizeof(chamadas[0]);

    printf("Chamadas recebidas (não ordenadas):\n");
    imprimirChamadas(chamadas, n);

    heapSort(chamadas, n);

    printf("\nChamadas ordenadas por prioridade (mais urgente primeiro):\n");
    for (int i = n - 1; i >= 0; i--)  // Inverter para mostrar da maior prioridade
        printf("Chamada com prioridade %d\n", chamadas[i]);

    return 0;
}
