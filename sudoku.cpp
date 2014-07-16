#include<iostream>
#include<ctime>
#include<cfloat>
#include<fstream>
#include<cstdio>
#include<cstring>
#include<cmath>

#define DIMENSION 9

using namespace std;

class sudoku
{
        private: int *grid[DIMENSION];

                 inline void readConfig(const char *str);
                 inline void prepareGrid();
                 inline bool isSolved(int &i, int &j);
                 inline bool checkGridValidity(int i, int j, int value);

        public:
                 sudoku();
                 sudoku(const char *str);
                 void prepare(const char *str);
                 ~sudoku();
                 bool solve();
                 bool solver(int i, int j);
                 inline void printgrid();
};

//Constructor
sudoku::sudoku()
{
        int i;
        for(i=0; i<DIMENSION; i++)
        {
                grid[i]=new int[DIMENSION];
        }
}

//Constructor
sudoku::sudoku(const char *str)
{
        int i;
        int j;
        if(strlen(str)!=DIMENSION*DIMENSION)
        {
                return;
        }
        for(i=0; i<DIMENSION; i++)
        {
                grid[i]=new int[DIMENSION];
        }
        readConfig(str);
        prepareGrid();
}

//Destructor
sudoku::~sudoku()
{
        int i;
        for(i=0; i<DIMENSION; i++)
        {
                delete[] grid[i];
        }
}

//Function to bypass unnecessary constructor invocation
void sudoku::prepare(const char *str)
{
        readConfig(str);
        prepareGrid();
}

//Check if all the squares are filled
inline bool sudoku::isSolved(int &i, int &j)
{
        int row=i, col;
        for(col=j; col<DIMENSION; col++)
        {
                if(grid[row][col]>DIMENSION)
                {
                        j=col;
                        return false;
                }
        }
        for(row=i+1; row<DIMENSION; row++)
        {
                for(col=0; col<DIMENSION; col++)
                {
                        if(grid[row][col]>DIMENSION)
                        {
                                i=row;
                                j=col;
                                return false;
                        }
                }
        }
        return true;
}

//Populate the member grid using str
inline void sudoku::readConfig(const char *str)
{
        int i, j;
        for(i=0; i<DIMENSION; i++)
        {
                for(j=0; j<DIMENSION; j++)
                {
                        if(str[i*DIMENSION+j]=='.')
                        {
                                grid[i][j]=0;
                        }
                        else
                        {
                                grid[i][j]=str[i*DIMENSION+j]-'0';
                        }
                }
        }
}

inline void sudoku::prepareGrid()
{
        bool *present=new bool[DIMENSION+1];
        int i, j, k, l;
        int row, column;
        int step=sqrt(DIMENSION);
        int number;
        for(i=0; i<DIMENSION; i++)
        {
                for(j=0; j<DIMENSION; j++)
                {
                        if(grid[i][j]==0)
                        {
                                for(k=0; k<DIMENSION+1; k++)
                                {
                                        present[k]=false;
                                }

                                for(k=0; k<DIMENSION; k++)
                                {
                                        if(grid[i][k]<=DIMENSION)
                                        {
                                                present[grid[i][k]]=true;
                                        }
                                }

                                for(k=0; k<DIMENSION; k++)
                                {
                                        if(grid[k][j]<=DIMENSION)
                                        {
                                                present[grid[k][j]]=true;
                                        }
                                }

                                row=i/step;
                                row=row*step;
                                column=j/step;
                                column=column*step;

                                for(k=row; k<row+step; k++)
                                {
                                        for(l=column; l<column+step; l++)
                                        {
                                                if(grid[k][l]<=DIMENSION)
                                                {
                                                        present[grid[k][l]]=true;
                                                }
                                        }
                                }

                                number=0;
                                for(k=1; k<DIMENSION+1; k++)
                                {
                                        if(present[k]==false)
                                        {
                                                number=number*10+k;
                                        }
                                }
                                grid[i][j]=number;
                        }
                }
        }
        delete[] present;
}

//Check if the number tried at index i, j conforms to the sudoku rules
inline bool sudoku::checkGridValidity(int i, int j, int number)
{
        int row, column;
        int step=sqrt(DIMENSION);
        int k, l;
        for(k=0; k<DIMENSION; k++)
        {
                if(grid[i][k]==number)
                {
                        return false;
                }
        }
        for(l=0;  l<DIMENSION; l++)
        {
                if(grid[l][j]==number)
                {
                        return false;
                }
        }
        row=i/step;
        row=row*step;
        column=j/step;
        column=column*step;
        for(k=row; k<row+step; k++)
        {
                for(l=column; l<column+step; l++)
                {
                        if(grid[k][l]==number)
                        {
                                return false;
                        }
                }
        }
        return true;
}

//Print the grid
inline void sudoku::printgrid()
{
        int i, j;
        for(i=0; i<DIMENSION; i++)
        {
                if(i>0 && i%3==0)
                {
                        cout<<"---------------------"<<endl;
                }
                for(j=0; j<DIMENSION; j++)
                {
                        if(j>0 && j%3==0)
                        {
                                printf("| ");
                        }
                        printf("%d ", grid[i][j]);
                        //cout<<grid[i][j]<<" ";
                }
                cout<<endl;
        }
}

//Wrapper
bool sudoku::solve()
{
        return solver(0, 0);
}

//Solver - uses backtracking to solve the grid. For sudokus with multiple solutions the function stops at the first solution found
bool sudoku::solver(int i, int j)
{
        int actual, temp;
        if(isSolved(i, j))
        {
                return true;
        }
        actual=grid[i][j];
        temp=actual;
        while(temp>0)
        {
                if(checkGridValidity(i, j, temp%10))
                {
                        grid[i][j]=temp%10;
                        if(solver(i, j+1))
                        {
                                return true;
                        }
                }
                temp=temp/10;
        }
        grid[i][j]=actual;
        return false;
}

int main(int argc, char *argv[])
{
        int i=1;
        clock_t t1, t2;
        bool status;
        string result;
        int passed=0;
        float diff;
        float total=0;
        float max_time=-FLT_MAX, min_time=FLT_MAX;
        int max_num, min_num;
        char str[81];
        ifstream f;

        sudoku s;

        f.open(argv[1], ios::in);

        while(f.read(str, 81))
        {
                t1=clock();

                s.prepare(str);
                status=s.solve();

                t2=clock();

                diff=(t2-t1)/(1.0*CLOCKS_PER_SEC);

                if(diff>max_time)
                {
                        max_time=diff;
                        max_num=i;
                }

                if(diff<min_time)
                {
                        min_time=diff;
                        min_num=i;
                }

                result="No";
                if(status)
                {
                        passed++;
                        result="Yes";
                }


                total=total+diff;

                cout<<"Sudoku number "<<i<<": Solved = "<<result<<"  Time taken = "<<diff<<"s"<<endl;

#ifdef PRINT_ON_SUCCESS
                cout<<"Solution grid"<<endl;
                s.printgrid();
#endif

                i++;
                f.read(str, 1);
        }
        
        f.close();

        cout<<endl<<endl<<"Statistics"<<endl<<endl;
        cout<<"Total number of Sudokus = "<<i-1<<endl;
        cout<<"Solved = "<<passed<<endl;
        cout<<"Failed = "<<i-1-passed<<endl;
        cout<<"Total time taken = "<<total<<"s"<<endl;
        cout<<"Max time "<<max_time<<"s"<<" taken for Sudoku number "<<max_num<<endl;
        cout<<"Min time "<<min_time<<"s"<<" taken for Sudoku number "<<min_num<<endl;
        cout<<"Average time = "<<total/(i-1)<<"s"<<endl;

        return 0;
}
