#include <iostream>
#include <vector>
#include <string>
#include <cstdlib> // para usar o random
#include <ctime>   // para inicializar o random e contar o tempo
#include <fstream> // file input output
#include <iomanip>
#include <limits> // std::numeric_limits<std::streamsize>::max()
#include <chrono>
#include <stdexcept>
using namespace std::chrono;


struct tuple
{
    //Um tuplo só para facilitar trabalhar com as coordenadas linhas/colunas
    int x;
    int y;
    tuple(int a, int b)
    {
        x = a;
        y = b;
    }
    
    bool equal(tuple tup1)
    {
        return tup1.x == x && tup1.y == y;
    }
    int get(int index)
    {
        return (index == 0) ? x : y;
    }
    void increment(int index)
    {
        if (index)
            y++;
        
        else
            x++;
    }
    void decrement(int index)
    {
        if (index)
            y--;
        else
            x--;
    }
};

class Map
{
    
    //Não pus um construtor na classe para dar a flexibilidade de criar um random map ou para dar load a outro
    //até posso mudar isto para private penso que não vou usar estas variaveis fora da classe
public: //Atributos
    //Vetor que vai conter o mapa
    std::vector<std::vector<char>> m_map;
    
    //Construir a grelha do mapa
    char m_fence = '*';
    char m_space = ' ';
    
    //Propriedades do mapa a definir depois de build map
    int m_map_lines;
    int m_map_columns;
    
    //Propriedades para definir como vou inserir fences dentro do mapa
    int m_in_map_fences = 0;
    int m_max_in_map_fences;
    // probabilidade de inserir dentro do mapa == 1/casos_possivei assim entre aspas
    // posso melhorar esta parte para impor que não haja muitas fences seguidas tipo num grupo  futuramente ?
    int m_casos_possiveis = 6;
    
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
        
        char to_place;
        for (int l = 0; l < linhas; l++)
        {
            std::vector<char> line;
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
            std::vector<char> linha;
            for (int col = 0; col < read_line.size(); col++)
            {
                char to_push = read_line[col];
                linha.push_back(to_push);
            }
            m_map.push_back(linha);
        }
        rfile.close();
    }
    //Para fora da classe não ter que mexer com m_map
    // e assim futuramente ate posso por m_map como private?
    void change_tile(int line, int col, char novo)
    {
        m_map[line][col] = novo;
    }
    
    char get_tile(int line, int col)
    {
        return m_map[line][col];
    }
};

class Entity
{
public: // Atributos
    bool m_is_alive;
    char m_alive;
    char m_dead;
    char m_corrent;
    char m_entity_square[3][3];
    Map *m_pointermap;
    int m_p_line;
    int m_p_col;
    
    Entity(int line, int col, char alive_str, Map &mapa)
    {
        m_is_alive = 1;
        m_p_line = line;
        m_p_col = col;
        m_alive = alive_str;
        m_corrent = alive_str;
        // m_alive é uma string de len 1 o at(0) devolve me uma char ao somar 32 estou a convertÊlo para minuscula
        // na tabela de ascii as minusculas distam 32 das maiusculas
        //m_dead = std::string(1, (m_alive.at(0)) + 32);
        m_dead = char(m_alive + 32);
        
        m_pointermap = &mapa;
        //update_entity_square() provavelmente vou remover esta funcao;
    }
    
public: // Métodos
    //dá para mandar embora
    void display_entity_square()
    {
        
        for (int line = 0; line < 3; line++)
        {
            char *line_pointer = (char *)(m_entity_square) + 3 * line;
            for (int col = 0; col < 3; col++)
                std::cout << *(line_pointer + col);
            
            std::cout << std::endl;
        }
    }
    //dá para mandar embora
    void update_entity_square()
    {
        //Tenho que alterar pois se a posicao do player é 0,0 vou tentar acedar a -1,-1
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
    tuple position()
    {
        return tuple(m_p_line, m_p_col);
    }
    void general_move(int newline, int newcol)
    {
        //O movimento de uma entidade consiste em mover para a nova posicão  preencher a antiga com um espaço e dar update á member variable posicao
        
        (*m_pointermap).change_tile(m_p_line, m_p_col, ' ');
        m_p_line = newline;
        m_p_col = newcol;
        
        (*m_pointermap).change_tile(m_p_line, m_p_col, m_corrent);
    }
    bool move_player(char move, std::vector<Entity> &entity_vec)
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
        
        int newline = m_p_line + movement.x;
        int newcol = m_p_col + movement.y;
        tuple position(newline, newcol);
        char conteudo_nova_posicao = (*m_pointermap).get_tile(newline, newcol);
        
        if (conteudo_nova_posicao == 'r')
        {
            std::cout << "Invalid Movement!" << std::endl;
            return 0;
        }
        
        else if (conteudo_nova_posicao == '*')
        {
            m_is_alive = 0;
            m_corrent = m_dead;
        }
        else if (conteudo_nova_posicao == 'R')
        {
            for (int i = 0; i < entity_vec.size(); i++)
            {
                if (entity_vec[i].position().equal(position))
                {
                    entity_vec[i].m_is_alive = 0;
                }
            }
            m_is_alive = 0;
            m_corrent = m_dead;
        }
        general_move(newline, newcol);
        return canmove;
    }
    
    void move_robot(Entity &player, std::vector<Entity> &entity_vec)
    {
        if (m_is_alive)
        {
            tuple positionplayer = player.position();
            tuple positionthis = position();
            tuple newposition = positionthis;
            for (int i = 0; i < 2; i++)
            {
                int diff = positionplayer.get(i) - positionthis.get(i);
                if (diff > 0)
                {
                    newposition.increment(i);
                }
                else if (diff < 0)
                {
                    newposition.decrement(i);
                }
            }
            int newline = newposition.x;
            int newcol = newposition.y;
            char conteudo_nova_posicao = (*m_pointermap).get_tile(newline, newcol);
            if (conteudo_nova_posicao != ' ')
            {
                m_is_alive = 0;
                m_corrent = m_dead;
                if (conteudo_nova_posicao == 'R')
                {
                    for (int i = 0; i < entity_vec.size(); i++)
                    {
                        if (entity_vec[i].position().equal(newposition))
                        {
                            entity_vec[i].m_is_alive = 0;
                        }
                    }
                }
                else if (conteudo_nova_posicao == 'H')
                {
                    player.m_is_alive = 0;
                    //Para quando um robot comer um player aparecer o player
                    m_corrent = 'h';
                }
            }
            general_move(newline, newcol);
        }
    }
    
    void get_robots_player(char robotstr, std::vector<Entity> &vecrobots)
    {
        
        int lines = (*m_pointermap).m_map_lines;
        int columns = (*m_pointermap).m_map_columns;
        for (int line = 0; line < lines; line++)
        {
            
            for (int col = 0; col < columns; col++)
            {
                char conteudo_tile = (*m_pointermap).m_map[line][col];
                if (conteudo_tile == m_alive)
                {
                    m_p_line = line;
                    m_p_col = col;
                }
                else if (conteudo_tile == robotstr)
                {
                    Entity robot(line, col, robotstr, *m_pointermap);
                    vecrobots.push_back(robot);
                }
            }
        }
    }
};
/* Experimentei ter dois constructors mas acabava por me repetir então vou pô-lo na parent class*/

void clean(bool showmessege = 0)
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (showmessege)
        std::cout << "\nInvalid input. ";
}

char get_input()
{
    
    //a funcao verifica se a ação inserida é valida e se estiver em maiuscula passa a minuscula
    char action;
    
    const std::string commands_lower = "qweasdzxc";
    const std::string commands_upper = "QWEASDZXC";
    do
    {
        std::cout << "Enter your next move: ";
        std::cin >> action;
        //Só vamos verificar se a acão é possivel e  caso seja torna-la maiuscula caso o user tenha digitado apenas um caracter
        if (std::cin.good() && std::cin.peek() == '\n')
        {
            
            // npos none position ou seja não encontrou
            if (commands_upper.find(action) != std::string::npos)
            {
                return action;
            }
            
            else if (commands_lower.find(action) != std::string::npos)
            {
                action = commands_upper[commands_lower.find(action)];
                return action;
            }
        }
        //Caso não tenhamos um input valido vamos limpar o buffer do teclado
        clean(1);
        
    } while (true);
    //porque sem isto não funciona?
    return 's';
}

bool order_sort(std::string i1, std::string i2)
{
    try{
        
        size_t index1 = i1.find_last_of(' ') , index2 = i1.size() - index1;
        size_t index3 = i2.find_last_of(' ') , index4 = i2.size() - index3;
        return (i1.substr(index1,index2)<i2.substr(index3,index4));
    }
    
    catch(const std::out_of_range& oor){
        
        return false;
    }
    
}



void clean_leaderboard(std::string mazenumber){
    
    std::vector<std::string> scores;
    std::string current;
    int count=0;
    std::fstream ofs;
    std::ifstream myfile("MAZE_"+mazenumber+"_WINNERS.txt");
    
    if (myfile.is_open())
    {
        while (getline(myfile, current))
        {
            count++;
            if (count>=3){scores.push_back(current);} //para nao meter no vetor o cabecalho
        }
    }
    
    
    sort(scores.begin(),scores.end(),order_sort);
    
    myfile.close();
    
    ofs.open( "MAZE_"+mazenumber+"_WINNERS.txt", std::ios::out | std::ios::trunc) ;
    
    ofs << "Player          - Time\n----------------------\n";
    
    for (int i=0;i<scores.size();i++){
        
        ofs<<scores[i]<<std::endl;
        
    }
    
    ofs.close();
    
}


void leaderboard(int gametime, std::string mazenumber){
    
    std::ofstream scores;
    std::string player;
    
    std::cout<<"Please write your name (up to 15 characters): ";
    getline(std::cin,player);
    
    while(player.size()>15){
        std::cout<<"Invalid input. Please write up to 15 characters: ";
        getline(std::cin,player);
    }
    
    scores.open("MAZE_"+mazenumber+"_WINNERS.txt");
    
    
    if (scores.fail()) {
        
        std::ofstream scores("MAZE_"+mazenumber+"_WINNERS.txt");
        
        scores <<"Player          - Time\n----------------------\n";
    }
    
    //quer tenha sido agora criado o ficheiro ou quer ele ja tenha existido esta aberto portanto posso usar esta forma
    
    
    int spaces = 15 - player.size();
    int length = 5 - (std::to_string(gametime).length());
    
    scores <<player<<std::string(spaces, ' ') <<'-'<<std::string(length, ' ')<<gametime;
    
    scores.close();
    
    clean_leaderboard("XX"); //falta a parte do numero do maze
    
    return;
}

void display_rules()
{
    std::string rules;
    std::ifstream myfile("rules.txt");
    if (myfile.is_open())
    {
        while (getline(myfile, rules))
        {
            std::cout << rules << '\n';
        }
        myfile.close();
    }
}

void play_game() //Jogar o jogo e dar update aos winners caso ganhe
{
    //Secalhar sair logo quando o player morrer?
    //Tratar melhor de quando o player é comido ficar a mostrar o player
    
    auto start = high_resolution_clock::now(); //comecar o relogio
    
    std::vector<Entity> vec_robots;
    Map first_try;
    first_try.load(0);
    Entity player(1, 1, 'H', first_try);
    player.get_robots_player('R', vec_robots);
    first_try.display_map();
    bool robots_alive;
    char move;
    do
    {
        move = get_input();
        
        player.move_player(move, vec_robots);
        
        //Mover os robots
        for (int i = 0; i < vec_robots.size(); i++)
        {
            vec_robots[i].move_robot(player, vec_robots);
            if (!player.m_is_alive)
                break;
        }
        
        //Não posso fazer o teste no mesmo ciclo em que movo os robots pois um pode estar vivo e depois ser morto por outro robot
        robots_alive = 0;
        for (int i = 0; i < vec_robots.size(); i++)
        {
            robots_alive |= vec_robots[i].m_is_alive;
        }
        
        first_try.display_map();
    } while (player.m_is_alive && robots_alive);
    
    auto stop = high_resolution_clock::now();
    
    int gametime = (duration_cast<seconds>(stop - start)).count();
    
    if (player.m_is_alive){
        
        // nao faco ideia onde ir buscar o numero do maze
        leaderboard(gametime, "XX");
    }
    
}

void menu()
{
    
    int option;
    //Menu do while em vez de voltar recursivamente ao menu em cada caso assim é mais eficiente
    do
    {
        std::cout << "Enter 0 to exit, 1 for the rules and 2 to play: ";
        if (std::cin >> option)
        {
            
            if (option == 1) //Mostrar regras
            {
                display_rules();
            }
            
            else if (option == 2) //Entrar no jogo
            {
                play_game();
            }
            //Para criar um novo mapa
            else if (option == 3)
            {
                Map randommap;
                randommap.build_map(10, 20);
                randommap.save();
            }
        }
        //Se houver erro vai limpar o buffer , independentemente
        clean();
    } while (option);
}




int main()
{
    
    clean_leaderboard("XX");
    return 0;
}

