#include <iostream>
#include <vector>
#include <string>
#include <cstdlib> // para usar o random
#include <ctime>   // para inicializar o random e contar o tempo
#include <fstream> // file input output
#include <iomanip>

class Map
{

    //Não pus um construtor na classe para dar a flexibilidade de criar um random map ou para dar load a outro

    //até posso mudar isto para private penso que não vou usar estas variaveis fora da classe
public: //Atributos
    //Vetor que vai conter o mapa
    std::vector<std::vector<std::string>> m_map;

    //Construir a grelha do mapa
    std::string m_fence = "*";
    std::string m_space = " ";

    //Propriedades do mapa a definir depois de build map
    int m_map_lines;
    int m_map_columns;

    //Propriedades para definir como vou inserir fences dentro do mapa
    int m_in_map_fences = 0;
    int m_max_in_map_fences;
    // probabilidade de inserir dentro do mapa == 1/casos_possivei assim entre aspas
    // posso melhorar esta parte para impor que não haja muitas fences seguidas tipo num grupo  futuramente ?
    int m_casos_possiveis = 8;

public: //Metodos
    //Random method para colocar ou não fences dentro do mapa
    // quanto maior é casos_possiveis  menor é a probailidade obv
    bool rand_place(int col, int line)
    {
        bool place = 0;
        if (m_in_map_fences < m_max_in_map_fences)
        {
            place = (1 == (rand() % m_casos_possiveis));
            if (place)
                m_in_map_fences++;
        }

        return place;
    }
    // Tendo em conta a funcao funcao rand_place e se está nas bordas retorna true se for um desses casos
    bool place_fence(int col, int line)
    {

        return (line == 0) || (line == m_map_lines - 1) || (col == 0) || (col == (m_map_columns - 1)) || (rand_place(col, line));
    }

    //Constroi o mapa  e coloca fences conforme place_fence
    void build_map(int linhas, int colunas)
    {

        m_map_lines = linhas;
        m_map_columns = colunas;
        m_max_in_map_fences = (m_map_lines * m_map_columns) / 5;
        srand(time(NULL));

        std::string to_place;
        for (int l = 0; l < linhas; l++)
        {
            std::vector<std::string> line;
            for (int c = 0; c < colunas; c++)
            {
                to_place = m_space;
                if (place_fence(c, l))
                {
                    to_place = m_fence;
                }
                line.push_back(to_place);
            }
            m_map.push_back(line);
        }
    }

    void display_map()
    {

        for (int l = 0; l < m_map.size(); l++)
        {

            for (int c = 0; c < m_map[l].size(); c++)
            {

                std::cout << m_map[l][c];
            }
            std::cout << std::endl;
        }
    }
    //Falta tratar disto
    std::string namefile(int filenumber)
    {
        std::string unique = "Teste";
        return "MAZE_" + unique + ".txt";
    }

    //Escrever/guardar o mapa num ficheiro txt o nome tem a ver com o número que é dado pelo user ainda não percebi
    // vector -> txt
    void save()
    {

        std::ofstream wfile; //output file

        //Não me vou preocupar com o tratemento do nome nesta func ela já recebe o nome tratado
        wfile.open(namefile(0));

        //Primeira linha com as dimensões do mapa
        wfile << m_map_lines << "x" << m_map_columns << std::endl;

        //resto do mapa estou a usar o size() para poder ter mapas variaveis no futuro?
        for (int linhas = 0; linhas < m_map.size(); linhas++)
        {

            int n_col = m_map[linhas].size();
            for (int colunas = 0; colunas < n_col; colunas++)
            {
                wfile << m_map[linhas][colunas];
            }
            wfile << std::endl;
        }
        wfile.close();
    }

    //Carregar mapa txt -> vector
    void load(int filenumber)
    {

        std::ifstream rfile; // input from file

        //Novamente qual é o ficheiro e o tratamento do nome do tal está abstraido deste metodo
        rfile.open(namefile(filenumber));

        //ler a primeira linha e define-se umas propriedades do mapa (member properties)
        char sep;
        rfile >> m_map_lines >> sep >> m_map_columns;

        /*Ler o resto do mapa usei o metodo getline para experimentar 
         os unicos problemas que tive foi porque escolhi usar strings e este metodo  acho que usa cstrings
         logo ao indexar read_lina é como se fosse um array de chars logo tenho que o transformar numa strin
        */
        std::string read_line;
        while (getline(rfile, read_line))
        {
            std::vector<std::string> linha;
            for (int col = 0; col < read_line.size(); col++)
            {
                std::string to_push = std::string(1, read_line[col]);
                linha.push_back(to_push);
            }
            m_map.push_back(linha);
        }
        rfile.close();
    }
};

void clean(){
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cout<<"\nInvalid input. ";
}


void menu(){
    
    int redu;
    string rules;
    
    cout<<"Enter 0 to exit, 1 for the rules and 2 to play: ";
    
    // verificar se o input esta correto
    if (cin>>redu){
        
        if (redu==0){
            return;
        }
        
        else if(redu==1){
            
            ifstream myfile ("rules.txt");
            if (myfile.is_open())
            {
                while ( getline (myfile,rules) )
                {
                    cout << rules << '\n';
                    cout<<"chupa";
                }
                myfile.close();
                
                return menu();
            }
                
        }
        
        else if(redu==2){
            //como chamar o jogo?
        }
        
        else{
            clean();
            return menu();
        }
    }
    
    //para lidar com o bad input
    
    else{
        clean();
        return menu();
    }
}

void action_control(){
    
    //a funcao verifica se a ação inserida é valida e se estiver em maiuscula passa a minuscula
    
    char action;
    
    const string commands_lower="qweasdzxc";
    const string commands_upper="QWEASDZXC";
    
    cout<<"Enter your next move: ";
    
    
    if (cin>>action){
        
        if (commands_lower.find(action) != string::npos){
            //aqui era a altura em que se chamava a funcao para mexer o H
        }
    
    
        else if (commands_upper.find(action) != string::npos){
            action=commands_lower[commands_upper.find(action)]; //se o caracter estiver em maiuscula passa a minuscula
            
            //aqui era a altura em que se chamava a funcao para mexer o H (na funcao os comandos tem de estar em minuscula!
        }
    
        else{
            clean();
            return action_control();
        }

    }
    
    else {
        clean();
        return action_control();
    }
    
    return;

}

int main()
{

    Map first_try;
    first_try.load(0);
    first_try.display_map();
    //oi

    return 0;
}
