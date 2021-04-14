#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>

class Map
{
    //até posso mudar isto para private penso que não vou usar estas variaveis fora da classe
public:
    //Vetor que vai conter o mapa
    std::vector<std::vector<std::string>> map;

    //Construir a grelha do mapa
    std::string fence = "*";
    std::string space = " ";

    //Propriedades do mapa a definir depois de build map
    int map_lines;
    int map_columns;

    //Propriedades para definir como vou inserir fences dentro do mapa
    int in_map_fence_c = 0;
    int max_in_max_fence;
    // probabilidade de inserir dentro do mapa == 1/casos_possivei assim entre aspas
    // posso melhorar esta parte para impor que não haja muitas fences seguidas tipo num grupo  futuramente ?

    int casos_possiveis = 8;

public:
    bool rand_place(int col, int line)
    {
        bool place = 0;
        if (in_map_fence_c < max_in_max_fence)
        {
            place = (1 == (rand() % casos_possiveis));
        }

        return place;
    }
    bool place_fence(int col, int line)
    {

        return (line == 0) || (line == (map_lines - 1)) || (col == 0) || (col == (map_columns - 1)) || (rand_place(col, line));
    }

    void build_map(int linhas, int colunas)
    {

        map_lines = linhas;
        map_columns = colunas;
        max_in_max_fence = (map_lines * map_columns) / 5;
        srand(time(NULL));

        std::string to_place;
        for (int l = 0; l < linhas; l++)
        {
            std::vector<std::string> line;
            for (int c = 0; c < colunas; c++)
            {
                to_place = space;
                if (place_fence(c, l))
                {
                    to_place = fence;
                }
                line.push_back(to_place);
            }
            map.push_back(line);
        }
    }

    void display_map()
    {

        for (int l = 0; l < map.size(); l++)
        {

            for (int c = 0; c < map[l].size(); c++)
            {

                std::cout << map[l][c];
            }
            std::cout << std::endl;
        }
    }

    std::string namefile(int filenumber)

    {
        std::string unique = "Teste";
        return "MAZER_" + unique + ".txt";
    }

    void save()
    {

        std::ofstream wfile; //output file

        //Não me vou preocupar com o tratemento do nome nesta func ela já recebe o nome tratado
        wfile.open(namefile(0));

        //Primeira linha com as dimensões do mapa
        wfile << map_lines << "x" << map_columns << std::endl;

        //resto do mapa estou a usar o size() para poder ter mapas variaveis no futuro?
        for (int linhas = 0; linhas < map.size(); linhas++)
        {

            int n_col = map[linhas].size();
            for (int colunas = 0; colunas < n_col; colunas++)
            {
                wfile << map[linhas][colunas];
            }
            wfile << std::endl;
        }
    }

    void load(int filenumber)
    {

        std::ifstream rfile; // input from file

        //Novamente qual é o ficheiro e o tratamento do nome do tal está abstraido deste metodo
        rfile.open(namefile(filenumber));

        //ler a primeira linha e define-se umas propriedades do mapa (member properties)
        char sep;
        rfile >> map_lines >> sep >> map_columns;

        /*Ler o resto do mapa usei o metodo getline para experimentar 
         os unicos problemas que tive foi porque escolhi usar strings e este metodo  acho que usa cstrings
         logo ao indexar read_lina é como se fosse um array de chars logo tenho que o transformar numa strin
        */
        std::string read_line;
        int counter = 0;
        while (getline(rfile, read_line))
        {
            std::vector<std::string> linha;
            for (int col = 0; col < read_line.size(); col++)
            {
                std::string to_push = std::string(1, read_line[col]);
                linha.push_back(to_push);
            }
            map.push_back(linha);
        }
        counter++;
    }
};

int main()
{

    Map first_try;
    first_try.load(0);
    first_try.display_map();
    //oi

    return 0;
}