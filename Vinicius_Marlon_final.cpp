#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

// Trabalho feito por : Vinicius Passos Oliveira, Marlon Silva Araujo

struct Universidade {
    int identificador;
    char nome[100];
    int quantidadeCursos;
    char cidade[100];
    int posicao;
};

void imprimirUniversidades(const Universidade& universidade) {
    cout << "Identificador: " << universidade.identificador << endl;
    cout << "Nome: " << universidade.nome << endl;
    cout << "Quantidade de cursos: " << universidade.quantidadeCursos << endl;
    cout << "Cidade: " << universidade.cidade << endl;
    cout << "Posição: " << universidade.posicao << endl;
    cout << "-------------------------------------" << endl;
}

void exibirUniversidades(const Universidade universidades[], int tamanho) {
    if (tamanho == 0) {
        cout << "Nenhuma universidade cadastrada." << endl;
    } else {
        cout << "Universidades cadastradas:" << endl;
        cout << "-------------------------------------" << endl;
        for (int i = 0; i < tamanho; i++) {
            imprimirUniversidades(universidades[i]);
        }
    }
}

int stringParaInt(const char* pegar) {
    int result = 0;
    int i = 0;

    while (pegar[i] == ' ') {
        i++;
    }

    while (pegar[i] != '\0') {
        if (pegar[i] >= '0' && pegar[i] <= '9') {
            result = result * 10 + (pegar[i] - '0');
        } 
        i++;
    }

    return result;
}
int particionar(Universidade universidades[], int inicio, int fim) {
    int pivo = universidades[(inicio + fim) / 2].identificador;
    int i = inicio - 1;
    int j = fim + 1;

    while (true) {
        do {
            i++;
        } while (universidades[i].identificador < pivo);

        do {
            j--;
        } while (universidades[j].identificador > pivo);

        if (i >= j) {
            return j;
        }

        Universidade temp = universidades[i];
        universidades[i] = universidades[j];
        universidades[j] = temp;
    }
}

void quickSort(Universidade universidades[], int inicio, int fim) {
    if (inicio < fim) {
        int p = particionar(universidades, inicio, fim);
        quickSort(universidades, inicio, p);
        quickSort(universidades, p + 1, fim);
    }
}

int carregarUniversidadesBinario(Universidade*& universidades, const string& arquivo) {
    ifstream lerArquivo(arquivo, ios::binary);
    if (!lerArquivo) {
        cout << "Erro ao abrir o arquivo " << arquivo << endl;
        return 0;
    }

    const int capacidadeInicial = 100;
    int capacidadeAtual = capacidadeInicial;
    int tamanho = 0;
    universidades = new Universidade[capacidadeAtual];

    Universidade universidade;

    while (lerArquivo.read((char*)&universidade, sizeof(Universidade))) {
        if (tamanho == capacidadeAtual) {
            capacidadeAtual *= 2;
            Universidade* novoArray = new Universidade[capacidadeAtual];
            for (int i = 0; i < tamanho; i++) {
                novoArray[i] = universidades[i];
            }
            delete[] universidades;
            universidades = novoArray;
        }

        universidades[tamanho] = universidade;
        tamanho++;
    }

    lerArquivo.close();
    return tamanho;
}


void salvarUniversidades(const Universidade universidades[], int tamanho, const string& arquivo) {
    ofstream arquivoUni(arquivo, ios::binary);
    if (!arquivoUni) {
        cerr << "Erro ao abrir o arquivo " << arquivo << endl;
        return;
    }

    for (int i = 0; i < tamanho; i++) {
        arquivoUni << universidades[i].identificador << ","
               << universidades[i].nome << ","
               << universidades[i].quantidadeCursos << ","
               << universidades[i].cidade << ","
               << universidades[i].posicao << "\n";
    }

    cout << "Universidades salvas no arquivo " << arquivo << endl;
}

void adicionarUniversidade(Universidade universidades[], int& tamanho) {
    Universidade universidade;

    cout << "-------------------------------------" << endl;
    cout << "Adicionar uma nova universidade" << endl;
    cout << "-------------------------------------" << endl;

    cout << "Identificador: ";
    cin >> universidade.identificador;

    cout << "Nome: ";
    cin.ignore();
    cin.getline(universidade.nome, sizeof(universidade.nome));

    cout << "Quantidade de cursos: ";
    cin >> universidade.quantidadeCursos;

    cout << "Cidade: ";
    cin.ignore();
    cin.getline(universidade.cidade, sizeof(universidade.cidade));

    cout << "Posição: ";
    cin >> universidade.posicao;

    universidades[tamanho] = universidade;
    tamanho++;

    ofstream arquivoUni("arquivoBinario.dat", ios::binary | ios::app);
    if (arquivoUni) {
        arquivoUni << universidade.identificador << ","
               << universidade.nome << ","
               << universidade.quantidadeCursos << ","
               << universidade.cidade << ","
               << universidade.posicao << "\n";
        arquivoUni.close();
    }

    cout << "Universidade adicionada com sucesso." << endl;
}
int lerPosicaoUniversidade(const Universidade universidades[], int tamanho) {
    int posicao = -1;
    char busca[100];
    cout << "Digite o identificador ou nome da universidade: ";
    cin.ignore();
    cin.getline(busca, sizeof(busca));

    bool encontrou = false;
    for (int i = 0; i < tamanho && !encontrou; i++) {
        if (universidades[i].identificador == stringParaInt(busca) || strcmp(universidades[i].nome, busca) == 0) {
            posicao = universidades[i].posicao;
            encontrou = true;
            cout << "Universidade encontrada:" << endl;
            cout << "---------------------------" << endl;
            imprimirUniversidades(universidades[i]);
        }
    }

    if (!encontrou) {
        cout << "Universidade não encontrada." << endl;
    }

    return posicao;
}

void removerUniversidade(Universidade universidades[], int& tamanho) {
    if (tamanho == 0) {
        cout << "Nenhuma universidade cadastrada." << endl;
        return;
    }

    int identificador;
    cout << "-------------------------------------" << endl;
    cout << "Remover uma universidade" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Digite o identificador da universidade que deseja remover: ";
    cin >> identificador;

    bool encontrou = false;
    int indice = -1;

    for (int i = 0; i < tamanho; i++) {
        if (universidades[i].identificador == identificador) {
            encontrou = true;
            indice = i;
        }
    }

    if (encontrou) {
        for (int i = indice; i < tamanho - 1; i++) {
            universidades[i] = universidades[i + 1];
        }

        tamanho--;
        cout << "Universidade removida com sucesso." << endl;
    } else {
        cout << "Universidade não encontrada." << endl;
    }
}

void editarUniversidade(Universidade universidades[], int tamanho) {
    if (tamanho == 0) {
        cout << "Nenhuma universidade cadastrada." << endl;
        return;
    }

    int identificador;
    cout << "-------------------------------------" << endl;
    cout << "Editar uma universidade" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Digite o identificador da universidade que deseja editar: ";
    cin >> identificador;

    bool encontrou = false;
    int indice = -1;

    for (int i = 0; i < tamanho; i++) {
        if (universidades[i].identificador == identificador) {
            encontrou = true;
            indice = i;
            break;
        }
    }

    if (encontrou) {
        Universidade universidade = universidades[indice];

        cout << "-------------------------------------" << endl;
        cout << "Editar universidade" << endl;
        cout << "-------------------------------------" << endl;

        cout << "Identificador: ";
        cin >> universidade.identificador;

        cout << "Nome: ";
        cin.ignore();
        cin.getline(universidade.nome, sizeof(universidade.nome));

        cout << "Quantidade de cursos: ";
        cin >> universidade.quantidadeCursos;

        cout << "Cidade: ";
        cin.ignore();
        cin.getline(universidade.cidade, sizeof(universidade.cidade));

        cout << "Posição: ";
        cin >> universidade.posicao;

        universidades[indice] = universidade;
        cout << "Universidade editada com sucesso." << endl;
    } else {
        cout << "Universidade não encontrada." << endl;
    }
}

void criaArquivoBinarioARRAY(Universidade universidades[], int tamanho) {
    ofstream arquivo("arquivoBinario.dat", ios::binary);
    if(arquivo) {
        arquivo.write((const char *) (universidades), sizeof(Universidade)*tamanho);
        cout << "Escreveu com sucesso o arquivo binário" << endl;
        arquivo.close();
    }else{
        cout<<"Deu errado";
    }
}

void salvarUniversidadesCSV(const Universidade universidades[], int tamanho, const string& arquivo) {
    ofstream arquivoUni(arquivo);
    if (!arquivoUni) {
        cout << "Erro ao abrir o arquivo " << arquivo << endl;
        return;
    }

    for (int i = 0; i < tamanho; i++) {
        arquivoUni << universidades[i].identificador << ","
               << universidades[i].nome << ","
               << universidades[i].quantidadeCursos << ","
               << universidades[i].cidade << ","
               << universidades[i].posicao << "\n";
    }

    cout << "Universidades salvas no arquivo CSV " << arquivo << endl;
    arquivoUni.close();
}

void salvarUniversidadesBinario(const Universidade universidades[], int tamanho, const string& arquivo) {
    ofstream arquivoUni(arquivo, ios::binary);
    if (!arquivoUni) {
        cout << "Erro ao abrir o arquivo " << arquivo << endl;
        return;
    }

    for (int i = 0; i < tamanho; i++) {
        arquivoUni.write((const char*)&universidades[i], sizeof(Universidade));
    }

    cout << "Universidades salvas no arquivo binário " << arquivo << endl;
    arquivoUni.close();
}
int carregarUniversidades(Universidade*& universidades, const string& arquivo) {
    ifstream input(arquivo, ios::binary);
    if (!input) {
        cout << "Erro ao abrir o arquivo " << arquivo << endl;
        return 0;
    }
    const int capacidadeInicial = 100;
    int capacidadeAtual = capacidadeInicial;
    int tamanho = 0;

    universidades = new Universidade[capacidadeAtual];

    string linha;
    while (getline(input, linha)) {
        char campo[100];
        int posicaoCSV = 0;
        int posicaoCampo = 0;

        for (char c : linha) {
            if (c == ',') {
                campo[posicaoCampo] = '\0';
                if (posicaoCSV == 0) {
                    universidades[tamanho].identificador = stringParaInt(campo);
                } else if (posicaoCSV == 1) {
                    strcpy(universidades[tamanho].nome, campo);
                } else if (posicaoCSV == 2) {
                    universidades[tamanho].quantidadeCursos = stringParaInt(campo);
                } else if (posicaoCSV == 3) {
                    strcpy(universidades[tamanho].cidade, campo);
                }

                posicaoCampo = 0;
                posicaoCSV++;
            } else {
                campo[posicaoCampo++] = c;
            }
        }
        campo[posicaoCampo] = '\0'; 
        universidades[tamanho].posicao = stringParaInt(campo);

        tamanho++;
            if (tamanho >= capacidadeAtual) {
            capacidadeAtual *= 2;

            Universidade* novoVetor = new Universidade[capacidadeAtual];

            for (int i = 0; i < tamanho; i++) {
                novoVetor[i] = universidades[i];
            }

            delete[] universidades;

            universidades = novoVetor;
        }
    }

    return tamanho;

    }


int main() {

    const string arquivoUniversidades = "universidades.csv";
    Universidade* universidades = new Universidade[100];
    int tamanhoUniversidades = 0;
    
    tamanhoUniversidades = carregarUniversidadesBinario(universidades, "arquivoBinario.dat");
    
    if (tamanhoUniversidades == 0) {
        tamanhoUniversidades = carregarUniversidades(universidades, arquivoUniversidades);
        if (tamanhoUniversidades == 0) {
            cout << "Erro ao carregar as universidades do arquivo." << endl;
            return 1;
        } 
        criaArquivoBinarioARRAY(universidades, tamanhoUniversidades);
    }

    int opcao;
    do {
        cout << "-------------------------" << endl;
        cout << "Sistema de Universidades" << endl;
        cout << "-------------------------" << endl;
        cout << "Digite o número correspondente à opção desejada"<< endl;
        cout << "1. Exibir universidades " << endl;
        cout << "2. Remover universidade" << endl;
        cout << "3. Editar universidade" << endl;
        cout << "4. Buscar universidade" << endl;
        cout << "5. Adicionar universidade" << endl;
        cout << "6. Exportar universidades" << endl;
        cout << "0. Sair " << endl;
        cout << "-------------------------" << endl;
        cout << "Digite sua opção: ";
        cin >> opcao;

              switch (opcao) {
            case 1:
                quickSort(universidades, 0, tamanhoUniversidades - 1);
                exibirUniversidades(universidades, tamanhoUniversidades);
                break;
            case 2:
                removerUniversidade(universidades, tamanhoUniversidades);
                break;
            case 3:
                editarUniversidade(universidades, tamanhoUniversidades);
                break;
            case 4:
                lerPosicaoUniversidade(universidades, tamanhoUniversidades);
                break;
            case 5:
                adicionarUniversidade(universidades, tamanhoUniversidades);
                break;
            case 6:
                salvarUniversidadesCSV(universidades, tamanhoUniversidades, arquivoUniversidades);
                salvarUniversidadesBinario(universidades, tamanhoUniversidades, "arquivoBinario.dat");
                break;
            case 0:
                cout << "Encerrando o programa..." << endl;
                break;
            default:
                cout << "Opção inválida." << endl;
                break;
        }
    } while (opcao != 0);

    delete[] universidades;
    
    return 0;
}