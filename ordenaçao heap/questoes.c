class No{
    public:
           char nome;
           No *left;
           No *right;
           No(char n){
            nome = n;
            left= NULL;
            right = NULL;
           }
};

class Arvore{
    public:
    No *raiz;
    No *pai, *no;
    int h, quantNos;


    Arvore(){
        raiz= NULL;
        pai=NULL;
        no= NULL;
        h= -1;
        quantNos=0;
    }
    void x(No *arv, No *n){
        if(isEmpty()==1)
        raiz = n;
        else{
            No*percorre= raiz;
            while(percorre!NULL){
                if(percorre->nome<-n->nome){
                    if(percorre->right==NULL){
                        percorre->right=n;
                        break;
                    }
                    else
                    percorre=percorre->right;
                  if(percorre->nome>n->nome)
                  if(percorre->left==NULL){
                    percorre->left=n;
                    break;
                  }
                  else
                  percorre=percorre->left;
                }
            }
        }
    }
    void imprime(No*arv){
        if(arv!= NULL){
            cout<<"<"<<arv->nome;
            imprime(arv-> left);
            imprime(arv->right);
            cout<<">";
        }
        else
        cout<<"<>";
    }
}


main(){
    Arvore *arvore=new Arvore();
    No *w = new No('W');
    No *z = new No('Z');
    No *a = new No('A');
    No *b = new No('B');
    arvore->x(arvore->raiz,w);
    arvore->x(arvore->raiz,b);
    arvore->x(arvore->raiz,z);
    arvore->x(arvore->raiz,a);
    arvore->imprime(arvore->raiz,);
}

main(){
    Arvore *arvore=new Arvore();
    No *w = new No('W');
    No *z = new No('Z');
    No *a = new No('A');
    No *b = new No('B');
    arvore->x(arvore,w);
    arvore->x(arvore,b);
    arvore->x(arvore,z);
    arvore->x(arvore,a);
    arvore->imprime(arvore);
}