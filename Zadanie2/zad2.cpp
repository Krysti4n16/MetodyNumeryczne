#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

const double pi = 3.14159265358979323846;

//funkcja generujaca wektor b
vector<double> generateRHS(int N){
    vector<double> b(N);
    for(int i=0;i<N;i++){
        b[i]=cos((4.0*pi*i)/N);
    }
    return b;
}

vector<double> thomasAlgorithm(int N, vector<double> a, vector<double> b, vector<double> c, vector<double> d) {
    
    //eliminacja w przod
    c[0]= c[0]/b[0];
    d[0]= d[0]/b[0];

    for(int i=1;i<N;i++){
        double temp= 1.0/(b[i]-a[i]*c[i-1]);
        c[i]= c[i]*temp;
        d[i]=(d[i]-a[i]*d[i-1])*temp;
    }

    //podstawianie wsteczne
    vector<double> x(N);
    x[N-1]= d[N-1];

    for(int i=N-2;i>=0;i--){
        x[i]=d[i]-c[i]*x[i+1];
    }

    return x;
}

//sherman-morrison
vector<double> solveShermanMorrison(int N){
    double h= 2.0/(N-1);
    double val= 1.0/(h*h);

    vector<double> a(N, val);          
    vector<double> diag(N, -2.0*val);
    vector<double> c(N, val);          
    
    //korekta dla macierzy T w metodzie Shermana-Morrisona
    diag[0] -= val;
    diag[N-1] -= val;
    a[0]= 0.0;
    c[N-1]= 0.0;

    vector<double> rhs_b= generateRHS(N);
    
    vector<double> rhs_u(N, 0.0);  
    rhs_u[0]= val;
    rhs_u[N-1]= val;

    
    //uklad Ty = b
    vector<double> y= thomasAlgorithm(N, a, diag, c, rhs_b);

    //uklad Tz = u
    vector<double> z= thomasAlgorithm(N, a, diag, c, rhs_u);

    //laczenie wynikow
    double v_y= y[0]+y[N-1];
    double v_z= z[0]+z[N-1];
    double factor= v_y/(1.0+v_z);

    vector<double> x(N);
    for(int i=0;i<N;i++){
        x[i]= y[i]-factor*z[i];
    }
    return x;
}

//metoda Gaussa–Seidela
vector<double> solveGaussSeidel(int N){
    double h= 2.0/(N-1);
    double h2=h*h;
    vector<double> b= generateRHS(N);
    vector<double> x(N, 0.0);

    //ograniczenie liczby iteracji
    int maxIter = (N > 5000) ? 100 : 2000; 
    
    for(int k=0;k<maxIter;k++){
        for(int i=0;i<N;i++){
            int prev= (i-1+N)% N;
            int next= (i+1)% N;

            x[i]= -0.5*(b[i]*h2-x[prev]-x[next]);
        }
    }
    return x;
}

//eliminacja Gaussa
vector<double> solveFullMatrix(int N){
    //N wierszy i N+1 kolumn, ostatnia kolumna to wektor b
    vector<vector<double>> A(N, vector<double>(N+1, 0.0));
    double h= 2.0/(N-1);
    double val= 1.0/(h*h);
    double diag= -2.0*val;
    vector<double> b= generateRHS(N);

    for(int i=0;i<N;i++){
        int prev= (i-1+N) % N;
        int next= (i+1)% N;

        A[i][i]= diag;      
        A[i][prev] += val;   
        A[i][next] += val;   
        A[i][N]= b[i];      
    }

    for(int i=0;i<N;i++){
        //pivoting
        int maxRow= i;
        for(int k=i+1;k<N;k++){
            if(abs(A[k][i])>abs(A[maxRow][i])) maxRow= k;
        }
        swap(A[i], A[maxRow]);

        if(abs(A[i][i]) < 1e-12) continue;

        for(int k=i+1;k<N;k++){
            double c= -A[k][i]/A[i][i];
            for(int j=i;j<=N;j++){
                if(i==j) A[k][j]= 0.0;
                else A[k][j] += c*A[i][j];
            }
        }
    }

    //podstawianie wsteczne
    vector<double> x(N);
    for(int i=N-1;i>=0;i--){
        double sum= 0.0;
        for(int j=i+1;j<N;j++){
            sum += A[i][j]*x[j];
        }
        
        if(abs(A[i][i])>1e-12){
            x[i]= (A[i][N]- sum)/A[i][i];
        }
        else{
            x[i] = 0.0;
        }
    }

    return x;
}

int main(){
    vector<int> testNs = {10, 50, 100, 500, 1000, 2000, 5000, 10000}; 

    ofstream timeFile("czas_wykonania.csv");
    timeFile << "N,ShermanMorrison,GaussSeidel,FullMatrix\n";

    cout << "start testow\n";
    cout << setw(10) << "N" << setw(20) << "Sherman(s)" << setw(20) << "G-Seidel(s)" << setw(20) << "FullMatrix(s)\n";

    for(int index=0;index<testNs.size();index++){
        int N= testNs[index];
        
        double tSherman, tIter, tFull= -1.0;

        auto start= chrono::high_resolution_clock::now();
        vector<double> solSherman= solveShermanMorrison(N);
        auto end= chrono::high_resolution_clock::now();
        tSherman= chrono::duration<double>(end - start).count();

        start= chrono::high_resolution_clock::now();
        vector<double> solGS= solveGaussSeidel(N);
        end= chrono::high_resolution_clock::now();
        tIter= chrono::duration<double>(end - start).count();

        vector<double> solFull;
        if(N<=1000){
            start= chrono::high_resolution_clock::now();
            solFull= solveFullMatrix(N);
            end= chrono::high_resolution_clock::now();
            tFull= chrono::duration<double>(end - start).count();
        }

        cout << setw(10) << N << setw(20) << tSherman << setw(20) << tIter << setw(20) << (tFull >= 0 ? to_string(tFull) : "-") << "\n";

        timeFile << N << "," << tSherman << "," << tIter << ",";
        if(tFull>=0) timeFile << tFull;
        timeFile << "\n";

        if(N == 1000){
            double h=2.0/(N-1);
            
            ofstream f1("rozwiazanie_sherman.csv");
            f1 << "n,x_n,u_n\n";
            for(int i=0;i<N;i++){
                double x_n=i*h;
                f1 << (i+1) << "," << x_n << "," << solSherman[i] << "\n";
            }
            f1.close();

            ofstream f2("rozwiazanie_gauss_seidel.csv");
            f2 << "n,x_n,u_n\n";
            for(int i=0;i<N;i++){
                double x_n= i*h;
                f2 << (i+1) << "," << x_n << "," << solGS[i] << "\n";
            }
            f2.close();
            
            cout << "zapisano rozwiazania do plikow CSV dla N = 1000\n";

            ofstream f3("rozwiazanie_full.csv");
            f3 << "n,x_n,u_n\n";
            for(int i=0;i<N;i++){
                double x_n= i*h;
                f3 << (i+1) << "," << x_n << "," << solFull[i] << "\n";
            }
            f3.close();
        }
    }

    timeFile.close();
    cout << "Zakonczono\n";
    return 0;
}
