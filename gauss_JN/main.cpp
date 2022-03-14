#include <iostream>
#include <iostream>
#include <fstream>

using namespace std;

void wczytaj(double** &A, double* &b, unsigned &matrix_size)
{
    ifstream source_file("dane.csv");
    if(!source_file.is_open())
    {
        cout <<"The file has not been open!"<<endl;
        return;
    }
    source_file >> matrix_size;
    A = new double*[matrix_size];
    A[0] = new double[matrix_size*matrix_size];
    for(unsigned i = 1; i< matrix_size; i++)
    {
        A[i] = A[i-1] + matrix_size;
    }
    b = new double[matrix_size];
    char semicolumn;
    for (unsigned i = 0; i < matrix_size+1; i++)
    {
        source_file >> semicolumn;
    }
    for (unsigned i = 0; i < matrix_size; i++)
    {
        for (unsigned j = 0; j < matrix_size; j++)
        {
            source_file >> A[i][j];
            source_file >> semicolumn;
        }
        source_file >> semicolumn;
        source_file >> b[i];
    }
    source_file.close();
    for(int i=0;i<matrix_size;i++)
    {
        for(int j=0;j<matrix_size;j++)
        {
            cout<<A[i][j]<<" ";
        }
        cout<<endl;
    }
}

void show_zad2(double** A, double *b, unsigned matrix_size, double *kolejnosc)
{
    double* wyniki=new double[matrix_size];
    for(int i=matrix_size-1; i>=0; i--)
    {
        for(int j=matrix_size-1; j>=0; j--)
        {
            if(j==i && i==matrix_size-1)
            {
                wyniki[i]=b[i]/A[i][j];
                break; //wyznaczamy Xn, co oznacza ze pozostale wartosci w wierszu "i" sa rowne 0, dlatego petla dla tego "i" wykona sie raz
            }
            else
            {
                if(j==matrix_size-1)
                {
                    wyniki[i]=b[i];
                }
                if(j!=i)
                {
                    wyniki[i]=wyniki[i]-(A[i][j]*wyniki[j]);
                }
                else
                {
                    wyniki[i]=wyniki[i]/A[i][j];
                    break; //gdy j==i, to kazde nastepne j-- bedzie rowne 0, wiec przerywamy wykonywanie petli dla danego "i"
                }
            }
        }
    }
    for(int i=0; i<matrix_size; i++)
    {
        cout<<"x"<<defaultfloat<<kolejnosc[i]<<"="<<fixed<<wyniki[i]<<defaultfloat<<endl;
        //dzieki fixed output wynosi 0. bez tego komputer zwracal liczbe "e", rowna -4,12931E-15, czyli praktycznie 0
        //blad spowodowany dzieleniem podczas obliczen(liczac ulamkami, na kartce cos by sie skrocilo, a tu zostalo zaokraglone)
    }
}

void zadanie_2(double** A, double *b, unsigned matrix_size, double *kolejnosc)
{
    for(int i=1; i<matrix_size; i++)
    {
        int temp=i; //pomocnicza zmienna do indeksowania, poniewaz w ponizszej petli zmieniam wartosc "i" tymczasowo
        for(int j=i; j<matrix_size; j++)
        {
            A[i][j]=A[i][j]-((A[i][temp-1]/A[temp-1][temp-1])*(A[temp-1][j]));
            if(j==i)
            {
                b[i]=b[i]-((A[i][temp-1]/A[temp-1][temp-1])*(b[temp-1]));
            }
            if(j+1==matrix_size && i+1<matrix_size)
            {
                j=temp-1;
                i=i+1;
            }
        }
        i=temp;
        for(int k=i; k<matrix_size; k++)
        {
            A[k][i-1]=0.0; //zeruje wspolczynniki pod elementem referencyjnym
        }
    }
    show_zad2(A, b, matrix_size, kolejnosc);
}

//wybierajac z wiersza, swapujemy kolumne
void zadanie_31(double** A, double *b, unsigned matrix_size, double *kolejnosc)
{
    double maks=A[0][0];
    int z=1;
    for(int i=1; i<matrix_size; i++)
    {
        if(A[0][i]>maks || (A[0][i]*(-1)>maks))
        {
            maks=A[0][i];
            z=i;
        }
    }
    if(maks!=A[0][0])
    {
        swap(kolejnosc[0], kolejnosc[z]);
        for(int j=0; j<matrix_size; j++)
        {
            swap(A[j][0], A[j][z]);
        }
    }
    zadanie_2(A, b, matrix_size, kolejnosc);
}

//wybierajac z kolumny, swapujemy wiersze
void zadanie_32(double** A, double *b, unsigned matrix_size, double *kolejnosc)
{
    double maks=A[0][0];
    int z=1;
    for(int i=1; i<matrix_size; i++)
    {
        if(A[i][0]>maks || (A[i][0]*(-1)>maks))
        {
            maks=A[i][0];
            z=i;
        }
    }
    if(maks!=A[0][0])
    {
        for(int j=0; j<matrix_size; j++)
        {
            swap(A[0][j], A[z][j]);
        }
        swap(b[0], b[z]);
    }
    zadanie_2(A, b, matrix_size, kolejnosc);
}

//polaczenie zad31 i 32
void zadanie_4(double** &A, double *&b, unsigned matrix_size, double *&kolejnosc, int indeks)
{
    double maks=A[indeks][indeks];
    int z=indeks, k=indeks; //zmienne pomocnicze <indeksy>
    for(int i=indeks; i<matrix_size; i++)
    {
        for(int j=indeks; j<matrix_size; j++)
        {
            if(A[i][j]>maks || (A[i][j]*(-1)>maks))
            {
                maks=A[i][j];
                z=i;
                k=j;
            }
        }
    }
    if(maks!=A[indeks][indeks])
    {
        if(k!=indeks) //swap kolumn
        {
            swap(kolejnosc[indeks], kolejnosc[k]);
            for(int j=0; j<matrix_size; j++)
            {
                swap(A[j][indeks], A[j][k]);
            }
        }
        if(z!=indeks) //swap wierszy
        {
            for(int j=indeks; j<matrix_size; j++) //zaczynam o indeksu, bo wszystko po lewo od niego jest = 0
            {
                swap(A[indeks][j], A[z][j]);
            }
            swap(b[indeks], b[z]);
        }
    }
}

void zadanie_2_modified(double** A, double *b, unsigned matrix_size, double *kolejnosc) //zad4
{
    zadanie_4(A, b, matrix_size, kolejnosc, 0);
    for(int i=1; i<matrix_size; i++)
    {
        int temp=i; //pomocnicza zmienna do indeksowania, poniewaz w ponizszej petli zmieniam wartosc "i" tymczasowo
        for(int j=i; j<matrix_size; j++)
        {
            A[i][j]=A[i][j]-((A[i][temp-1]/A[temp-1][temp-1])*(A[temp-1][j]));
            if(j==i)
            {
                b[i]=b[i]-((A[i][temp-1]/A[temp-1][temp-1])*(b[temp-1]));
            }
            if(j+1==matrix_size && i+1<matrix_size)
            {
                j=temp-1;
                i=i+1;
            }
        }
        i=temp;
        for(int k=i; k<matrix_size; k++)
        {
            A[k][i-1]=0.0; //zeruje wspolczynniki pod elementem referencyjnym
        }
        zadanie_4(A, b, matrix_size, kolejnosc, i);
    }
    show_zad2(A, b, matrix_size, kolejnosc);
}

int main()
{
    double** A;
    double* b;
    unsigned matrix_size;
    double *kolejnosc=new double[matrix_size];
    int zadanie;
    while(true)
    {
        cout<<"1)  Wczytaj dane"<<endl;
        cout<<"2)  Metoda eliminacji Gaussa bez wyboru elementu"<<endl;
        cout<<"31) Metoda eliminacji Gaussa z wyborem elementu w wierszach"<<endl;
        cout<<"32) Metoda eliminacji Gaussa z wyborem elementu w kolumnach"<<endl;
        cout<<"4)  Metoda eliminacji Gaussa z pelnym wyborem elementu"<<endl;
        cout<<"5)  Zakoncz program"<<endl<<endl;
        cout<<"Ktore z powyzszych polecen chcesz wykonac? Wpisz odpowiednia cyfre: "<<endl;
        cin>>zadanie;
        cout<<endl;
        switch(zadanie)
        {
            case 1:
                {
                    wczytaj(A, b, matrix_size);
                    for(int i=1; i<=matrix_size; i++) //przydatna przy zad 31, 32, 4
                    {
                        kolejnosc[i-1]=i;
                    }
                    cout<<endl;
                    break;
                }
            case 2:
                {
                    zadanie_2(A, b, matrix_size, kolejnosc);
                    cout<<endl;
                    break;
                }
            case 31:
                {
                    zadanie_31(A, b, matrix_size, kolejnosc);
                    cout<<endl;
                    break;
                }
            case 32:
                {
                    zadanie_32(A, b, matrix_size, kolejnosc);
                    cout<<endl;
                    break;
                }
            case 4:
                {
                    zadanie_2_modified(A, b, matrix_size, kolejnosc);
                    cout<<endl;
                    break;
                }
            case 5:
                {
                    delete [] b;
                    delete [] A[0];
                    delete [] A;
                    return 0;
                }
            default:
                {
                    cout<<"Wpisz poprawna cyfre"<<endl;
                    cout<<endl;
                    break;
                }
        }
    }
}
