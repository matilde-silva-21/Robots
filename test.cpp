#include <iostream>
#include <vector>
#include <string>
#include <cstdlib> // para usar o random
#include <ctime>   // para inicializar o random e contar o tempo
#include <fstream> // file input output
#include <iomanip>
#include <limits> // std::numeric_limits<std::streamsize>::max()

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
        //ao tirar as dimensoes do mapa e o separador ainda fica lá a primeira linha isto é a forma imediata da tirar;
        getline(rfile, read_line);
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

struct tuple
{
    int m_a;
    int m_b;
    tuple(int a, int b)
    {
        m_a = a;
        m_b = b;
    }
};

class Entity
{
public: // Atributos
    bool m_is_alive;
    std::string m_alive;
    std::string m_dead;
    std::string m_entity_square[3][3];
    Map *m_pointermap;
    int m_p_line;
    int m_p_col;
    Entity(int line, int col, std::string alive_str, Map &mapa)
    {
        m_is_alive = 1;
        m_p_line = line;
        m_p_col = col;
        m_alive = alive_str;
        m_pointermap = &mapa;
        update_entity_square();
    }

public: // Métodos
    void display_entity_square()
    {

        int strsize = sizeof(std::string);
        for (int line = 0; line < 3; line++)
        {
            char *line_pointer = (char *)(m_entity_square) + 3 * line * strsize;
            for (int col = 0; col < 3; col++)
                std::cout << *((std::string *)(line_pointer + col * strsize));

            std::cout << std::endl;
        }
    }
    void update_entity_square()
    {
        int pos_line = m_p_line - 1;
        int pos_col = m_p_col - 1;
        for (int l = 0; l < 3; l++)
        {
            for (int c = 0; c < 3; c++)
            {
                m_entity_square[l][c] = (*m_pointermap).m_map[pos_line + l][pos_col + c];
            }
        }
    }
    bool move_player(char move)
    {
        bool canmove = 1;
        tuple movement(0, 0);
        switch (move)

        {
        case 'Q':
        {
            movement = tuple(-1, -1);
            break;
        }
        case 'W':
        {
            movement = tuple(-1, 0);
            break;
        }
        case 'E':
        {
            movement = tuple(-1, 1);
            break;
        }
        case 'A':
        {
            movement = tuple(0, -1);
            break;
        }
        case 'S':
        {
            movement = tuple(0, 0);
            break;
        }
        case 'D':
        {
            movement = tuple(0, 1);
            break;
        }
        case 'Z':
        {
            movement = tuple(1, -1);
            break;
        }
        case 'X':
        {
            movement = tuple(1, 0);
            break;
        }
        case 'C':
        {
            movement = tuple(1, 1);
            break;
        }
        }

        int newline = m_p_line + movement.m_a;
        int newcol = m_p_col + movement.m_b;
        if ((*m_pointermap).m_map[newline][newcol] == "r")
        {
            std::cout << "Movimento inválido!" << std::endl;

            return 0;
        }
        (*m_pointermap).m_map[m_p_line][m_p_col] = " ";
        m_p_line = newline;
        m_p_col = newcol;
        (*m_pointermap).m_map[m_p_line][m_p_col] = m_alive;
        return canmove;
    }
};
/* Experimentei ter dois constructors mas acabava por me repetir então vou pô-lo na parent class*/

void clean()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "\nInvalid input. ";
}

void menu()
{

    int redu;
    std::string rules;

    std::cout << "Enter 0 to exit, 1 for the rules and 2 to play: ";

    // verificar se o input esta correto
    if (std::cin >> redu)
    {

        if (redu == 0)
        {
            return;
        }

        else if (redu == 1)
        {

            std::ifstream myfile("rules.txt");
            if (myfile.is_open())
            {
                while (getline(myfile, rules))
                {
                    std::cout << rules << '\n';
                    std::cout << "chupa";
                }
                myfile.close();

                return menu();
            }
        }

        else if (redu == 2)
        {
            //como chamar o jogo?
        }

        else
        {
            clean();
            return menu();
        }
    }

    //para lidar com o bad input

    else
    {
        clean();
        return menu();
    }
}

void action_control()
{

    //a funcao verifica se a ação inserida é valida e se estiver em maiuscula passa a minuscula

    char action;

    const std::string commands_lower = "qweasdzxc";
    const std::string commands_upper = "QWEASDZXC";

    std::cout << "Enter your next move: ";

    if (std::cin >> action)
    {

        if (commands_lower.find(action) != std::string::npos)
        {
            //aqui era a altura em que se chamava a funcao para mexer o H
        }

        else if (commands_upper.find(action) != std::string::npos)
        {
            action = commands_lower[commands_upper.find(action)]; //se o caracter estiver em maiuscula passa a minuscula

            //aqui era a altura em que se chamava a funcao para mexer o H (na funcao os comandos tem de estar em minuscula!
        }

        else
        {
            clean();
            return action_control();
        }
    }

    else
    {
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
    Entity player(5, 15, "H", first_try);
    Entity robot1(1, 1, "R", first_try);

    char move;
    do
    {

        std::cout << "Digite para onde pretende ir :";
        std::cin >> move;
        player.move_player(move);
        first_try.display_map();
    } while (player.m_is_alive);

    return 0;
}
