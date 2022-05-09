#include "Matrix.h"
#include "Structs.h"


using namespace std;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Need file name" << std::endl;
        exit(1);
    }
    auto start = std::chrono::high_resolution_clock::now();
    std::ifstream myfile;
    myfile.open(argv[1]);
    if (!myfile.is_open())
    {
        std::cout << "file not opened" << std::endl;
        exit(1);
    }
    // std::stringstream ss(myfile.rdbuf());
    // myfile.close();

    int num_ver, num_edge, num_triangle;
    myfile >> num_ver >> num_edge >> num_triangle;
    std::vector<std::array<float, 3>> vertices;
    for (int i = 0; i < num_ver; i++)
    {
        std::array<float, 3> buf;
        for (int j = 0; j < 3; j++)
        {
            myfile >> buf[j];
        }
        vertices.push_back(buf);
    }

    std::vector<std::array<int, 2>> edges;
    for (int i = 0; i < num_edge; i++)
    {
        std::array<int, 2> buf;
        for (int j = 0; j < 2; j++)
        {
            myfile >> buf[j];
            buf[j]--;
        }
        if (buf[0] > buf[1])
            std::swap(buf[0], buf[1]);
        edges.push_back(buf);
    }

    std::vector<std::array<int, 3>> triangles;
    for (int i = 0; i < num_triangle; i++)
    {
        std::array<int, 3> buf;
        for (int j = 0; j < 3; j++)
        {
            myfile >> buf[j];
            buf[j]--;
        }
        triangles.push_back(buf);
    }

    boundary_matrix bm = make_boundary(edges, num_ver);
    Matrix matrix(bm.num_rows, bm.num_cols, 0);
    matrix.populate(bm.num_rows, bm.num_cols, bm.mat);
    // cout<<"Before RREF"<<endl;
    // matrix.print();
    // matrix.rref();
    // cout<<"After RREF"<<endl;
    // matrix.print();
    vector<int> pivots = matrix.get_pivots();
    // for x in pivotPoints_tuple:
    //     if(x > num_edg-1):
    //         holes0.append(x-num_edg + 1)
    vector<int> holes0;
    
    for(int i=0;i<pivots.size();i++)
    {
        if(pivots[i]>num_edge-1)
        {
            // cout<<"{"<<pivots[i]-num_edge+1<<"}"<<" ";
            holes0.push_back(pivots[i]-num_edge+1);
        }
    }
    cout<<"Number of 0-Homology Groups Formed: "<<holes0.size()<<endl;
    cout<<"Representative 0 Cycles: "<<endl;
    for(int i=0;i<holes0.size();i++)
    {
        cout<<"{"<<holes0[i]<<"}"<<" ";
    }
    cout<<endl;
    
    // cout<<"Rank of matrix:"<<matrix.get_rank()<<endl;
    // std::cout << bm.num_cols - matrix.get_rank() << std::endl;
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Time Taken is = " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << std::endl;
    // matrix.print();
    // Matrix *ker_space = matrix.get_ker();
    // ker_space->print();
    // int **array;
    // array = new int *[3];
    // for (int i = 0; i < 3; i++)
    //     array[i] = new int[5];
}

struct boundary_matrix make_boundary(std::vector<std::array<int, 2>> edges, int num_ver)
{
    boundary_matrix bm = {num_ver, edges.size() + num_ver, new int *[num_ver]};
    for (int i = 0; i < num_ver; i++)
    {
        bm.mat[i] = new int[edges.size() + num_ver];
    }

    for (int i = 0; i < num_ver; i++)
    {
        for (int j = 0; j < edges.size() + num_ver; j++)
        {
            bm.mat[i][j] = 0;
        }
    }

    for (int e = 0; e < edges.size(); e++)
    {
        int vertex0 = edges[e][0];
        int vertex1 = edges[e][1];
        bm.mat[vertex1][e] = 1;
        bm.mat[vertex0][e] = -1;
    }
    int row = 0;
    for (int e = edges.size(); e < edges.size() + num_ver; e++)
    {
        bm.mat[row][e] = 1;
        row++;
    }
    // cout<<"Cols"<<bm.num_cols<<endl;
    return bm;
}